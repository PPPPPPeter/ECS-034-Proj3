#include "CSVBusSystem.h"
#include <iostream>

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