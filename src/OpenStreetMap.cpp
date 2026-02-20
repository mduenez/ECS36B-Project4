#include "OpenStreetMap.h"
#include <unordered_map>

struct COpenStreetMap::SImplementation{
    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";
    const std::string DWayTag = "way";


    struct SNode: public CStreetMap::SNode{
        const std::string DNodeIDAttr = "id";
        const std::string DNodeLatAttr = "lat";
        const std::string DNodeLonAttr = "lon";
        TNodeID DID;
        SLocation DLocation;
        std::unordered_map<std::string,std::string> DAttributes;
        std::vector<std::string> DAttributeKeys;

        SNode(const SXMLEntity &entity){
            auto NodeID = std::stoull(entity.AttributeValue(DNodeIDAttr));
            auto NodeLat = std::stod(entity.AttributeValue(DNodeLatAttr));
            auto NodeLon = std::stod(entity.AttributeValue(DNodeLonAttr));
            DID = NodeID;
            DLocation = SLocation{NodeLat,NodeLon};
        }
        ~SNode(){

        }

        TNodeID ID() const noexcept override{
            return DID;
        }
        
        SLocation Location() const noexcept override{
            return DLocation;
        }
        
        std::size_t AttributeCount() const noexcept override{
            return DAttributes.size();
        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if(index < DAttributeKeys.size()){
                return DAttributeKeys[index];
            }
            return "";
        }
        
        bool HasAttribute(const std::string &key) const noexcept override{
            return DAttributes.find(key) != DAttributes.end();
        }
        
        std::string GetAttribute(const std::string &key) const noexcept override{
            auto it = DAttributes.find(key);
            if(it != DAttributes.end()){
                return it->second;
            }
            return "";
        }
        
    };

    struct SWay: public CStreetMap::SWay{
        TWayID DID;
        std::vector<TNodeID> DNodeIDs;
        std::unordered_map<std::string,std::string> DAttributes;
        std::vector<std::string> DAttributeKeys;

        ~SWay(){

        }

        TWayID ID() const noexcept override{
            return DID;
        }
        
        std::size_t NodeCount() const noexcept override{
            return DNodeIDs.size();
        }
        
        TNodeID GetNodeID(std::size_t index) const noexcept override{
            if(index < DNodeIDs.size()){
                return DNodeIDs[index];
            }
            return CstreetMap::InvalidNodeID;
        }
        
        std::size_t AttributeCount() const noexcept override{
            return DAttributes.size();
        }
        
        std::string GetAttributeKey(std::size_t index) const noexcept override{
            if(index < DAttributeKeys.size()){
                return DAttributeKeys[index];
            }
            return "";
        }
        
        bool HasAttribute(const std::string &key) const noexcept override{
            return DAttributes.find(key) != DAttributes.end();
        }
        
        std::string GetAttribute(const std::string &key) const noexcept override{
            auto it = DAttributes.find(key);
            if(it != DAttributes.end()){
                return it->second;
            }
            return "";
        }
        
    };

    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID,std::shared_ptr<SNode>> DNodesByID;

    bool FindStartTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::StartElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    bool FindEndTag(std::shared_ptr< CXMLReader > xmlsource, const std::string &starttag){
        SXMLEntity TempEntity;
        while(xmlsource->ReadEntity(TempEntity,true)){
            if((TempEntity.DType == SXMLEntity::EType::EndElement)&&(TempEntity.DNameData == starttag)){
                return true;
            }
        }
        return false;
    }

    bool ParseOSM(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;
        if(!FindStartTag(src,DOSMTag)){
            return false;
        }
        while(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){//if node found create new node and add to index and id maps
                auto NewNode = std::make_shared<SNode>(TempEntity);
                DNodesByIndex.push_back(NewNode);
                DNodesByID[NewNode->ID()] = NewNode;
                FindEndTag(src,DNodeTag);
            }
            else if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == "tag"){
                auto Key = TempEntity.AttributeValue("k");
                auto Value = TempEntity.AttributeValue("v");
                CurrentNode->DAttributes[Key] = Value;
                CurrentNode->DAttributeKeys.push_back(Key);
            }
            else if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DWayTag){
                auto Way = std::make_shared<SWay>(std::stoull(TempEntity.AttributeValue("id")));
                if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == "nd"){
                    auto ref = std::stoull(TempEntity.AttributeValue("ref"));
                    Way->DNodeIDs.push_back(ref);
                }
                else if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == "tag"){
                    auto Key = TempEntity.AttributeValue("k");
                    auto Value = TempEntity.AttributeValue("v");
                    Way->DAttributes[Key] = Value;
                    Way->DAttributeKeys.push_back(Key);
                }
                DWaysByIndex.push_back(Way);
                DWaysByID[Way->ID()] = Way;
            }
            else if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DOSMTag){
                break;
            }
        }



    }

    SImplementation(std::shared_ptr<CXMLReader> src){
        ParseOSM(src);
    }

    std::size_t NodeCount() const noexcept{
        return DNodesByIndex.size();
    }

    std::size_t WayCount() const noexcept{
        return 0;
    }

    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        if(index < DNodesByIndex.size()){
            return DNodesByIndex[index];
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        auto Search = DNodesByID.find(id);
        if(Search != DNodesByID.end()){
            return Search->second;
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        if (index < DNodesByIndex.size()){
            return DNodesByIndex[index];
        }
        return nullptr;
    }

    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        
    }

};


COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){
    //unique_ptr will clean up automtically
}

std::size_t COpenStreetMap::NodeCount() const noexcept{
    return DImplementation->NodeCount();
}

std::size_t COpenStreetMap::WayCount() const noexcept{
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept{
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept{
    return DImplementation->NodeByID(id);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept{
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept{
    return DImplementation->WayByID(id);
}