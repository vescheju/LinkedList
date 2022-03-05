#pragma once
#include<iostream>
using std::cout; using std::endl; using std::boolalpha; using std::ostream;
#include<initializer_list>
using std::initializer_list;
#include <algorithm> 
using std::max; using std::copy; using std::swap; using std::rotate;
#include <utility> 
using std::make_pair; using std::pair;
#include <stdexcept> 
using std::runtime_error;
#include<sstream>
using std::ostringstream;
const static size_t element_array_size = 5;

/*
	Justin Vesche
	4/18/2019
	Project 11
*/


template<typename K, typename V>
struct Element {
public:
	K key_;
	V values_[element_array_size];
	Element* next_ = nullptr;

	//Optional 
	//Element* previous = nullptr; 

	size_t count_ = 0;

	Element() = default;
	Element(K, initializer_list<V>);
	Element(const Element&);
	bool operator==(const Element&)const;
	bool operator<(const K&) const;
	friend ostream& operator<<(ostream& out, Element& ele) {
		ostringstream oss; // how to place element in outstream
		oss << ele.key_ << ":";
		for (size_t i = 0; i < ele.count_; i++) { // go through array and get values
			oss << ele.values_[i] << ",";
		}
		out << ((oss.str()).substr(0, (oss.str()).size() - 1)); // remove tail
		return out;
	}

};

template<typename K, typename V>
Element<K, V>::Element(K key, initializer_list<V> list) {
	// construct element
	key_ = key; 
	count_ = list.size();
	// copy into values
	copy(list.begin(), list.end(), values_);
	next_ = nullptr;
}

template<typename K, typename V>
Element<K, V>::Element(const Element & compare_element){
	// copy ctor, copy keys, count, and values
	key_ = compare_element.key_;
	count_ = compare_element.count_;
	copy(compare_element.values_, compare_element.values_ + count_, values_);
	next_ = nullptr;
}


template<typename K, typename V>
bool Element<K, V>::operator==(const Element & element2) const {
	// check if keys or if sizes are diff.
	if (key_ != element2.key_ || count_ != element2.count_)
		return false;
	// check each array values
	else {
		for (size_t i = 0; i < count_; i++) {
			if (values_[i] != element2.values_[i])
				return false;
		}
	}
	return true;

}


template<typename K, typename V>
bool Element<K, V>::operator<(const K& compared_element) const {
	return (key_ < compared_element); // compare keys
}


template<typename K, typename V>
class MVM {
public:
	Element<K, V> * data_head_ = nullptr;

	//optional
	//Element<K, V> * data_tail_ = nullptr;
	//size_t num_elements_ = 0;

	size_t num_keys_ = 0;

	pair <Element<K, V>*, Element<K, V> *> find_key(K);
	MVM find_value(V);

public:
	//Constructors
	MVM() = default;
	MVM(initializer_list<pair<K, initializer_list<V>>>);

	//Rule of Three
	MVM(const MVM&);
	MVM& operator=(const MVM& other);
	~MVM();

	//Other Functions
	size_t size();
	bool add(K, V);
	bool remove_key(K);
	MVM remove_value(V);

	friend ostream& operator<<(ostream& out, MVM& mv) {
		ostringstream oss;
		Element<K, V> *ptr;

		for (ptr = mv.data_head_; ptr != nullptr; ptr = ptr->next_) { // go through each list value
			oss << (*ptr) << " "; // overload element insertion op
		}
		out << ((oss.str()).substr(0, (oss.str()).size() - 1)); // remove tail
		return out;
	}
};

template<typename K, typename V>
MVM<K, V>::MVM(initializer_list<pair<K, initializer_list<V>>> list){
	// set size and follow ptr
	num_keys_ = list.size();
	data_head_ = nullptr;
	Element<K, V> * ptr = data_head_;
	// go through each pair in the list
	for (pair<K, initializer_list<V>> ele : list) {
		// check if head is set to a value, if not then set it
		if (data_head_ == nullptr) {
			data_head_ = new Element<K, V>(ele.first, ele.second);
			ptr = data_head_;
		}
		else {
			// use follow pointer to add new elements to the list
			ptr->next_ = new Element<K, V>(ele.first, ele.second);
			ptr = ptr->next_;
		}
	}
}

