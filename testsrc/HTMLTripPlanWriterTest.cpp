#include <gtest/gtest.h>
#include <fstream>
#include <memory>
#include <cstdio> // for std::remove
#include "HTMLTripPlanWriter.h"

// -------------------- Helper: Create Mock Stops & Routes --------------------
struct MockStop {
    std::string name;
    double latitude;
    double longitude;
};

struct MockRoute {
    std::string name;
};

struct MockTravelPlan {
    std::vector<std::shared_ptr<MockStop>> stops;
    std::vector<std::shared_ptr<MockRoute>> routes;
};

// -------------------- Test Fixture --------------------
class HTMLTripPlanWriterTest : public ::testing::Test {
protected:
    std::string filename = "test_trip.html";

    void TearDown() override {
        // Remove test file after each test
        std::remove(filename.c_str());
    }
};

// -------------------- Tests --------------------
TEST_F(HTMLTripPlanWriterTest, CanWriteEmptyPlan) {
    MockTravelPlan plan;
    HTMLTripPlanWriter writer(filename);
    writer.Write(plan);

    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string contents((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    EXPECT_NE(contents.find("<html>"), std::string::npos);
    EXPECT_NE(contents.find("<body>"), std::string::npos);
    EXPECT_NE(contents.find("Trip Plan"), std::string::npos);
    file.close();
}

TEST_F(HTMLTripPlanWriterTest, WritesStopsAndRoutes) {
    MockTravelPlan plan;

    auto stop1 = std::make_shared<MockStop>(MockStop{"Stop 1", 38.5, -121.7});
    auto stop2 = std::make_shared<MockStop>(MockStop{"Stop 2", 38.6, -121.8});
    plan.stops.push_back(stop1);
    plan.stops.push_back(stop2);

    auto route1 = std::make_shared<MockRoute>(MockRoute{"RouteA"});
    plan.routes.push_back(route1);

    HTMLTripPlanWriter writer(filename);
    writer.Write(plan);

    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    std::string contents((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    
    // Check that stops appear in HTML
    EXPECT_NE(contents.find("Stop 1"), std::string::npos);
    EXPECT_NE(contents.find("38.5"), std::string::npos);
    EXPECT_NE(contents.find("-121.7"), std::string::npos);
    EXPECT_NE(contents.find("Stop 2"), std::string::npos);

    // Check that routes appear in HTML
    EXPECT_NE(contents.find("RouteA"), std::string::npos);
    file.close();
}

TEST_F(HTMLTripPlanWriterTest, HandlesSpecialCharacters) {
    MockTravelPlan plan;
    auto stop = std::make_shared<MockStop>(MockStop{"<Stop & 1>", 0, 0});
    plan.stops.push_back(stop);
    auto route = std::make_shared<MockRoute>(MockRoute{"Route <A>"});
    plan.routes.push_back(route);

    HTMLTripPlanWriter writer(filename);
    writer.Write(plan);

    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    std::string contents((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    // Ensure special characters appear correctly escaped
    EXPECT_NE(contents.find("<Stop & 1>"), std::string::npos);
    EXPECT_NE(contents.find("Route <A>"), std::string::npos);

    file.close();
}