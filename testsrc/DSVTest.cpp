#include <gtest/gtest.h>
#include "StringDataSink.h"
#include "StringDataSource.h"
#include "DSVWriter.h"
#include "DSVReader.h"



TEST(DSVReader, Read0){
auto Source = std::make_shared<CStringDataSource>("Hello&World!");
CDSVReader Reader(Source,'&');
std::vector<std::string> output;

EXPECT_TRUE(Reader.ReadRow(output));
ASSERT_EQ(output.size(),2);
EXPECT_EQ(output[0],"Hello");
EXPECT_EQ(output[1],"World!");
EXPECT_TRUE(Reader.End());
}

TEST(DSVReader, Read1){
auto Source = std::make_shared<CStringDataSource>("\"abc\" hi,12,83");
CDSVReader Reader(Source,',');
std::vector<std::string> output;

EXPECT_TRUE(Reader.ReadRow(output));
ASSERT_EQ(output.size(),3);
EXPECT_EQ(output[0],"abc hi");
EXPECT_EQ(output[1],"12");
EXPECT_EQ(output[2],"83");
EXPECT_TRUE(Reader.End());
}

TEST(DSVReader, Read2){
auto Source = std::make_shared<CStringDataSource>(",a,,");
CDSVReader Reader(Source,',');
std::vector<std::string> output;

EXPECT_TRUE(Reader.ReadRow(output));
ASSERT_EQ(output.size(),4);
EXPECT_EQ(output[0],"");
EXPECT_EQ(output[1],"a");
EXPECT_EQ(output[2],"");
EXPECT_EQ(output[3],"");
EXPECT_TRUE(Reader.End());
}

TEST(DSVReader, Read3){
auto Source = std::make_shared<CStringDataSource>(",a,,\n,");
CDSVReader Reader(Source,',');
std::vector<std::string> output;

EXPECT_TRUE(Reader.ReadRow(output));
ASSERT_EQ(output.size(),4);
EXPECT_EQ(output[0],"");
EXPECT_EQ(output[1],"a");
EXPECT_EQ(output[2],"");
EXPECT_EQ(output[3],"");
output.clear();
EXPECT_TRUE(Reader.ReadRow(output));
ASSERT_EQ(output.size(),2);
EXPECT_EQ(output[0],"");
EXPECT_EQ(output[1],"");
EXPECT_TRUE(Reader.End());
}

TEST(DSVReader, Read4){
auto Source = std::make_shared<CStringDataSource>(",\"\"\"a\"\"b\t\",,\n,");
CDSVReader Reader(Source,',');
std::vector<std::string> output;

EXPECT_TRUE(Reader.ReadRow(output));
ASSERT_EQ(output.size(),4);
EXPECT_EQ(output[0],"");
EXPECT_EQ(output[1],"\"a\"b\t");
EXPECT_EQ(output[2],"");
EXPECT_EQ(output[3],"");
output.clear();
EXPECT_TRUE(Reader.ReadRow(output));
ASSERT_EQ(output.size(),2);
EXPECT_EQ(output[0],"");
EXPECT_EQ(output[1],"");
EXPECT_TRUE(Reader.End());
}

TEST(DSVWriter, Write0){
auto Sink = std::make_shared<CStringDataSink>();
CDSVWriter Writer(Sink,'&',false);
std::vector<std::string> input = {"Hello", "World!"};

EXPECT_TRUE(Writer.WriteRow(input));
EXPECT_EQ(Sink->String(),"Hello&World!");
}

TEST(DSVWriter, Write1){
auto Sink = std::make_shared<CStringDataSink>();
CDSVWriter Writer(Sink,'&',true);
std::vector<std::string> input = {"Hello", "World!"};

EXPECT_TRUE(Writer.WriteRow(input));
EXPECT_EQ(Sink->String(),"\"Hello\"&\"World!\"");
}

TEST(DSVWriter, Write2){
auto Sink = std::make_shared<CStringDataSink>();
CDSVWriter Writer(Sink,'&',false);
std::vector<std::string> input = {"\"Hello", "World!"};

EXPECT_TRUE(Writer.WriteRow(input));
EXPECT_EQ(Sink->String(),"\"\"\"Hello\"&World!");
}

TEST(DSVWriter, Write3){
auto Sink = std::make_shared<CStringDataSink>();
CDSVWriter Writer(Sink,'&',false);
std::vector<std::string> input = {"Hello\n", "World!"};

EXPECT_TRUE(Writer.WriteRow(input));
EXPECT_EQ(Sink->String(),"\"Hello\n\"&World!");
}

TEST(DSVWriter, Write4){
auto Sink = std::make_shared<CStringDataSink>();
CDSVWriter Writer(Sink,'&',true);
std::vector<std::string> input = {"\"Hello", "World!"};

EXPECT_TRUE(Writer.WriteRow(input));
EXPECT_EQ(Sink->String(),"\"\"\"Hello\"&\"World!\"");
}