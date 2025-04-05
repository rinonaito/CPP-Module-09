#pragma once

#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>

class BitcoinExchange{
	public:
		BitcoinExchange(char *price_db_file_name, char * rate_db_file_name);
		BitcoinExchange(const BitcoinExchange &other);
		~BitcoinExchange();
		BitcoinExchange	&operator=(const BitcoinExchange &other);
		char *getPriceDbFileName() const;
		char *getRateDbFileName() const;
		void exchange();

	private:
		char *price_db_file_name_;
		char *rate_db_file_name_;
		std::map<std::string, float> rate_db_;
		BitcoinExchange();
		void updateRateDb();
		void printExchangeResult() const;
		void printLine(const std::string date, const float price) const;
		std::map<std::string, float>::const_iterator findClosestDate(const std::string date) const;
};