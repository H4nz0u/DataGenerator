#include "manager.h"
#include <string>
#include <variant>
#include <vector>
#include <iostream>
#include <limits.h>

/**
 * @brief format options:
 * %f.3 for float with precision 3
 * %i for integer
 * %s for string
 *
 * x<%i<=y for integer < x and >= y
 * x<%s<y for string with length between x and y
 * ; as seperator
 */
std::vector<std::string> Manager::generateRow(std::list<Generator *> &generators)
{
    std::vector<std::string> result;
    for (auto generator : generators)
    {
        result.push_back(generator->getRandom());
    }
    return result;
};

void Manager::interpretFormat(std::string format, std::list<Generator *> &generators, int seed)
{
    if (format.back() != ';')
        format.append(";");
    size_t start = 0;
    size_t end = format.find(";");

    while (end != std::string::npos)
    {
        std::string sub_string = format.substr(start, end - start);
        size_t pos = sub_string.find("%i");
        if (pos != std::string::npos)
        {
            std::pair<int, int> borders = get_min_max(sub_string, "%i");
            UniformGenerator<int> *g = new UniformGenerator<int>(borders.first, borders.second, seed + generators.size());
            generators.push_back(g);
        }
        else
        {
            size_t pos = sub_string.find("%f");
            if (pos != std::string::npos)
            {
                std::pair<float, float> borders = get_min_max(sub_string, "%f");
                size_t dot_position = sub_string.find(".");
                if (dot_position != std::string::npos)
                {
                    size_t end_pos = sub_string.find("<");
                    if (end_pos == std::string::npos)
                        end_pos = sub_string.find(";");
                    int precision = std::stoi(sub_string.substr(dot_position + 1, end_pos - 2));
                    UniformGenerator<float> *g = new UniformGenerator<float>(borders.first, borders.second, seed + generators.size());
                    g->setPrecision(precision);
                    generators.push_back(g);
                }
                else
                {
                    UniformGenerator<float> *g = new UniformGenerator<float>(borders.first, borders.second, seed + generators.size());
                    generators.push_back(g);
                }
            }
            else
            {
                size_t pos = sub_string.find("%s");
                if (pos != std::string::npos)
                {
                    std::pair<int, int> borders = get_min_max(sub_string, "%s");
                    if (borders.first < 0)
                    {
                        borders.first = 0;
                        std::cout << "The Minimum border for the Stringgenerator was < 0 and therefor set to 0" << std::endl;
                    }
                    if (borders.second > 50)
                    {
                        borders.second = 50;
                        std::cout << "The Maximum border for the Stringgenerator was > 50 and therefor set to 50" << std::endl;
                    }
                    StringGenerator *g = new StringGenerator(borders.first, borders.second, seed + generators.size());
                    generators.push_back(g);
                }
            }
        }
        start = end + 1;
        end = format.find(";", start);
    }
}

std::pair<float, float> Manager::get_min_max(std::string sub_string, std::string type)
{
    float min = 0, max = __FLT_MAX__;
    size_t pos = sub_string.find(type);
    size_t less_pos = sub_string.find("<");
    if (less_pos != std::string::npos)
    {
        if (less_pos < pos)
        {
            min = std::stoi(sub_string.substr(0, less_pos));
            size_t max_pos = sub_string.find("<", less_pos + 1);
            if (max_pos != std::string::npos)
            {
                if (max_pos > pos)
                {
                    max = std::stof(sub_string.substr(max_pos + 1, sub_string.size()));
                }
            }
        }
        else
        {
            if (less_pos != std::string::npos)
            {
                if (less_pos > pos)
                {
                    max = std::stof(sub_string.substr(less_pos + 1, sub_string.size()));
                }
            }
        }
    }
    return std::make_pair(min, max);
}

std::vector<std::string> Manager::generateRow(Generator &generator, int amount_of_columns)
{
    std::vector<std::string> result;
    for (int i = 0; i < amount_of_columns; i++)
    {
        result.push_back(generator.getRandom());
    }
    return result;
};
std::list<std::vector<std::string>> Manager::generateEntry(Generator &generator, int amount_of_entries, int amount_of_columns)
{
    std::list<std::vector<std::string>> result;
    for (int i = 0; i < amount_of_entries; i++)
    {
        result.push_back(generateRow(generator, amount_of_columns));
    }
    return result;
};

std::list<std::vector<std::string>> Manager::generateEntry(std::string format, int amount_of_entries, int seed)
{
    std::list<std::vector<std::string>> result;
    std::list<Generator *> generators;
    interpretFormat(format, generators, seed);
    for (int i = 0; i < amount_of_entries; i++)
    {
        result.push_back(generateRow(generators));
    }
    return result;
}