template<typename K, typename V>
MVM<K, V>::MVM(const MVM &s1){
	// copy ctr set keys to be same
	num_keys_ = s1.num_keys_;
	// check if list empty 
	if (s1.data_head_ == nullptr) {
		data_head_ = nullptr;
	}
	else {
		// if not empty copy each element, use follow pointer to so
		data_head_ = new Element<K, V>(*(s1.data_head_));
		Element<K,V> * ptr = s1.data_head_->next_;
		Element<K,V>* new_element = data_head_;
		// check if follow pointer has an element. Copy follow ptr to new_element
		while (ptr != nullptr) {
			new_element->next_ = new Element<K,V>(*ptr);
			new_element = new_element->next_;
			ptr = ptr->next_;
		}
	}
}

template<typename K, typename V>
MVM<K, V>& MVM<K, V>::operator=(const MVM& other){
	// use copy ctor to make a new other_new
	MVM<K, V> other_new(other);
	// swap the head_pointers and num keys
	swap(data_head_, other_new.data_head_);
	num_keys_ = other_new.num_keys_;
	// deconstuctor takes care of other_new
	return *this;
}

template<typename K, typename V>
MVM<K, V>::~MVM(){
	Element<K,V>* to_del = data_head_;
	// go through each element in list and delete the pointer
	while (to_del != nullptr) {
		// use a follow pointer to delete new head, continue to end
		data_head_ = data_head_->next_;
		delete to_del;
		to_del = data_head_;
	}
	data_head_ = nullptr;
}

template<typename K, typename V>
pair<Element<K, V>*, Element<K, V>*> MVM<K, V>::find_key(K key){

	Element<K, V>* infront = nullptr,* behind = nullptr;
	//  loop to check each key in list
	for (Element<K, V> *n = data_head_; n != nullptr; n = n->next_) {
		// if key is found to be greater than or equal to key we want to find, set infront pointer to that location
		if (n->key_ >= key) {
			infront = n;
			break;
		}
		// if key not found yet, keep setting behind to the element being pointed at
		behind = n;
	} 
	// now pair the pointers together
	pair<Element<K, V>*, Element<K, V>*> made_pair(behind, infront);
	return made_pair;
}

template<typename K, typename V>
MVM<K,V> MVM<K, V>::find_value(V val){
	// make a new MVM and follow pointer
	MVM<K, V> key_values;
	key_values.data_head_ = nullptr;
	key_values.num_keys_ = 0;
	Element<K, V>* ptr = key_values.data_head_;
	// check each element in the list
	for (Element<K, V> *n = data_head_; n != nullptr; n = n->next_) {
		// now check each value in each element
		for (size_t i = 0; i < n->count_; ++i) {
			// if value is in the element
			if (n->values_[i] == val) {
				// check if the new MVM is empty
				if (key_values.data_head_ == nullptr) {
					// if it is empty set the new head to the element, and adjust the follow ptr
					key_values.data_head_ = new Element<K, V>(n->key_, { val });
					ptr = key_values.data_head_;
					++(key_values.num_keys_);
				}
				else {
					// if not empty use follow pointer to add to mvm
					ptr->next_ = new Element<K, V>(n->key_, { val });
					ptr = ptr->next_;
					++(key_values.num_keys_);
				}
			}
		}
	}
	return key_values;
}

// return size
template<typename K, typename V>
size_t MVM<K, V>::size(){
	return this->num_keys_;
}

