#pragma once
#include <vector>
#include <memory>
#include "Stop.h"
#include "Route.h"

// Time type for stops (e.g., seconds since midnight or minutes)
using StopTime = size_t;

// Travel plan structure
struct TravelPlan {
    std::vector<std::shared_ptr<Stop>> stops;   // stops in this plan
    std::vector<std::shared_ptr<Route>> routes; // routes in this plan

    StopTime leaveTime = 0;  // optional: departure time from first stop
    StopTime arriveTime = 0; // optional: arrival time at last stop
    size_t start = 0;        // first stop ID
    size_t end = 0;          // last stop ID
};