#include "stringGenerator.h"

value StringGenerator::getRandom(int min, int max, bool deterministic){
    int length = std::get<int>(intGen.getRandom(min, max, deterministic));
    std::string result;
    result.reserve(length);
    for(int i=0; i < length; i++){
        result += charset[std::get<int>(intGen.getRandom(0, 63, deterministic))];
    }
    return result;
}