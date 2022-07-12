#ifndef __STRING_GENERATOR_DEF__
#define __STRING_GENERATOR_DEF__

#include <string>
#include <iostream>
#include "uniformGenerator.h"

class StringGenerator : public Generator
{
private:
    UniformGenerator<int> intGen;
    UniformGenerator<int> lengthGen;
    const char charset[63] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

public:
    /**
     * @brief Construct a new Stringgenerator object
     *        The int generator is used get a random index of the charset
     *        The length generator is used to get a random length between min and max
     *
     * @param min lowest lenght the generator can create
     * @param max highest length the generator can create
     * @param seed used to seed the generators
     */
    StringGenerator(int min, int max, int seed) : intGen(min, max, seed), lengthGen(0, 61, seed){};
    /**
     * @brief Construct a new Stringgenerator object
     *        The int generator is used get a random index of the charset
     *        The length generator is used to get a random length between min and max
     *
     * @param min lowest lenght the generator can create
     * @param max highest length the generator can create
     */
    StringGenerator(int min, int max) : intGen(min, max), lengthGen(0, 61){};
    ~StringGenerator(){};
    /**
     * @brief returns a random string between the length min and max with the elements of charset
     * 
     * @return std::string 
     */
    virtual std::string getRandom();
};
#endif