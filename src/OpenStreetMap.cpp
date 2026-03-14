#include "OpenStreetMap.h"
#include <memory>
#include <iostream>

OpenStreetMap::OpenStreetMap() = default;
OpenStreetMap::~OpenStreetMap() = default;

// Stub implementations to satisfy compiler
void OpenStreetMap::ParseOSM() {
    std::cout << "Parsing OSM data (stub)\n";
}

size_t OpenStreetMap::NodeCount() const noexcept {
    return nodes.size();
}

size_t OpenStreetMap::WayCount() const noexcept {
    return ways.size();
}

std::shared_ptr<Node> OpenStreetMap::NodeByIndex(size_t index) const noexcept {
    if (index >= nodes.size()) return nullptr;
    return nodes[index];
}

std::shared_ptr<Node> OpenStreetMap::NodeByID(size_t id) const noexcept {
    for (auto &n : nodes) if (n->id == id) return n;
    return nullptr;
}

std::shared_ptr<Way> OpenStreetMap::WayByIndex(size_t index) const noexcept {
    if (index >= ways.size()) return nullptr;
    return ways[index];
}

std::shared_ptr<Way> OpenStreetMap::WayByID(size_t id) const noexcept {
    for (auto &w : ways) if (w->id == id) return w;
    return nullptr;
}

