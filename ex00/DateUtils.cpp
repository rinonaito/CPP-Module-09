#include "DateUtils.hpp"

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool parseDate(const std::string& dateStr, tm &tm_original) {
	int year, month, day;
	if (sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day) != 3)
	  return false;
	if (month < 1 || month > 12)
		throw std::runtime_error("Error: date does not exist.");
	int last_day[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 31, 31};
	if (month == 2 && isLeapYear(year))
		last_day[month - 1] = 29;
	if (day < 0 || day > last_day[month - 1])
		throw std::runtime_error("Error: date does not exist.");
	tm_original = tm();
	tm_original.tm_year = year - 1900;
	tm_original.tm_mon = month - 1;
	tm_original.tm_mday = day;
	return true;
}

std::string formatDate(const tm& tm) {
	char buffer[11];
	snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	return std::string(buffer);
}