#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <list>

class PmergeMe{
	public:
		static void	sort(int argc, char **argv);
	private:
		static void initElements(size_t argc, char **argv, std::vector<int> &elements);
		static void sortVector(std::vector<int> &elements);
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		~PmergeMe();
		PmergeMe &operator=(const PmergeMe &other);
		static const std::string ERROR_MSG_INVALID_PARAM_NUM;
		static const std::string ERROR_MSG_INVALID_PARAM;
};