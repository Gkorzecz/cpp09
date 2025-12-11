#include "BitcoinExchange.hpp"
#include <iostream>
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage : \"./btc input.txt\"" << std::endl;
        return (1);
    }

    try
    {
        BitcoinExchange btc("data.csv");
        btc.processInputFile(argv[1]);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return (1);
    }
    return (0);
}
