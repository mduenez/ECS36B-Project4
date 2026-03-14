#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include "TripPlannerCommandLine.h"
#include "BusSystem.h"
#include "StreetMap.h"
#include "StringDataSink.h"

// -------------------- Helper: Create a Mock BusSystem --------------------
std::shared_ptr<CBusSystem> CreateMockBusSystem() {
    auto busSystem = std::make_shared<CBusSystem>();

    auto stop1 = std::make_shared<SStop>(1, "Stop 1");
    auto stop2 = std::make_shared<SStop>(2, "Stop 2");
    auto stop3 = std::make_shared<SStop>(3, "Stop 3");

    busSystem->AddStop(stop1);
    busSystem->AddStop(stop2);
    busSystem->AddStop(stop3);

    auto routeA = std::make_shared<SRoute>("RouteA");
    routeA->AddStop(stop1->ID(), TStopTime{8*3600});
    routeA->AddStop(stop2->ID(), TStopTime{8*3600 + 600});
    routeA->AddStop(stop3->ID(), TStopTime{8*3600 + 1200});
    busSystem->AddRoute(routeA);

    return busSystem;
}

// -------------------- Tests --------------------
TEST(TripPlannerCommandLineTest, LeaveAtCommand) {
    auto busSystem = CreateMockBusSystem();
    auto streetMap = std::make_shared<StreetMap>();
    auto sink = std::make_shared<CStringDataSink>();

    TripPlannerCommandLine cli(busSystem, streetMap, sink);

    // Simulate leaveat command
    std::istringstream input("leaveat 28800 1 3\nexit\n"); // 8:00AM, stop 1 to 3
    std::streambuf* cinBackup = std::cin.rdbuf(input.rdbuf());

    cli.ProcessCommands();

    std::cin.rdbuf(cinBackup); // restore

    std::string output = sink->String();
    EXPECT_NE(output.find("From stop 1 to 3"), std::string::npos);
    EXPECT_NE(output.find("Route: RouteA"), std::string::npos);
}

TEST(TripPlannerCommandLineTest, ArriveByCommand) {
    auto busSystem = CreateMockBusSystem();
    auto streetMap = std::make_shared<StreetMap>();
    auto sink = std::make_shared<CStringDataSink>();

    TripPlannerCommandLine cli(busSystem, streetMap, sink);

    // Simulate arriveby command
    std::istringstream input("arriveby 30000 1 3\nexit\n"); // 8:20AM, stop 1 to 3
    std::streambuf* cinBackup = std::cin.rdbuf(input.rdbuf());

    cli.ProcessCommands();

    std::cin.rdbuf(cinBackup); // restore

    std::string output = sink->String();
    EXPECT_NE(output.find("From stop 1 to 3"), std::string::npos);
    EXPECT_NE(output.find("Route: RouteA"), std::string::npos);
}

TEST(TripPlannerCommandLineTest, HelpAndUnknownCommand) {
    auto busSystem = CreateMockBusSystem();
    auto streetMap = std::make_shared<StreetMap>();
    auto sink = std::make_shared<CStringDataSink>();

    TripPlannerCommandLine cli(busSystem, streetMap, sink);

    std::istringstream input("help\nfoobar\nexit\n");
    std::streambuf* cinBackup = std::cin.rdbuf(input.rdbuf());

    // Redirect cout to a stringstream to capture help/unknown output
    std::ostringstream coutCapture;
    std::streambuf* coutBackup = std::cout.rdbuf(coutCapture.rdbuf());

    cli.ProcessCommands();

    std::cin.rdbuf(cinBackup);
    std::cout.rdbuf(coutBackup);

    std::string output = coutCapture.str();
    EXPECT_NE(output.find("Commands: help, exit, leaveat, arriveby, save"), std::string::npos);
    EXPECT_NE(output.find("Unknown command"), std::string::npos);
}

TEST(TripPlannerCommandLineTest, NoRouteFound) {
    auto busSystem = CreateMockBusSystem();
    auto streetMap = std::make_shared<StreetMap>();
    auto sink = std::make_shared<CStringDataSink>();

    TripPlannerCommandLine cli(busSystem, streetMap, sink);

    std::istringstream input("leaveat 1000 1 999\nexit\n"); // Nonexistent destination
    std::streambuf* cinBackup = std::cin.rdbuf(input.rdbuf());

    std::ostringstream coutCapture;
    std::streambuf* coutBackup = std::cout.rdbuf(coutCapture.rdbuf());

    cli.ProcessCommands();

    std::cin.rdbuf(cinBackup);
    std::cout.rdbuf(coutBackup);

    std::string output = coutCapture.str();
    EXPECT_NE(output.find("No route found"), std::string::npos);
}

// -------------------- Main --------------------
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}