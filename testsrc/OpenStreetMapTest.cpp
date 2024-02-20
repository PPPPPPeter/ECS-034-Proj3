#include <gtest/gtest.h>
#include "StringDataSource.h"
#include "OpenStreetMap.h"
#include "XMLReader.h"
#include <iostream>
#include "StringDataSink.h"
#include "StringDataSource.h"
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
    
    EXPECT_EQ(StreetMap.NodeCount(),1);
    EXPECT_EQ(StreetMap.WayCount(),0);
    EXPECT_EQ(StreetMap.NodeByIndex(0)->ID(),1);
 }

TEST(OpenStreetMap, MultipleNodesTest){
        //Test case Multiple Nodes
     auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"1\" lat=\"1.0\" lon=\"-1.0\"/>"
                                                        "<node id=\"2\" lat=\"2.0\" lon=\"-2.0\"/>"
                                                        "<node id=\"3\" lat=\"3.0\" lon=\"-3.0\"/>"
                                                        "<node id=\"4\" lat=\"4.0\" lon=\"-4.0\"/>"
                                                        "<node id=\"5\" lat=\"5.0\" lon=\"-5.0\"/>"
                                                        "<node id=\"6\" lat=\"6.0\" lon=\"-6.0\"/>"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);

    EXPECT_EQ(StreetMap.NodeCount(), 6);
    EXPECT_EQ(StreetMap.WayCount(), 0);
    EXPECT_EQ(StreetMap.NodeByIndex(0)->ID(),1);
    EXPECT_EQ(StreetMap.NodeByIndex(1)->ID(),2);
    EXPECT_EQ(StreetMap.NodeByIndex(2)->ID(),3);
    EXPECT_EQ(StreetMap.NodeByIndex(3)->ID(),4);
    EXPECT_EQ(StreetMap.NodeByIndex(4)->ID(),5);
    EXPECT_EQ(StreetMap.NodeByIndex(5)->ID(),6);
    EXPECT_EQ(StreetMap.NodeByID(1), StreetMap.NodeByIndex(0));
    EXPECT_EQ(StreetMap.NodeByID(2), StreetMap.NodeByIndex(1));
    EXPECT_EQ(StreetMap.NodeByID(3), StreetMap.NodeByIndex(2));
    EXPECT_EQ(StreetMap.NodeByID(4), StreetMap.NodeByIndex(3));
    EXPECT_EQ(StreetMap.NodeByID(5), StreetMap.NodeByIndex(4));
    EXPECT_EQ(StreetMap.NodeByID(6), StreetMap.NodeByIndex(5));
}


TEST(OpenStreetMap, CountTest)
{
        // Test Case testing single way count
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<way id=\"10747760\">"
                                                        "</osm>");
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);

    EXPECT_EQ(StreetMap.NodeCount(), 0);
    EXPECT_EQ(StreetMap.WayCount(), 1);
    EXPECT_EQ(StreetMap.WayByIndex(0)->ID(), 10747760);
    EXPECT_EQ(StreetMap.WayByID(10747760), StreetMap.WayByIndex(0));
}

TEST(OpenStreetMap, MultipleWayTest){
        // Test Case testing single way count
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
    "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
    "<way id=\"33102119\">"
		"<nd ref=\"95713486\"/>"
		"<nd ref=\"5674148930\"/>"
		"<nd ref=\"95713487\"/>"
	    "<tag k=\"cycleway\" v=\"lane\"/>"
		"<tag k=\"highway\" v=\"residential\"/>"
		"<tag k=\"name\" v=\"B Street\"/>"
	"</way>"
    "<way id=\"331022119\">"
		"<nd ref=\"95713486\"/>"
		"<nd ref=\"5674148930\"/>"
		"<nd ref=\"95713487\"/>"
	    "<tag k=\"cycleway\" v=\"lane\"/>"
		"<tag k=\"highway\" v=\"residential\"/>"
		"<tag k=\"name\" v=\"B Street\"/>"
	"</way>"
    "<way id=\"33102139\">"
		"<nd ref=\"95713486\"/>"
		"<nd ref=\"5674148930\"/>"
		"<nd ref=\"95713487\"/>"
	    "<tag k=\"cycleway\" v=\"lane\"/>"
		"<tag k=\"highway\" v=\"residential\"/>"
		"<tag k=\"name\" v=\"B Street\"/>"
	"</way>"
    "</osm>"
	);
    auto Reader = std::make_shared<CXMLReader>(InStream);
    COpenStreetMap StreetMap(Reader);
    
    EXPECT_EQ(StreetMap.WayCount(), 3);
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
