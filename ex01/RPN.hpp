#pragma once

#include <iostream>
#include <string>
#include <stack>

class RPN{
	public:
		static void calculate(std::string expression);
		static void calculateCore(std::stack<double> &numbers, char entry);
	private:
		static const char PLUS;
		static const char MINUS;
		static const char MULTIPLY;
		static const char DIVIDE;
		RPN();
		RPN(const RPN &other);
		~RPN();
		RPN &operator=(const RPN &other);
};