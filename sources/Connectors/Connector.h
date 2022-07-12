#ifndef __CONNECTOR_IMPORT__
#define __CONNECTOR_IMPORT__
#include <pqxx/pqxx>
#include <string>
#include <cassert>
#include <iostream>

//to get pqxx to work: brew install version 7+, make sure to expand the include path and make a symbolic link from {pqxx_path}/lib/libpqxx-7.7.so to /usr/lib/libpqxx-7.7.so
class Connector
{
private:
    Connector(const Connector &other) = delete;             // prevent copying
    Connector &operator=(const Connector &other) = delete;  // prevent assignments
    Connector &operator=(const Connector &&other) = delete; // prevent move
public:
    Connector(){};
    ~Connector(){};
    /**
     * @brief pure virtual function working as a template to guarantee all connectors have that function
     * 
     * @param data 
     * @param locaion can be either a database table name or a filepath
     */
    virtual void insert_data(std::list<std::vector<std::string>> data, std::string locaion) = 0;
};
#endif