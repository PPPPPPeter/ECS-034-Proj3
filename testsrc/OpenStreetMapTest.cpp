#include <gtest/gtest.h>
#include "StringDataSource.h"
#include "OpenStreetMap.h"
#include "XMLReader.h"
#include <iostream>
// comment from here to end of first test out
#include <fstream>
#include <streambuf>

TEST(OpenStreetMap, SimpleExampleTest)
{
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"1\" lat=\"1.0\" lon=\"-1.0\"/>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);

    EXPECT_EQ(StreetMap.NodeCount(), 1);
    EXPECT_EQ(StreetMap.WayCount(), 0);
    EXPECT_EQ(StreetMap.NodeByIndex(0)->ID(), 1);
}

TEST(COpenStreetMap, XMLReadFileTest)
{

    std::ifstream davisStream("data/davis.osm");
    std::stringstream davisBuffer;
    davisBuffer << davisStream.rdbuf();

    std::shared_ptr<CStringDataSource> davisSource(new CStringDataSource(davisBuffer.str()));
    // std::shared_ptr<CXMLReader> davisReader (new CXMLReader(davisSource));

    std::shared_ptr<CXMLReader> davisReader = std::make_shared<CXMLReader>(davisSource);
    COpenStreetMap map = COpenStreetMap(davisReader);

    EXPECT_EQ(map.NodeCount(), 10259);
    EXPECT_EQ(map.WayCount(), 1644);

    EXPECT_EQ(map.NodeByID(1834910027)->AttributeCount(), 1);
    EXPECT_EQ(map.NodeByID(1834910027)->GetAttributeKey(0), "highway");
    EXPECT_EQ(map.NodeByID(1834910027)->GetAttribute("highway"), "crossing");

    EXPECT_EQ(map.WayByID(10745716)->AttributeCount(), 2);
    EXPECT_EQ(map.WayByID(10745716)->GetAttributeKey(0), "highway");
    EXPECT_EQ(map.WayByID(10745716)->GetAttributeKey(1), "name");
    EXPECT_EQ(map.WayByID(10745716)->GetAttribute("highway"), "residential");
    EXPECT_EQ(map.WayByID(10745716)->GetAttribute("name"), "Citadel Drive");
    EXPECT_EQ(map.WayByID(10745716)->NodeCount(), 6);
    EXPECT_EQ(map.WayByID(10745716)->GetNodeID(0), 95712886);
    EXPECT_EQ(map.WayByID(10745716)->GetNodeID(1), 3935167455);
    EXPECT_EQ(map.WayByID(10745716)->GetNodeID(2), 3935167454);
    EXPECT_EQ(map.WayByID(10745716)->GetNodeID(3), 3935167453);
    EXPECT_EQ(map.WayByID(10745716)->GetNodeID(4), 3935167452);
    EXPECT_EQ(map.WayByID(10745716)->GetNodeID(5), 95712887);
}

TEST(COpenStreetMap, CountTest)
{

    // creating shared pointer source to an instance of the class CStringDataSource, feed it test data, new is bc of pointer
    std::shared_ptr<CStringDataSource> src(new CStringDataSource("<node id=\"124987773\" lat=\"38.5442984\" lon=\"-121.6897062\"/>"));
    std::shared_ptr<CXMLReader> srcReader(new CXMLReader(src));
    COpenStreetMap map = COpenStreetMap(srcReader);

    EXPECT_EQ(map.NodeCount(), 1);
    EXPECT_EQ(map.WayCount(), 0);

    EXPECT_EQ(map.NodeByIndex(0)->ID(), 124987773);
    EXPECT_EQ(map.NodeByIndex(1), nullptr);
    EXPECT_EQ(map.NodeByID(124987773)->ID(), 124987773);
    EXPECT_EQ(map.NodeByID(000), nullptr);
    EXPECT_EQ(map.NodeByID(124987773)->Location().first, 38.5442984);
    EXPECT_EQ(map.NodeByID(124987773)->Location().second, -121.6897062);
}