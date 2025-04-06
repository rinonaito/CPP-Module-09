#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <list>

class PmergeMe{
	public:
		static void	sort(int argc, char **argv);
	private:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		~PmergeMe();
		PmergeMe &operator=(const PmergeMe &other);
};