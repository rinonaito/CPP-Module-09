#include "PmergeMe.hpp"

const std::string PmergeMe::ERROR_MSG_INVALID_PARAM_NUM = "Error: invalid number of arguments.";
const std::string PmergeMe::ERROR_MSG_INVALID_PARAM = "Error: invalid parameter detected.";

void PmergeMe::initElements(size_t argc, char **argv, std::vector<int> &elements){
	if (argc < 2)
		throw std::runtime_error(ERROR_MSG_INVALID_PARAM_NUM);
	for (size_t index = 1; index < argc; index++){
		int element = atoi(argv[index]);
		if (element <= 0)
			throw std::runtime_error(ERROR_MSG_INVALID_PARAM);
		elements.push_back(element);	
	}
}

static void printElements(const std::vector<int>& elements, const std::string &status){
	std::cout << status;
	for (std::vector<int>::const_iterator it = elements.begin();
		it != elements.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << "\n";
}

void PmergeMe::sortVector(std::vector<int> &elements){
	if (elements.size() < 2)
		return;
	std::vector<int> winners, losers;
	std::vector<int>::iterator right_it, left_it;
	for (left_it = elements.begin(); left_it != elements.end(); left_it += 2){
		right_it = left_it + 1;
		// TODO: 奇数の場合、winnerに入れるかloserに入れるか考える
		if (right_it == elements.end()){
			winners.push_back(*left_it);
			break;
		}
		winners.push_back(std::max(*right_it, *left_it));
		losers.push_back(std::min(*right_it, *left_it));
	}
	sortVector(winners);
	std::vector<int> sorted_elements = winners;
	// TODO: 検索範囲はbegin ~ ペアのindexまででいいはず。
	// でもwinnerはソートされているので、良い方法を考える。
	// TODO: グループに分けてソートするのが良い的な？規則があったはず
	for (int i = losers.size() - 1; i >= 0; i--) {
		std::vector<int>::iterator pos = std::lower_bound(sorted_elements.begin(), sorted_elements.end(), losers[i]);
		sorted_elements.insert(pos, losers[i]);
	}
	elements = sorted_elements;
}

void	PmergeMe::sort(int argc, char **argv){
	std::vector<int> elements;
	initElements(static_cast<size_t>(argc), argv, elements);
	printElements(elements, "Before: ");
	sortVector(elements);
	printElements(elements, "After: ");
}