template<typename K, typename V>
bool MVM<K, V>::add(K key, V value){
	// check if the MVM list is empty
	if (data_head_ == nullptr) {
		// if it is add to head
		data_head_ = new Element<K, V>(key, { value });
		++num_keys_;
		return true;
	}
	// find key to see where to add
	pair<Element<K, V>*, Element<K, V>*> made_pair;
	made_pair = find_key(key);

	// at very end of the list
	if (made_pair.second == nullptr && made_pair.first != nullptr && (made_pair.first)->key_ != key) {
		// set last values next to the new value
		(made_pair.first)->next_ = new Element<K, V>(key, { value });
		++num_keys_;
		return true;
	}
	else if (made_pair.second != nullptr && made_pair.first == nullptr && (made_pair.second)->key_ != key) {
		// if at the start of list
		Element<K, V>* ptr = new Element<K, V>(key, { value });
		ptr->next_ = data_head_;
		data_head_ = ptr;
		// change the head to the new element and set its next_ element
		ptr = nullptr;
		++num_keys_;
		return true;
	}
	else if ((made_pair.second)->key_ != key) {
		// if in middle and key doesnt exist yet
		Element<K, V>* ptr = new Element<K, V>(key, { value });
		(made_pair.first)->next_ = ptr;
		// use the follow ptr to insert into middle
		ptr->next_ = (made_pair.second);
		// use prev. ptr to link the list back together
		ptr = nullptr;
		++num_keys_;
		return true;
	}
	else {
		// if key DNE
		// check if the count in array is full, if is return false
		if ((made_pair.second)->count_ == element_array_size)
			return false;
		for (size_t i = 0; i < (made_pair.second)->count_; ++i) {
			// check each value to see if it is already in values_
			if ((made_pair.second)->values_[i] == value)
				return false;
		}
		// if not, add the value to the end of values_
		(made_pair.second)->values_[(made_pair.second)->count_] = value;
		++((made_pair.second)->count_);
	}
	return true;
}

template<typename K, typename V>
bool MVM<K, V>::remove_key(K key){
	pair<Element<K, V>*, Element<K, V>*> made_pair;
	made_pair = find_key(key);
	// if the key DNE, return false, or if the keys arent == return false
	if (made_pair.second == nullptr)
		return false;
	else if ((made_pair.second)->key_ != key)
		return false;
	else {
		// if the key does exist, check if the key is == to the head
		Element<K, V>* to_del;
		// if == to head
		if (made_pair.first == nullptr) {
			// adjust the head then delete the old head
			to_del = data_head_;
			data_head_ = data_head_->next_;
			delete to_del;
			--(num_keys_);
		}
		else {
			// if not at head, use prev. ptr and link its next to the deleted ptr's next value
			to_del = made_pair.second;
			(made_pair.first)->next_ = to_del->next_;
			// now delete the ptr and dec. keys
			delete to_del;
			--(num_keys_);
		}
	}
	return true;
}


template<typename K, typename V>
MVM<K, V> MVM<K, V>::remove_value(V value) {
	// mvm_values to make new mvm with keys that have the value
	MVM<K, V> mvm_values = find_value(value);
	// loop to check each mvm_values element
	for (Element<K, V> *n = mvm_values.data_head_; n != nullptr; n = n->next_) {
		// rotate add to assisst rotate fn
		size_t rotate_add = 0;
		pair<Element<K, V>*, Element<K, V>*> made_pair;
		made_pair = find_key(n->key_); // find each key in mvm_values and point to it with the pair
		// now check the elements values
		for (size_t j = 0; j < (made_pair.second)->count_; ++j) {
			// when the element is found, set the position to rotate add
			if ((made_pair.second)->values_[j] == value) {
				rotate_add = j;
			}
		}
		// now rotate each element to the end of the array, and then decrease the count so it goes away
		rotate(((made_pair.second)->values_) + rotate_add, ((made_pair.second)->values_) + rotate_add + 1,
			((made_pair.second)->values_) + (made_pair.second)->count_);

		((made_pair.second)->count_) = ((made_pair.second)->count_) - 1;
	}
	return mvm_values;
}