#include <gtest/gtest.h>
#include "StringDataSource.h"
#include "CSVBusSystem.h"
#include "DSVReader.h"
#include <iostream>
#include <fstream>
#include <streambuf>

TEST(CSVBusSystem, SimpleExampleTest)
{
    auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id\n1,1001");
    auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id");
    auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops, ',');
    auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes, ',');
    CCSVBusSystem BusSystem(CSVReaderStops, CSVReaderRoutes);

    EXPECT_EQ(BusSystem.StopCount(), 1);
    EXPECT_EQ(BusSystem.RouteCount(), 0);
    EXPECT_EQ(BusSystem.StopByIndex(0)->ID(), 1);
}


TEST(CCSVBusSystem, StopAndRouteCountTest)
{

    // creating shared pointer source to an instance of the class CStringDataSource, feed it test data, new is bc of pointer
    std::shared_ptr<CStringDataSource> StopData(new CStringDataSource("stop_id,node_id\n22079,2580285987\n22071,4595133310\n22036,95715556\n22048,5352536590"));
    std::shared_ptr<CStringDataSource> RouteData(new CStringDataSource("route,stop_id\nA,22300\nA,22243\nA,22070\nA,22075"));
    std::shared_ptr<CDSVReader> StopReader(new CDSVReader(StopData, ','));
    std::shared_ptr<CDSVReader> RouteReader(new CDSVReader(RouteData, ','));
    CCSVBusSystem BusSystem = CCSVBusSystem(StopReader, RouteReader);

    EXPECT_EQ(BusSystem.StopCount(), 4);
    EXPECT_EQ(BusSystem.RouteCount(), 1);
}

TEST(CCSVBusSystem, StopByIndexTest)
{
    // Creating shared pointer source to an instance of the class CStringDataSource, feed it test data
    std::shared_ptr<CStringDataSource> StopData(new CStringDataSource("stop_id,node_id\n22079,2580285987\n22071,4595133310\n22036,95715556\n22048,5352536590"));
    std::shared_ptr<CStringDataSource> RouteData(new CStringDataSource("route,stop_id\nA,22300\nA,22243\nA,22070\nA,22075"));
    std::shared_ptr<CDSVReader> StopReader(new CDSVReader(StopData, ','));
    std::shared_ptr<CDSVReader> RouteReader(new CDSVReader(RouteData, ','));
    CCSVBusSystem BusSystem = CCSVBusSystem(StopReader, RouteReader);

    // Testing StopByIndex method
    EXPECT_EQ(BusSystem.StopByIndex(0)->ID(), 22079);
    EXPECT_EQ(BusSystem.StopByIndex(1)->ID(), 22071);
    EXPECT_EQ(BusSystem.StopByIndex(2)->ID(), 22036);
    EXPECT_EQ(BusSystem.StopByIndex(3)->ID(), 22048);
}

TEST(CCSVBusSystem, StopByIDTest)
{
    // Creating shared pointer source to an instance of the class CStringDataSource, feed it test data
    std::shared_ptr<CStringDataSource> StopData(new CStringDataSource("stop_id,node_id\n22079,2580285987\n22071,4595133310\n22036,95715556\n22048,5352536590"));
    std::shared_ptr<CStringDataSource> RouteData(new CStringDataSource("route,stop_id\nA,22300\nA,22243\nA,22070\nA,22075"));
    std::shared_ptr<CDSVReader> StopReader(new CDSVReader(StopData, ','));
    std::shared_ptr<CDSVReader> RouteReader(new CDSVReader(RouteData, ','));
    CCSVBusSystem BusSystem = CCSVBusSystem(StopReader, RouteReader);

    // Testing StopByID method
    EXPECT_EQ(BusSystem.StopByID(22079), BusSystem.StopByIndex(0));
    EXPECT_EQ(BusSystem.StopByID(22071), BusSystem.StopByIndex(1));
    EXPECT_EQ(BusSystem.StopByID(22036), BusSystem.StopByIndex(2));
    EXPECT_EQ(BusSystem.StopByID(22048), BusSystem.StopByIndex(3));
}


TEST(CCSVBusSystem, RouteByNameTest)
{
    // Creating shared pointer source to an instance of the class CStringDataSource, feed it test data
    std::shared_ptr<CStringDataSource> StopData(new CStringDataSource("stop_id,node_id\n22079,2580285987\n22071,4595133310\n22036,95715556\n22048,5352536590"));
    std::shared_ptr<CStringDataSource> RouteData(new CStringDataSource("route,stop_id\nA,22300\nA,22243\nA,22070\nA,22075"));
    std::shared_ptr<CDSVReader> StopReader(new CDSVReader(StopData, ','));
    std::shared_ptr<CDSVReader> RouteReader(new CDSVReader(RouteData, ','));
    CCSVBusSystem BusSystem = CCSVBusSystem(StopReader, RouteReader);

    // Testing RouteByName method
    EXPECT_EQ(BusSystem.RouteByName("A")->StopCount(), 4);
    // Test for non-existing route
    EXPECT_EQ(BusSystem.RouteByName("B"), nullptr);
}
