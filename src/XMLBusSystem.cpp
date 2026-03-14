#include "XMLBusSystem.h"
#include "XMLEntity.h"
#include <memory>
#include <string>
#include <iostream>

struct CXMLBusSystem::SImplementation {
    // Stop and Route internal structures
    struct SStop : public CBusSystem::SStop {
        TStopID DID;
        CStreetMap::TNodeID DNodeID;
        std::string DDescription;

        SStop(TStopID id, CStreetMap::TNodeID nodeId, const std::string &desc)
            : DID(id), DNodeID(nodeId), DDescription(desc) {}

        TStopID ID() const noexcept override { return DID; }
        CStreetMap::TNodeID NodeID() const noexcept override { return DNodeID; }
        std::string Description() const noexcept override { return DDescription; }
        std::string Description(const std::string &desc) noexcept override { DDescription = desc; return DDescription; }
    };

    struct SRoute : public CBusSystem::SRoute {
        std::string DName;
        std::vector<TStopID> DStopIDs;

        std::string Name() const noexcept override { return DName; }
        const std::vector<TStopID>& StopIDs() const noexcept { return DStopIDs; }
    };

    std::vector<std::shared_ptr<SStop>> StopsByIndex;
    std::unordered_map<TStopID, std::shared_ptr<SStop>> StopsByID;

    std::vector<std::shared_ptr<SRoute>> RoutesByIndex;
    std::unordered_map<std::string, std::shared_ptr<SRoute>> RoutesByName;

    // -------------------------------
    // Parsing helpers
    // -------------------------------
    bool ReadUntilStartTag(std::shared_ptr<CXMLReader> reader, const std::string &tag) {
        SXMLEntity entity;
        while (reader->ReadEntity(entity)) {
            if (entity.DType == SXMLEntity::EType::StartElement && entity.DNameData == tag) return true;
        }
        return false;
    }

    bool ReadUntilEndTag(std::shared_ptr<CXMLReader> reader, const std::string &tag) {
        SXMLEntity entity;
        while (reader->ReadEntity(entity)) {
            if (entity.DType == SXMLEntity::EType::EndElement && entity.DNameData == tag) return true;
        }
        return false;
    }

    void ParseStop(std::shared_ptr<CXMLReader> reader, const SXMLEntity &entity) {
        TStopID stopId = std::stoull(entity.AttributeValue("id"));
        CStreetMap::TNodeID nodeId = std::stoull(entity.AttributeValue("node"));
        std::string desc = entity.AttributeValue("description");

        auto stop = std::make_shared<SStop>(stopId, nodeId, desc);
        StopsByIndex.push_back(stop);
        StopsByID[stopId] = stop;

        ReadUntilEndTag(reader, "stop");
    }

    void ParseStops(std::shared_ptr<CXMLReader> reader) {
        SXMLEntity entity;
        while (reader->ReadEntity(entity)) {
            if (entity.DType == SXMLEntity::EType::StartElement && entity.DNameData == "stop") {
                ParseStop(reader, entity);
            } else if (entity.DType == SXMLEntity::EType::EndElement && entity.DNameData == "stops") {
                break;
            }
        }
    }

    void ParseRoute(std::shared_ptr<CXMLReader> reader, const SXMLEntity &entity) {
        auto route = std::make_shared<SRoute>();
        route->DName = entity.AttributeValue("name");

        SXMLEntity child;
        while (reader->ReadEntity(child)) {
            if (child.DType == SXMLEntity::EType::StartElement && child.DNameData == "stop") {
                route->DStopIDs.push_back(std::stoull(child.AttributeValue("ref")));
            } else if (child.DType == SXMLEntity::EType::EndElement && child.DNameData == "route") {
                break;
            }
        }

        RoutesByIndex.push_back(route);
        RoutesByName[route->DName] = route;
    }

    void ParseRoutes(std::shared_ptr<CXMLReader> reader) {
        SXMLEntity entity;
        while (reader->ReadEntity(entity)) {
            if (entity.DType == SXMLEntity::EType::StartElement && entity.DNameData == "route") {
                ParseRoute(reader, entity);
            } else if (entity.DType == SXMLEntity::EType::EndElement && entity.DNameData == "routes") {
                break;
            }
        }
    }

    void ParseBusSystem(std::shared_ptr<CXMLReader> reader) {
        if (!ReadUntilStartTag(reader, "bussystem")) return;
        if (!ReadUntilStartTag(reader, "stops")) return;
        ParseStops(reader);

        if (!ReadUntilStartTag(reader, "routes")) return;
        ParseRoutes(reader);
    }

    // -------------------------------
    // Constructor
    // -------------------------------
    SImplementation(std::shared_ptr<CXMLReader> systemSource) {
        ParseBusSystem(systemSource);
    }

    // -------------------------------
    // Accessors
    // -------------------------------
    std::size_t StopCount() const noexcept { return StopsByIndex.size(); }
    std::size_t RouteCount() const noexcept { return RoutesByIndex.size(); }

    std::shared_ptr<SStop> StopByIndex(std::size_t idx) const noexcept {
        if (idx < StopsByIndex.size()) return StopsByIndex[idx];
        return nullptr;
    }

    std::shared_ptr<SStop> StopByID(TStopID id) const noexcept {
        auto it = StopsByID.find(id);
        if (it != StopsByID.end()) return it->second;
        return nullptr;
    }

    std::shared_ptr<SRoute> RouteByIndex(std::size_t idx) const noexcept {
        if (idx < RoutesByIndex.size()) return RoutesByIndex[idx];
        return nullptr;
    }

    std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept {
        auto it = RoutesByName.find(name);
        if (it != RoutesByName.end()) return it->second;
        return nullptr;
    }

    std::shared_ptr<SPath> PathByStopIDs(TStopID start, TStopID end) const noexcept {
        // Simple placeholder for paths
        return nullptr;
    }
};

// -------------------------------
// CXMLBusSystem public API
// -------------------------------
CXMLBusSystem::CXMLBusSystem(std::shared_ptr<CXMLReader> systemSource, std::shared_ptr<CXMLReader> pathSource) {
    DImplementation = std::make_unique<SImplementation>(systemSource);
}

CXMLBusSystem::~CXMLBusSystem() = default;

std::size_t CXMLBusSystem::StopCount() const noexcept { return DImplementation->StopCount(); }
std::size_t CXMLBusSystem::RouteCount() const noexcept { return DImplementation->RouteCount(); }
std::shared_ptr<CBusSystem::SStop> CXMLBusSystem::StopByIndex(std::size_t idx) const noexcept { return DImplementation->StopByIndex(idx); }
std::shared_ptr<CBusSystem::SStop> CXMLBusSystem::StopByID(TStopID id) const noexcept { return DImplementation->StopByID(id); }
std::shared_ptr<CBusSystem::SRoute> CXMLBusSystem::RouteByIndex(std::size_t idx) const noexcept { return DImplementation->RouteByIndex(idx); }
std::shared_ptr<CBusSystem::SRoute> CXMLBusSystem::RouteByName(const std::string &name) const noexcept { return DImplementation->RouteByName(name); }
std::shared_ptr<CBusSystem::SPath> CXMLBusSystem::PathByStopIDs(TStopID start, TStopID end) const noexcept { return DImplementation->PathByStopIDs(start, end); }
