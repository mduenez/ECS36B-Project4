#include "OpenStreetMap.h"

// Full definition of SImplementation (pImpl pattern)
struct COpenStreetMap::SImplementation {
    int Dummy;
};

COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src)
{
    DImplementation = std::make_unique<SImplementation>();
}

COpenStreetMap::~COpenStreetMap() = default;

// Stub implementations

std::size_t COpenStreetMap::NodeCount() const noexcept {
    return 0;
}

std::size_t COpenStreetMap::WayCount() const noexcept {
    return 0;
}

std::shared_ptr<COpenStreetMap::SNode>
COpenStreetMap::NodeByIndex(std::size_t) const noexcept {
    return nullptr;
}

std::shared_ptr<COpenStreetMap::SNode>
COpenStreetMap::NodeByID(TNodeID) const noexcept {
    return nullptr;
}

std::shared_ptr<COpenStreetMap::SWay>
COpenStreetMap::WayByIndex(std::size_t) const noexcept {
    return nullptr;
}

std::shared_ptr<COpenStreetMap::SWay>
COpenStreetMap::WayByID(TWayID) const noexcept {
    return nullptr;
}
