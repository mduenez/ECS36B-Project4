#include <gtest/gtest.h>
#include "BusSystemIndexer.h"
#include "BusSystem.h"
#include <memory>
#include <unordered_set>

// -------------------- Helper: Create a simple bus system --------------------
std::shared_ptr<CBusSystem> CreateSimpleBusSystem() {
    auto busSystem = std::make_shared<CBusSystem>();

    // Stops
    auto stop1 = std::make_shared<SStop>(1, "Stop 1");
    auto stop2 = std::make_shared<SStop>(2, "Stop 2");
    auto stop3 = std::make_shared<SStop>(3, "Stop 3");
    busSystem->AddStop(stop1);
    busSystem->AddStop(stop2);
    busSystem->AddStop(stop3);

    // Route A: Stop1 -> Stop2
    auto routeA = std::make_shared<SRoute>("RouteA");
    routeA->AddStop(stop1->ID(), TStopTime{8*3600});
    routeA->AddStop(stop2->ID(), TStopTime{8*3600 + 600});
    busSystem->AddRoute(routeA);

    // Route B: Stop2 -> Stop3
    auto routeB = std::make_shared<SRoute>("RouteB");
    routeB->AddStop(stop2->ID(), TStopTime{9*3600});
    routeB->AddStop(stop3->ID(), TStopTime{9*3600 + 600});
    busSystem->AddRoute(routeB);

    return busSystem;
}

// -------------------- Tests --------------------
TEST(BusSystemIndexerTest, CreateDestroy) {
    auto busSystem = CreateSimpleBusSystem();
    CBusSystemIndexer indexer(busSystem);
    SUCCEED(); // Just construction/destruction
}

TEST(BusSystemIndexerTest, StopAndRouteCounts) {
    auto busSystem = CreateSimpleBusSystem();
    CBusSystemIndexer indexer(busSystem);

    EXPECT_EQ(indexer.StopCount(), 3);
    EXPECT_EQ(indexer.RouteCount(), 2);
}

TEST(BusSystemIndexerTest, SortedStopByIndex) {
    auto busSystem = CreateSimpleBusSystem();
    CBusSystemIndexer indexer(busSystem);

    auto stop = indexer.SortedStopByIndex(0);
    ASSERT_NE(stop, nullptr);
    EXPECT_EQ(stop->ID(), 1);

    stop = indexer.SortedStopByIndex(2);
    ASSERT_NE(stop, nullptr);
    EXPECT_EQ(stop->ID(), 3);

    // Out-of-bounds
    stop = indexer.SortedStopByIndex(3);
    EXPECT_EQ(stop, nullptr);
}

TEST(BusSystemIndexerTest, SortedRouteByIndex) {
    auto busSystem = CreateSimpleBusSystem();
    CBusSystemIndexer indexer(busSystem);

    auto route = indexer.SortedRouteByIndex(0);
    ASSERT_NE(route, nullptr);
    EXPECT_EQ(route->Name(), "RouteA");

    route = indexer.SortedRouteByIndex(1);
    ASSERT_NE(route, nullptr);
    EXPECT_EQ(route->Name(), "RouteB");

    // Out-of-bounds
    route = indexer.SortedRouteByIndex(2);
    EXPECT_EQ(route, nullptr);
}

TEST(BusSystemIndexerTest, RouteByName) {
    auto busSystem = CreateSimpleBusSystem();
    CBusSystemIndexer indexer(busSystem);

    auto route = indexer.RouteByName("RouteA");
    ASSERT_NE(route, nullptr);
    EXPECT_EQ(route->Name(), "RouteA");

    route = indexer.RouteByName("NonExistent");
    EXPECT_EQ(route, nullptr);
}

TEST(BusSystemIndexerTest, RoutesByStopID) {
    auto busSystem = CreateSimpleBusSystem();
    CBusSystemIndexer indexer(busSystem);

    std::unordered_set<std::string> routes;
    EXPECT_TRUE(indexer.RoutesByStopID(2, routes));
    EXPECT_TRUE(routes.find("RouteA") != routes.end());
    EXPECT_TRUE(routes.find("RouteB") != routes.end());

    routes.clear();
    EXPECT_FALSE(indexer.RoutesByStopID(999, routes));
    EXPECT_TRUE(routes.empty());
}

TEST(BusSystemIndexerTest, RoutesByStopIDs) {
    auto busSystem = CreateSimpleBusSystem();
    CBusSystemIndexer indexer(busSystem);

    std::unordered_set<std::string> routes;
    EXPECT_TRUE(indexer.RoutesByStopIDs(2, 3, routes));
    EXPECT_TRUE(routes.find("RouteB") != routes.end());

    routes.clear();
    EXPECT_FALSE(indexer.RoutesByStopIDs(1, 3, routes)); // No direct route
    EXPECT_TRUE(routes.empty());
}

TEST(BusSystemIndexerTest, StopIDsByRoutes) {
    auto busSystem = CreateSimpleBusSystem();
    CBusSystemIndexer indexer(busSystem);

    std::unordered_set<TStopID> stops;
    EXPECT_TRUE(indexer.StopIDsByRoutes("RouteA", "RouteB", stops));
    EXPECT_TRUE(stops.find(2) != stops.end()); // Stop 2 is shared

    stops.clear();
    EXPECT_FALSE(indexer.StopIDsByRoutes("RouteA", "NonExistent", stops));
    EXPECT_TRUE(stops.empty());
}

// -------------------- Main --------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}