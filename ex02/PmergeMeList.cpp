/***********************************
		List
************************************/
#include "PmergeMe.hpp"

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

void mergeList(std::list<std::pair<int, int> >&elements, int start_index, int middle_index, int end_index){

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

void PmergeMe::mergeSortList(std::list<std::pair<int, int> > &elements, int start_index, int end_index){
	if (start_index >= end_index)
		return;
	int middle_index = (start_index + end_index) / 2;
	mergeSortList(elements, start_index, middle_index);
	mergeSortList(elements, middle_index + 1, end_index);
	mergeList(elements, start_index, middle_index, end_index);
}

static int binarySearch(const std::list<int>& array, int target, int begin, int end)
{
	int mid;
	while (begin <= end)
	{
		mid = begin + (end - begin) / 2;
		std::list<int>::const_iterator it = array.begin();
		std::advance(it, mid);
		if (target == *it)
			return mid;
		if (target > *it)
			begin = mid + 1;
		else
			end = mid - 1;
	}
	return begin;
}

static std::list<int> getInsertIndexInOrder(std::list<int> insertable)
{
	std::list<int> target_index;
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

void PmergeMe::insertSortList(std::list<std::pair<int, int> >& elements) {
	std::list<int> insertable;
	size_t first_index_insertable = 0;
	
	size_t index = 0;
	for (std::list<std::pair<int, int> >::iterator it = elements.begin(); it != elements.end(); ++it, ++index) {
		if (it->first == DUMMY_ELEMENT) {
			first_index_insertable = 1;
			continue;
		}
		// smaller one of the first pair should locate at the head
		if (index == first_index_insertable) {
			this->sorted_list_.push_back(it->second);
		} else {
			insertable.push_back(it->second);
		}
		this->sorted_list_.push_back(it->first);
	}
	// add unpaired element to sorted vector
	if (first_index_insertable == 1) {
		insertable.push_back(elements.front().second);
	}
	if (insertable.empty())
		return;
	std::list<int> insert_index_in_order = getInsertIndexInOrder(insertable);
	int add_count = 0;
	for (std::list<int>::iterator index_it = insert_index_in_order.begin(); index_it != insert_index_in_order.end(); index_it++) {
		int index = *index_it - 1;
		std::list<int>::iterator insertable_it = insertable.begin();
		std::advance(insertable_it, index);
		int target = *insertable_it;
		int search_end_index = *index_it + add_count;
		int insert_index = binarySearch(this->sorted_list_, target, 0, search_end_index);
		std::list<int>::iterator insert_pos = this->sorted_list_.begin();
		std::advance(insert_pos, insert_index);
		this->sorted_list_.insert(insert_pos, target);
		add_count++;
	}
}

void PmergeMe::execSortList(int argc, char **argv){
	this->initElementsList(argc, argv);
	if (this->elements_list_.size() == 1){
		this->sorted_list_ = this->elements_list_;
		return;
	}
	std::list<std::pair<int, int> > pair_list_ = initPairList();
	mergeSortList(pair_list_, 0, pair_list_.size() - 1);
	insertSortList(pair_list_);
}

void PmergeMe::printListInput() const {printElements(this->elements_list_);}
void PmergeMe::printListSorted() const {printElements(this->sorted_list_);}
std::list<int> PmergeMe::getElementsList() const {return this->elements_list_;}
std::list<int> PmergeMe::getSortedList() const {return this->sorted_list_;}
