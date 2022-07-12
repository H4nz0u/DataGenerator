#include "uniformGenerator.h"
#include "stringGenerator.h"
#include <list>
class Manager
{
private:
    std::pair<float, float> get_min_max(std::string sub_string, std::string type);

public:
    Manager(){};
    /**
     * @brief generates amount_of_columns random numbers using the generator reference
     * 
     * @param generator 
     * @param amount_of_columns 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> generateRow(Generator& generator, int amount_of_columns);
    /**
     * @brief uses the reference to the generators list to create and insert generators with the specified parameters from the format string
     * 
     * @param format the format string
     * @param generators reference to a list of generators, reference because otherwise the list and all elements are being copied, destroying the vtable
     * @param seed seed for the generators
     */
    void interpretFormat(std::string format, std::list<Generator*> &generators, int seed);
    /**
     * @brief gets a list of generators and uses them to create random values in order of insertion
     * 
     * @param generators 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> generateRow(std::list<Generator*> &generators);
    /**
     * @brief used when no format string is specified and all data is of an equal type and therefor uses a single generator
     *        internally loops amount_of_entries times and calls generateEntry with the generator
     * 
     * @param generator 
     * @param amount_of_entries 
     * @param amount_of_columns 
     * @return std::list<std::vector<std::string>> 
     */
    std::list<std::vector<std::string>> generateEntry(Generator& generator, int amount_of_entries, int amount_of_columns);
    /**
     * @brief used when the format string is specified and calls interpretFormat and uses the generator list to then call 
     * generateEntry with it
     * 
     * @param format format string
     * @param amount_of_entries 
     * @param seed 
     * @return std::list<std::vector<std::string>> 
     */
    std::list<std::vector<std::string>> generateEntry(std::string format, int amount_of_entries, int seed);
};