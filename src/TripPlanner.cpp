#include "TripPlanner.h"        // own header
#include "BusSystemIndexer.h"   // to access stops and routes
#include "RouteIndexer.h"
#include "BusSystem.h"
#include <memory>
#include <vector>
#include <string>
#include <algorithm>

TripPlanner::TripPlanner(std::shared_ptr<BusSystem> bs) : busSystem(bs) {}
TripPlanner::~TripPlanner() {}

std::shared_ptr<Route> TripPlanner::FindDirectRouteLeaveTime(TStopID src, TStopID dest, StopTime leaveAt) const {
    std::shared_ptr<Route> bestRoute = nullptr;
    StopTime earliestArrival = std::numeric_limits<StopTime>::max();

    for (auto &route : busSystem->GetRoutes()) {
        auto stopIds = route->GetStopIDsBetween(src, dest);
        if (!stopIds.empty()) {
            StopTime arrival = route->GetArrivalTime(src, leaveAt);
            if (arrival < earliestArrival) {
                earliestArrival = arrival;
                bestRoute = route;
            }
        }
    }
    return bestRoute;
}

std::shared_ptr<Route> TripPlanner::FindDirectRouteArrivalTime(TStopID src, TStopID dest, StopTime arriveBy) const {
    std::shared_ptr<Route> bestRoute = nullptr;
    StopTime latestDeparture = 0;

    for (auto &route : busSystem->GetRoutes()) {
        auto stopIds = route->GetStopIDsBetween(src, dest);
        if (!stopIds.empty()) {
            StopTime departure = route->GetDepartureTime(dest, arriveBy);
            if (departure > latestDeparture) {
                latestDeparture = departure;
                bestRoute = route;
            }
        }
    }
    return bestRoute;
}

bool TripPlanner::FindRouteLeaveTime(TStopID src, TStopID dest, StopTime leaveAt, TravelPlan &plan) const {
    auto route = FindDirectRouteLeaveTime(src, dest, leaveAt);
    if (!route) return false;
    plan.routes.push_back(route);
    plan.start = src;
    plan.end = dest;
    plan.leaveTime = leaveAt;
    plan.arriveTime = route->GetArrivalTime(src, leaveAt);
    return true;
}

bool TripPlanner::FindRouteArrivalTime(TStopID src, TStopID dest, StopTime arriveBy, TravelPlan &plan) const {
    auto route = FindDirectRouteArrivalTime(src, dest, arriveBy);
    if (!route) return false;
    plan.routes.push_back(route);
    plan.start = src;
    plan.end = dest;
    plan.arriveTime = arriveBy;
    plan.leaveTime = route->GetDepartureTime(dest, arriveBy);
    return true;
}
