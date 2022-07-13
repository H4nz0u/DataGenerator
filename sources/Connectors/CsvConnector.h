#include "Connector.h"
#include <fstream>
class CsvConnector : public Connector
{
public:
    CsvConnector(){};
    /**
     * @brief opens a file in append mode and writes the values seperated by a comma to a stringstream to save io options 
     *        and then writes the stringstream to the file
     * 
     * @param data the data to be written to the file 
     * @param filepath path to the file to be written
     */
    virtual void insert_data(std::list<std::vector<std::string>> data, std::string filepath)
    {
        std::ofstream file;
        file.open(filepath, std::ios_base::app);
        std::stringstream temp;
        for (auto entry : data)
        {
            const char *seperator = std::string("").c_str();
            
            for (auto v : entry)
            {
                temp << seperator << v;
                seperator = std::string(",").c_str();
            }
            temp << "\n";
        }
        file << temp.str();
        file.close();
    }
};
