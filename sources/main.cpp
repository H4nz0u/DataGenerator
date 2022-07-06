#include "boost/program_options.hpp"
#include <iostream>
#include "dataGenerator/uniformGenerator.h"
#include "Connectors/postgresConnector.cpp"
#include "Connectors/CsvConnector.h"
#include "dataGenerator/formatInterpreter.h"
#include "dataGenerator/stringGenerator.cpp"
namespace po = boost::program_options;
typedef std::variant<int, float, uint64_t, std::string> value;
int main(int argc, char *argv[])
{
    int seed;
    bool deterministic;
    int n_columns;
    int n_entries;
    int min;
    int max;
    std::string url;
    po::options_description desc{"Options"};
    desc.add_options()("help,h", "produce help message")("min", po::value<int>(&min)->default_value(0), "sets the min value")("max", po::value<int>(&max)->default_value(INT_MAX), "sets max value")("amount_of_columns", po::value<int>(&n_columns)->default_value(0), "set the amount of columns")("amount_of_entries", po::value<int>(&n_entries)->default_value(0), "set the amount of entries")("deterministic", po::value<bool>(&deterministic)->default_value(true), "whether or not the data generation is supposed to be deterministic")("seed", po::value<int>(&seed)->default_value(0), "the seed the deterministic data generator should use")("url", po::value<std::string>(&url), "the connection string to the database");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help") > 0)
    {
        std::cout << desc << "\n";
        return 0;
    }

    UniformGenerator<float> floatGenerator(seed);
    std::cout << "Random float: " << std::get<float>(floatGenerator.getRandom(min, max, deterministic)) << std::endl;

    UniformGenerator<int> intGenerator(seed);
    std::cout << "Random int: " << std::get<int>(intGenerator.getRandom(min, max, deterministic)) << std::endl;

    UniformGenerator<uint64_t> uint64_tGenerator(seed);
    std::cout << "Random uint64_t: " << std::get<uint64_t>(uint64_tGenerator.getRandom(min, max, deterministic)) << std::endl;

    /*
    PostgresConnector pc(url);
    // PostgresConnector pc(std::string("localhost"), 5432, std::string("postgres"), std::string("toor"), std::string("db_test"));
    std::list<std::vector<value>> to_be_inserted;
    for(int i = 0; i < n_entries; i++){
        to_be_inserted.push_back(intGenerator.generateEntry(n_columns, min, max, deterministic));
    }
    pc.insert_data(to_be_inserted, std::string("data"));

    CsvConnector csv;
    csv.insert_data(to_be_inserted, "test.csv");
    */

    FormatInterpreter fi(std::string("5<%i<200;3<%s<11;12,1<%f.2<20,94232321;%i;%i;%i"));
    std::vector<value> result = fi.generateRow();
    for (auto entry : result){
        try{
            std::cout << "int: " << std::get<int>(entry);
        }catch(const std::bad_variant_access& ex){
            try{
                std::cout << std::cout.precision(12) << "float: " << std::get<float>(entry);
            }catch(const std::bad_variant_access& ex){
                std::cout << "string: " << std::get<std::string>(entry);
            }
     
        }
        std::cout << std::endl;
    }

    return 0;
}