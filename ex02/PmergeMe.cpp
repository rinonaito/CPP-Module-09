#include "PmergeMe.hpp"

const std::string PmergeMe::ERROR_MSG_INVALID_PARAM_NUM = "Error: invalid number of arguments.";
const std::string PmergeMe::ERROR_MSG_INVALID_PARAM = "Error: invalid parameter detected.";
const int PmergeMe::DUMMY_ELEMENT = -1;

PmergeMe::PmergeMe(){}
PmergeMe::PmergeMe(const PmergeMe &other){ *this = other; }
PmergeMe::~PmergeMe(){}
PmergeMe &PmergeMe::operator=(const PmergeMe &other){
	this->elements_vector_ = other.getElementsVector();
	this->elements_list_ = other.getElementsList();
	this->sorted_vector_ = other.getSortedVector();
	this->sorted_list_ = other.getSortedList();
	return *this;
}

