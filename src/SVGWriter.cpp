#include "SVGWriter.h"
#include <iostream>

class CSVGWriter : public SVGWriter {
public:
    CSVGWriter() = default;
    ~CSVGWriter() override = default;

    bool Circle(const SVGPoint &center, TSVGReal radius, const TAttributes &style) override {
        std::cout << "Circle at (" << center.x << "," << center.y << ") radius " << radius << "\n";
        return true;
    }

    bool Line(const SVGPoint &start, const SVGPoint &end, const TAttributes &style) override {
        std::cout << "Line from (" << start.x << "," << start.y << ") to (" << end.x << "," << end.y << ")\n";
        return true;
    }

    bool Text(const SVGPoint &pos, const std::string &text, const TAttributes &style) override {
        std::cout << "Text '" << text << "' at (" << pos.x << "," << pos.y << ")\n";
        return true;
    }

    bool Rect(const SVGPoint &topleft, const SVGPoint &size, const TAttributes &style) override {
        std::cout << "Rect at (" << topleft.x << "," << topleft.y << ") size (" << size.x << "," << size.y << ")\n";
        return true;
    }

    bool SimplePath(const std::vector<SVGPoint> &points, const TAttributes &style) override {
        std::cout << "Path with " << points.size() << " points\n";
        return true;
    }
};