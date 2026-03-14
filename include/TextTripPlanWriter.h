#ifndef TEXTTRIPPLANWRITER_H
#define TEXTTRIPPLANWRITER_H

#include <memory>
#include "DataSink.h"
#include "TravelPlan.h"

class TextTripPlanWriter {
public:
    TextTripPlanWriter() = default;
    ~TextTripPlanWriter() = default;

    // Writes the travel plan to the provided DataSink
    bool WritePlan(std::shared_ptr<DataSink> sink, const TravelPlan &plan);

private:
    // Optional: add formatting options here later
};

#endif