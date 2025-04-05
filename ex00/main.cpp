#include "BitcoinExchange.hpp"

int main(int argc, char **argv){
	char *price_db_file_name = "data.csv";
	const std::string error_msg_arg = "[Error]Invalid number of argument: Exactly one argument must be provided.";
	try
	{
		if (argc != 2)
			throw std::runtime_error(error_msg_arg);
		char *rate_db_file_name = argv[1];
		BitcoinExchange btc(price_db_file_name, rate_db_file_name);
		btc.exchange();
		return 0;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 1;
}