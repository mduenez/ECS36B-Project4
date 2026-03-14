#include "BusSystem.h"
#include "BusSystemIndexer.h"
#include "TravelPlan.h"
#include "HTMLTripPlanWriter.h"

#include <memory>
#include <vector>
#include <iostream>

int main() {
    // 1️⃣ Create stops
    auto stop1 = std::make_shared<Stop>(1, "Stop A", 37.7749, -122.4194);
    auto stop2 = std::make_shared<Stop>(2, "Stop B", 37.7849, -122.4094);
    auto stop3 = std::make_shared<Stop>(3, "Stop C", 37.7949, -122.3994);

    // 2️⃣ Create routes
    auto route1 = std::make_shared<Route>("Route 1");
    route1->stops.push_back(stop1);
    route1->stops.push_back(stop2);

    auto route2 = std::make_shared<Route>("Route 2");
    route2->stops.push_back(stop2);
    route2->stops.push_back(stop3);

    // 3️⃣ Create bus system
    auto busSystem = std::make_shared<BusSystem>();
    busSystem->stops = {stop1, stop2, stop3};
    busSystem->routes = {route1, route2};

    // 4️⃣ Index the system
    BusSystemIndexer indexer(busSystem);

    std::cout << "Total stops: " << indexer.StopCount() << "\n";
    std::cout << "Total routes: " << indexer.RouteCount() << "\n";

    // 5️⃣ Create a travel plan
    TravelPlan plan;
    plan.stops = {stop1, stop2};
    plan.routes = {route1};

    // 6️⃣ Write HTML trip plan
    HTMLTripPlanWriter writer("trip_plan.html");
    writer.Write(plan);

    std::cout << "Trip plan written to trip_plan.html\n";

    return 0;
}
