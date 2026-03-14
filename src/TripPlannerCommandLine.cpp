#include "TripPlannerCommandLine.h" // own header
#include "TripPlanner.h"
#include "TextTripPlanWriter.h"
#include "SVGTripPlanWriter.h"
#include "HTMLTripPlanWriter.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>

TripPlannerCommandLine::TripPlannerCommandLine(std::shared_ptr<BusSystem> bs,
                                               std::shared_ptr<StreetMap> sm,
                                               std::shared_ptr<DataSink> sink)
    : tripPlanner(std::make_shared<TripPlanner>(bs)), dataSink(sink) {}

void TripPlannerCommandLine::ProcessCommands() {
    std::string cmd;
    while (true) {
        std::cout << "> ";
        std::cin >> cmd;
        if (cmd == "exit") break;
        else if (cmd == "help") {
            std::cout << "Commands: help, exit, leaveat, arriveby, save\n";
        } else if (cmd == "leaveat") {
            StopTime t; TStopID start, end;
            std::cin >> t >> start >> end;
            TravelPlan plan;
            if (tripPlanner->FindRouteLeaveTime(start, end, t, plan))
                textWriter.WritePlan(dataSink, plan);
            else std::cout << "No route found.\n";
        } else if (cmd == "arriveby") {
            StopTime t; TStopID start, end;
            std::cin >> t >> start >> end;
            TravelPlan plan;
            if (tripPlanner->FindRouteArrivalTime(start, end, t, plan))
                textWriter.WritePlan(dataSink, plan);
            else std::cout << "No route found.\n";
        } else if (cmd == "save") {
            std::cout << "Saving last plan...\n";
        } else {
            std::cout << "Unknown command.\n";
        }
    }
}
