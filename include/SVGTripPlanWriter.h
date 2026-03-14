#ifndef SVGTRIPPLANWRITER_H
#define SVGTRIPPLANWRITER_H

#include "SVGWriter.h"
#include "TravelPlan.h"
#include "DataSink.h"
#include <memory>
#include <vector>

class SVGTripPlanWriter {
public:
    SVGTripPlanWriter() = default;
    ~SVGTripPlanWriter() = default;

    std::vector<SVGPoint> PlanToPoints(const TravelPlan &plan);

    bool WritePlan(std::shared_ptr<DataSink> sink, const TravelPlan &plan);

private:
    // You can store settings or colors if needed
};

#endif // SVGTRIPPLANWRITER_H