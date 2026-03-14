#pragma once
#include <vector>
#include <string>
#include <cstddef>
#include <algorithm>

using TStopID = size_t;

class Route {
public:
    std::string name;             // Needed by BusSystemIndexer
    std::vector<TStopID> stops;   // Stops on the route

    Route() = default;
    explicit Route(const std::string &routeName) : name(routeName) {}

    // Get stops between two IDs (inclusive)
    std::vector<TStopID> GetStopIDsBetween(TStopID start, TStopID end) const {
        std::vector<TStopID> result;
        bool started = false;
        for (TStopID id : stops) {
            if (id == start) started = true;
            if (started) result.push_back(id);
            if (id == end && started) break;
        }
        return result;
    }

    // Optional: check if a stop exists in this route
    bool HasStop(TStopID stopID) const {
        return std::find(stops.begin(), stops.end(), stopID) != stops.end();
    }
};