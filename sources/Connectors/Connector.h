#ifndef __CONNECTOR_IMPORT__
#define __CONNECTOR_IMPORT__
#include <pqxx/pqxx>
#include <string>
#include <cassert>
#include <iostream>

//to get pqxx to work: brew install version 7+, make sure to expand the include path and make a symbolic link from {pqxx_path}/lib/libpqxx-7.7.so to /usr/lib/libpqxx-7.7.so
typedef std::variant<int, float, uint64_t, std::string> value;
class Connector
{
private:
    Connector(const Connector &other) = delete;             // prevent copying
    Connector &operator=(const Connector &other) = delete;  // prevent assignments
    Connector &operator=(const Connector &&other) = delete; // prevent move
public:
    Connector(){};
    ~Connector(){};
    virtual void insert_data(std::list<std::vector<value>> data, std::string dbname) = 0;
};
#endif