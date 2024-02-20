#include "XMLReader.h"
#include <expat.h>
#include <queue>
#include<iostream>
struct CXMLReader::SImplementation{
    std::shared_ptr<CDataSource> DDataSource;
    XML_Parser DXMLParser;
    std::queue<SXMLEntity> DEntityQueue;


    void StartElementHandler(const std::string &name,const std::vector<std::string> &atts){
        SXMLEntity TempEntity;
        TempEntity.DNameData = name;
        TempEntity.DType = SXMLEntity::EType::StartElement;
        for(size_t Index = 0; Index < atts.size(); Index += 2){
            TempEntity.SetAttribute(atts[Index],atts[Index+1]);
        }
        DEntityQueue.push(TempEntity);
    };
        
    void EndElementHandler(const std::string &name){
        SXMLEntity TempEntity;
        TempEntity.DNameData = name;
        TempEntity.DType = SXMLEntity::EType::EndElement;
        DEntityQueue.push(TempEntity);
    };
    
    void CharacterDataHandler(const std::string &cdata){
        SXMLEntity TempEntity;
        TempEntity.DNameData = cdata;
        TempEntity.DType = SXMLEntity::EType::CharData;
        DEntityQueue.push(TempEntity);
    };

    static void StartElementHandlerCallBack(void *context, const XML_Char *name, const XML_Char **atts){
        SImplementation *ReaderObject = static_cast<SImplementation *> (context);
        std::vector<std::string> Attributes;
        auto AttrPtr = atts;
        while(*AttrPtr){
            Attributes.push_back(*AttrPtr);
            AttrPtr++;
        }
        ReaderObject->StartElementHandler(name,Attributes); 
     };    
    
    static void EndElementHandlerCallBack(void *context, const XML_Char *name){
        SImplementation *ReaderObject = static_cast<SImplementation *> (context);
        ReaderObject->EndElementHandler(name); 
    };    

    static void CharacterDataHandlerCallback(void *context,const XML_Char *s,int len){
        SImplementation *ReaderObject = static_cast<SImplementation *> (context);
        ReaderObject->CharacterDataHandler(std::string(s,len));
    };

    SImplementation(std::shared_ptr< CDataSource > src){
        DDataSource=src;
        DXMLParser = XML_ParserCreate(NULL);
        XML_SetStartElementHandler(DXMLParser,StartElementHandlerCallBack);
        XML_SetEndElementHandler(DXMLParser,EndElementHandlerCallBack);
        XML_SetCharacterDataHandler(DXMLParser,CharacterDataHandlerCallback);
        XML_SetUserData(DXMLParser,this);
    };

    bool End() const{
        return DDataSource->End();
    };
    
    // Returns true if the entity is successfully read if skipcdata
    // is true only element type entities will be returned
    bool ReadEntity(SXMLEntity &entity, bool skipcdata){
        //Reader from source if necessary
        //pass to XML_Parse function
        //return entity

        std::vector<char> DataBuffer;
        while(DEntityQueue.empty()){
            if(DDataSource->Read(DataBuffer,256)){
                XML_Parse(DXMLParser,DataBuffer.data(),DataBuffer.size(),DataBuffer.size()<256);
            }
            else{
                XML_Parse(DXMLParser, nullptr, 0, true);
                break;
            }
        }        
        if(DEntityQueue.empty()&&DDataSource->End()){
            return false;
        }
        
        entity = DEntityQueue.front();
        if(skipcdata){
            while (entity.DType==SXMLEntity::EType::CharData)
            {
                 DEntityQueue.pop();
                 entity = DEntityQueue.front();
            }
        }        
        DEntityQueue.pop();
        return true;
 
    }

};

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique< SImplementation >( src );
};

CXMLReader::~CXMLReader(){

};
        
bool CXMLReader::End() const{
    return DImplementation->End();
};

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
    return DImplementation->ReadEntity(entity,skipcdata);
};
