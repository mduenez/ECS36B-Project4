#pragma once
#include <string>

class Stop {
public:
    using StopID = long;

    StopID id;
    double latitude;
    double longitude;
    std::string name; // Optional stop name

    Stop(StopID id_, double lat_, double lon_, const std::string& name_ = "")
        : id(id_), latitude(lat_), longitude(lon_), name(name_) {}

    // Optional: convenience method to get coordinates as string
    std::string Coordinates() const {
        return "(" + std::to_string(latitude) + ", " + std::to_string(longitude) + ")";
    }
};