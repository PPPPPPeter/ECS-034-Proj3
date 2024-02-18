#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    std::string baseString = "Hello!";
    EXPECT_EQ(StringUtils::Slice(baseString,0,5),"Hello");
    EXPECT_EQ(StringUtils::Slice(baseString,0,4),"Hell");
    EXPECT_EQ(StringUtils::Slice(baseString,1,4),"ell");
    EXPECT_EQ(StringUtils::Slice(baseString,1),"ello!");
    EXPECT_EQ(StringUtils::Slice(baseString,-1),"!");
    EXPECT_EQ(StringUtils::Slice(baseString,-1,-2),"");
    EXPECT_EQ(StringUtils::Slice(baseString,-2,-1),"o");
}

TEST(StringUtilsTest, Capitalize){
    EXPECT_EQ(StringUtils::Capitalize("hello"),"Hello");
    EXPECT_EQ(StringUtils::Capitalize("heLLo"),"Hello");
    EXPECT_EQ(StringUtils::Capitalize("12hello"),"12hello");
    }

TEST(StringUtilsTest, Upper){
    EXPECT_EQ(StringUtils::Upper("hello"),"HELLO");
    EXPECT_EQ(StringUtils::Upper("heLLo"),"HELLO");
    EXPECT_EQ(StringUtils::Upper("12hello"),"12HELLO");
}

TEST(StringUtilsTest, Lower){
    EXPECT_EQ(StringUtils::Lower("HELLO"),"hello");
    EXPECT_EQ(StringUtils::Lower("heLLo"),"hello");
    EXPECT_EQ(StringUtils::Lower("12HELlo"),"12hello");
}

TEST(StringUtilsTest, LStrip){
    EXPECT_EQ(StringUtils::LStrip("           hello            "),"hello            ");
    EXPECT_EQ(StringUtils::LStrip("      hello            "),"hello            ");
    EXPECT_EQ(StringUtils::LStrip("                                                                        hello            "),"hello            ");
    EXPECT_EQ(StringUtils::LStrip("hello"),"hello");

}

TEST(StringUtilsTest, RStrip){
    EXPECT_EQ(StringUtils::RStrip("           hello            "),"           hello");
    EXPECT_EQ(StringUtils::RStrip("      hello            "),"      hello");
    EXPECT_EQ(StringUtils::RStrip("          hello                                                                          "),"          hello");
    EXPECT_EQ(StringUtils::RStrip("hello"),"hello");
}

TEST(StringUtilsTest, Strip){
    EXPECT_EQ(StringUtils::Strip("           hello            "),"hello");
    EXPECT_EQ(StringUtils::Strip("      hello            "),"hello");
    EXPECT_EQ(StringUtils::Strip("                                                                        hello            "),"hello");
    EXPECT_EQ(StringUtils::Strip("hello"),"hello"); 
}

TEST(StringUtilsTest, Center){
    EXPECT_EQ(StringUtils::Center("hello ",10,' '),"  hello   ");
    EXPECT_EQ(StringUtils::Center("hello",10,' '),"  hello   ");
    EXPECT_EQ(StringUtils::Center("hello",10,'-'),"--hello---");
    EXPECT_EQ(StringUtils::Center("ab",5,'-'),"-ab--");
    EXPECT_EQ(StringUtils::Center("ab",6,'-'),"--ab--");
    EXPECT_EQ(StringUtils::Center("ab",2,'-'),"ab");
}

TEST(StringUtilsTest, LJust){
    EXPECT_EQ(StringUtils::LJust("hello ",10,' '),"hello     ");
    EXPECT_EQ(StringUtils::LJust("hello",10,' '),"hello     ");
    EXPECT_EQ(StringUtils::LJust("hello",10,'-'),"hello-----");
    EXPECT_EQ(StringUtils::LJust("ab",5,'-'),"ab---");
    EXPECT_EQ(StringUtils::LJust("ab",6,'-'),"ab----");
    EXPECT_EQ(StringUtils::LJust("ab",2,'-'),"ab");
}

