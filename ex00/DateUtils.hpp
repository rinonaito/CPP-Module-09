#pragma once
#include <string>
#include <cmath>
#include <sstream>
#include <exception>
#include <cstdio>

bool isLeapYear(int year);
bool parseDate(const std::string& dateStr, tm &tm_original);
std::string formatDate(const tm& tm);
