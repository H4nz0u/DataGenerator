#include <string>
#include <variant>
#include <vector>
#include <iostream>
#include "uniformGenerator.h"
#include "stringGenerator.h"
typedef std::variant<int, float, uint64_t, std::string> value;

class FormatInterpreter
{
private:
    std::string format;

public:
    FormatInterpreter(std::string format) : format(format){};
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
    std::vector<value> generateRow()
    {
        if (format.back() != ';')
            format.append(";");
        size_t start = 0;
        size_t end = format.find(";");

        std::vector<value> result;
        while (end != std::string::npos)
        {
            int min = 0, max = 0;
            std::string sub_string = format.substr(start, end - start);
            size_t pos = sub_string.find("%i");
            if (pos != std::string::npos)
            {
                std::pair<int, int> borders = get_min_max(sub_string, "%i");
                UniformGenerator<int> intGen;
                result.push_back(intGen.getRandom(borders.first, borders.second, true));
                std::cout
                    << "min = " << borders.first << " max = " << borders.second << std::endl;
            }
            else
            {
                size_t pos = sub_string.find("%f");
                if (pos != std::string::npos)
                {
                    std::pair<float, float> borders = get_min_max_float(sub_string);
                    UniformGenerator<float> floatGen;
                    std::cout << "min = " << borders.first << " max = " << borders.second << std::endl;

                    size_t dot_position = sub_string.find(".");
                    if (dot_position != std::string::npos)
                    {
                        size_t end_pos = sub_string.find("<");
                        if (end_pos == std::string::npos)
                            end_pos = sub_string.find(";");
                        for (int64_t i = 0; i < 100000000; i++)
                            ;
                        int precision = std::stoi(sub_string.substr(dot_position + 1, end_pos - 2));
                        float number = std::round(std::get<float>(floatGen.getRandom(borders.first, borders.second, true)) * std::pow(10, precision)) / std::pow(10, precision);
                        result.push_back(number);
                    }
                    else
                    {
                        result.push_back(floatGen.getRandom(borders.first, borders.second, true));
                    }
                }
                else
                {
                    size_t pos = sub_string.find("%s");
                    if (pos != std::string::npos){
                        std::pair<int, int> borders = get_min_max(sub_string, "%s");
                        StringGenerator sg;
                        std::cout << "substring" << sub_string << std::endl;
                        std::cout << "min = " << borders.first << " max = " << borders.second << std::endl;
                        result.push_back(sg.getRandom(borders.first, borders.second, true));
                    }
                }
            }
            start = end + 1;
            end = format.find(";", start);
        }
        return result;
    };

    std::pair<int, int> get_min_max(std::string sub_string, std::string type)
    {
        int min = 0, max = INT_MAX;
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
                        max = std::stoi(sub_string.substr(max_pos + 1, sub_string.size()));
                    }
                }
            }
            else
            {
                if (less_pos != std::string::npos)
                {
                    if (less_pos > pos)
                    {
                        max = std::stoi(sub_string.substr(less_pos + 1, sub_string.size()));
                    }
                }
            }
        }
        return std::make_pair(min, max);
    }
    std::pair<float, float> get_min_max_float(std::string sub_string)
    {
        float min = 0, max = FLT_MAX;
        size_t pos = sub_string.find("%f");
        size_t less_pos = sub_string.find("<");
        if (less_pos != std::string::npos)
        {
            if (less_pos < pos)
            {
                min = std::stof(sub_string.substr(0, less_pos));
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
};