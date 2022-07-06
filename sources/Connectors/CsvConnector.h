#include "Connector.h"
#include <fstream>  
class CsvConnector: public Connector
{
    public:
        CsvConnector(){};
        virtual void insert_data(std::list<std::vector<value>> data, std::string filepath){ 
            std::ofstream file(filepath);
            for(auto entry: data){
                for (auto v : entry){
                    try{
                        file << std::get<int>(v) << ",";
                    }catch (const std::bad_variant_access& ex){
                        try{
                            file << std::get<uint64_t>(v) << ",";
                        }catch(const std::bad_variant_access& ex){
                            file << std::get<float>(v) << ",";
                        }
                    }  
                }
                file.seekp((long)file.tellp() - 1);
                file << std::endl;
            }
            file.close();
        }
};
