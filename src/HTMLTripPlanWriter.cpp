#include "HTMLTripPlanWriter.h"
#include <fstream>
#include <string>
#include <iostream>

HTMLTripPlanWriter::HTMLTripPlanWriter(const std::string& filename)
    : m_filename(filename) {}

void HTMLTripPlanWriter::Write(const TravelPlan& plan) {
    std::ofstream file(m_filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << m_filename << std::endl;
        return;
    }

    file << "<!DOCTYPE html>\n<html>\n<head>\n<title>Trip Plan</title>\n</head>\n<body>\n";
    file << "<h1>Trip Plan</h1>\n";

    file << "<h2>Stops:</h2>\n<ul>\n";
    for (auto& stop : plan.stops) {
        file << "<li>" << stop->name
             << " (" << stop->latitude
             << ", " << stop->longitude << ")</li>\n";
    }
    file << "</ul>\n";

    file << "<h2>Routes:</h2>\n<ul>\n";
    for (auto& route : plan.routes) {
        file << "<li>" << route->name << "</li>\n";
    }
    file << "</ul>\n";

    file << "</body>\n</html>\n";
    file.close();
}
