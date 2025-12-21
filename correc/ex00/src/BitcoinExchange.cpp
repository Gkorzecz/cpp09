#include "BitcoinExchange.hpp"
#include <cstdlib>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {*this = other;}
BitcoinExchange::~BitcoinExchange() {}
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this != &other)
		_db = other._db;
	return (*this);
}

bool	BitcoinExchange::loadDatabase(const std::string &filename) {
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		return false;

	std::string line;
	std::getline(file, line);

	while (std::getline(file, line)) {
		if (line.empty())
			continue;

		size_t pos = line.find(',');
		if (pos == std::string::npos)
			continue;

		std::string date = line.substr(0, pos);
		std::string priceStr = line.substr(pos + 1);

		float price = static_cast<float>(atof(priceStr.c_str()));
		_db[date] = price;
	}
	return true;
}

std::string BitcoinExchange::trim(const std::string &s) {
	size_t start = s.find_first_not_of(" \t");
	size_t end = s.find_last_not_of(" \t");
	if (start == std::string::npos)
		return "";
	return s.substr(start, end - start + 1);
}

bool	isThirtyOne(int month) {
	return month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12;
}

bool	isThirty(int month)
{
	return month == 4 || month == 6 || month == 9 || month == 11;
}

bool	BitcoinExchange::validDate(const std::string &date) {
	if (date.size() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;

	int year = atoi(date.substr(0, 4).c_str());
	int month = atoi(date.substr(5, 2).c_str());
	int day = atoi(date.substr(8, 2).c_str());

	if (year < 0)
		return false;
	if (month < 1 || month > 12)
		return false;
	if (isThirtyOne(month) && (day < 1 || day > 31))
		return false;
	if (isThirty(month) && (day < 1 || day > 30))
		return false;
	if (month == 2 && (year % 4) == 0 && (day < 1 || day > 29))
		return false;
	if (month == 2 && year % 4 && (day < 1 || day > 28))
		return false;

	return true;
}

bool BitcoinExchange::validNumber(const std::string &s, float &out)
{
	if (s.empty())
		return std::cout << "Error: wrong amount : " << s << "\n", false;
	bool	seenDot = false;
	size_t	i = 0;

	if (s[i] == '-')
		i++;
	for (; i < s.size(); ++i)
	{
		if (s[i] == '.') {
			if (seenDot || i == 0 || i == s.size() - 1)
				return std::cout << "Error: wrong amount: " << s << "\n", false;
			seenDot = true;
		} else if (!std::isdigit(s[i])) {
			return std::cout << "Error: wrong amount : " << s << "\n", false;
		}
	}
	std::stringstream ss(s);
	ss >> out;

	if (ss.fail())
		return false;
	if (out < 0)
		return std::cout << "Error: not a positive number." << "\n", false;
	if (out > 1000)
		return std::cout << "Error: too large a number." << "\n", false;

	return true;
}

bool	BitcoinExchange::findRate(const std::string &date, float &rate) const {
	std::map<std::string, float>::const_iterator it = _db.lower_bound(date);

	if (it == _db.end()) {
		--it;
		rate = it->second;
		return true;
	}
	if (it->first == date) {
		rate = it->second;
		return true;
	}
	if (it == _db.begin())
		return false;

	--it;
	rate = it->second;
	return true;
}

void	BitcoinExchange::processInput(const std::string &filename) const {
	std::ifstream input(filename.c_str());
	if (!input.is_open()) {
		std::cout << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	std::getline(input, line);

	while (std::getline(input, line)) {
		if (line.empty())
			continue;

		size_t pos = line.find('|');
		if (pos == std::string::npos) {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date = trim(line.substr(0, pos));
		std::string valueStr = trim(line.substr(pos + 1));

		if (!validDate(date)) {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		float value = 0.0f;
		if (!validNumber(valueStr, value))
			continue;
		float rate = 0.0f;
		if (!findRate(date, rate)) {
			std::cout << "Error: bad input => " << line << std::endl;
			continue;
		}

		float result = rate * value;
		std::cout << date << " => " << value << " = " << result << std::endl;
	}
}
