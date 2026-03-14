#pragma once
#include <vector>
#include <memory>
#include "Stop.h"
#include "Route.h"

struct BusSystem {
    std::vector<std::shared_ptr<Stop>> stops;
    std::vector<std::shared_ptr<Route>> routes;

    void AddStop(std::shared_ptr<Stop> stop) {
        stops.push_back(stop);
    }

    void AddRoute(std::shared_ptr<Route> route) {
        routes.push_back(route);
    }

    const std::vector<std::shared_ptr<Stop>>& GetStops() const {
        return stops;
    }

    const std::vector<std::shared_ptr<Route>>& GetRoutes() const {
        return routes;
    }
};
