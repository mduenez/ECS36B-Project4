#ifndef STREETMAP_INDEXER_H
#define STREETMAP_INDEXER_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include "StreetMap.h"  // Make sure this header defines StreetMap, Node, Way, Location

class StreetMapIndexer {
public:
    explicit StreetMapIndexer(std::shared_ptr<StreetMap> sm);
    ~StreetMapIndexer() = default;

    size_t NodeCount() const noexcept;
    size_t WayCount() const noexcept;

    std::shared_ptr<StreetMap::Node> SortedNodeByIndex(size_t index) const noexcept;
    std::shared_ptr<StreetMap::Way> SortedWayByIndex(size_t index) const noexcept;

    std::unordered_set<std::shared_ptr<StreetMap::Way>> WaysInRange(
        const StreetMap::Location &lowerLeft,
        const StreetMap::Location &upperRight
    ) const noexcept;

    std::unordered_set<std::shared_ptr<StreetMap::Way>> WaysByNodeID(
        StreetMap::NodeID nodeID
    ) const noexcept;

private:
    std::shared_ptr<StreetMap> streetMap;
    std::vector<std::shared_ptr<StreetMap::Node>> nodes;
    std::vector<std::shared_ptr<StreetMap::Way>> ways;
    std::unordered_map<StreetMap::NodeID, std::unordered_set<std::shared_ptr<StreetMap::Way>>> nodeToWays;
};

#endif // STREETMAP_INDEXER_H