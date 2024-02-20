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

TEST(DSVReader, CBS){
auto InStreamStops = std::make_shared<CStringDataSource>("stop_id,node_id\n1,1001");
auto InStreamRoutes = std::make_shared<CStringDataSource>("route,stop_id");
auto CSVReaderStops = std::make_shared<CDSVReader>(InStreamStops, ',');
auto CSVReaderRoutes = std::make_shared<CDSVReader>(InStreamRoutes, ',');
std::vector<std::string> output;
EXPECT_TRUE(CSVReaderStops->ReadRow(output));
output.clear();
EXPECT_TRUE(CSVReaderStops->ReadRow(output));
EXPECT_EQ(std::stoull(output[0]),1);
EXPECT_EQ(std::stoull(output[1]),1001);
output.clear();
EXPECT_TRUE(CSVReaderRoutes->ReadRow(output));
}

TEST(DSVReader, CBS_harder){
auto Source = std::make_shared<CStringDataSource>("stop_id,node_id\n22079,2580285987\n22071,4595133310\n22036,95715556\n22048,5352536590");
CDSVReader Reader(Source,',');
std::vector<std::string> output;
EXPECT_TRUE(Reader.ReadRow(output));
output.clear();
EXPECT_TRUE(Reader.ReadRow(output));
EXPECT_EQ(std::stoull(output[0]),22079);
EXPECT_EQ(std::stoull(output[1]),2580285987);
output.clear();
EXPECT_TRUE(Reader.ReadRow(output));
EXPECT_EQ(std::stoull(output[0]),22071);
EXPECT_EQ(std::stoull(output[1]),4595133310);
output.clear();
EXPECT_TRUE(Reader.ReadRow(output));
EXPECT_EQ(std::stoull(output[0]),22036);
EXPECT_EQ(std::stoull(output[1]),95715556);
output.clear();
EXPECT_TRUE(Reader.ReadRow(output));
EXPECT_EQ(std::stoull(output[0]),22048);
EXPECT_EQ(std::stoull(output[1]),5352536590);
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