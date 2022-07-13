#include "boost/program_options.hpp"
#include <iostream>
#include "dataGenerator/uniformGenerator.h"
#include "Connectors/postgresConnector.cpp"
#include "Connectors/CsvConnector.h"
#include "dataGenerator/manager.cpp"
#include "dataGenerator/stringGenerator.cpp"
#include <regex>
#include <thread>
#include <future>
namespace po = boost::program_options;
int seed;
int n_columns;
int n_entries;
float min;
float max;
int threads;
std::string url;
std::string output;
std::string format;
std::string type;
std::string tablename;
std::string filename;

namespace lib
{
    /**
     * @brief Used to check the connection string with a regex expression for a valid format, need special type since otherwise it checks every string
     * 
     */
    class ConnectionString
    {
    public:
        std::string s;
        ConnectionString(std::string s) : s(s){};
    };
    /**
     * @brief Overwrites the intern validate function of the boost library with my own
     * 
     * @param v 
     * @param values 
     * @param target_type 
     */
    void validate(boost::any &v,
                  const std::vector<std::string> &values,
                  ConnectionString *target_type, int)
    {
        static std::regex r("(user=[a-zA-Z0-9]* host=([a-zA-Z]*|[0-9.]*) password=.* dbname=.* port=[0-9]{1,5})");

        po::validators::check_first_occurrence(v);
        const std::string &s = po::validators::get_single_string(values);
        std::smatch match;
        if (std::regex_match(s, match, r))
        {

            v = boost::any(ConnectionString((match[1])).s);
        }
        else
        {
            throw po::validation_error(po::validation_error::invalid_option_value);
        }
    }
}
/**
 * @brief used the boost libary to parse the command line arguments, it uses a lot of references to global arguments for easier access
 * 
 * @param argc amount of command line arguments
 * @param argv list of the arguments
 * @return po::variables_map which is a map-like structure with the keys and values
 */
po::variables_map parseCommandLine(int argc, char *argv[])
{
    po::options_description desc{"General Options"};
    desc.add_options()("help,h", "produce help message")("output,o", po::value<std::string>(&output)->required(), "currently supported formats: postgresql or csv")("filename", po::value<std::string>(&filename), "the name of the file")("threads,t", po::value<int>(&threads)->default_value(1), "The amount of Threads to be used\nCAREFUL! Multiple Threads break the deterministic behaviour");
    po::options_description generation{"Generation Options"};
    generation.add_options()("type", po::value<std::string>(&type)->required(), "The type of data to be generated: \n\tstring\n\tint\n\tfloat\nformat")("min", po::value<float>(&min)->default_value(INT_MIN), "sets the min value of all numbers or the minimum length of the strings to be generated")("max", po::value<float>(&max)->default_value(INT_MAX), "sets the max value of all numbers or the maximum length of the strings to be generated")("amount_of_columns,nc", po::value<int>(&n_columns)->required(), "set the amount of columns")("amount_of_entries,ne", po::value<int>(&n_entries)->required(), "set the amount of entries")("seed,s", po::value<int>(&seed), "the seed the deterministic data generator should use")("format,f", po::value<std::string>(&format), "Specifies an individual format");
    po::options_description connection{"Connection Options"};
    connection.add_options()("url", po::value<lib::ConnectionString>(), "the connection string to the database\n\t'user= {username} host={ip of db} password={password for user} dbname={database name} port={port of db}'")("user,u", po::value<std::string>(), "The user for the database")("host", po::value<std::string>(), "The ip of the database")("password, p", po::value<std::string>(), "The password for the user")("dbname", po::value<std::string>(), "The name of the database")("tablename", po::value<std::string>(&tablename), "The name of the table")("port", po::value<int>(), "The port of the database");
    po::options_description total{};
    total.add(desc).add(connection).add(generation);
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, total), vm);

    if (vm.count("help") > 0)
    {
        std::cout << total << "\n";
        exit(-1);
    }

    po::notify(vm);
    if (vm["output"].as<std::string>() != std::string("postgresql") && vm["output"].as<std::string>() != std::string("csv"))
    {
        std::cout << "The output format must be either postgresql or csv"
                  << "\n";
        exit(-1);
    }
    else
    {
        if (vm["output"].as<std::string>() == std::string("postgresql"))
        {
            if (vm.count("url") == 0)
            {
                if (vm.count("username") && vm.count("host") && vm.count("password") && vm.count("port") && vm.count("dbname"))
                {
                    std::stringstream crafted_url;
                    crafted_url << "username=" << vm["username"].as<std::string>() << " ";
                    crafted_url << "host=" << vm["host"].as<std::string>() << " ";
                    crafted_url << "password=" << vm["password"].as<std::string>() << " ";
                    crafted_url << "dbname=" << vm["dbname"].as<std::string>() << " ";
                    crafted_url << "port=" << vm["port"].as<std::string>() << " ";
                    url = crafted_url.str();
                }
                else
                {
                    std::cout << "The connection is not properly specified!" << std::endl;
                    std::cout << connection << std::endl;
                    exit(-1);
                }
            }
            else
            {
                url = vm["url"].as<std::string>();
            }
            if (vm.count("tablename") == 0)
            {
                std::cout << "The name of the table the data is supposed to be inserted has to be specified!" << std::endl;
                std::cout << connection << std::endl;
                exit(-1);
            }
        }
        else
        {
            if (vm["output"].as<std::string>() == "csv")
            {
                if (vm.count("filename") == 0)
                {
                    std::cout << "Please specify the name of the output file!" << std::endl;
                    std::cout << desc << std::endl;
                    exit(-1);
                }
            }
            else
            {
                std::cout << "The output type is invalid!" << std::endl;
                std::cout << desc << std::endl;
                exit(-1);
            }
        }
    }
    if (vm["type"].as<std::string>() != "string" && vm["type"].as<std::string>() != "int" && vm["type"].as<std::string>() != "float" && vm["type"].as<std::string>() != "format")
    {
        std::cout << "The type is invalid! It has to be 'string', 'int', 'float' or 'format'." << std::endl;
        std::cout << generation << std::endl;
        exit(-1);
    }
    return vm;
}

