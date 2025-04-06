#include "RPN.hpp"

const char RPN::PLUS = '+';
const char RPN::MINUS = '-';
const char RPN::MULTIPLY = '*';
const char RPN::DIVIDE = '/';

void RPN::calculateCore(std::stack<double> &numbers, char entry){
	if (numbers.size() < 2)
		throw std::runtime_error("Error");
	double num2 = numbers.top();
	numbers.pop();
	double num1 = numbers.top();
	numbers.pop();
	switch(entry){
		case PLUS:
			numbers.push(num1 + num2);
			break;
		case MINUS:
			numbers.push(num1 - num2);
			break;
		case MULTIPLY:
			numbers.push(num1 * num2);
			break;
		case DIVIDE:
			numbers.push(num1 / num2);
			break;
		default:
			throw std::runtime_error("Error");
	}
}

void RPN::calculate(std::string expression){
	std::stack<double> numbers;
	for (size_t index = 0; index < expression.size(); index++){
		char entry = expression[index];
		if (entry == ' ' || entry == '\t')
			continue;
		if (isdigit(entry)){
			numbers.push(static_cast<int>(entry - '0'));
			continue;
		}
		calculateCore(numbers, entry);
	}
	if (numbers.size() != 1)
		throw std::runtime_error("Error");
	std::cout << numbers.top() << std::endl;
}