#include <gtest/gtest.h>
#include "SVGTripPlanWriter.h"
#include "StringDataSink.h"
#include "TripPlannerTypes.h" // for TTravelPlan, SStop, etc.

TEST(SVGTripPlanWriterTest, CreateDestroyTest) {
    CSVGTripPlanWriter writer;
    SUCCEED(); // test passes if no exceptions occur during construction/destruction
}

TEST(SVGTripPlanWriterTest, WriteEmptyPlan) {
    CSVGTripPlanWriter writer;
    auto sink = std::make_shared<CStringDataSink>();
    TTravelPlan plan; // empty travel plan

    // Should succeed even if the plan is empty
    EXPECT_TRUE(writer.WritePlan(sink, plan));
    EXPECT_TRUE(sink->String().empty());
}

TEST(SVGTripPlanWriterTest, WriteSimplePlan) {
    CSVGTripPlanWriter writer;
    auto sink = std::make_shared<CStringDataSink>();

    TTravelPlan plan;
    // Add a simple leg to the travel plan
    plan.DRouteName = "Route1";
    plan.DStops.push_back(1);
    plan.DStops.push_back(2);
    plan.DStartTime = 1000;
    plan.DEndTime = 1030;

    EXPECT_TRUE(writer.WritePlan(sink, plan));

    std::string output = sink->String();
    EXPECT_NE(output.find("<svg"), std::string::npos);
    EXPECT_NE(output.find("Route1"), std::string::npos);
}