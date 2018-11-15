#include "LinearStringSearch.hpp"
using namespace std;
vector<size_t> LinearStringSearch::Find(const string& line,const string& desired)
{
    vector<size_t> result;
    for(size_t i = 0; i < line.size() - desired.size()+1;++i)
    {
        bool equal = true;
        for(size_t j = 0; j < desired.size();++j)
        {
            if(desired[j] != line[i+j])
            {
                equal = false;
                break;
            }
        }
        if(equal)
        {
            result.push_back(i);
        }
    }
    return result;
}
vector<size_t> LinearStringSearch::Find(const string& line,const string& desired,string& output)
{
    output+="Search of all \"" +desired+"\" in string \""+line + "\"\n";
    vector<size_t> result;
    for(size_t i = 0; i < line.size() - desired.size()+1;++i)
    {
        string first,second;
        for(size_t j = 0; j<desired.size();++j)
        {
            first+=desired[j];
            second+=line[i+j];
        }
        output+= "Step "  + to_string(i+1) + " Comparing \"" +first + "\" adn \"" + second+"\"\n";
        if(first==second)
        {
            output+="They are equal. Adding index of \""+desired+"\"" + to_string(i+1)+"\n";
            result.push_back(i);
        }
    }
    return result;
}
