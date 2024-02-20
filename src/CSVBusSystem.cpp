#include "CSVBusSystem.h"
#include <vector>


struct CCSVBusSystem::SImplementation{
    struct SStop: public CBusSystem::SStop{
        TStopID StopId;
        CStreetMap::TNodeID NodeId; 
        
        SStop(TStopID sid, CStreetMap::TNodeID nid){
            StopId = sid;
            NodeId = nid;
        };
        ~SStop(){};
        TStopID ID() const noexcept{
            return StopId;
        };
        CStreetMap::TNodeID NodeID() const noexcept{
            return NodeId;
        };
        };

    struct SRoute: public CBusSystem::SRoute{
        std::string name;
        std::vector<TStopID> stops;
        SRoute(std::string str){
            name = str;
        };
        ~SRoute(){};
        std::string Name() const noexcept{
            return name;
        };
        std::size_t StopCount() const noexcept{
            return stops.size();
        };
        TStopID GetStopID(std::size_t index) const noexcept{
            return stops[index];
        };
        void AddStop(TStopID sid){
            stops.push_back(sid);
        }
        };

    std::vector<std::shared_ptr <SStop>> DStops;
    std::vector<std::shared_ptr <SRoute>> DRoutes;

    SImplementation(std::shared_ptr<CDSVReader> CSVReaderStops, std::shared_ptr<CDSVReader> CSVReaderRoutes){
        std::shared_ptr <SStop> TStop;
        std::shared_ptr <SRoute> TRoute;
        std::vector<std::string> tempS;
        CSVReaderStops->ReadRow(tempS);
        CSVReaderRoutes->ReadRow(tempS);
        tempS.clear();
        while(!CSVReaderStops->End()){
            CSVReaderStops->ReadRow(tempS);
            if(tempS.size()==2){
            TStopID sid=(TStopID)std::stoull(tempS[0]);
            CStreetMap::TNodeID nid=(CStreetMap::TNodeID)std::stoull(tempS[1]);
            TStop = std::make_shared<SStop>(sid,nid);
            DStops.push_back(TStop);}
            tempS.clear();
        }
        while(!CSVReaderRoutes->End()){
            CSVReaderRoutes->ReadRow(tempS);
            if(tempS.size()==2){
            bool find = false;
            for (const auto& route : DRoutes) {
                if (route->Name() == tempS[0]) {
                    route->AddStop((TStopID)std::stoull(tempS[1]));
                    tempS.clear();
                    find=true;
                    break;
                }
            }
            if(!find){
                TRoute =  std::make_shared<SRoute>(tempS[0]);
                TRoute->AddStop((TStopID)std::stoull(tempS[1]));
                DRoutes.push_back(TRoute);
                tempS.clear();
            }
            }
        }
    };

        std::size_t StopCount() const noexcept {
            return DStops.size();
        };
        std::size_t RouteCount() const noexcept {
            return DRoutes.size();
        };
        std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept {
            return DStops[index];
        };
        std::shared_ptr<SStop> StopByID(TStopID id) const noexcept {
        for (const auto& stop : DStops) {
            if (stop->ID() == id) {
                return stop;
            }
        }
		return nullptr;
        };
        std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept {
             return DRoutes[index];
        };
        std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept {
        for (const auto& route : DRoutes) {
            if (route->Name() == name) {
                return route;
            }
        }
		return nullptr;
        };


};


CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc)
{
    DImplementation = std::make_unique<SImplementation>(stopsrc, routesrc);
}

CCSVBusSystem::~CCSVBusSystem()
{
}

std::size_t CCSVBusSystem::StopCount() const noexcept
{
    return DImplementation->StopCount();
}

std::size_t CCSVBusSystem::RouteCount() const noexcept
{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept
{
    return DImplementation->StopByIndex(index);
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept
{
    return DImplementation->StopByID(id);
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept
{
    return DImplementation->RouteByIndex(index);
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept
{
    return DImplementation->RouteByName(name);
}