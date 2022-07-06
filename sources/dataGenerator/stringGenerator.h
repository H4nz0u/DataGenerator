#ifndef __STRING_GENERATOR_DEF__
#define __STRING_GENERATOR_DEF__

#include <string>
#include <iostream>
#include "uniformGenerator.h"

class StringGenerator: public Generator{
    private:
        UniformGenerator<int> intGen;
        const char charset[63] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    public:
        StringGenerator(){};
        virtual value getRandom(int min, int max, bool deterministic=true);
};
#endif