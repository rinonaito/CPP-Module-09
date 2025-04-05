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
		
		rate_db_.insert(std::make_pair(date, atof(rate.c_str())));	
	}	
}

static bool parseDate(const std::string& dateStr, tm &tm_original) {
    int year, month, day;
    if (sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        return false; // パース失敗
    }

    tm_original = tm();  // 初期化
    tm_original.tm_year = year - 1900;  // tm_year は1900年からの年数
    tm_original.tm_mon = month - 1;     // tm_mon は 0〜11
    tm_original.tm_mday = day;

    return true;
}

static std::string formatDate(const tm& tm) {
    char buffer[11]; // "YYYY-MM-DD" + null文字
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", 
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return std::string(buffer);
}

std::map<std::string, float>::const_iterator BitcoinExchange::findClosestDate(const std::string date) const{
	tm tm_original;
	if (!parseDate(date, tm_original))
		std::runtime_error("[Error]invalid date format");
	size_t count_prev = 0;
	tm tm_prev;
	for (tm_prev = tm_original; this->rate_db_.find(formatDate(tm_prev)) == this->rate_db_.end(); tm_prev.tm_mday -= 1){
		count_prev++;
	}
	size_t count_next = 0;
	tm tm_next;
	for (tm_next = tm_original; this->rate_db_.find(formatDate(tm_next)) == this->rate_db_.end(); tm_next.tm_mday += 1){
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
			float price = atof(price_str.c_str());
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