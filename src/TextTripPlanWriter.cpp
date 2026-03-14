#include "TextTripPlanWriter.h"
#include "SVGTripPlanWriter.h"
#include "HTMLTripPlanWriter.h"
#include "TripPlanner.h"
#include <memory>
#include <string>
#include <sstream>
#include <vector>

// --------------------
// TextTripPlanWriter
// --------------------
bool TextTripPlanWriter::WritePlan(std::shared_ptr<DataSink> sink, const TravelPlan &plan) {
    std::ostringstream out;
    out << "Trip Plan:\n";
    out << "From stop " << plan.start << " to stop " << plan.end << "\n";
    out << "Leave at: " << plan.leaveTime << ", arrive at: " << plan.arriveTime << "\n";

    for (auto &route : plan.routes) {
        if (!route) continue;
        out << "Route: " << route->name << "\n";
        out << "Stops: ";
        auto stopIDs = route->GetStopIDsBetween(plan.start, plan.end);
        for (size_t i = 0; i < stopIDs.size(); ++i) {
            out << stopIDs[i];
            if (i + 1 < stopIDs.size()) out << " -> ";
        }
        out << "\n";
    }

    sink->Write(out.str());
    return true;
}

// --------------------
// SVGTripPlanWriter
// --------------------
bool SVGTripPlanWriter::WritePlan(std::shared_ptr<DataSink> sink, const TravelPlan &plan) {
    double y = 0.0;
    double spacing = 20.0;

    for (auto &route : plan.routes) {
        if (!route) continue;
        double x = 0.0;
        auto stopIDs = route->GetStopIDsBetween(plan.start, plan.end);
        for (auto stopId : stopIDs) {
            svgWriter.Text({x, y}, "Stop " + std::to_string(stopId), {});
            x += spacing;
        }
        y += spacing; // next route below
    }

    return true;
}

// --------------------
// HTMLTripPlanWriter
// --------------------
bool HTMLTripPlanWriter::WritePlan(std::shared_ptr<DataSink> sink, const TravelPlan &plan) {
    std::ostringstream html;
    html << "<!DOCTYPE html>\n<html>\n<head>\n<title>Trip Plan</title>\n</head>\n<body>\n";
    html << "<h1>Trip Plan</h1>\n";
    html << "<p>From stop " << plan.start << " to stop " << plan.end << "</p>\n";
    html << "<p>Leave at: " << plan.leaveTime << ", arrive at: " << plan.arriveTime << "</p>\n";

    html << "<h2>Routes:</h2>\n<ul>\n";
    for (auto &route : plan.routes) {
        if (!route) continue;
        html << "<li>" << route->name << ": ";
        auto stopIDs = route->GetStopIDsBetween(plan.start, plan.end);
        for (size_t i = 0; i < stopIDs.size(); ++i) {
            html << stopIDs[i];
            if (i + 1 < stopIDs.size()) html << " -> ";
        }
        html << "</li>\n";
    }
    html << "</ul>\n</body>\n</html>\n";

    sink->Write(html.str());
    return true;
}