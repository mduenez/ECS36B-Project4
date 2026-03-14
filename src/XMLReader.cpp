#include "XMLReader.h"
#include <expat.h>
#include <queue>
#include <cstring>

struct CXMLReader::SImplementation{
    std::shared_ptr<CDataSource> DSource;
    XML_Parser DParser;
    std::queue<SXMLEntity> DQueue;
    bool DEnd = false;

    SImplementation(std::shared_ptr<CDataSource> src)
        : DSource(src){

        DParser = XML_ParserCreate(nullptr);
        XML_SetUserData(DParser, this);
        XML_SetElementHandler(DParser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(DParser, CharacterDataHandler);
    }

    ~SImplementation(){
        XML_ParserFree(DParser);
    }

    static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts){

        auto impl = static_cast<SImplementation*>(userData);

        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::StartElement;
        entity.DNameData = name;

        for(int i = 0; atts[i]; i += 2){
            entity.SetAttribute(atts[i], atts[i+1]);
        }

        impl->DQueue.push(entity);
    }

    static void EndElementHandler(void *userData, const XML_Char *name){

        auto impl = static_cast<SImplementation*>(userData);

        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::EndElement;
        entity.DNameData = name;

        impl->DQueue.push(entity);
    }

    static void CharacterDataHandler(void *userData, const XML_Char *s, int len){

        auto impl = static_cast<SImplementation*>(userData);

        std::string data(s, len);

        if(data.find_first_not_of(" \t\n\r") == std::string::npos){
            return;
        }

        SXMLEntity entity;
        entity.DType = SXMLEntity::EType::CharData;
        entity.DNameData = data;

        impl->DQueue.push(entity);
    }
};


CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){

    DImplementation = std::make_unique< SImplementation >(src);

}

CXMLReader::~CXMLReader() = default;

bool CXMLReader::End() const{
    return DImplementation->DEnd &&
           DImplementation->DQueue.empty();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){

    auto &impl = *DImplementation;

    while(impl.DQueue.empty() && !impl.DEnd){

        std::vector<char> buffer(4096);

        if(!impl.DSource->Read(buffer, buffer.size())){
            XML_Parse(impl.DParser, nullptr, 0, true);
            impl.DEnd = true;
            break;
        }

        if(XML_Parse(impl.DParser,
                     buffer.data(),
                     buffer.size(),
                     false) == XML_STATUS_ERROR){
            impl.DEnd = true;
            return false;
        }
    }

    while(!impl.DQueue.empty()){
        entity = impl.DQueue.front();
        impl.DQueue.pop();

        if(skipcdata && entity.DType == SXMLEntity::EType::CharData){
            continue;
        }

        return true;
    }

    return false;
}
