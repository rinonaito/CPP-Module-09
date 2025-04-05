#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(char *price_db_file_name, char *rate_db_file_name) :
	price_db_file_name_(price_db_file_name), rate_db_file_name_(rate_db_file_name){
	const std::string msg = "[Error]Missing required file: '" + std::string(price_db_file_name) + "' must be exist at the same directory with the executable.";
	const std::fstream database(price_db_file_name);
	if (database.bad())
		throw std::runtime_error(msg);
};
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {*this = other;}
BitcoinExchange::~BitcoinExchange(){}
BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &other) {
	this->price_db_file_name_ = other.getPriceDbFileName();
	this->rate_db_file_name_ = other.getRateDbFileName();
	return *this;
}
char *BitcoinExchange::getPriceDbFileName() const{return this->price_db_file_name_;};
char *BitcoinExchange::getRateDbFileName() const{return this->rate_db_file_name_;};

void BitcoinExchange::exchange(){}