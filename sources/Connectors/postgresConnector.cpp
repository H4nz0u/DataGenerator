#include "postgresConnector.h"
#include <cassert>
PostgresConnector::PostgresConnector(std::string host, int p, std::string username, std::string pwd, std::string dbname) : ip(host), port(p), username(username), password(pwd), dbname(dbname), connection_(get_connection_string()){
    assert(is_connected() && "establishing a connection failed");
};

PostgresConnector::PostgresConnector(std::string url): connection_(url){
    assert(is_connected() && "establishing a connection failed");
};

/** 
 * @return a string used to connect to the database consisting of the relevant parameters
 * */
std::string PostgresConnector::get_connection_string()
{
    std::stringstream connection_ss;
    connection_ss << "user=" << username << " host=" << ip << " password=" << password << " dbname=" << dbname << " port=" << port;
    return connection_ss.str();
}

void PostgresConnector::close_connection(){
    connection_.close();
}


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