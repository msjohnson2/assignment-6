#include<utility>
// for std::pair
#include <iostream>
using std::string;

#include"llnode2.hpp"

#include"da6.hpp"


int main()
{

    std::string x; 

    std::pair<int, double> pair = {5, 22.3};

    std::cout << pair.first << " " << pair.second << std::endl;

    SlowMap<int, string> m;
    m.set(2, "hello");
    m.set(4, "hi");
    m.set(6, "hi");
    m.set(6,"wack");

    try
    {
        x = m.get(7);
    }   
    catch(const std::out_of_range& e)
    {
        std::cout << "caught";
    }

    m.erase(3);

    std::cout << m.size() << std::endl;

    try
    {
        std::cout << m.get(2) << std::endl;

    }   
    catch(const std::out_of_range& e)
    {
        std::cout << "caught";
    }

    return 0;
}