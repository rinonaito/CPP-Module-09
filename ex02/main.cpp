#include "PmergeMe.hpp"

static void printTime(clock_t start, clock_t end, const std::string &container_type, size_t size){
	clock_t diff_ms = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
	std::cout << "Time to process a range of " << size << " elements with "
		<< container_type << " : " << diff_ms << " us" << std::endl;
}

int	main(int argc, char **argv){
	
	try
	{
		PmergeMe pm;
		
		clock_t start_vector = clock();
		pm.execSortVector(argc, argv);
		clock_t end_vector = clock();

		clock_t start_list = clock();
		pm.execSortList(argc, argv);
		clock_t end_list = clock();

		//check result
		std::vector<int> sorted_vec = pm.getSortedVector();
		std::list<int> sorted_list = pm.getSortedList();
		if (sorted_vec.size() != sorted_list.size()){
			pm.printVectorSorted();
			pm.printListSorted();
			throw std::runtime_error("Error: size of result of does not match.");
		}
		std::vector<int>::iterator vec_it = sorted_vec.begin();
		std::list<int>::iterator list_it = sorted_list.begin();
		int prev = 0;
		while (vec_it != sorted_vec.end()){
			if (*vec_it != *list_it){
				pm.printVectorSorted();
				pm.printListSorted();
				throw std::runtime_error("Error: result of sort does not match.");
			}
			if (prev > *vec_it){
				throw std::runtime_error("Error: result not sorted property.");
			}
			prev = *vec_it;
			vec_it++;
			list_it++;
		}
		std::cout << "Before: ";
		pm.printVectorInput();
		std::cout << "After: ";
		pm.printVectorSorted();
		printTime(start_vector, end_vector, "std::vector", pm.getElementsVector().size());
		printTime(start_list, end_list, "std::list", pm.getElementsList().size());

	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
