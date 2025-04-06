#include "PmergeMe.hpp"

static void initElements(size_t argc, char **argv, std::vector<int> &elements){
	if (argc < 2)
		throw std::runtime_error("Error: invalid number of arguments.");
	for (size_t index = 1; index < argc; index++){
		int element = atoi(argv[index]);
		if (element <= 0)
			throw std::runtime_error("Error: invalid parameter detected.");
		elements.push_back(element);	
	}
}

static void printElements(std::vector<int> elements){
	std::cout << "Before: ";
	for (std::vector<int>::iterator it = elements.begin();
		it != elements.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << "\n";
}

void	PmergeMe::sort(int argc, char **argv){
	std::vector<int> elements;
	initElements(static_cast<size_t>(argc), argv, elements);
	printElements(elements);
}