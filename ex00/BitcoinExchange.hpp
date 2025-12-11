#pragma once

#include <map>
#include <string>
#include <iostream>

class BitcoinExchange
{
    private:
        std::map<std::string, double> _exchangeRates;

        void loadDatabase(const std::string &filename);
        static bool isValidDate(const std::string &date);
        static std::string trim(const std::string &s);

    public:
        BitcoinExchange(const std::string &databaseFile);
        ~BitcoinExchange();
        BitcoinExchange(const BitcoinExchange &other);
        BitcoinExchange &operator=(const BitcoinExchange &other);

        double getRate(const std::string &date) const;
        void processInputFile(const std::string &inputFile) const;

    class OpenFileExc : public std::exception
    {
        public :
            const char* what() const throw();
    };

    class InvalidRateExc : public std::exception
    {
        public :
            const char* what() const throw();
    };
};