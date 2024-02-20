#include "OpenStreetMap.h"
#include <unordered_map>
#include <vector>

struct COpenStreetMap::SImplementation
{

    struct SNode : public CStreetMap::SNode
    {
        TNodeID DID;
        TLocation DLocation;
        std::vector<std::pair< std::string, std::string >> DAttributes;

        void addAttribute(std::string k,std::string v){
            DAttributes.push_back(std::make_pair(k,v));
        }

        SNode(CStreetMap::TNodeID id, double lat, double lon){
            DID = id;
            DLocation.first = lat;
            DLocation.second = lon;
        };

        ~SNode(){

        };
        TNodeID ID() const noexcept override
        {
            return DID;
        };
        TLocation Location() const noexcept override
        {
            return DLocation;
        };
        size_t AttributeCount() const noexcept override
        {
            return DAttributes.size();
        };
        std::string GetAttributeKey(size_t index) const noexcept override
        {
            if (index < DAttributes.size())
            {
                return DAttributes[index].first;
            }
            return "";
        };
        bool HasAttribute(const std::string &key) const noexcept override
        {
    	    for (const auto& Attr : DAttributes) {
                if (Attr.first == key) {
                return true;
                }
            }
            return false;
        };
        std::string GetAttribute(const std::string &key) const noexcept override
        {
    	    for (const auto& Attr : DAttributes) {
                if (Attr.first == key) {
                return Attr.second;
                }
            }
            return "";
        };
    };

    struct SWay : public CStreetMap::SWay
    {
        TWayID WID;
        std::vector<std::pair< std::string, std::string >> DAttributes;
        std::vector<TNodeID> DNodeIds;

        void addAttribute(std::string k,std::string v){
            DAttributes.push_back(std::make_pair(k,v));
        }

        void addNode(TNodeID id){
            DNodeIds.push_back(id);
        }

        SWay(COpenStreetMap::TWayID id)
        {
            WID=id;
        }
        ~SWay(){

        };
        TWayID ID() const noexcept override{
            return WID;
        };
        size_t NodeCount() const noexcept override{
            return DNodeIds.size();
        };
        TNodeID GetNodeID(size_t index) const noexcept override{
            return DNodeIds.at(index);
        };
        size_t AttributeCount() const noexcept override{
            return DAttributes.size();
        };
        std::string GetAttributeKey(size_t index) const noexcept override
        {
            if (index < DAttributes.size())
            {
                return DAttributes[index].first;
            }
            return "";
        };
        bool HasAttribute(const std::string &key) const noexcept override
        {
    	    for (const auto& Attr : DAttributes) {
                if (Attr.first == key) {
                return true;
                }
            }
            return false;
        };
        std::string GetAttribute(const std::string &key) const noexcept override
        {
    	    for (const auto& Attr : DAttributes) {
                if (Attr.first == key) {
                return Attr.second;
                }
            }
            return "";
        };
    };
        
    std::vector< std::shared_ptr<SNode> > DNodes;
    std::vector< std::shared_ptr<SWay> >  DWays;   
    
    SImplementation(std::shared_ptr<CXMLReader> src)
    {
        SXMLEntity entity;
        std::shared_ptr<SNode> TNode;
        std::shared_ptr<SWay> TWay;
        bool isInNode = false;
        while(src->ReadEntity(entity)){
            if(entity.DType == SXMLEntity::EType::StartElement && entity.DNameData == "node"){
                uint64_t id = std::stol(entity.AttributeValue("id"));
                CStreetMap::TNodeID TNId =(CStreetMap::TNodeID)id;
                TNode = std::make_shared<SNode>(TNId,std::stod(entity.AttributeValue("lat")),std::stod(entity.AttributeValue("lon")));
                DNodes.push_back(TNode);
                isInNode=true;
            }
            else if(entity.DType == SXMLEntity::EType::EndElement && entity.DNameData == "node"){
                isInNode=false;
            }
            else if(entity.DType == SXMLEntity::EType::StartElement && entity.DNameData == "way"){
                uint64_t id = std::stol(entity.AttributeValue("id"));
                CStreetMap::TWayID TWId =(CStreetMap::TWayID)id;
                TWay = std::make_shared<SWay>(TWId);
                DWays.push_back(TWay);
            }
            else if(entity.DType == SXMLEntity::EType::StartElement && entity.DNameData == "nd"){
                uint64_t id = std::stol(entity.AttributeValue("ref"));
                TWay->addNode((COpenStreetMap::TNodeID)id);
            }
            else if(entity.DType == SXMLEntity::EType::StartElement && entity.DNameData == "tag"){
                std::string k = entity.AttributeValue("k");
				std::string v = entity.AttributeValue("v");
                if(isInNode){
                    TNode->addAttribute(k,v);
                }
                else{
                    TWay->addAttribute(k,v);
                }
            }
        }

    }


    std::size_t NodeCount() const noexcept{
		return DNodes.size();
	}
	
	std::size_t WayCount() const noexcept{
		return DWays.size();
	}
	std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
		if(index < DNodes.size() && index >= 0){
    		return DNodes.at(index);
    	}
		return nullptr;
	}
	std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
		for (const auto& node : DNodes) {
            if (node->ID() == id) {
                return node;
            }
        }
		return nullptr;
	}
	
	std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
		if(index < DWays.size() && index >= 0){
    		return DWays.at(index);
    	}
    	return nullptr;
	}

	std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
		for (const auto& way : DWays) {
            if (way->ID() == id) {
                return way;
            }
        }
		return nullptr;
	}
};



COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){

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