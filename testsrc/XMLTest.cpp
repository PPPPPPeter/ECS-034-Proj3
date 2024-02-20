#include <gtest/gtest.h>
#include "StringDataSink.h"
#include "StringDataSource.h"
#include "XMLWriter.h"
#include "XMLReader.h"

TEST(Reader,simpletest){
auto InputStream = std::make_shared<CStringDataSource>("<example attr=\"123\"></example>");
CXMLReader Reader(InputStream);
SXMLEntity E;

EXPECT_TRUE(Reader.ReadEntity(E));
EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
EXPECT_EQ(E.AttributeValue("attr"), "123");

EXPECT_TRUE(Reader.ReadEntity(E));
EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
}

TEST(Reader, attributes){
    std::shared_ptr<CStringDataSource> source (new CStringDataSource("<tag team=\"Real Madrid\" player=\"Jude Bellingham\"></tag>"));
    CXMLReader reader(source);
    SXMLEntity E;

    EXPECT_TRUE(reader.ReadEntity(E, false));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.DNameData, "tag");

    EXPECT_EQ(E.DAttributes.size(), 2);

    EXPECT_EQ(E.DAttributes[0].first, "team");
    EXPECT_EQ(E.DAttributes[0].second, "Real Madrid");
    EXPECT_EQ(E.DAttributes[1].first, "player");
    EXPECT_EQ(E.DAttributes[1].second, "Jude Bellingham");

    EXPECT_TRUE(reader.ReadEntity(E, false));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(E.DNameData, "tag");

    EXPECT_TRUE(reader.End());

}

TEST(Reader, skipdata){
    std::shared_ptr<CStringDataSource> source (new CStringDataSource("<example attr=\"Hello World\">adjsfhalksdf</example>"));
    CXMLReader reader(source);
    SXMLEntity E;
    EXPECT_TRUE(reader.ReadEntity(E, true));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.AttributeValue("attr"), "Hello World");

    EXPECT_TRUE(reader.ReadEntity(E, true));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(E.DNameData, "example");
    

    EXPECT_TRUE(reader.End());

}

TEST(Reader, CompleteElement){
    std::shared_ptr<CStringDataSource> source (new CStringDataSource("<example attr=\"Hello World\"/>"));
    CXMLReader reader(source);
    SXMLEntity E;
    EXPECT_TRUE(reader.ReadEntity(E, true));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.AttributeValue("attr"), "Hello World");
    EXPECT_TRUE(reader.ReadEntity(E, true));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);

    EXPECT_TRUE(reader.End());
}

TEST(Reader,OSM)
{
    auto InStream = std::make_shared<CStringDataSource>("<?xml version='1.0' encoding='UTF-8'?>"
                                                        "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">"
                                                        "<node id=\"1\" lat=\"1.0\" lon=\"-1.0\"/>"
                                                        "</osm>");
    CXMLReader reader(InStream);
    SXMLEntity E;

    EXPECT_TRUE(reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.DNameData, "osm");

    EXPECT_TRUE(reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(E.DNameData, "node");

    EXPECT_EQ(E.DAttributes[0].first, "id");
    EXPECT_EQ(E.DAttributes[0].second, "1");
    EXPECT_EQ(E.DAttributes[1].first, "lat");
    EXPECT_EQ(E.DAttributes[1].second, "1.0");
    EXPECT_EQ(E.DAttributes[2].first, "lon");
    EXPECT_EQ(E.DAttributes[2].second, "-1.0");

    EXPECT_TRUE(reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(E.DNameData, "node");

    EXPECT_TRUE(reader.ReadEntity(E));
    EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(E.DNameData, "osm");

     EXPECT_TRUE(!reader.ReadEntity(E));
}


TEST(Reader,OSM_Harder)
{
    auto InStream = std::make_shared<CStringDataSource>("<xml>""<way id=\"33102119\">"
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
    "</xml>");
    CXMLReader reader(InStream);
    SXMLEntity E;
    int i=0;
    while(reader.ReadEntity(E)){
        if(E.DNameData=="way"&&E.DType== SXMLEntity::EType::StartElement){   
            i++;
        }
    }
    EXPECT_EQ(i,3);
}




TEST(Writer, Write0){
auto OutputStream = std::make_shared<CStringDataSink>();
CXMLWriter Writer(OutputStream);

EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "example", {{"attr","Hello World"}}}));
EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "example", {}}));

EXPECT_EQ(OutputStream->String(), "<example attr=\"Hello World\"></example>");
}

TEST(Writer, Write1){
auto OutputStream = std::make_shared<CStringDataSink>();
CXMLWriter Writer(OutputStream);

EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "example", {{"attr","Hello World"}}}));
EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::CharData, "aabbbcc"}));
EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "example", {}}));

EXPECT_EQ(OutputStream->String(), "<example attr=\"Hello World\">aabbbcc</example>");
}

TEST(Writer, Write2){
auto OutputStream = std::make_shared<CStringDataSink>();
CXMLWriter Writer(OutputStream);

EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "example", {{"attr","Hello&World"}}}));
EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "example", {}}));

EXPECT_EQ(OutputStream->String(), "<example attr=\"Hello&amp;World\"></example>");
}

TEST(Writer, Write3){
auto OutputStream = std::make_shared<CStringDataSink>();
CXMLWriter Writer(OutputStream);

EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "example", {{"attr","Hello&\"World"}}}));
EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "aaa", {}}));
EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "aaa", {}}));
EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::EndElement, "example", {}}));

EXPECT_EQ(OutputStream->String(), "<example attr=\"Hello&amp;&quot;World\"><aaa></aaa></example>");
}


TEST(Writer, Flush){
auto OutputStream = std::make_shared<CStringDataSink>();
CXMLWriter Writer(OutputStream);

EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "example", {{"attr","Hello<\'>World"}}}));
EXPECT_TRUE(Writer.WriteEntity({SXMLEntity::EType::StartElement, "aaa", {}}));
EXPECT_TRUE(Writer.Flush());

EXPECT_EQ(OutputStream->String(), "<example attr=\"Hello&lt;&apos;&gt;World\"><aaa></aaa></example>");
}