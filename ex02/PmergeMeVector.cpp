#include "PmergeMe.hpp"

/***********************************
		Vector
************************************/
void PmergeMe::initElementsVector(size_t argc, char **argv){
	if (argc < 2)
		throw std::runtime_error(ERROR_MSG_INVALID_PARAM_NUM);
	std::seti<int without_duplication;
	for (size_t index = 1; index < argc; index++){
		int element = std::atoi(argv[index]);
		if (element <= 0)
			throw std::runtime_error(ERROR_MSG_INVALID_PARAM);
		this->elements_vector_.push_back(element);	
	}
}

std::vector<std::pair<int, int> > PmergeMe::initPairVector(){
	std::vector<std::pair<int, int> > pair_vector_;
	size_t element_size = this->elements_vector_.size();
	//size_t pair_size = element_size % 2 == 0 ? element_size / 2 : (element_size / 2) + 1;
	size_t index = 0;
	// devide elements into pair
	while (index + 1 < element_size){
		int left = elements_vector_.at(index);
		int right = elements_vector_.at(index + 1);
		if (left < right){
			int tmp = left;
			left = right;
			right = tmp;
		}
		// first: bigger, second: smaller
		pair_vector_.push_back(std::make_pair(left, right));
		index += 2;
	}
	// the last unpaired element is is treated as a smaller one
	if (index < element_size){
		int last = this->elements_vector_.at(index);
		pair_vector_.push_back(std::make_pair(DUMMY_ELEMENT, last));
	}
	return pair_vector_;
}

void PmergeMe::mergeVector(std::vector<std::pair<int, int> >&elements, int start_index, int middle_index, int end_index){
	std::vector<std::pair<int, int> > devided_left(elements.begin() + start_index, elements.begin() + middle_index + 1);
	std::vector<std::pair<int, int> > devided_right(elements.begin() + middle_index + 1, elements.begin() + end_index + 1);
	size_t left_index = 0;
	size_t right_index = 0;
	while (left_index < devided_left.size() && right_index < devided_right.size()){
		 if (devided_left[left_index].first < devided_right[right_index].first){
			elements[start_index] = devided_left.at(left_index);
			left_index++;
		 } else {
			elements[start_index] = devided_right.at(right_index);
			right_index++;
		 }
		start_index++;
	}
	while (left_index < devided_left.size()){
		elements[start_index] = devided_left.at(left_index);
		start_index++;
		left_index++;
	}
	while (right_index < devided_right.size()){
		elements[start_index] = devided_right.at(right_index);
		start_index++;
		right_index++;
	}
}

void PmergeMe::mergeSortVector(std::vector<std::pair<int, int> >&elements, int start_index, int end_index){
	if (start_index >= end_index)
		return;
	int middle_index = (start_index + end_index) / 2;
	mergeSortVector(elements, start_index, middle_index);
	mergeSortVector(elements, middle_index + 1, end_index);
	mergeVector(elements, start_index, middle_index, end_index);
}

static int jacobsthal(int index){
		if (index == 0)
			return 0;
		if (index == 1)
			return 1;
		return jacobsthal(index - 1) + 2 * jacobsthal(index - 2);
}

static std::vector<int> getJacobsthalIndex(size_t size){
	std::vector<int> jacobsthalIndexVector;
	size_t current_size = 0;
	int index = 1;
	while (current_size < size){
		int jcobstalIndex = jacobsthal(index);
		jacobsthalIndexVector.push_back(jcobstalIndex);
		current_size += jcobstalIndex;
		index++;
	}
	return jacobsthalIndexVector;
}

void PmergeMe::insertSortVector(std::vector<std::pair<int, int> >&elements){
	// smaller one of first pair should locate at the head
	this->sorted_vector_.push_back(elements.at(0).second);
	std::vector<int> insertable;
	for (size_t index = 0; index < elements.size(); index++){
			this->sorted_vector_.push_back(elements.at(index).first);
			insertable.push_back(elements.at(index).second);
	}
	std::vector<int> jacobsthal_indexs = getJacobsthalIndex(insertable.size());
	///debug
	std::cout << "JACONBTHAL: ";
	printElements(jacobsthal_indexs);
	std::cout << "SORTED: ";
	printElements(this->sorted_vector_);
	std::cout << "INSERTABLE: ";
	printElements(insertable);
	///
	size_t end_index = 1;
	for (size_t group_index = 0; group_index < jacobsthal_indexs.size(); group_index++)
	{
		size_t start_index = end_index + (jacobsthal_indexs.at(group_index) * 2);
		if (start_index > insertable.size())
			start_index = insertable.size() - 1;
		for(size_t index = start_index; index >= end_index; index--){
			// insert into sorted_vector
			int value = insertable.at(index);
			std::vector<int>::iterator it = std::lower_bound(sorted_vector_.begin(), sorted_vector_.end(), value);
			sorted_vector_.insert(it, value);
		}
		end_index = start_index + 1;
	}
}

void PmergeMe::execSortVector(int argc, char **argv){
	this->initElementsVector(argc, argv);
	if (this->elements_vector_.size() == 1){
		this->sorted_vector_ = this->elements_vector_;
		return;
	}
	std::vector<std::pair<int, int> > pair_vector_ = initPairVector();
	mergeSortVector(pair_vector_, 0, pair_vector_.size() - 1);
	insertSortVector(pair_vector_);
}

void PmergeMe::printVectorInput() const {printElements(this->elements_vector_);}
void PmergeMe::printVectorSorted() const {printElements(this->sorted_vector_);}
std::vector<int> PmergeMe::getElementsVector() const {return this->elements_vector_;}
std::vector<int> PmergeMe::getSortedVector() const {return this->sorted_vector_;}
