#pragma once
#include <memory>
#include <vector>
#include "Route.h"
#include "Stop.h"
#include "BusSystem.h" // <-- include BusSystem

using TStopID = size_t;
using TStopTime = int;

class TripPlanner {
public:
    TripPlanner(std::shared_ptr<BusSystem> bs);

    std::shared_ptr<Route> FindDirectRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat) const;
    std::shared_ptr<Route> FindDirectRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby) const;

    bool FindRouteLeaveTime(TStopID src, TStopID dest, TStopTime leaveat, TravelPlan &plan) const;
    bool FindRouteArrivalTime(TStopID src, TStopID dest, TStopTime arriveby, TravelPlan &plan) const;

private:
    std::shared_ptr<BusSystem> busSystem;
};