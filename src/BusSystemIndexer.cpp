#include "BusSystemIndexer.h"
#include <algorithm>
#include <stdexcept>

// -----------------------
// Constructor
// -----------------------
BusSystemIndexer::BusSystemIndexer(std::shared_ptr<BusSystem> busSystem)
    : DBusSystem(busSystem)  // now valid
{
    if (!busSystem) {
        throw std::invalid_argument("BusSystem pointer is null");
    }

    // Copy stops and routes
    stops = busSystem->GetStops();
    routes = busSystem->GetRoutes();

    // Sort stops alphabetically by name
    std::sort(stops.begin(), stops.end(),
              [](const std::shared_ptr<Stop>& a, const std::shared_ptr<Stop>& b) {
                  return a->name < b->name;
              });

    // Sort routes alphabetically by name
    std::sort(routes.begin(), routes.end(),
              [](const std::shared_ptr<Route>& a, const std::shared_ptr<Route>& b) {
                  return a->name < b->name;
              });
}

// -----------------------
// Destructor
// -----------------------
BusSystemIndexer::~BusSystemIndexer() = default;

// -----------------------
// Count accessors
// -----------------------
size_t BusSystemIndexer::StopCount() const noexcept {
    return stops.size();
}

size_t BusSystemIndexer::RouteCount() const noexcept {
    return routes.size();
}

// -----------------------
// Access sorted stops and routes
// -----------------------
std::shared_ptr<Stop> BusSystemIndexer::SortedStopByIndex(size_t index) const noexcept {
    if (index < stops.size()) {
        return stops[index];
    }
    return nullptr;
}

std::shared_ptr<Route> BusSystemIndexer::SortedRouteByIndex(size_t index) const noexcept {
    if (index < routes.size()) {
        return routes[index];
    }
    return nullptr;
}