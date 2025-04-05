#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(char *rate_db_file_name, char *price_db_file_name) :
	price_db_file_name_(price_db_file_name), rate_db_file_name_(rate_db_file_name){
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
void BitcoinExchange::exchange(){
	updateRateDb();
	printExchangeResult();
}

void BitcoinExchange::updateRateDb(){
	this->rate_db_.clear();
	std::ifstream db_file(this->rate_db_file_name_);
	if (!db_file.is_open()){
		const std::string msg = "[Error]Missing required file: '" + std::string(rate_db_file_name_) + "' must be exist at the same directory with the executable.";
		throw std::runtime_error(msg);
	}
	std::string line;
	std::getline(db_file, line);
	while (std::getline(db_file, line)){
		std::istringstream line_stream(line);
		std::string date, rate;
		std::getline(line_stream, date, ',');
		std::getline(line_stream, rate);
		
		rate_db_.insert(std::make_pair(date, stof(rate)));	
	}	
}

static std::tm parseDate(const std::string& dateStr) {
	std::tm tm = {};
	std::istringstream ss(dateStr);
	ss >> std::get_time(&tm, "%Y-%m-%d");
	tm.tm_hour = 0; tm.tm_min = 0; tm.tm_sec = 0;
	return tm;
}

static std::string formatDate(const std::tm& timeStruct) {
	std::ostringstream oss;
	oss << std::put_time(&timeStruct, "%Y-%m-%d");
	return oss.str();
}

std::map<std::string, float>::const_iterator BitcoinExchange::findClosestDate(const std::string date) const{
	std::tm target_tm = parseDate(date);
	size_t count_prev = 0;
	std::tm tm_prev;
	for (tm_prev = target_tm; this->rate_db_.find(formatDate(tm_prev)) == this->rate_db_.end(); tm_prev.tm_mday -= 1){
		count_prev++;
	}
	size_t count_next = 0;
	std::tm tm_next;
	for (tm_next = target_tm; this->rate_db_.find(formatDate(tm_next)) == this->rate_db_.end(); tm_next.tm_mday += 1){
		count_next++;
	}
	return count_next > count_prev ?
		this->rate_db_.find(formatDate(tm_prev)) : this->rate_db_.find(formatDate(tm_next));
}

void BitcoinExchange::printLine(const std::string date, const float price) const{
	std::map<std::string, float>::const_iterator it = this->rate_db_.find(date);
	if (it == this->rate_db_.end())
		it = findClosestDate(date);
	std::cout << date << " => " << price << " = "<< price * it->second << std::endl;
}

void BitcoinExchange::printExchangeResult() const{
	std::ifstream db_file(this->price_db_file_name_);
	if (!db_file.is_open()){
		throw std::runtime_error("[Error]Failed to open the file: the file provided as a patrameter '" + std::string(this->price_db_file_name_) + "' is not acccessible.");
	}
	std::string line;
	std::getline(db_file, line);
	while (std::getline(db_file, line)){
		std::istringstream line_stream(line);
		std::string date, price_str;
		std::getline(line_stream, date, '|');
		std::getline(line_stream, price_str);
		try
		{
			if (price_str.empty())
				throw std::runtime_error("Error: bad input => " + date);
			float price = stof(price_str);
			if (price < 0)
				throw std::runtime_error("Error: not a positive number.");
			if (price > 1000)
				throw std::runtime_error("Error: too large a number.");
			printLine(date, price);	
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}