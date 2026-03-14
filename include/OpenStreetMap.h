#ifndef OPENSTREETMAP_H
#define OPENSTREETMAP_H

#include "StreetMap.h"
#include <vector>
#include <memory>

class OpenStreetMap : public StreetMap {
public:
    OpenStreetMap() = default;
    ~OpenStreetMap() override = default;

    const std::vector<std::shared_ptr<Node>>& GetNodes() const override {
        return nodes;
    }

    const std::vector<std::shared_ptr<Way>>& GetWays() const override {
        return ways;
    }

    // Functions to add nodes and ways
    void AddNode(std::shared_ptr<Node> node) { nodes.push_back(node); }
    void AddWay(std::shared_ptr<Way> way) { ways.push_back(way); }

private:
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Way>> ways;
};

#endif // OPENSTREETMAP_H