TEST(StringUtilsTest, RJust){
    EXPECT_EQ(StringUtils::RJust("hello ",10,' '),"    hello ");
    EXPECT_EQ(StringUtils::RJust("hello",10,' '),"     hello");
    EXPECT_EQ(StringUtils::RJust("hello",10,'-'),"-----hello");
    EXPECT_EQ(StringUtils::RJust("ab",5,'-'),"---ab");
    EXPECT_EQ(StringUtils::RJust("ab",6,'-'),"----ab");
    EXPECT_EQ(StringUtils::RJust("ab",2,'-'),"ab");
} 

TEST(StringUtilsTest, Replace){
    EXPECT_EQ(StringUtils::Replace("abcabcabcabcabc","abc","def"),"defdefdefdefdef");
    EXPECT_EQ(StringUtils::Replace("abcdefabcdefabc","abc","def"),"defdefdefdefdef");
    EXPECT_EQ(StringUtils::Replace("def","abc","def"),"def");
    EXPECT_EQ(StringUtils::Replace("123333333","3","4"),"124444444");
    EXPECT_EQ(StringUtils::Replace("one one one ","one ","two"),"twotwotwo");
    EXPECT_EQ(StringUtils::Replace("11111","11","2"),"221");  
    EXPECT_EQ(StringUtils::Replace("<&\"\'>","&","&amp;"),"<&amp;\"\'>");
    EXPECT_EQ(StringUtils::Replace("<&\"\'>","&","&amp;"),"<&amp;\"\'>");
}

TEST(StringUtilsTest, Replace2){
    std::string s="<&\"\'>";
        s = StringUtils::Replace(s, "&", "&amp;");
        s = StringUtils::Replace(s, "\"", "&quot;");
        s = StringUtils::Replace(s, "'", "&apos;");
        s = StringUtils::Replace(s, ">", "&gt;");
        s = StringUtils::Replace(s, "<", "&lt;");
    EXPECT_EQ(s,"&lt;&amp;&quot;&apos;&gt;");
}

TEST(StringUtilsTest, Split){
    std::vector<std::string> a= {"Hello","World!"};
    EXPECT_EQ(StringUtils::Split("Hello World!"),a);
    a={"","1","2","3","4","5","6","7"};
    EXPECT_EQ(StringUtils::Split(" 1 2 3 4 5 6 7"),a);
    a={"Hello","World!"};
    EXPECT_EQ(StringUtils::Split("Hello-World!","-"),a);
    a={"Hello","World!"};
    EXPECT_EQ(StringUtils::Split("HelloOoooWorld!","Oooo"),a);
}

TEST(StringUtilsTest, Join){
    std::vector<std::string> a= {"Hello","World!"};
    EXPECT_EQ(StringUtils::Join("hello",a),"helloHelloWorld!");
    EXPECT_EQ(StringUtils::Join("",a),"HelloWorld!");
    a={"","1","2","3","4","5","6","7"};
    EXPECT_EQ(StringUtils::Join("0",a),"01234567");
    a={};
    EXPECT_EQ(StringUtils::Join("Hello-World!",a),"Hello-World!");
}

TEST(StringUtilsTest, ExpandTabs){
    EXPECT_EQ(StringUtils::ExpandTabs("\t1\t11\t",1)," 1 11 ");
    EXPECT_EQ(StringUtils::ExpandTabs("\t1\t11\t",2),"  1  11  ");
    EXPECT_EQ(StringUtils::ExpandTabs("\t1\t11\t",4),"    1    11    ");
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(StringUtils::EditDistance("kitten","sitting"),3);
    EXPECT_EQ(StringUtils::EditDistance("kitten","Sitting"),3);
    EXPECT_EQ(StringUtils::EditDistance("kitten","sItting"),4);
    EXPECT_EQ(StringUtils::EditDistance("kitten","sItting",true),3);
    EXPECT_EQ(StringUtils::EditDistance("kitten","sIttin",true),2);
    EXPECT_EQ(StringUtils::EditDistance("kitten","kitten"),0);
}
