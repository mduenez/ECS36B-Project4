#include <gtest/gtest.h>
#include <memory>
#include <algorithm>
#include "TripPlanner.h"
#include "BusSystem.h"
#include "BusSystemIndexer.h"
#include "StreetMap.h"

// -------------------- Helper: Create Mock BusSystem --------------------
std::shared_ptr<CBusSystem> CreateMockBusSystem() {
    auto busSystem = std::make_shared<CBusSystem>();

    // Create stops
    auto stop1 = std::make_shared<SStop>(1, "Stop 1");
    auto stop2 = std::make_shared<SStop>(2, "Stop 2");
    auto stop3 = std::make_shared<SStop>(3, "Stop 3");
    busSystem->AddStop(stop1);
    busSystem->AddStop(stop2);
    busSystem->AddStop(stop3);

    // Create route
    auto routeA = std::make_shared<SRoute>("RouteA");
    routeA->AddStop(stop1->ID(), TStopTime{8*3600});        // 8:00 AM
    routeA->AddStop(stop2->ID(), TStopTime{8*3600 + 600});  // 8:10 AM
    routeA->AddStop(stop3->ID(), TStopTime{8*3600 + 1200}); // 8:20 AM
    busSystem->AddRoute(routeA);

    return busSystem;
}

// -------------------- Direct TripPlanner Tests --------------------
TEST(TripPlannerTest, CanInstantiate) {
    auto busSystem = CreateMockBusSystem();
    CTripPlanner planner(busSystem);
    SUCCEED();
}

TEST(TripPlannerTest, DirectRouteLeaveTime) {
    auto busSystem = CreateMockBusSystem();
    CTripPlanner planner(busSystem);

    auto route = planner.FindDirectRouteLeaveTime(1,3, TStopTime{7*3600 + 50*60});
    ASSERT_NE(route, nullptr);
    EXPECT_EQ(route->Name(), "RouteA");
}

TEST(TripPlannerTest, DirectRouteArrivalTime) {
    auto busSystem = CreateMockBusSystem();
    CTripPlanner planner(busSystem);

    auto route = planner.FindDirectRouteArrivalTime(1,3, TStopTime{8*3600 + 1200});
    ASSERT_NE(route, nullptr);
    EXPECT_EQ(route->Name(), "RouteA");
}

TEST(TripPlannerTest, FindRouteLeaveTimePlan) {
    auto busSystem = CreateMockBusSystem();
    CTripPlanner planner(busSystem);
    TTravelPlan plan;
    bool found = planner.FindRouteLeaveTime(1,3, TStopTime{7*3600 + 50*60}, plan);
    EXPECT_TRUE(found);
    EXPECT_EQ(plan.stops.front(), 1);
    EXPECT_EQ(plan.stops.back(), 3);
}

TEST(TripPlannerTest, FindRouteArrivalTimePlan) {
    auto busSystem = CreateMockBusSystem();
    CTripPlanner planner(busSystem);
    TTravelPlan plan;
    bool found = planner.FindRouteArrivalTime(1,3, TStopTime{8*3600 + 1200}, plan);
    EXPECT_TRUE(found);
    EXPECT_EQ(plan.stops.front(), 1);
    EXPECT_EQ(plan.stops.back(), 3);
}

TEST(TripPlannerTest, NoRouteAvailable) {
    auto busSystem = CreateMockBusSystem();
    CTripPlanner planner(busSystem);

    auto route = planner.FindDirectRouteLeaveTime(1,999, TStopTime{7*3600});
    EXPECT_EQ(route, nullptr);

    TTravelPlan plan;
    bool found = planner.FindRouteLeaveTime(1,999, TStopTime{7*3600}, plan);
    EXPECT_FALSE(found);
}

TEST(TripPlannerTest, StopIndicesCorrect) {
    auto busSystem = CreateMockBusSystem();
    auto indexer = CBusSystemIndexer(busSystem);

    EXPECT_EQ(indexer.StopCount(), 3);
    EXPECT_EQ(indexer.RouteCount(), 1);

    auto stop = indexer.SortedStopByIndex(0);
    ASSERT_NE(stop, nullptr);
    EXPECT_EQ(stop->ID(), 1);

    auto route = indexer.SortedRouteByIndex(0);
    ASSERT_NE(route, nullptr);
    EXPECT_EQ(route->Name(), "RouteA");
}

// -------------------- Edge Cases --------------------
TEST(TripPlannerTest, NoAvailableTripDueToTime) {
    auto busSystem = CreateMockBusSystem();
    CTripPlanner planner(busSystem);

    TTravelPlan plan;
    bool found = planner.FindRouteLeaveTime(1,3, TStopTime{9*3600}, plan); // 9:00 AM
    EXPECT_FALSE(found);

    found = planner.FindRouteArrivalTime(1,3, TStopTime{7*3600}, plan); // 7:00 AM
    EXPECT_FALSE(found);
}

// -------------------- One-Stop Extra Credit --------------------
TEST(TripPlannerTest, OneStopRouteExtraCredit) {
    auto busSystem = std::make_shared<CBusSystem>();

    // Stops
    auto stop1 = std::make_shared<SStop>(1, "Stop 1");
    auto stop2 = std::make_shared<SStop>(2, "Stop 2");
    auto stop3 = std::make_shared<SStop>(3, "Stop 3");
    auto stop4 = std::make_shared<SStop>(4, "Stop 4");
    busSystem->AddStop(stop1);
    busSystem->AddStop(stop2);
    busSystem->AddStop(stop3);
    busSystem->AddStop(stop4);

    // Route A: Stop1 -> Stop2 -> Stop3
    auto routeA = std::make_shared<SRoute>("RouteA");
    routeA->AddStop(stop1->ID(), TStopTime{8*3600});
    routeA->AddStop(stop2->ID(), TStopTime{8*3600 + 600});
    routeA->AddStop(stop3->ID(), TStopTime{8*3600 + 1200});
    busSystem->AddRoute(routeA);

    // Route B: Stop3 -> Stop4
    auto routeB = std::make_shared<SRoute>("RouteB");
    routeB->AddStop(stop3->ID(), TStopTime{8*3600 + 1500});
    routeB->AddStop(stop4->ID(), TStopTime{8*3600 + 1800});
    busSystem->AddRoute(routeB);

    CTripPlanner planner(busSystem);

    TTravelPlan plan;
    bool found = planner.FindRouteLeaveTime(1, 4, TStopTime{7*3600 + 50*60});
    EXPECT_TRUE(found);
    EXPECT_EQ(plan.stops.front(), 1);
    EXPECT_EQ(plan.stops.back(), 4);
    EXPECT_NE(std::find(plan.stops.begin(), plan.stops.end(), 3), plan.stops.end()); // transfer at stop 3
}

// -------------------- Main --------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}