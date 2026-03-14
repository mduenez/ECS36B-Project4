#pragma once
#include <memory>
#include <string>
#include "TripPlanner.h"
#include "DataSink.h"
#include "TextTripPlanWriter.h"

class TripPlannerCommandLine {
public:
    TripPlannerCommandLine(std::shared_ptr<TripPlanner> planner,
                           std::shared_ptr<DataSink> sink);
    ~TripPlannerCommandLine();

    void ProcessCommands();

private:
    std::shared_ptr<TripPlanner> m_tripPlanner;
    std::shared_ptr<DataSink> m_dataSink;
    TextTripPlanWriter m_textWriter;  // for printing trip plans
};