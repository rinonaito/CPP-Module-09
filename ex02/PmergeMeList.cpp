#include "PmergeMe.hpp"

/***********************************
		List
************************************/
void PmergeMe::initElementsList(size_t argc, char **argv){
	if (argc < 2)
		throw std::runtime_error(ERROR_MSG_INVALID_PARAM_NUM);
	for (size_t index = 1; index < argc; index++){
		int element = std::atoi(argv[index]);
		if (element <= 0)
			throw std::runtime_error(ERROR_MSG_INVALID_PARAM);
		this->elements_list_.push_back(element);	
	}
}

void PmergeMe::initPairList(){}

void PmergeMe::mergeSortList(std::list<std::pair<int, int> > &elements, int start_index, int last_index){
	(void)elements;
	(void)start_index;
	(void)last_index;
}

void PmergeMe::mergeList(std::list<std::pair<int, int> > &elements){
	(void)elements;
}

void PmergeMe::insertSortList(){}

void PmergeMe::execSortList(int argc, char **argv){
	this->initElementsList(argc, argv);
}

void PmergeMe::printListInput() const {printElements(this->elements_list_);}
void PmergeMe::printListSorted() const {printElements(this->sorted_list_);}
std::list<int> PmergeMe::getElementsList() const {return this->elements_list_;}
std::list<int> PmergeMe::getSortedList() const {return this->sorted_list_;}
