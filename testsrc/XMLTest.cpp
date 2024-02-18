#include <gtest/gtest.h>
#include "StringDataSink.h"
#include "StringDataSource.h"
#include "XMLWriter.h"
#include "XMLReader.h"

TEST(Reader,Read0){
auto InputStream = std::make_shared<CStringDataSource>("<example attr=\"Hello World\"></example>");
CXMLReader Reader(InputStream);
SXMLEntity E;

EXPECT_TRUE(Reader.ReadEntity(E));
EXPECT_EQ(E.DType, SXMLEntity::EType::StartElement);
EXPECT_EQ(E.AttributeValue("attr"), "Hello World");

EXPECT_TRUE(Reader.ReadEntity(E));
EXPECT_EQ(E.DType, SXMLEntity::EType::EndElement);
}

TEST(Reader, Read1){
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

TEST(Reader, Read2){
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