/**
 * @brief Method that is handed over to the Thread to generate the entries from a given Format string
 * 
 * @param temp_seed each Thread needs a own seed, otherwise all threads generate the same numbers
 * @param format the format string used to generate the data
 */
void generateRandomEntriesFormat(int temp_seed, std::string format)
{
    Manager fi;
    std::list<std::vector<std::string>> temp = fi.generateEntry(format, n_entries / threads, temp_seed);
    Connector * c;
    std::string name;
    if (output == "postgresql")
    {
        c = new PostgresConnector(url);
        name = tablename;
    }
    else
    {
        c = new CsvConnector;
        name = filename;
    }
    c->insert_data(temp, name);
}
/**
 * @brief Method that is handed over to the Thread to generate the entries from a given type string
 * 
 * @param temp_seed each Thread needs a own seed, otherwise all threads generate the same numbers
 * @param type the type string used to create the generator
 */
void generateRandomEntriesType(int temp_seed, std::string type)
{
    Manager fi;
    Generator *generator;
    if (type == "int"){
        generator = new UniformGenerator<int>(min, max, temp_seed);
    }else{
        if (type == "float"){
            generator = new UniformGenerator<float>(min, max, temp_seed);
        }else{
            generator = new StringGenerator(min, max, temp_seed);
        }
    }
    std::list<std::vector<std::string>> temp = fi.generateEntry(*generator, n_entries / threads, n_columns);
    if (output == "postgresql")
    {
        PostgresConnector pc(url);
        pc.insert_data(temp, tablename);
    }
    else
    {
        CsvConnector csv;
        csv.insert_data(temp, filename);
    }
}

/**
 * @brief Interprets the command line arguments, creates the threads and joins them after completion
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    po::variables_map vm = parseCommandLine(argc, argv);
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    start_time = std::chrono::steady_clock::now();
    std::thread thread_list[threads];
    UniformGenerator<int> *intGenerator;
    if (vm.count("seed") == 0)
        intGenerator = new UniformGenerator<int>(INT_MIN, INT_MAX);
    else
        intGenerator = new UniformGenerator<int>(INT_MIN, INT_MAX, seed);
    if (output == "csv")
        remove(filename.c_str());

    for (int i = 0; i < threads; i++)
    {
        int temp_seed = std::stoi(intGenerator->getRandom());
        if (vm["type"].as<std::string>() == "format")
            thread_list[i] = std::thread(generateRandomEntriesFormat, temp_seed, format);
        else
            thread_list[i] = std::thread(generateRandomEntriesType, temp_seed, type);
    }
    delete intGenerator;
    for (int i = 0; i < threads; i++)
    {
        thread_list[i].join();
    }
    end_time = std::chrono::steady_clock::now();
    std::cout << "Data Generation took " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << " milliseconds to run" << std::endl;
    return 0;
}
