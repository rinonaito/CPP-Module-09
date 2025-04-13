#pragma once

#include <iostream>
#include <vector>

template <typename T>
void printElements(const T& elements){
	for (typename T::const_iterator it = elements.begin();
		it != elements.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << "\n";
}

int jacobsthal(int index);
std::vector<int> getJacobsthalIndex(size_t size);


