#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Stop.h"
#include "Route.h"
#include "BusSystem.h"

class BusSystemIndexer {
public:
    // Constructor / Destructor
    BusSystemIndexer(std::shared_ptr<BusSystem> busSystem);
    ~BusSystemIndexer();

    // Count accessors
    size_t StopCount() const noexcept;
    size_t RouteCount() const noexcept;

    // Access sorted stops and routes
    std::shared_ptr<Stop> SortedStopByIndex(size_t index) const noexcept;
    std::shared_ptr<Route> SortedRouteByIndex(size_t index) const noexcept;

private:
    std::shared_ptr<BusSystem> DBusSystem;          // original bus system
    std::vector<std::shared_ptr<Stop>> stops;       // sorted copy of stops
    std::vector<std::shared_ptr<Route>> routes;    // sorted copy of routes
};