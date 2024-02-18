#include "DSVReader.h"
#include <StringDataSource.h>

struct CDSVReader::SImplementation{
    std::shared_ptr<CDataSource> DDataSource;
    char Ddelimiter;

    SImplementation(std::shared_ptr< CDataSource > src,char delim){
        DDataSource=src;
        Ddelimiter=delim;
        if(Ddelimiter=='\"')
            Ddelimiter=',';
    };

    bool End() const{
        return DDataSource->End();
    };

    bool ReadRow(std::vector<std::string> &row){
        char TempC;
        std::string TempS="";
        bool IsQuoted=false;

        while(DDataSource->Get(TempC)){
            
            //if current status is unquoted and a quote appears
            //it means that the content after it should be quoted

            if(TempC=='\"'&&!IsQuoted){
                IsQuoted=true;
                continue;         //continue to avoid the quote to be record into TempS
            }

            //if current status is quoted and a quote appears
            //it means that it is whether a quote sign in the content or it is the end of the content

            else if(TempC=='\"'&&IsQuoted){

            //We can get the next character to determine what to do next if we could, else it is the end of quote

                if(DDataSource->Get(TempC)){

            //if the next character is not a quote, that means this is the end of quote and it shoule be a delimiter which could 
            //be handled by the code block below
                if(TempC!='\"'){
                    IsQuoted=false;
                    if(TempC==Ddelimiter){
                        row.push_back(TempS);
                        TempS="";
                    }
                    else
                        TempS+=TempC;
                    continue;
                }
            //if it is a quote, then simply add the character " to TempS 
                else
                    TempS+=TempC;
                    continue;
                }

                else{
                    IsQuoted=false;
                    break;
                }
            }
            
            //when an unquoted delimiter appears, we push TempS to row and clear Temps
            if(TempC==Ddelimiter&&!IsQuoted){
                row.push_back(TempS);
                TempS="";
                continue;
            }

            //when an unquoted newline apppears, that's the end of row and we break the loop
            else if(TempC=='\n'&&!IsQuoted){
                break;
            }

            TempS+=TempC;
        }
        

        //it is illegal when there is a open quote when the loop is broke
        if(IsQuoted==true){
            return false;
        }

        //output last TempS
        row.push_back(TempS);

        return true;
    }
};

CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src, delimiter);
};


CDSVReader::~CDSVReader(){

};

bool CDSVReader::End() const{
    return DImplementation->End();
};


bool CDSVReader::ReadRow(std::vector<std::string> &row){
    return DImplementation->ReadRow(row);

};
