#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <cstdlib>
#include <algorithm>
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
		//Vector
		std::vector<int> elements_vector_;
		std::vector<int> sorted_vector_;
		void initElementsVector(size_t argc, char **argv);
		std::vector<std::pair<int, int> > initPairVector();
		void mergeInsertVectorDevide(
					std::vector<std::pair<int, int> >&pair_vector, 
					std::vector<std::vector<std::pair<int, int> > >& all_vectors);
		void mergeInsertVectorMerge(
					std::vector<std::pair<int, int> >&pair_vector, 
					std::vector<std::vector<std::pair<int, int> > >& all_vectors);
		std::vector<int> insertSortVector(std::vector<std::pair<int, int> >&elements);

		//List
		std::list<int> elements_list_;
		std::list<int> sorted_list_;
		void initElementsList(size_t argc, char **argv);
		std::list<std::pair<int, int> > initPairList();
		void mergeInsertListDevide(
					std::list<std::pair<int, int> >&pair_list, 
					std::list<std::list<std::pair<int, int> > >& all_lists);
		void mergeInsertListMerge(
					std::list<std::pair<int, int> >&pair_list, 
					std::list<std::list<std::pair<int, int> > >& all_lists);
		std::list<int> insertSortList(std::list<std::pair<int, int> >&elements);

		static const std::string ERROR_MSG_INVALID_PARAM_NUM;
		static const std::string ERROR_MSG_INVALID_PARAM;
		static const std::string ERROR_MSG_PARAM_DUPLICATED;
		static const int DUMMY_ELEMENT;
};
