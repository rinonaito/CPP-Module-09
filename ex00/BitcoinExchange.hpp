#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include "DateUtils.hpp"

class BitcoinExchange{
	public:
		BitcoinExchange(char *price_db_file_name, char * rate_db_file_name);
		BitcoinExchange(const BitcoinExchange &other);
		~BitcoinExchange();
		BitcoinExchange	&operator=(const BitcoinExchange &other);
		char *getPriceDbFileName() const;
		char *getRateDbFileName() const;
		std::map<std::string, float> getRateDb() const;
		void exchange();

	private:
		char *price_db_file_name_;
		char *rate_db_file_name_;
		std::map<std::string, float> rate_db_;
		static const std::string ERROR_MSG_MISSING_CSV_FILE;
		static const std::string ERROR_INVALID_CSV_FIRST_LINE;
		static const std::string ERROR_MSG_INVALID_CSV_FORMAT;
		static const std::string ERROR_MSG_INVALID_DATE_FORMAT;
		static const std::string ERROR_MSG_DATE_TOO_EARLY;
		static const std::string ERROR_MSG_BAD_INPUT;
		static const std::string ERROR_MSG_PRICE_NOT_POSITIVE;
		static const std::string ERROR_MSG_PRICE_TOO_HIGH;
		static const std::string ERROR_MSG_PRICE_DB_NOT_OPEN;
		static const std::string ERROR_INVALID_FIRST_LINE_PRICE;
		static const std::string ERROR_INVALID_PRICE_FORMAT;
		static const std::string FIRST_LINE_RATE;
		static const std::string FIRST_LINE_PRICE;
		static const char RATE_DELIMITER;
		static const char PRICE_DELIMITER;
		BitcoinExchange();
		void updateRateDb();
		void printExchangeResult() const;
		void printLine(const std::string date, const std::string price) const;
		std::map<std::string, float>::const_iterator findClosestDate(const std::string date) const;
};
