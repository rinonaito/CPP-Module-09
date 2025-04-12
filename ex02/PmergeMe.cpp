#include "PmergeMe.hpp"

const std::string PmergeMe::ERROR_MSG_INVALID_PARAM_NUM = "Error: invalid number of arguments.";
const std::string PmergeMe::ERROR_MSG_INVALID_PARAM = "Error: invalid parameter detected.";
const int PmergeMe::DUMMY_ELEMENT = -1;

PmergeMe::PmergeMe(){}
PmergeMe::PmergeMe(const PmergeMe &other){ *this = other; }
PmergeMe::~PmergeMe(){}
PmergeMe &PmergeMe::operator=(const PmergeMe &other){
	this->elements_vector_ = other.getElementsVector();
	this->elements_list_ = other.getElementsList();
	this->sorted_vector_ = other.getSortedVector();
	this->sorted_list_ = other.getSortedList();
	return *this;
}

template <typename T>
static void printElements(const T& elements){
	for (typename T::const_iterator it = elements.begin();
		it != elements.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << "\n";
}
void PmergeMe::printVectorInput() const {printElements(this->elements_vector_);}
void PmergeMe::printVectorSorted() const {printElements(this->sorted_vector_);}
void PmergeMe::printListInput() const {printElements(this->elements_list_);}
void PmergeMe::printListSorted() const {printElements(this->sorted_list_);}
std::vector<int> PmergeMe::getElementsVector() const {return this->elements_vector_;}
std::list<int> PmergeMe::getElementsList() const {return this->elements_list_;}
std::vector<int> PmergeMe::getSortedVector() const {return this->sorted_vector_;}
std::list<int> PmergeMe::getSortedList() const {return this->sorted_list_;}

/********************************
						Vector
********************************/
void PmergeMe::initElementsVector(size_t argc, char **argv){
	if (argc < 2)
		throw std::runtime_error(ERROR_MSG_INVALID_PARAM_NUM);
	for (size_t index = 1; index < argc; index++){
		int element = atoi(argv[index]);
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
	std::vector<std::pair<int, int> > devided_left(elements.begin() + start_index, elements.begin() + middle_index);
	std::vector<std::pair<int, int> > devided_right(elements.begin() + middle_index + 1, elements.begin() + end_index + 1);
	size_t left_index = 0;
	size_t right_index = 0;
	while (left_index < devided_left.size() && right_index < devided_right.size()){
		 if (devided_left.at(left_index) > devided_right.at(right_index)){
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
	if (start_index == end_index)
		return;
	int middle_index = (start_index + end_index) / 2;
	mergeSortVector(elements, start_index, middle_index);
	mergeSortVector(elements, middle_index + 1, end_index);
	mergeVector(elements, start_index, middle_index, end_index);
}

static size_t jacobsthal(int index){
		if (index == 0)
			return (0);
		if (index == 1)
			return (1);
		return (jacobsthal(index - 1) + 2 * jacobsthal(index - 2));
}

static std::vector<int> getJacobsthalIndex(size_t size){
	std::vector<int> jacobsthalIndexVector;
	size_t jcobstalIndex;
	for (int index = 3; (jcobstalIndex = jacobsthal(index)) < size - 1; index++)
	{
		jacobsthalIndexVector.push_back(jcobstalIndex);
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
	size_t end_index = 0;
	for (size_t group_index = 0; group_index < jacobsthal_indexs.size(); group_index++)
	{
		size_t start_index = jacobsthal_indexs.at(group_index);
		for(size_t index = start_index; index >= end_index; index--){
			// insert into sorted_vector
			int value = insertable.at(index);
			std::vector<int>::iterator it = std::lower_bound(sorted_vector_.begin(), sorted_vector_.end(), value);
			sorted_vector_.insert(it, value);
		}
		end_index = start_index;
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

/********************************
						List
********************************/
void PmergeMe::initElementsList(size_t argc, char **argv){
	if (argc < 2)
		throw std::runtime_error(ERROR_MSG_INVALID_PARAM_NUM);
	for (size_t index = 1; index < argc; index++){
		int element = atoi(argv[index]);
		if (element <= 0)
			throw std::runtime_error(ERROR_MSG_INVALID_PARAM);
		this->elements_list_.push_back(element);	
	}
}

void PmergeMe::initPairList(){}

void PmergeMe::mergeSortList(std::list<std::pair<int, int> > &elements, int start_index, int last_index){
	(void)elements;
	(void)start_index;
	(void)last_index;
}

void PmergeMe::mergeList(std::list<std::pair<int, int> > &elements){
	(void)elements;
}

void PmergeMe::insertSortList(){}

void PmergeMe::execSortList(int argc, char **argv){
	this->initElementsList(argc, argv);
}