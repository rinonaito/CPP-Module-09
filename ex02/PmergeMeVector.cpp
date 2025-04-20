/***********************************
		Vector
************************************/
#include "PmergeMe.hpp"

void PmergeMe::initElementsVector(size_t argc, char **argv){
	std::set<int> without_duplication;
	for (size_t index = 1; index < argc; index++){
		int element = std::atoi(argv[index]);
		if (element <= 0)
			throw std::runtime_error(ERROR_MSG_INVALID_PARAM);
		this->elements_vector_.push_back(element);	
		without_duplication.insert(element);
	}
}

std::vector<std::pair<int, int> > PmergeMe::initPairVector(){
	std::vector<std::pair<int, int> > pair_vector_;
	size_t element_size = this->elements_vector_.size();
	size_t index = 0;
	// devide elements into pair
	while (index + 1 < element_size){
		int left = elements_vector_.at(index);
		int right = elements_vector_.at(index + 1);
		if (left < right)
	            std::swap(left, right);
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

void	PmergeMe::mergeInsertVectorDevide(
			std::vector<std::pair<int, int> >&pair_vector, 
			std::vector<std::vector<std::pair<int, int> > >& all_vectors){
	
	if (pair_vector.size() <= 1)
		return;
	std::vector<std::pair<int, int> >for_next_call;
	// ペアが作れる限り作る
	for (size_t i = 0; i + 1 < pair_vector.size(); i += 2){
		int first = std::max(pair_vector[i].first, pair_vector[i + 1].first);
		int second = std::min(pair_vector[i].first, pair_vector[i + 1].first);
		for_next_call.push_back(std::make_pair(first, second));
	}
	// あまりがある場合、小さい要素の末尾に追加
	if (pair_vector.size() % 2 != 0){
			int remaining = pair_vector[pair_vector.size() - 1].first;
			for_next_call.push_back(std::make_pair(DUMMY_ELEMENT, remaining));
	}
	all_vectors.insert(all_vectors.begin(), for_next_call);
	//再帰呼び出し
	mergeInsertVectorDevide(for_next_call, all_vectors);
	pair_vector = for_next_call;
}

void	PmergeMe::mergeInsertVectorMerge(
			std::vector<std::pair<int, int> >&pair_vector, 
			std::vector<std::vector<std::pair<int, int> > >& all_vectors){
	std::vector<std::pair<int, int> > new_element = pair_vector;
	for (size_t i = 1; i < all_vectors.size(); i++){
		std::vector<int> sorted = insertSortVector(new_element);
		new_element.clear();
		std::vector<std::pair<int, int> > src = all_vectors[i];
		std::cout << "<sorted>" << std::endl;
		printElements(sorted);
		std::cout << "<src>" << std::endl;
		for (size_t i = 0; i < src.size(); i++){
			std::cout << "[" << i << "] first: " << src[i].first << 
				", second: " << src[i].second << std::endl;
		}	
		for (size_t i = 0; i < sorted.size(); i++){
			int first = sorted[i];
			int second = 0;
			for (size_t j = 0; j < src.size(); j++){
				if (src[j].first == first){
					second = src[j].second;
					break;
				}
			}
			std::cout << "first: " << first << " second: " << second <<  std::endl;
			if (second == 0)
				return ;
			new_element.push_back(std::make_pair(first, second));
		}
		if (src.size() > sorted.size()){
			new_element.push_back(src[src.size() - 1]);
		}
		std::cout << new_element.size() << std::endl;
		std::cout <<  all_vectors[all_vectors.size() - 1].size() << std::endl;
		if (new_element.size() == all_vectors[all_vectors.size() - 1].size())
			break;
	}
	pair_vector = new_element;
	return;
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

std::vector<int> PmergeMe::insertSortVector(std::vector<std::pair<int, int> >&elements){
	std::vector<int> insertable;
	std::vector<int> sorted;
	size_t first_index_insertable = 0;
	for (size_t index = 0; index < elements.size(); index++){
			if(elements.at(index).first == DUMMY_ELEMENT){
				first_index_insertable = 1;
				continue;
			}
			// smaller one of the first pair should locate at the head
			if (index == first_index_insertable){
				sorted.push_back(elements.at(index).second);
			} else {
				insertable.push_back(elements.at(index).second);
			}
			sorted.push_back(elements.at(index).first);
	}
	// add unpaired element at the end of insertable
	if (elements.at(0).first == DUMMY_ELEMENT)
		insertable.push_back(elements.at(0).second);
	if (insertable.empty())
		return sorted;
	std::vector<int> insert_index_in_order = getInsertIndexInOrder(insertable);
	int add_count = 0;
	for (std::vector<int>::iterator index_it = insert_index_in_order.begin();
		index_it < insert_index_in_order.end(); index_it++)
	{
		int target = insertable.at(*index_it - 1);
		int search_end_index = *index_it + add_count;
		int insert_index = binarySearch(sorted, target, 0, search_end_index);
		sorted.insert(sorted.begin() + insert_index, target);
		add_count++;
	}
	return sorted;
}

void PmergeMe::execSortVector(int argc, char **argv){
	this->initElementsVector(argc, argv);
	if (this->elements_vector_.empty())
		throw std::runtime_error(ERROR_MSG_INVALID_PARAM_NUM);
	if (this->elements_vector_.size() == 1){
		this->sorted_vector_ = this->elements_vector_;
		return;
	}
	std::vector<std::pair<int, int> > pair_vector_ = initPairVector();
	std::vector<std::vector<std::pair<int, int> > > all_vectors;
	all_vectors.push_back(pair_vector_);
	mergeInsertVectorDevide(pair_vector_, all_vectors);
	std::cout << "***************************************" << std::endl;
	std::cout << "all" << std::endl;
	for (size_t j = 0; j < all_vectors.size(); j++){
		std::cout << "[" << j << "]" << std::endl;
		for (size_t i = 0; i < all_vectors[j].size(); i++){
			std::cout << "first: " << all_vectors[j][i].first << " second: " << all_vectors[j][i].second << std::endl;
		}
	}
	std::cout << "############ before finnal insert ################" <<  std::endl;
	for (size_t i = 0; i < pair_vector_.size(); i++){
		std::cout << "[" << i << "] first: " << pair_vector_[i].first << 
			", second: " << pair_vector_[i].second << std::endl;
	}
	mergeInsertVectorMerge(pair_vector_, all_vectors);
	this->sorted_vector_ = insertSortVector(pair_vector_);
}

void PmergeMe::printVectorInput() const {printElements(this->elements_vector_);}
void PmergeMe::printVectorSorted() const {printElements(this->sorted_vector_);}
std::vector<int> PmergeMe::getElementsVector() const {return this->elements_vector_;}
std::vector<int> PmergeMe::getSortedVector() const {return this->sorted_vector_;}