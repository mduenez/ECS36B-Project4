#include <gtest/gtest.h>
#include "TextTripPlanWriter.h"
#include "StringDataSink.h"
#include "BusSystem.h"
#include "TripPlanner.h"

// -------------------- Helper: Create a Travel Plan --------------------
TTravelPlan CreateMockPlan() {
    TTravelPlan plan;
    plan.stops = {1, 2, 3};
    plan.routes = {"RouteA"};
    plan.times = {TStopTime{8*3600}, TStopTime{8*3600 + 600}, TStopTime{8*3600 + 1200}}; // 8:00, 8:10, 8:20
    return plan;
}

// -------------------- Tests --------------------
TEST(TextTripPlanWriterTest, CreateDestroy) {
    CTextTripPlanWriter writer;
    SUCCEED(); // Just testing construction/destruction
}

TEST(TextTripPlanWriterTest, WriteSimplePlan) {
    CTextTripPlanWriter writer;
    auto sink = std::make_shared<CStringDataSink>();

    TTravelPlan plan = CreateMockPlan();

    bool result = writer.WritePlan(sink, plan);
    EXPECT_TRUE(result);

    std::string output = sink->String();
    EXPECT_NE(output.find("Stop 1"), std::string::npos);
    EXPECT_NE(output.find("Stop 2"), std::string::npos);
    EXPECT_NE(output.find("Stop 3"), std::string::npos);
    EXPECT_NE(output.find("RouteA"), std::string::npos);
}

TEST(TextTripPlanWriterTest, WriteEmptyPlan) {
    CTextTripPlanWriter writer;
    auto sink = std::make_shared<CStringDataSink>();

    TTravelPlan emptyPlan;
    bool result = writer.WritePlan(sink, emptyPlan);
    EXPECT_TRUE(result); // Should succeed even if plan is empty

    std::string output = sink->String();
    EXPECT_TRUE(output.empty() || output.find("Stop") == std::string::npos);
}

TEST(TextTripPlanWriterTest, VerboseOutputExtraCredit) {
    CTextTripPlanWriter writer;
    auto sink = std::make_shared<CStringDataSink>();

    TTravelPlan plan = CreateMockPlan();

    writer.SetVerbose(true); // Enable verbose output (if implemented)
    bool result = writer.WritePlan(sink, plan);
    EXPECT_TRUE(result);

    std::string output = sink->String();
    EXPECT_NE(output.find("8:00"), std::string::npos);
    EXPECT_NE(output.find("8:10"), std::string::npos);
    EXPECT_NE(output.find("8:20"), std::string::npos);
}

// -------------------- Main --------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}