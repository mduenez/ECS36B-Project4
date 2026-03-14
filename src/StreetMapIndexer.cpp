#include "StreetMapIndexer.h"
#include <algorithm>

StreetMapIndexer::StreetMapIndexer(std::shared_ptr<StreetMap> sm) : streetMap(sm) {
    // Copy nodes
    for (auto &node : streetMap->GetNodes()) {
        nodes.push_back(node);
    }
    std::sort(nodes.begin(), nodes.end(), [](auto a, auto b) { return a->id < b->id; });

    // Copy ways and build nodeToWays map
    for (auto &way : streetMap->GetWays()) {
        ways.push_back(way);
        for (auto nid : way->NodeIDs()) {
            nodeToWays[nid].insert(way);
        }
    }
    std::sort(ways.begin(), ways.end(), [](auto a, auto b) { return a->id < b->id; });
}

size_t StreetMapIndexer::NodeCount() const noexcept {
    return nodes.size();
}

size_t StreetMapIndexer::WayCount() const noexcept {
    return ways.size();
}

std::shared_ptr<StreetMap::Node> StreetMapIndexer::SortedNodeByIndex(size_t index) const noexcept {
    if (index >= nodes.size()) return nullptr;
    return nodes[index];
}

std::shared_ptr<StreetMap::Way> StreetMapIndexer::SortedWayByIndex(size_t index) const noexcept {
    if (index >= ways.size()) return nullptr;
    return ways[index];
}

std::unordered_set<std::shared_ptr<StreetMap::Way>> StreetMapIndexer::WaysInRange(
    const StreetMap::Location &lowerLeft,
    const StreetMap::Location &upperRight
) const noexcept {
    std::unordered_set<std::shared_ptr<StreetMap::Way>> result;
    for (auto &way : ways) {
        if (way->IsInRange(lowerLeft, upperRight)) {
            result.insert(way);
        }
    }
    return result;
}

std::unordered_set<std::shared_ptr<StreetMap::Way>> StreetMapIndexer::WaysByNodeID(
    StreetMap::NodeID nodeID
) const noexcept {
    auto it = nodeToWays.find(nodeID);
    if (it != nodeToWays.end()) {
        return it->second;
    }
    return {};
}