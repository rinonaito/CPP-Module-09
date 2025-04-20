#include "DateUtils.hpp"

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

#include <iostream>
bool parseDate(const std::string& dateStr, tm &tm_original) {
	const std::string ERROR_DATE_NOT_EXIST = "Error: date does not exist.";
	if (dateStr.size() != 10 || dateStr[4] != '-'|| dateStr[7] != '-'){
		return false;
	}
	int year = atoi(dateStr.substr(0, 4).c_str());
	int month = atoi(dateStr.substr(5, 2).c_str());
	int day = atoi(dateStr.substr(8, 2).c_str());
	if (month < 1 || month > 12)
		throw std::runtime_error(ERROR_DATE_NOT_EXIST);
	int last_day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 31, 31};
	if (month == 2 && isLeapYear(year))
		last_day[month - 1] = 29;
	if (day < 0 || day > last_day[month - 1])
		throw std::runtime_error(ERROR_DATE_NOT_EXIST);
	tm_original = tm();
	tm_original.tm_year = year - 1900;
	tm_original.tm_mon = month - 1;
	tm_original.tm_mday = day;
	return true;
}

std::string formatDate(const tm& tm) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(4) << (tm.tm_year + 1900) << '-'
        << std::setw(2) << (tm.tm_mon + 1) << '-'
        << std::setw(2) << tm.tm_mday;
    return oss.str();
}