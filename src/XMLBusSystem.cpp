#include "XMLBusSystem.h"

struct CXMLBusSystem::SImplementation {
    int test;
};

CXMLBusSystem::CXMLBusSystem(std::shared_ptr<CXMLReader> systemsource,
                             std::shared_ptr<CXMLReader> pathsource)
{
    DImplementation = std::make_unique<SImplementation>();
}

CXMLBusSystem::~CXMLBusSystem() = default;

// Stub implementations so linker is happy
std::size_t CXMLBusSystem::StopCount() const noexcept {
    return 0;
}

std::size_t CXMLBusSystem::RouteCount() const noexcept {
    return 0;
}

std::shared_ptr<CXMLBusSystem::SStop>
CXMLBusSystem::StopByIndex(std::size_t) const noexcept {
    return nullptr;
}

std::shared_ptr<CXMLBusSystem::SStop>
CXMLBusSystem::StopByID(TStopID) const noexcept {
    return nullptr;
}

std::shared_ptr<CXMLBusSystem::SRoute>
CXMLBusSystem::RouteByIndex(std::size_t) const noexcept {
    return nullptr;
}

std::shared_ptr<CXMLBusSystem::SRoute>
CXMLBusSystem::RouteByName(const std::string &) const noexcept {
    return nullptr;
}

std::shared_ptr<CXMLBusSystem::SPath>
CXMLBusSystem::PathByStopIDs(TStopID, TStopID) const noexcept {
    return nullptr;
}