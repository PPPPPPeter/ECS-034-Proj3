#include "XMLWriter.h"
#include <StringDataSink.h>
#include <stack>
#include <StringUtils.h>

struct CXMLWriter::SImplementation{
    std::shared_ptr<CDataSink> DDataSink;
    std::stack<std::string> StartElements;
    
    SImplementation(std::shared_ptr< CDataSink > sink){
        DDataSink=sink;
    };

    //process escaped chars
    std::string ReplaceEscapedAttributes(std::string s){
        s = StringUtils::Replace(s, "&", "&amp;");
        s = StringUtils::Replace(s, "\"", "&quot;");
        s = StringUtils::Replace(s, "'", "&apos;");
        s = StringUtils::Replace(s, ">", "&gt;");
        s = StringUtils::Replace(s, "<", "&lt;");   
        return s;
    }
    
    //To empty the unclosed start element in LIFO sequence
    bool Flush(){
        if(StartElements.empty()) return false;
        while(!StartElements.empty()){
            std::string EndName = StartElements.top();
            DDataSink->Put('<');
            DDataSink->Put('/');
            std::vector<char> v(EndName.begin(),EndName.end());
            DDataSink->Write(v);
            DDataSink->Put('>');
            StartElements.pop();
        }
        return true;
    }

    //Part to Write Entity function that output attributes to datasink
    void WriteAttributes(const SXMLEntity &entity,std::shared_ptr<CDataSink> DDataSink){
        if(!entity.DAttributes.empty()){
            for (auto att:entity.DAttributes){
                DDataSink->Put(' ');
                std::vector<char> v (att.first.begin(),att.first.end());
                DDataSink->Write(v);
                DDataSink->Put('=');
                DDataSink->Put('\"');
                std::string str = ReplaceEscapedAttributes(att.second);
                v.assign(str.begin(),str.end());
                DDataSink->Write(v);
                DDataSink->Put('\"');
            }
        }
    }

    //Output XML entity to datasink by entity type
    bool WriteEntity(const SXMLEntity &entity){
       switch (entity.DType) {
            case SXMLEntity::EType::StartElement:{
                DDataSink->Put('<');
                std::vector<char> val(entity.DNameData.begin(),entity.DNameData.end());
                DDataSink->Write(val);
                WriteAttributes(entity,DDataSink);
                DDataSink->Put('>');
                StartElements.push(entity.DNameData);
                break;
            }
            case SXMLEntity::EType::EndElement:{
                std::string EndName = StartElements.top();
                if (EndName!=entity.DNameData)
                {
                    return false;
                }
                DDataSink->Put('<');
                DDataSink->Put('/');
                std::vector<char> v(EndName.begin(),EndName.end());
                DDataSink->Write(v);
                DDataSink->Put('>');
                StartElements.pop();
                break;
            }
            case SXMLEntity::EType::CharData:{
                std::string str = ReplaceEscapedAttributes(entity.DNameData);
                std::vector<char> v(str.begin(),str.end());
                DDataSink->Write(v);
                break;
            }
            case SXMLEntity::EType::CompleteElement:{
                DDataSink->Put('<');
                std::vector<char> val(entity.DNameData.begin(),entity.DNameData.end());
                DDataSink->Write(val);
                WriteAttributes(entity,DDataSink);
                DDataSink->Put('/');
                DDataSink->Put('>');
                break;
            }
        }
        return true;
    }
};

 
 
CXMLWriter::CXMLWriter(std::shared_ptr< CDataSink > sink){
    DImplementation = std::make_unique<SImplementation>(sink);
};

CXMLWriter::~CXMLWriter(){

};
        
bool CXMLWriter::Flush(){
    return DImplementation->Flush();
};
bool CXMLWriter::WriteEntity(const SXMLEntity &entity){
    return DImplementation->WriteEntity(entity);
};