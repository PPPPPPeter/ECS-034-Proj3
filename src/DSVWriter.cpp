#include "DSVWriter.h"
#include <StringDataSink.h>

struct CDSVWriter::SImplementation{

    /// initialize Simplementation
    std::shared_ptr<CDataSink> DDataSink;
    char Ddelimiter;
    bool Isquoted;

    SImplementation(std::shared_ptr< CDataSink > sink,char delim, bool quoteall){
        DDataSink=sink;
        Ddelimiter=delim;
        Isquoted=quoteall;
    };

    bool WriteRow(const std::vector<std::string> &row){   
        auto Copy = row;
        size_t pos = 0;
        size_t index = 0;

        for(std::string s:Copy){
            //handling double quotes
            if(s.find('\"')!=std::string::npos){
                pos=s.find('\"',pos);
                while(pos!=std::string::npos){
                    s.insert(pos,1,'\"');
                    pos+=2;
                    pos=s.find('\"',pos);
                }
                s='\"'+s+'\"';
            }
            //other cases that need to quote the data 
            else if (Isquoted||s.find('\n')!=std::string::npos||s.find(Ddelimiter)!=std::string::npos)
            {
                 s='\"'+s+'\"';
            }
            //add delimiter between data
            if (index!=0)
            {
                s=Ddelimiter+s;
            } 
            //push to datasink
            for(size_t i=0;i<s.size();i++){
                DDataSink->Put(s.at(i));
            }

            index++;

        }
        return true;
    }
};

CDSVWriter::CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall){
    DImplementation = std::make_unique<SImplementation>(sink, delimiter, quoteall);
}

CDSVWriter::~CDSVWriter(){

};

bool CDSVWriter::WriteRow(const std::vector<std::string> &row){
    return DImplementation->WriteRow(row);
};