#include "RPN.hpp"

int main(int argc, char **argv){
	if (argc != 2){
		std::cerr << "Error: ./RPN <expression>" << std::endl;
		return 1;
	}
	std::string expression = argv[1];
	if (expression[0] == '"' && expression[expression.size() - 1] == '"')
		expression.substr(1, expression.size() - 2);
	try
	{
		RPN::calculate(expression);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
