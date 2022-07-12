#include "stringGenerator.h"

std::string StringGenerator::getRandom(){
    int length = std::stoi(intGen.getRandom());
    std::string result;
    result.reserve(length);
    for(int i=0; i < length; i++){
        result += charset[std::stoi(lengthGen.getRandom())];
    }
    return result;
}