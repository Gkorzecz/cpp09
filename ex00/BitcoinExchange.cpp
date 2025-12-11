#include "BitcoinExchange.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange(const std::string& dbFile)
{
    loadDatabase(dbFile);
}

BitcoinExchange::~BitcoinExchange()
{}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
    this->_exchangeRates = other._exchangeRates;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
    {
        this->_exchangeRates = other._exchangeRates;  
    }
    return (*this);
}

const char *BitcoinExchange::OpenFileExc::what() const throw()
{
    return ("Error : could not open file");
}

const char *BitcoinExchange::InvalidRateExc::what() const throw()
{
    return ("Error : no valid rate for this date");
}

std::string BitcoinExchange::trim(const std::string& s)
{
    const std::size_t first = s.find_first_not_of(" ");
    if (first == std::string::npos)
        return ("");
    const std::size_t last  = s.find_last_not_of(" ");
    return (s.substr(first, last - first + 1));
}

bool BitcoinExchange::isValidDate(const std::string& d)
{
    if (d.size() != 10 || d[4] != '-' || d[7] != '-')
        return (false);

    int y, m, day;
    char c1, c2;
    int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    std::istringstream ss(d);
    if (!(ss >> y >> c1 >> m >> c2 >> day))
        return (false);

    if (m < 1 || m > 12 || y < 1)
        return (false);

    if (day > mdays[m - 1] || day < 1)
        return (false);
    return (true);
}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file)
        throw OpenFileExc();

    std::string line;
    std::string date;
    double rate;
    // if (std::getline(file, line) && line != "date,exchange_rate")
    // {
    //     std::istringstream ss(line);
    //     if (std::getline(ss, date, ',') && (ss >> rate))
    //         _exchangeRates[date] = rate;
    // }
    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        if (std::getline(ss, date, ',') && (ss >> rate))
            _exchangeRates[date] = rate;
    }
}

double BitcoinExchange::getRate(const std::string& date) const
{
    std::map<std::string,double>::const_iterator it = _exchangeRates.find(date);
    if (it != _exchangeRates.end())
        return (it->second);

    it = _exchangeRates.lower_bound(date);
    if (it == _exchangeRates.begin())
        throw std::runtime_error("Error: no rate available before " + date);
    --it;
    return (it->second);
}

void BitcoinExchange::processInputFile(const std::string& inputFile) const
{
    std::ifstream file(inputFile.c_str());
    if (!file)
        throw OpenFileExc();

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        size_t sep = line.find('|');
        if (sep == std::string::npos)
        {
            std::cout << "Error: bad input => " << line << '\n';
            continue;
        }

        const std::string date = trim(line.substr(0, sep));
        const std::string valS = trim(line.substr(sep + 1));
        if (!isValidDate(date))
        {
            std::cout << "Error: bad input => " << line << '\n';
            continue;
        }

        std::istringstream vs(valS);
        double value;
        char extra;
        if (!(vs >> value) || vs >> extra)
        {
            std::cout << "Error: bad input => " << line << '\n';
            continue;
        }
        if (value < 0)
        {
            std::cout << "Error: not a positive number.\n";
            continue;
        }
        if (value > 1000)
        {
            std::cout << "Error: too large a number.\n";
            continue;
        }
        try
        {
            const double rate = getRate(date);
            const double result = rate * value;
            std::cout << std::setprecision(10) << date << " => " << valS << " = " << result << '\n';
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}
