#ifndef __UNIFORM_GENERATOR_DEFINITION__
#define __UNIFORM_GENERATOR_DEFINITION__
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

    int precision = -1;
    std::random_device nondeterministic_generator;
    std::mt19937 deterministic_generator;
    uniform_distribution distribution;

public:
    ~UniformGenerator(){};
    /**
     * @brief Construct a new Uniform Generator object with the type T
     * 
     * @param min lowest value the generator can create
     * @param max highest value the generator can create
     * @param seed the starting seed for the generator
     */
    UniformGenerator(D min, D max, int seed) : distribution(min, max), deterministic_generator(seed){};
    /**
     * @brief Construct a new Uniform Generator object with the type T
     *        The deterministic generator that is used to create the random values is seeded with a true random number
     * 
     * @param min lowest value the generator can create
     * @param max highest value the generator can create
     */
    UniformGenerator(D min, D max) : distribution(min, max), deterministic_generator(nondeterministic_generator()){};
    /**
     * @brief Set the precision, -1 indicates no value is set
     * 
     * @param p amount of digits after the comma
     */
    void setPrecision(int p)
    {
        precision = p;
    }
    /**
     * @brief returns a random number as string with the precision p if it is specified (!=-1)
     * 
     * @return std::string 
     */
    virtual std::string getRandom()
    {
        deterministic_generator.discard(deterministic_generator.state_size);
        D result = distribution(deterministic_generator);
        if (precision > -1)
        {
            result = std::round(result * std::pow(10, precision)) / std::pow(10, precision);
        }
        return std::to_string(result);
    };
};
#endif