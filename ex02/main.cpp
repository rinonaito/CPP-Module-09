#include "PmergeMe.hpp"

int	main(int argc, char **argv){
	try
	{
		PmergeMe::sort(argc, argv);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}