#include "BitcoinExchange.hpp"

const std::string BitcoinExchange::ERROR_MSG_MISSING_CSV_FILE = "Error: Missing required CSV file.";
const std::string BitcoinExchange::ERROR_INVALID_CSV_FIRST_LINE = "Error: invalid first line (should be 'date,exchange_rate').";
const std::string BitcoinExchange::ERROR_MSG_INVALID_CSV_FORMAT = "Error: invalid csv format.";
const std::string BitcoinExchange::ERROR_MSG_INVALID_DATE_FORMAT = "Error: invalid date format.";
const std::string BitcoinExchange::ERROR_MSG_DATE_TOO_EARLY = "Error: date is before the earliest available rate.";
const std::string BitcoinExchange::ERROR_MSG_BAD_INPUT = "Error: bad input => ";
const std::string BitcoinExchange::ERROR_MSG_PRICE_NOT_POSITIVE = "Error: not a positive number.";
const std::string BitcoinExchange::ERROR_MSG_PRICE_TOO_HIGH= "Error: too large a number.";
const std::string BitcoinExchange::ERROR_MSG_PRICE_DB_NOT_OPEN = "Error: Failed to open the file: the file provided as a patrameter is not acccessible.";
const std::string BitcoinExchange::ERROR_INVALID_FIRST_LINE_PRICE = "Error: invalid first line (should be 'date | value').";
const std::string BitcoinExchange::ERROR_INVALID_PRICE_FORMAT = "Error: invalid input file format.";
const std::string BitcoinExchange::FIRST_LINE_RATE = "date,exchange_rate";
const std::string BitcoinExchange::FIRST_LINE_PRICE = "date | value";
const char BitcoinExchange::RATE_DELIMITER = ',';
const char BitcoinExchange::PRICE_DELIMITER = '|';

BitcoinExchange::BitcoinExchange(char *rate_db_file_name, char *price_db_file_name) :
	price_db_file_name_(price_db_file_name), rate_db_file_name_(rate_db_file_name){
};

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {*this = other;}

BitcoinExchange::~BitcoinExchange(){}
BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &other) {
	this->price_db_file_name_ = other.getPriceDbFileName();
	this->rate_db_file_name_ = other.getRateDbFileName();
	this->rate_db_ = other.getRateDb();
	return *this;
}

char *BitcoinExchange::getPriceDbFileName() const{return this->price_db_file_name_;};

char *BitcoinExchange::getRateDbFileName() const{return this->rate_db_file_name_;};

std::map<std::string, float> BitcoinExchange::getRateDb() const{return this->rate_db_;};

void BitcoinExchange::exchange(){
	updateRateDb();
	printExchangeResult();
}

void BitcoinExchange::updateRateDb(){
	this->rate_db_.clear();
	std::ifstream db_file(this->rate_db_file_name_);
	if (!db_file.is_open())
		throw std::runtime_error(ERROR_MSG_MISSING_CSV_FILE);
	std::string line;
	std::getline(db_file, line);
	if (line != FIRST_LINE_RATE)
			throw std::runtime_error(ERROR_INVALID_CSV_FIRST_LINE);
	while (std::getline(db_file, line)){
		std::istringstream line_stream(line);
		std::string date, rate;
		if (!std::getline(line_stream, date, RATE_DELIMITER) || !std::getline(line_stream, rate))
			throw std::runtime_error(ERROR_MSG_INVALID_CSV_FORMAT);
		rate_db_.insert(std::make_pair(date, atof(rate.c_str())));	
	}	
}

std::map<std::string, float>::const_iterator BitcoinExchange::findClosestDate(const std::string date) const{
	tm tm_original;
	if (!parseDate(date, tm_original))
		throw std::runtime_error(ERROR_MSG_INVALID_DATE_FORMAT);
	if (date < this->rate_db_.begin()->first)
		throw std::runtime_error(ERROR_MSG_DATE_TOO_EARLY);
	tm tm_prev;
	for (tm_prev = tm_original;
		this->rate_db_.find(formatDate(tm_prev)) == this->rate_db_.end();
		tm_prev.tm_mday -= 1, mktime(&tm_prev)){}
	return this->rate_db_.find(formatDate(tm_prev));
}

void BitcoinExchange::printLine(const std::string date, const std::string price_str) const{
	if (price_str.empty())
		throw std::runtime_error(ERROR_MSG_BAD_INPUT + date);
	float price = atof(price_str.c_str());
	if (price < 0)
		throw std::runtime_error(ERROR_MSG_PRICE_NOT_POSITIVE);
	if (price > 1000)
		throw std::runtime_error(ERROR_MSG_PRICE_TOO_HIGH);
	std::map<std::string, float>::const_iterator rate_it = this->rate_db_.find(date);
	if (rate_it == this->rate_db_.end())
		rate_it = findClosestDate(date);
	std::cout << date << "=> " << price << " = "<< price * rate_it->second << std::endl;
}

void BitcoinExchange::printExchangeResult() const{
	std::ifstream db_file(this->price_db_file_name_);
	if (!db_file.is_open())
		throw std::runtime_error(ERROR_MSG_PRICE_DB_NOT_OPEN);
	std::string line;
	std::getline(db_file, line);
	if (line != FIRST_LINE_PRICE)
			throw std::runtime_error(ERROR_INVALID_FIRST_LINE_PRICE);
	while (std::getline(db_file, line)){
		std::istringstream line_stream(line);
		std::string date, price_str;
		std::getline(line_stream, date, PRICE_DELIMITER);
		std::getline(line_stream, price_str);
		try
		{
			printLine(date, price_str);	
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}
