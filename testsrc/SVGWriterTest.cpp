#include <gtest/gtest.h>
#include "SVGWriter.h"
#include "StringDataSink.h"

// -------------------- Basic Create / Destroy --------------------
TEST(SVGWriterTest, CreateDestroyTest){
<<<<<<< HEAD
    std::shared_ptr<CStringDataSink> Sink = std::make_shared<CStringDataSink>();
    {
        CSVGWriter Writer(Sink,100,50);
    }
    EXPECT_EQ(Sink->String(),"<?xml version=\"1.0\" encoding=\"UTF-8\"?><svg width=\"100\" height=\"50\" xmlns=\"http://www.w3.org/2000/svg\"></svg>");
=======
    auto Sink = std::make_shared<CStringDataSink>();
    {
        CSVGWriter Writer(Sink, 100, 50);
    }
    // After destruction, Sink should contain empty svg
    EXPECT_EQ(Sink->String(),
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<svg width=\"100\" height=\"50\" xmlns=\"http://www.w3.org/2000/svg\"></svg>"
    );
>>>>>>> main
}

// -------------------- Drawing Shapes --------------------
TEST(SVGWriterTest, CircleTest){
    auto Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 200, 200);

    EXPECT_TRUE(Writer.Circle(50,50,25)); // cx, cy, radius
    std::string output = Sink->String();
    EXPECT_NE(output.find("<circle"), std::string::npos);
    EXPECT_NE(output.find("cx=\"50\""), std::string::npos);
    EXPECT_NE(output.find("cy=\"50\""), std::string::npos);
    EXPECT_NE(output.find("r=\"25\""), std::string::npos);
}

TEST(SVGWriterTest, RectangleTest){
    auto Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 200, 200);

    EXPECT_TRUE(Writer.Rectangle(10,20,30,40)); // x,y,width,height
    std::string output = Sink->String();
    EXPECT_NE(output.find("<rect"), std::string::npos);
    EXPECT_NE(output.find("x=\"10\""), std::string::npos);
    EXPECT_NE(output.find("y=\"20\""), std::string::npos);
    EXPECT_NE(output.find("width=\"30\""), std::string::npos);
    EXPECT_NE(output.find("height=\"40\""), std::string::npos);
}

TEST(SVGWriterTest, LineTest){
    auto Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 200, 200);

    EXPECT_TRUE(Writer.Line(1,2,3,4)); // x1,y1,x2,y2
    std::string output = Sink->String();
    EXPECT_NE(output.find("<line"), std::string::npos);
    EXPECT_NE(output.find("x1=\"1\""), std::string::npos);
    EXPECT_NE(output.find("y1=\"2\""), std::string::npos);
    EXPECT_NE(output.find("x2=\"3\""), std::string::npos);
    EXPECT_NE(output.find("y2=\"4\""), std::string::npos);
}

// -------------------- Simple Path Test --------------------
TEST(SVGWriterTest, SimplePathTest){
    auto Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 200, 200);

    std::vector<std::pair<int,int>> Points = {{0,0},{10,10},{20,5}};
    EXPECT_TRUE(Writer.Path(Points));
    std::string output = Sink->String();
    EXPECT_NE(output.find("<path"), std::string::npos);
    EXPECT_NE(output.find("M0,0"), std::string::npos);
    EXPECT_NE(output.find("L10,10"), std::string::npos);
    EXPECT_NE(output.find("L20,5"), std::string::npos);
}

// -------------------- Grouping --------------------
TEST(SVGWriterTest, GroupTest){
    auto Sink = std::make_shared<CStringDataSink>();
    CSVGWriter Writer(Sink, 200, 200);

    Writer.GroupPush("class='grp'");
    EXPECT_TRUE(Writer.Circle(5,5,2));
    Writer.GroupPop();

    std::string output = Sink->String();
    EXPECT_NE(output.find("<g"), std::string::npos);
    EXPECT_NE(output.find("</g>"), std::string::npos);
}

// -------------------- Error Handling --------------------
class CFailingSink : public CDataSink{
public:
    int DValidCalls = 0;
    virtual ~CFailingSink(){}

    bool Put(const char &ch) noexcept override{
        if(DValidCalls){
            DValidCalls--;
            return true;
        }
        return false;
    }

    bool Write(const std::vector<char> &buf) noexcept override{
        if(DValidCalls){
            DValidCalls--;
            return true;
        }
        return false;
    }
};

TEST(SVGWriterTest, ErrorTests){
    CFailingSink Sink;
    Sink.DValidCalls = 0; // force failure

    CSVGWriter Writer(std::shared_ptr<CFailingSink>(&Sink, [](CFailingSink*){}), 100, 100);
    
    // All drawing calls should fail gracefully
    EXPECT_FALSE(Writer.Circle(1,1,1));
    EXPECT_FALSE(Writer.Rectangle(1,1,1,1));
    EXPECT_FALSE(Writer.Line(0,0,1,1));
    EXPECT_FALSE(Writer.Path({{0,0},{1,1}}));
}