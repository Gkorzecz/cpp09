#include "RPN.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage : ./RPN \"[math expression]\"]" << std::endl;
        return (1);
    }
    RPN calc;
    try
    {
        std::cout << calc.evaluate(argv[1]) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
        return (1);
    }
    return (0);
}
