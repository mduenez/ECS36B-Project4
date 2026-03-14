#include "SVGTripPlanWriter.h"
#include <cmath>
#include <iostream>

// Helper function to map latitude/longitude to SVG coordinates
static SVGPoint LatLonToPoint(double lat, double lon, double minLat, double maxLat,
                              double minLon, double maxLon, double width, double height) {
    double x = (lon - minLon) / (maxLon - minLon) * width;
    double y = height - (lat - minLat) / (maxLat - minLat) * height;
    return {x, y};
}

// Convert a TravelPlan to a list of SVG points
std::vector<SVGPoint> SVGTripPlanWriter::PlanToPoints(const TravelPlan &plan) {
    std::vector<SVGPoint> points;
    double minLat = plan.minLat, maxLat = plan.maxLat;
    double minLon = plan.minLon, maxLon = plan.maxLon;
    double width = 800, height = 600;  // SVG canvas size

    for (const auto &stop : plan.stops) {
        points.push_back(LatLonToPoint(stop->lat, stop->lon, minLat, maxLat, minLon, maxLon, width, height));
    }

    return points;
}

// Write the travel plan to a DataSink in SVG format
bool SVGTripPlanWriter::WritePlan(std::shared_ptr<DataSink> sink, const TravelPlan &plan) {
    std::vector<SVGPoint> points = PlanToPoints(plan);

    for (size_t i = 0; i < points.size(); ++i) {
        std::cout << "Stop " << i << " at (" << points[i].x << "," << points[i].y << ")\n";
    }

    // Example: using CSVGWriter
    CSVGWriter writer;
    for (const auto &pt : points) {
        writer.Circle(pt, 5, {{"fill", "red"}});
    }

    return true;
}