#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <map>

class BitcoinExchange {
	private:
		std::map<std::string, float>	_db;

		static std::string	trim(const std::string &s);
		static bool			validDate(const std::string &date);
		static bool			validNumber(const std::string &s, float &out);
		bool				findRate(const std::string &date, float &rate) const;
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange &operator=(const BitcoinExchange &other);
		~BitcoinExchange();

		bool	loadDatabase(const std::string &filename);
		void	processInput(const std::string &filename) const;
};

#endif
