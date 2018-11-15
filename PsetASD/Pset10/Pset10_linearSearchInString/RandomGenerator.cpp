#include "RandomGenerator.hpp"
#include <ctime>
#include <chrono>
using namespace  std;
RandomGenerator::RandomGenerator()
{
    random_device rand_dev;
    engine = default_random_engine();
    engine.seed(clock());
}

string RandomGenerator::generateName(size_t minSize,size_t maxSize,char min,char max)
{
    string name;
    uniform_int_distribution<char> charDist(min,max);
    size_t size = generateCount(minSize,maxSize);
    for(size_t i =0; i < size;++i)
    {
        name  += charDist(engine);
    }
    return name;
}
string RandomGenerator::generateName(size_t size,char min,char max)
{
    string name;
    uniform_int_distribution<char> charDist(min,max);
    for(size_t i =0; i < size;++i)
    {
        name  += charDist(engine);
    }
    return name;
}
size_t RandomGenerator::generateCount(size_t min, size_t max)
{
    return  uniform_int_distribution<size_t>(min,max)(engine);
}
