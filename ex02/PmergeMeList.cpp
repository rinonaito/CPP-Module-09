/***********************************
		List
************************************/
#include "PmergeMe.hpp"

void PmergeMe::initElementsList(size_t argc, char **argv){
	for (size_t index = 1; index < argc; index++){
		int element = std::atoi(argv[index]);
		if (element <= 0)
			throw std::runtime_error(ERROR_MSG_INVALID_PARAM);
		this->elements_list_.push_back(element);	
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

void	PmergeMe::mergeInsertListDevide(
			std::list<std::pair<int, int> >&pair_list, 
			std::list<std::list<std::pair<int, int> > >& all_lists){
	
	if (pair_list.size() <= 1)
		return;
	std::list<std::pair<int, int> >for_next_call;
	// make pairs
	for (std::list<std::pair<int, int> >::iterator it = pair_list.begin();
		it != pair_list.end();
		it++) {
		int left = it->first;
		it++;
		if (it == pair_list.end())
			break;
		int right = it->first;
		int first = std::max(left, right);
		int second = std::min(left, right);
		for_next_call.push_back(std::make_pair(first, second));
	}
	// if number of element is odd number, make pair with DUMMY
	if (pair_list.size() % 2 != 0){
			std::list<std::pair<int, int> >::iterator end_it = pair_list.end();
			end_it--;
			int remaining = end_it->first;
			for_next_call.push_back(std::make_pair(DUMMY_ELEMENT, remaining));
	}
	all_lists.insert(all_lists.begin(), for_next_call);
	// call the function recursively
	mergeInsertListDevide(for_next_call, all_lists);
	pair_list = for_next_call;
}

void	PmergeMe::mergeInsertListMerge(
			std::list<std::pair<int, int> >&pair_list, 
			std::list<std::list<std::pair<int, int> > >& all_lists){
	std::list<std::pair<int, int> > new_element = pair_list;
	//for (size_t i = 1; i < all_lists.size(); i++){
	std::list<std::list<std::pair<int, int> > >::iterator it_out = all_lists.begin();
	it_out++;
	while (it_out != all_lists.end()){
		std::list<int> sorted = insertSortList(new_element);
		new_element.clear();
		std::list<std::pair<int, int> > src = *it_out;
		//for (size_t i = 0; i < sorted.size(); i++){
		for (std::list<int>::iterator it_on_sorted = sorted.begin();
			it_on_sorted != sorted.end(); it_on_sorted++){
			int first = *it_on_sorted;
			int second = 0;
			//for (size_t j = 0; j < src.size(); j++){
			for (std::list<std::pair<int, int> >::iterator it_on_src = src.begin();
				it_on_src != src.end();
				it_on_src++) {
				if (it_on_src->first == first){
					second = it_on_src->second;
					break;
				}
			}
			if (second == 0)
				return ;
			new_element.push_back(std::make_pair(first, second));
		}
		if (src.size() > sorted.size()){
			std::list<std::pair<int, int> >::iterator it = src.end();
			it--;
			new_element.push_back(*it);
		}
		std::list<std::list<std::pair<int, int> > >::iterator it = all_lists.end();
		it++;
		if (new_element.size() == it->size())
			break;
		it_out++;
	}
	pair_list = new_element;
	return;
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

std::list<int> PmergeMe::insertSortList(std::list<std::pair<int, int> >&elements){
	std::list<int> insertable;
	std::list<int> sorted;
	size_t first_index_insertable = 0;
	
	size_t index = 0;
	for (std::list<std::pair<int, int> >::iterator it = elements.begin(); it != elements.end(); ++it, ++index) {
		if (it->first == DUMMY_ELEMENT) {
			first_index_insertable = 1;
			continue;
		}
		// smaller one of the first pair should locate at the head
		if (index == first_index_insertable) {
			sorted.push_back(it->second);
		} else {
			insertable.push_back(it->second);
		}
		sorted.push_back(it->first);
	}
	// add unpaired element to sorted list
	if (first_index_insertable == 1) {
		insertable.push_back(elements.front().second);
	}
	if (insertable.empty())
		return sorted;
	std::list<int> insert_index_in_order = getInsertIndexInOrder(insertable);
	int add_count = 0;
	for (std::list<int>::iterator index_it = insert_index_in_order.begin(); index_it != insert_index_in_order.end(); index_it++) {
		int index = *index_it - 1;
		std::list<int>::iterator insertable_it = insertable.begin();
		std::advance(insertable_it, index);
		int target = *insertable_it;
		int search_end_index = *index_it + add_count;
		int insert_index = binarySearch(sorted, target, 0, search_end_index);
		std::list<int>::iterator insert_pos = sorted.begin();
		std::advance(insert_pos, insert_index);
		sorted.insert(insert_pos, target);
		add_count++;
	}
	return sorted;
}

void PmergeMe::execSortList(int argc, char **argv){
	this->initElementsList(argc, argv);
	if (this->elements_list_.empty())
		throw std::runtime_error(ERROR_MSG_INVALID_PARAM_NUM);
	if (this->elements_list_.size() == 1){
		this->sorted_list_ = this->elements_list_;
		return;
	}
	std::list<std::pair<int, int> > pair_list = initPairList();
	std::list<std::list<std::pair<int, int> > > all_lists;
	all_lists.push_back(pair_list);
	mergeInsertListDevide(pair_list, all_lists);
	mergeInsertListMerge(pair_list, all_lists);
	this->sorted_list_ = insertSortList(pair_list);
}

void PmergeMe::printListInput() const {printElements(this->elements_list_);}
void PmergeMe::printListSorted() const {printElements(this->sorted_list_);}
std::list<int> PmergeMe::getElementsList() const {return this->elements_list_;}
std::list<int> PmergeMe::getSortedList() const {return this->sorted_list_;}
