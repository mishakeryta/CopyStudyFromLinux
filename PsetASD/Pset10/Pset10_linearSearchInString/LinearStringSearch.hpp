#ifndef LINEARSTRINGSEARCH_HPP
#define LINEARSTRINGSEARCH_HPP

#include <string>
#include <vector>
class LinearStringSearch
{

public:
    static std::vector<size_t> Find(const std::string& line, const std::string& desired);
    static std::vector<size_t> Find(const std::string& line, const std::string& desired,std::string& output);
};

#endif // LINEARSTRINGSEARCH_HPP
