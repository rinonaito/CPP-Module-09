#include "Utils.hpp"

int jacobsthal(int index){
		if (index == 0)
			return 0;
		if (index == 1)
			return 1;
		return jacobsthal(index - 1) + 2 * jacobsthal(index - 2);
}

std::vector<int> getJacobsthalIndex(size_t size){
	std::vector<int> jacobsthalIndexVector;
	size_t current_size = 0;
	int index = 3;
	while (current_size < size){
		int jcobstalIndex = jacobsthal(index);
		jacobsthalIndexVector.push_back(jcobstalIndex);
		current_size += jcobstalIndex;
		index++;
	}
	return jacobsthalIndexVector;
}