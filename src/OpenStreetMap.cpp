#include "OpenStreetMap.h"
#include <unordered_map>
#include <vector>
#include "StreetMap.h"

using namespace std;
struct COpenStreetMap::SImplementation
{
    struct SNode : public CStreetMap::SNode
    {
        TNodeID DID;
        TLocation DLocation;
        unordered_map<string, string> DAttributes;

        SNode(){

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
            return DAttributeKeys.size();
        };
        string GetAttributeKey(size_t index) const noexcept override
        {
            if (index < DAttributeKeys.size())
            {
                return DAttributeKeys[index];
            }
            return string();
        };
        bool HasAttribute(const string &key) const noexcept override
        {
            auto Search = DAttributes.find(key);
            return DAttributes.end() != Search;
        };
        string GetAttribute(const string &key) const noexcept override
        {
            auto Search = DAttributes.find(key);
            if (DAttributes.end() != Search)
            {
                return Search->second;
            }
        };
    };

    struct SWay : public CStreetMap::SWay
    {
        SWay()
        {
        }
        ~SWay(){

        };
        TWayID ID() const noexcept override{

        };
        size_t NodeCount() const noexcept override{

        };
        TNodeID GetNodeID(size_t index) const noexcept override{

        };
        size_t AttributeCount() const noexcept override{

        };
        string GetAttributeKey(size_t index) const noexcept override{

        };
        bool HasAttribute(const string &key) const noexcept override{

        };
        string GetAttribute(const string &key) const noexcept override{

        };
    };

    SImplementation(shared_ptr<CXMLReader> src)
    {
    }
}