/***********************************
		Vector
************************************/
#include "PmergeMe.hpp"

void PmergeMe::initElementsVector(size_t argc, char **argv){
	if (argc < 2)
		throw std::runtime_error(ERROR_MSG_INVALID_PARAM_NUM);
	std::set<int> without_duplication;
	for (size_t index = 1; index < argc; index++){
		int element = std::atoi(argv[index]);
		if (element <= 0)
			throw std::runtime_error(ERROR_MSG_INVALID_PARAM);
		this->elements_vector_.push_back(element);	
		without_duplication.insert(element);
	}
	if (without_duplication.size() != this->elements_vector_.size())
		throw std::runtime_error(ERROR_MSG_PARAM_DUPLICATED);
}

std::vector<std::pair<int, int> > PmergeMe::initPairVector(){
	std::vector<std::pair<int, int> > pair_vector_;
	size_t element_size = this->elements_vector_.size();
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
	// the last unpaired element is treated as a smaller one
	if (index < element_size){
		int last = this->elements_vector_.at(index);
		pair_vector_.push_back(std::make_pair(DUMMY_ELEMENT, last));
	}
	return pair_vector_;
}

void mergeVector(std::vector<std::pair<int, int> >&elements, int start_index, int middle_index, int end_index){
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

static int binarySearch(std::vector<int> array, int target, int begin, int end)
{
	int mid;
	while (begin <= end)
	{
		mid = begin + (end - begin) / 2;
		if (target == array.at(mid))
			return mid;
		if (target > array.at(mid))
			begin = mid + 1;
		else
			end = mid - 1;
	}
	if (target > array.at(mid))
		return mid + 1;
	else
		return mid;
}

static std::vector<int> getInsertIndexInOrder(std::vector<int> insertable)
{
	std::vector<int> target_index;
	if (insertable.empty())
		return target_index;
	std::vector<int> jacobsthal = getJacobsthalIndex(insertable.size());
	size_t last_pos = 1;
	for (size_t i = 0; i < jacobsthal.size(); i++)
	{
		int start_pos = jacobsthal.at(i);
		if (static_cast<size_t>(start_pos) > insertable.size()){
			break;
		}
		target_index.push_back(start_pos);
		for (size_t pos = start_pos - 1; pos > last_pos; pos--){
			target_index.push_back(pos);
		}
		last_pos = start_pos;
	}
	for (int i = insertable.size(); i >= 1; i--)
	{
		if (std::find(target_index.begin(), target_index.end(), i) == target_index.end())
			target_index.push_back(i);
	}
	return target_index;
}

void PmergeMe::insertSortVector(std::vector<std::pair<int, int> >&elements){
	std::vector<int> insertable;
	size_t first_index_insertable = 0;
	for (size_t index = 0; index < elements.size(); index++){
			if(elements.at(index).first == DUMMY_ELEMENT){
				first_index_insertable = 1;
				continue;
			}
			// smaller one of the first pair should locate at the head
			if (index == first_index_insertable){
				this->sorted_vector_.push_back(elements.at(index).second);
			} else {
				insertable.push_back(elements.at(index).second);
			}
			this->sorted_vector_.push_back(elements.at(index).first);
	}
	// add unpaired element to sorted_vector
	if (elements.at(0).first == DUMMY_ELEMENT)
		insertable.push_back(elements.at(0).second);
	if (insertable.empty())
		return ;
	std::vector<int> insert_index_in_order = getInsertIndexInOrder(insertable);
	int add_count = 0;
	for (std::vector<int>::iterator index_it = insert_index_in_order.begin();
		index_it < insert_index_in_order.end(); index_it++)
	{
		int target = insertable.at(*index_it - 1);
		int search_end_index = *index_it + add_count;
		int insert_index = binarySearch(this->sorted_vector_, target, 0, search_end_index);
		this->sorted_vector_.insert(this->sorted_vector_.begin() + insert_index, target);
		add_count++;
	}
	if (elements.size() % 2 != 0)
	{
		int target = insertable.at(insertable.size() - 1);
		int insert_index = binarySearch(this->sorted_vector_, target, 0, this->sorted_vector_.size() - 1);
		this->sorted_vector_.insert(this->sorted_vector_.begin() + insert_index, target);
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
