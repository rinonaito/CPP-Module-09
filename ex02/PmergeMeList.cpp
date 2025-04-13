#include "PmergeMe.hpp"

/***********************************
		List
************************************/
void PmergeMe::initElementsList(size_t argc, char **argv){
	if (argc < 2)
		throw std::runtime_error(ERROR_MSG_INVALID_PARAM_NUM);
	for (size_t index = 1; index < argc; index++){
		int element = std::atoi(argv[index]);
		if (element <= 0)
			throw std::runtime_error(ERROR_MSG_INVALID_PARAM);
		this->elements_list_.push_back(element);	
	}
}

void mergeVector(std::list<std::pair<int, int> >&elements, int start_index, int middle_index, int end_index){

	std::list<std::pair<int, int> >::iterator start_it = elements.begin();
	std::advance(start_it, start_index);
	
	std::list<std::pair<int, int> >::iterator middle_it = elements.begin();
	std::advance(middle_it, middle_index + 1);
	
	std::list<std::pair<int, int> >::iterator end_it = elements.begin();
	std::advance(end_it, end_index + 1);
	
	std::list<std::pair<int, int> > devided_left(start_it, middle_it);
	std::list<std::pair<int, int> > devided_right(middle_it, end_it);

	std::list<std::pair<int, int> >::iterator left_it = devided_left.begin();
	std::list<std::pair<int, int> >::iterator right_it = devided_right.begin();
	std::list<std::pair<int, int> >::iterator merge_it = start_it;
	
	while (left_it != devided_left.end() && right_it != devided_right.end()) {
		if (left_it->first < right_it->first) {
			*merge_it = *left_it;
			left_it++;
		} else {
			*merge_it = *right_it;
			right_it++;
		}
		merge_it++;
	}
	while (left_it != devided_left.end()) {
		*merge_it = *left_it;
		left_it++;
		merge_it++;
	}
	while (right_it != devided_right.end()) {
		*merge_it = *right_it;
		right_it++;
		merge_it++;
	}
}

std::list<std::pair<int, int> > PmergeMe::initPairList(){
	std::list<std::pair<int, int> > pair_list_;
	std::list<int>::iterator it = elements_list_.begin();
	while (it != elements_list_.end()) {
	    int left = *it;
	    it++;
	    if (it != elements_list_.end()) {
	        int right = *it;
	        it++;
	        if (left < right) {
	            std::swap(left, right);
	        }
	        pair_list_.push_back(std::make_pair(left, right));
	    } else {
	        pair_list_.push_back(std::make_pair(DUMMY_ELEMENT, left));
	    }
	}
	return pair_list_;
}

void PmergeMe::mergeSortList(std::list<std::pair<int, int> > &elements, int start_index, int last_index){
	if (start_index >= end_index)
		return;
	int middle_index = (start_index + end_index) / 2;
	mergeSortVector(elements, start_index, middle_index);
	mergeSortVector(elements, middle_index + 1, end_index);
	mergeVector(elements, start_index, middle_index, end_index);
}

static int binarySearch(std::list<int> array, int target, int begin, int end)
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

static std::list<int> getInsertIndexInOrder(std::list<int> insertable)
{
	std::list<int> target_index;
	if (insertable.empty())
		return target_index;
	std::list<int> jacobsthal = getJacobsthalIndex(insertable.size());
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

void PmergeMe::insertSortList(std::list<std::pair<int, int> >&elements){
	std::list<int> insertable;
	size_t first_index_insertable = 0;
	for (size_t index = 0; index < elements.size(); index++){
			if(elements.at(index).first == DUMMY_ELEMENT){
				first_index_insertable = 1;
				continue;
			}
			// smaller one of the first pair should locate at the head
			if (index == first_index_insertable){
				this->sorted_list_.push_back(elements.at(index).second);
			} else {
				insertable.push_back(elements.at(index).second);
			}
			this->sorted_list_.push_back(elements.at(index).first);
	}
	// add unpaired element to sorted_list
	if (elements.at(0).first == DUMMY_ELEMENT)
		insertable.push_back(elements.at(0).second);
	if (insertable.empty())
		return ;
	std::list<int> insert_index_in_order = getInsertIndexInOrder(insertable);
	int add_count = 0;
	for (std::list<int>::iterator index_it = insert_index_in_order.begin();
		index_it < insert_index_in_order.end(); index_it++)
	{
		int target = insertable.at(*index_it - 1);
		int search_end_index = *index_it + add_count;
		int insert_index = binarySearch(this->sorted_list_, target, 0, search_end_index);
		this->sorted_list_.insert(this->sorted_list_.begin() + insert_index, target);
		add_count++;
	}
	if (elements.size() % 2 != 0)
	{
		int target = insertable.at(insertable.size() - 1);
		int insert_index = binarySearch(this->sorted_list_, target, 0, this->sorted_list_.size() - 1);
		this->sorted_list_.insert(this->sorted_list_.begin() + insert_index, target);
	}
}

void PmergeMe::execSortList(int argc, char **argv){
	this->initElementsVector(argc, argv);
	if (this->elements_list_.size() == 1){
		this->sorted_list_ = this->elements_list_;
		return;
	}
	std::list<std::pair<int, int> > pair_list_ = initPairVector();
	mergeSortVector(pair_list_, 0, pair_list_.size() - 1);
	insertSortVector(pair_list_);
}

void PmergeMe::printListInput() const {printElements(this->elements_list_);}
void PmergeMe::printListSorted() const {printElements(this->sorted_list_);}
std::list<int> PmergeMe::getElementsList() const {return this->elements_list_;}
std::list<int> PmergeMe::getSortedList() const {return this->sorted_list_;}
