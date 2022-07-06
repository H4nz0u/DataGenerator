#ifndef __UNIFORM_GENERATOR_DEFINITION__
#define __UNIFORM_GENERATOR_DEFINITION__
#include <iostream>
#include <numeric>
#include <random>
#include <type_traits>
#include "generator.h"
#include <variant>
template <class D>
class UniformGenerator : public Generator
{
private:
    using uniform_distribution =
        typename std::conditional_t<
            std::is_floating_point<D>::value,
            std::uniform_real_distribution<D>,
            typename std::conditional_t<
                std::is_integral<D>::value,
                std::uniform_int_distribution<D>,
                void>>;

    std::mt19937 deterministic_generator;
    std::random_device nondeterministic_generator;

public:
    ~UniformGenerator(){};
    UniformGenerator(int seed) : deterministic_generator(seed){};
    UniformGenerator() : deterministic_generator(0){};
    virtual value getRandom(int min, int max, bool deterministic = true)
    {
        uniform_distribution distribution(min, max);
        value result;
        if (deterministic)
            result = static_cast<D>(distribution(deterministic_generator));
        else
            result = static_cast<D>(distribution(nondeterministic_generator));
        return result;
    };
    std::vector<value> generateEntry(int n_columns, D min, D max, bool deterministic=true){
        std::vector<value> result;
        for (int i = 0; i < n_columns; i++){
            result.push_back(getRandom(min, max, deterministic));
        }
        return result;
    }
};
#endif