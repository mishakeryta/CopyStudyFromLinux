#ifndef RANDOMGENERATOR_HPP
#define RANDOMGENERATOR_HPP
#include <random>
#include <string>
class RandomGenerator
{
public:
    RandomGenerator();
    std::string generateName(size_t minSize,size_t maxSize,char min = 'a', char max = 'z');
    std::string generateName(size_t size,char min = 'a', char max = 'z');
    size_t generateCount(size_t min, size_t max);
private:
    std::default_random_engine engine;

};

#endif // RANDOMGENERATOR_HPP
