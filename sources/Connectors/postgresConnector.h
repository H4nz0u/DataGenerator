#include <pqxx/pqxx>
#include <string>
#include <cassert>
#include <iostream>
#include "Connector.h"
//to get pqxx to work: brew install version 7+, make sure to expand the include path and make a symbolic link from {pqxx_path}/lib/libpqxx-7.7.so to /usr/lib/libpqxx-7.7.so
typedef std::variant<int, float, uint64_t, std::string> value;
class PostgresConnector: public Connector
{
private:
    std::string ip;
    int port;
    std::string username;
    std::string password;
    bool ssl;
    std::string dbname;
    pqxx::connection connection_;
    PostgresConnector() = delete;
    PostgresConnector(const PostgresConnector &other) = delete;             // prevent copying
    PostgresConnector &operator=(const PostgresConnector &other) = delete;  // prevent assignments
    PostgresConnector &operator=(const PostgresConnector &&other) = delete; // prevent move
public:
    PostgresConnector(std::string host, int p, std::string username, std::string pwd, std::string dbname);
    PostgresConnector(std::string url);
    ~PostgresConnector(){};
    pqxx::connection &get_connection() { return connection_; };
    void close_connection();
    std::string get_connection_string();
    bool is_connected() const { return connection_.is_open(); }
    /**
     * @brief using a stream to write efficiently to the database
     * 
     * @param data a list of vectors with the numbers to be inserted
     * @param dbname name of the database table
     */
    virtual void insert_data(std::list<std::vector<value>> data, std::string dbname)
    {
        auto columns = get_column_names(dbname);
        assert((*data.begin()).size() == columns.size() && (void*)("amount of data to be inserted does not match the amount of columns in the table"));
        pqxx::work w(connection_);
        pqxx::stream_to stream = pqxx::stream_to::raw_table(w, dbname);
        for (auto entry : data)
            stream.write_values(entry);
        stream.complete();
        w.commit();
    }
    std::vector<std::string> get_column_names(std::string dbname);
};