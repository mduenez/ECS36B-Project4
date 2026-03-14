#ifndef SVGWRITER_H
#define SVGWRITER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

struct SVGPoint {
    double x;
    double y;
};

using TAttributes = std::unordered_map<std::string, std::string>;
using TSVGReal = double;

class SVGWriter {
public:
    SVGWriter() = default;
    virtual ~SVGWriter() = default;

    virtual bool Circle(const SVGPoint &center, TSVGReal radius, const TAttributes &style) = 0;
    virtual bool Line(const SVGPoint &start, const SVGPoint &end, const TAttributes &style) = 0;
    virtual bool Text(const SVGPoint &pos, const std::string &text, const TAttributes &style) = 0;
    virtual bool Rect(const SVGPoint &topleft, const SVGPoint &size, const TAttributes &style) = 0;
    virtual bool SimplePath(const std::vector<SVGPoint> &points, const TAttributes &style) = 0;
};

#endif // SVGWRITER_H