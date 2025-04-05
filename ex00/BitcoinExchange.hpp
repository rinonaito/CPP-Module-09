#pragma once

#include <iostream>
#include <exception>
#include <fstream>

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
		BitcoinExchange();
};