#include "postgresConnector.h"
#include <cassert>

PostgresConnector::PostgresConnector(std::string url): connection_(url){
    assert(is_connected() && "establishing a connection failed");
};


std::vector<std::string> PostgresConnector::get_column_names(std::string dbname){
    connection_.prepare("get_columns", "SELECT * FROM INFORMATION_SCHEMA.COLUMNS where TABLE_NAME=$1::text;");
    pqxx::work w(connection_);
    pqxx::result r;
    r = w.exec_prepared("get_columns", dbname);
    std::vector<std::string> result;
    for (int i = 0; i < r.size(); i++){
        result.push_back(r[i][3].as<std::string>());
    }
    return result;
}