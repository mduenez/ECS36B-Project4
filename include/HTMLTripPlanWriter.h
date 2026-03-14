#pragma once
#include <string>
#include <memory>
#include "TravelPlan.h"

// --------------------
// HTMLTripPlanWriter
// --------------------
class HTMLTripPlanWriter {
public:
    // Construct with output filename
    explicit HTMLTripPlanWriter(const std::string &filename);

    // Write the travel plan to the HTML file
    void Write(const TravelPlan &plan);

private:
    std::string m_filename;
};