#include "svg.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

// -------------------- Helper Structs / Callbacks --------------------
struct STestOutput {
    std::vector<std::string> DLines;
    bool DDestroyed = false;

    std::string JoinOutput() const {
        std::string result;
        for (const auto& line : DLines) {
            result += line;
        }
        return result;
    }
};

svg_return_t write_callback(svg_user_context_ptr user, const char* text) {
    if (!user || !text) return SVG_ERR_NULL;
    auto* out = static_cast<STestOutput*>(user);
    out->DLines.push_back(text);
    return SVG_OK;
}

svg_return_t cleanup_callback(svg_user_context_ptr user) {
    if (!user) return SVG_ERR_NULL;
    auto* out = static_cast<STestOutput*>(user);
    if (out->DDestroyed) return SVG_ERR_STATE;
    out->DDestroyed = true;
    return SVG_OK;
}

svg_return_t write_error_callback(svg_user_context_ptr user, const char* /*text*/) {
    int* failureCount = static_cast<int*>(user);
    if (*failureCount) {
        (*failureCount)--;
        return SVG_OK;
    }
    return SVG_ERR_IO;
}

// -------------------- Test Fixture --------------------
class SVGTest : public ::testing::Test {
protected:
    STestOutput DOutput;
    svg_context_ptr DContext = nullptr;

    void SetUp() override {
        DContext = svg_create(write_callback, cleanup_callback, &DOutput, 100, 100);
        ASSERT_NE(DContext, nullptr);
    }

    void TearDown() override {
        if (DContext) {
            svg_destroy(DContext);
            DContext = nullptr;
        }
    }
};

// -------------------- Tests --------------------

// Basic creation / destruction
TEST_F(SVGTest, CreateAndDestroy) {
    EXPECT_FALSE(DOutput.DDestroyed);
    svg_destroy(DContext);
    DContext = nullptr;
    EXPECT_TRUE(DOutput.DDestroyed);
}

// Drawing a circle
TEST_F(SVGTest, Circle) {
    ASSERT_EQ(svg_circle(DContext, 50, 50, 25), SVG_OK);
    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<circle"), std::string::npos);
    EXPECT_NE(output.find("cx=\"50\""), std::string::npos);
    EXPECT_NE(output.find("cy=\"50\""), std::string::npos);
    EXPECT_NE(output.find("r=\"25\""), std::string::npos);
}

// Drawing a rectangle
TEST_F(SVGTest, Rectangle) {
    ASSERT_EQ(svg_rect(DContext, 10, 20, 30, 40), SVG_OK);
    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<rect"), std::string::npos);
    EXPECT_NE(output.find("x=\"10\""), std::string::npos);
    EXPECT_NE(output.find("y=\"20\""), std::string::npos);
    EXPECT_NE(output.find("width=\"30\""), std::string::npos);
    EXPECT_NE(output.find("height=\"40\""), std::string::npos);
}

// Drawing a line
TEST_F(SVGTest, Line) {
    ASSERT_EQ(svg_line(DContext, 1, 2, 3, 4), SVG_OK);
    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<line"), std::string::npos);
    EXPECT_NE(output.find("x1=\"1\""), std::string::npos);
    EXPECT_NE(output.find("y1=\"2\""), std::string::npos);
    EXPECT_NE(output.find("x2=\"3\""), std::string::npos);
    EXPECT_NE(output.find("y2=\"4\""), std::string::npos);
}

// Grouping test
TEST_F(SVGTest, Grouping) {
    svg_group_push(DContext, "class='grp'");
    ASSERT_EQ(svg_circle(DContext, 10,10,5), SVG_OK);
    svg_group_pop(DContext);

    std::string output = DOutput.JoinOutput();
    EXPECT_NE(output.find("<g"), std::string::npos);
    EXPECT_NE(output.find("</g>"), std::string::npos);
    EXPECT_NE(output.find("<circle"), std::string::npos);
}

// Zero dimensions edge case
TEST_F(SVGTest, ZeroDimensions) {
    ASSERT_EQ(svg_rect(DContext, 0, 0, 0, 0), SVG_OK);
    ASSERT_EQ(svg_circle(DContext, 0, 0, 0), SVG_OK);
}

// Null pointer / invalid context
TEST_F(SVGTest, NullPointPointer) {
    EXPECT_EQ(svg_circle(nullptr, 10, 10, 5), SVG_ERR_NULL);
    EXPECT_EQ(svg_rect(nullptr, 0,0,10,10), SVG_ERR_NULL);
    EXPECT_EQ(svg_line(nullptr, 1,1,2,2), SVG_ERR_NULL);
}

// Error / write failure simulation
TEST_F(SVGTest, IOErrorTest) {
    int failureCount = 0;
    auto ctx = svg_create(write_error_callback, cleanup_callback, &failureCount, 100, 100);
    ASSERT_NE(ctx, nullptr);
    EXPECT_EQ(svg_circle(ctx, 0,0,5), SVG_ERR_IO);
    svg_destroy(ctx);
}

// Double destroy
TEST_F(SVGTest, DestroyEdgeCases) {
    ASSERT_EQ(svg_destroy(DContext), SVG_OK);
    DContext = nullptr;
    EXPECT_EQ(svg_destroy(DContext), SVG_ERR_NULL); // already null
}