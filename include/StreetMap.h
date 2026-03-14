#ifndef STREETMAP_H
#define STREETMAP_H

#include <vector>
#include <memory>

class StreetMap {
public:
    using NodeID = long;
    using WayID = long;

    struct Location {
        double lat;
        double lon;
    };

    struct Node {
        NodeID id;
        Location loc;
    };

    struct Way {
        WayID id;
        std::vector<NodeID> nodeIDs;

        // Checks if any part of the way is in the bounding box
        bool IsInRange(const Location &lowerLeft, const Location &upperRight) const {
            // Simple check: does any node fall inside the box?
            // In practice, you would need node lookup here
            return true; // Placeholder: implement proper spatial check
        }

        const std::vector<NodeID>& NodeIDs() const { return nodeIDs; }
    };

    virtual ~StreetMap() = default;

    virtual const std::vector<std::shared_ptr<Node>>& GetNodes() const = 0;
    virtual const std::vector<std::shared_ptr<Way>>& GetWays() const = 0;
};

#endif // STREETMAP_H