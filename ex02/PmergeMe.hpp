#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <cstdlib>
#include "Utils.hpp"

class PmergeMe{

	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		~PmergeMe();
		PmergeMe &operator=(const PmergeMe &other);

		void execSortVector(int argc, char **argv);
		void execSortList(int argc, char **argv);
		void printVectorInput() const;
		void printVectorSorted() const;
		void printListInput() const;
		void printListSorted() const;
		std::vector<int> getElementsVector() const;
		std::list<int> getElementsList() const;
		std::vector<int> getSortedVector() const;
		std::list<int> getSortedList() const;

	private:
		std::vector<int> elements_vector_;
		std::list<int> elements_list_;
		std::vector<int> sorted_vector_;
		std::list<int> sorted_list_;

		void initElementsVector(size_t argc, char **argv);
		void initElementsList(size_t argc, char **argv);
		std::vector<std::pair<int, int> > initPairVector();
		void initPairList();
		void mergeSortVector(std::vector<std::pair<int, int> >&elements, int start_index, int last_index);
		void mergeSortList(std::list<std::pair<int, int> > &elements, int start_index, int last_index);
		void insertSortVector(std::vector<std::pair<int, int> >&elements);
		void insertSortList();

		static const std::string ERROR_MSG_INVALID_PARAM_NUM;
		static const std::string ERROR_MSG_INVALID_PARAM;
		static const std::string ERROR_MSG_PARAM_DUPLICATED;
		static const int DUMMY_ELEMENT;
};
