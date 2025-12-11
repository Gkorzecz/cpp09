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

std::string BitcoinExchange::trim(const std::string& s)
{
    const std::size_t first = s.find_first_not_of(" \t");
    if (first == std::string::npos)
        return ("");
    const std::size_t last  = s.find_last_not_of(" \t");
    return (s.substr(first, last - first + 1));
}

bool BitcoinExchange::isValidDate(const std::string& d)
{
    if (d.size() != 10 || d[4] != '-' || d[7] != '-')
        return (false);
    int y, m, day;
    char c1, c2;
    std::istringstream ss(d);
    if (!(ss >> y >> c1 >> m >> c2 >> day))
        return (false);
    // if (c1 != '-' || c2 != '-')
    //     return (false);
    if (m < 1 || m > 12 || day < 1)
        return (false);

    int mdays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    return (day <= mdays[m - 1]);
}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file)
        throw std::runtime_error("Error: could not open file.");

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
        throw std::runtime_error("Error: could not open file.");

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        size_t sep = line.find('|');
        if (sep == std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << '\n';
            continue;
        }

        const std::string date = trim(line.substr(0, sep));
        const std::string valS = trim(line.substr(sep + 1));
        if (!isValidDate(date))
        {
            std::cerr << "Error: bad input => " << line << '\n';
            continue;
        }

        std::istringstream vs(valS);
        double value;
        char extra;
        if (!(vs >> value) || vs >> extra)
        {
            std::cerr << "Error: bad input => " << line << '\n';
            continue;
        }
        if (value < 0)
        {
            std::cerr << "Error: not a positive number.\n";
            continue;
        }
        if (value > 1000)
        {
            std::cerr << "Error: too large a number.\n";
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
            std::cerr << e.what() << '\n';
        }
    }
}
