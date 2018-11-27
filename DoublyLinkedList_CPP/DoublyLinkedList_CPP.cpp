// DoublyLinkedList_CPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <ctime>

using namespace std;

//Large value of n for performance testing
const unsigned int BIG_N = 50000;

//For slow performance testing. Change to false for debugging
const bool DO_PERFORMANCE_TEST = true;

//this global variable keeps track of the number of DNode instances.
unsigned int dnode_census = 0;

//Doubly-Linked list node template class
template <class E>
class DNode {
public:
	//constructor for class DNode
	DNode(DNode<E>* prev, E value, DNode<E>* next) {
		_value = value;		//setting value from user		
		_prev = prev;		//setting prev from user
		_next = next;		//setting next from user
		dnode_census++;		//incrementing census for instance creation
	}
	//destructor for class DNode
	~DNode() {
		dnode_census--;
	}
	//Get member fucntions
	E value() { return _value; }
	DNode<E>* prev() { return _prev; }
	DNode<E>* next() { return _next; }
	//Set member fucntions
	void set_value(E value) { _value = value; }
	void set_prev(DNode<E>* prev) { _prev = prev; }
	void set_next(DNode<E>* next) { _next = next; }
private:
	E _value;
	DNode<E>* _prev;
	DNode<E>* _next;
};

//Deque iterator template class
template <class E>
class DequeIterator {
public:
	//constructor
	DequeIterator(DNode<E>* node) {
		_node = node;		//setting node from user
	}
	/***** member functions *****/
	//checking if end of deque has been reached
	bool at_end() {
		return (_node == nullptr);
	}
	//value of node
	E value() {
		assert(!at_end());
		return _node->value();
	}
	//set value of node
	void set_value(E x) {
		assert(!at_end());
		_node->set_value(x);
	}
	//go to next node
	void next() {
		assert(!at_end());
		_node = _node->next();
	}
	//return node
	DNode<E>* node() { return _node; }

private:
	DNode<E>* _node;	//iterator class is operating on the DNode class
};

//Doubly-Linked Deque implementation
template<class E>
class Deque {
public:
	//constructor
	Deque() {
		_head = nullptr;	//head pointer starts null
		_tail = nullptr;	//tail pointer starts null
		_size = 0;			//size starts as 0
	}
	//destructor
	~Deque() {
		clear();			//empty out the deque
	}
	/***** member functions *****/
	//return head node
	DNode<E>* head_node() { return _head; }
	//return tail node
	DNode<E>* tail_node() { return _tail; }
	//null out the deque
	void set_deque_null() {
		_head = nullptr;
		_tail = nullptr;
		_size = 0;
	}
	//Empty the deque
	void clear() {
		while (!is_empty()) {
			erase_back();
		}
	}
	//return size of deque
	unsigned int size() {
		return _size;
	}
	//Check if deque is empty. Return true if empty;
	bool is_empty() {
		return (size() == 0);
	}
	//return iterator at first position of the deque. 
	unique_ptr<DequeIterator<E>> iter_begin(){
		return unique_ptr<DequeIterator<E>>(new DequeIterator<E>(_head));
	}
	//return iterator at the index position of the deque. 
	unique_ptr<DequeIterator<E>> iter_at(unsigned int index) {
		assert((index >= 0) && (index <= size()));
		unique_ptr<DequeIterator<E>> iter(iter_begin());
		for (unsigned int j = 0; j < index; j++) {
			iter->next();
		}
		return iter;
	}
	//return the element at fron of deque
	E front() {
		assert(!is_empty());
		return _head->value();
	}
	//return the element at the end of the deque
	E back() {
		assert(!is_empty());
		return _tail->value();
	}
	//insert after iterator
	void insert_after(DequeIterator<E>& iter, E x) {
		if (_size == 0)
			insert_front(x);			//if deque is empty, put it in the first position
		else {
			assert(!iter.at_end());		//make sure iterator isn't at end
			if (iter.node() == _tail)
				insert_back(x);			//if iterator is at end of list, put value at end
			else {
				DNode<E>* before_node = iter.node();
				DNode<E>* after_node = before_node->next();
				DNode<E>* insert_node = new DNode<E>(before_node, x, after_node);
				before_node->set_next(insert_node);
				after_node->set_prev(insert_node);
				_size++;
			}
		}
	}
	//insert before iterator
	void insert_before(DequeIterator<E>& iter, E x) {
		if (_size == 0)
			insert_front(x);			//if deque is empty, put it in the first position
		else {
			assert(!is_empty());		//make sure iterator isn't at end
			if (iter.node() == _head)
				insert_front(x);			//if iterator is at end of list, put value at end
			else {
				DNode<E>* after_node = iter.node();
				DNode<E>* before_node = after_node->prev(); 
				DNode<E>* insert_node = new DNode<E>(before_node, x, after_node);
				before_node->set_next(insert_node);
				after_node->set_prev(insert_node);
				_size++;
			}
		}
	}
	//insert node to front of deque
	void insert_front(E x) {
		if (is_empty()) {
			DNode<E>* temp = new DNode<E>(nullptr, x, nullptr);
			_head = temp;
			_tail = temp;
		}
		else {
			DNode<E>* temp = new DNode<E>(nullptr, x, _head);
			_head->set_prev(temp);
			_head = temp;
		}
		_size++;
	}
	//insert node to back of deque
	void insert_back(E x) {
		if (is_empty()) {
			insert_front(x);		//we already handled this in insert_front fxn
		}
		else {
			assert(!is_empty());
			DNode<E>* temp = new DNode<E>(_tail, x, nullptr);
			_tail->set_next(temp);
			_tail = temp;
			_size++;
		}
	}
	//erase node from deque
	void erase(DequeIterator<E>&iter) {
		assert((!is_empty()) || !iter.at_end()); 
		if (iter.node() == _head) 
			erase_front(); 
		else if (iter.node() == _tail) 
			erase_back(); 
		else {
			DNode<E>*erase_node = iter.node(); DNode<E>*before_erase_node = erase_node->prev(); DNode<E>*after_erase_node = erase_node->next();
			before_erase_node->set_next(after_erase_node);
			after_erase_node->set_prev(before_erase_node);

			delete erase_node;
			_size--;
		}
	}
	//erase front of deque
	void erase_front() {
		assert(!is_empty());
		DNode<E>*temp = _head;
		_head = temp->next();
		delete temp;
		_size--;
	}
	//erase back of deque
	void erase_back() {
		assert(!is_empty());
		DNode<E>*temp = _tail;
		_tail = temp->prev();
		delete temp;
		_size--;
	}
	//return iterator at position of element, if it exists
	unique_ptr<DequeIterator<E>>find(E match) {
		assert(!is_empty());
		unique_ptr<DequeIterator<E>>iter(iter_begin());
		while (!(iter->at_end()) && iter->value() != match)
			iter->next();
		return iter;
	}
	//return the element value at position index, which must be valid
	E get(int index) {
		assert((index >= 0) && (index < size()));
		unique_ptr<DequeIterator<E>>iter(iter_begin());
		for (int j = 0; j < index; j++)
			iter->next();
		return iter->node()->value();
	}
	//remove all elements out of 'other' and append them to the end
	//of this deque. After the meld, deque should contain all the 
	//elements orignally in deque followed by 'other' elements
	void meld(Deque<E>&other)
	{
		if (size() == 0)
		{
			_head = other.head_node();
			_tail = other.tail_node();
			_size = other.size();
		}
		else if (other.size() == 0) {
			DNode<E>*temp_head = other.head_node();
			DNode<E>*temp_tail = other.tail_node();
			temp_head = _head;
			temp_tail = _tail;
		}
		else {
			DNode<E>*temp_tail = _tail;
			DNode<E>*temp_head = other.head_node();
			temp_tail->set_next(temp_head);
			temp_head->set_prev(temp_tail);
			_tail = other.tail_node();
			_size += other.size();
			other.set_deque_null();
		}
	}

private:
	DNode<E>* _head;
	DNode<E>* _tail;
	unsigned int _size;
};

//
void assert_no_leak(int expected) {
	if (dnode_census != expected) {
		cout << "MEMORYLEAKDETECTED" << endl
			<< "expectednodes:" << expected << endl
			<< "actualnodesallocated:" << dnode_census << endl
			<< endl;
	}
}

//
bool queue_equals_string(Deque<char>&q, const string&str) {
	if (q.size() != int(str.size()))
		return false;
	for (unsigned int i = 0; i < q.size(); i++) {
		if (q.get(i) != str[i])
			return false;
	}
	return true;
}

//
void print_test_title(const string&name) {
	cout << "Testing " << name << "..." << endl;
}

//
int main() {
	assert_no_leak(0); 
	//Element objects to reuse.All uppercase letters of the alphabet. 
	const int N=26; 
	char letters[N]; 
	
	for (int i=0;i<N;i++) 
		letters[i]='A' +i; 
	
	//Functional tests	
	print_test_title("Deque::constructor"); 
	unique_ptr<Deque<char>>q(new Deque<char>()); 
	assert_no_leak(0); 
	
	print_test_title("Deque::size"); 
	assert(q->size() ==0); 
	
	print_test_title("Deque::is_empty"); 
	assert(q->is_empty()); 
	
	print_test_title("Deque::insert_back"); 
	for (int i=0;i<N;i++){ 
		assert(q->size() ==i); 
		assert_no_leak(i); 
		q->insert_back(letters[i]); 
		assert(!q->is_empty()); 
		assert(q->size() == (i+1)); 
		assert_no_leak(i+1); 
	}
	
	print_test_title("iteration"); 
	unique_ptr<DequeIterator<char>>iter(q->iter_begin()); 
	assert(!iter->at_end()); 
	assert_no_leak(N); 
	for (int i=0;!iter->at_end(); i++,iter->next()) {
		assert(i<N); 
		assert(iter->value() ==letters[i]); 
		assert_no_leak(N); 
	}
	assert_no_leak(N); 

	print_test_title("Deque::find"); 
	iter=q->find('A'); 
	assert(!iter->at_end()); 
	assert(iter->value() =='A'); 
	assert_no_leak(N); 

	iter=q->find('T'); 
	assert(!iter->at_end());
	assert(iter->value() == 'T'); 
	assert_no_leak(N); 
	
	iter = q->find('?'); 
	assert(iter->at_end()); 
	assert_no_leak(N); 
	
	print_test_title("Deque::get"); 
	for (int index = 0; index < N; index++) {
		assert(q->get(index) == letters[index]); 
		assert_no_leak(N); 
	}
	
	print_test_title("Deque::front"); 
	assert(q->front() == 'A'); 
	assert_no_leak(N);

	print_test_title("Deque::back"); 
	assert(q->back() == 'Z'); 
	assert_no_leak(N); 
	
	print_test_title("Deque::erase_front"); 
	for (int i = 0; i < N; i++) { 
		assert(q->front() == letters[i]); 
		assert(q->size() == (N - i)); 
		assert_no_leak(N - i); 
		q->erase_front(); 
	}
	assert(q->is_empty()); 
	assert_no_leak(0); 
	
	print_test_title("Deque::erase_back");
	for (int i = 0; i < N; i++) 
		q->insert_back(letters[i]); 
	for (int i = 0; i < N; i++) { 
		assert(q->back() == letters[N - i - 1]); 
		assert(q->size() == (N - i)); 
		assert_no_leak(N - i); q->erase_back(); 
	}
	assert(q->is_empty()); 
	assert_no_leak(0); 
	
	print_test_title("Deque::insert_front"); 
	for (int i = 0; i < N; i++) { 
		q->insert_front(letters[i]); 
		assert(q->front() == letters[i]); 
		assert(q->size() == (i + 1)); 
		assert_no_leak(i + 1); 
	}
	assert(q->size() == N); 
	assert_no_leak(N); 
	
	print_test_title("Deque::clear"); 
	q->clear(); 
	assert(q->is_empty()); 
	assert_no_leak(0); 
	for (int size = 0; size <= N; size++) {
		assert(q->is_empty()); 
		assert_no_leak(0); 

		for (int i = 0; i < size; i++) 
			q->insert_back(letters[i]); 

		assert(q->size() == size); 
		assert_no_leak(size); 

		q->clear(); 
		assert(q->is_empty()); 
		assert_no_leak(0);
	}
	
	print_test_title("insert_after,insert_before,erase"); 
	assert(q->is_empty()); 
	iter = q->iter_begin(); 
	q->insert_before(*iter, 'X'); 
	assert(queue_equals_string(*q, "X"));
	assert_no_leak(1); 
	iter = q->iter_begin(); 
	q->insert_after(*iter, 'O'); 
	assert(queue_equals_string(*q, "XO")); 
	assert_no_leak(2); 
	q->insert_after(*iter, 'Z'); 
	assert(queue_equals_string(*q, "XZO"));
	assert_no_leak(3); 
	iter->next(); 
	q->insert_before(*iter, 'C'); 
	assert(queue_equals_string(*q, "XCZO")); 
	assert_no_leak(4); q->insert_after(*iter, 'A'); 
	assert(queue_equals_string(*q, "XCZAO")); 
	assert_no_leak(5); iter->next(); q->erase(*iter); 
	assert(queue_equals_string(*q, "XCZO")); 
	assert_no_leak(4); 
	iter = q->iter_begin(); 
	iter->next(); 
	iter->next(); 
	q->erase(*iter); 
	assert(queue_equals_string(*q, "XCO")); 
	assert_no_leak(3); 
	q->erase(*q->iter_begin()); 
	assert(queue_equals_string(*q, "CO")); 
	assert_no_leak(2); 
	iter = q->iter_begin(); 
	iter->next(); 
	q->erase(*iter); 
	assert(queue_equals_string(*q, "C")); 
	assert_no_leak(1); 
	q->erase(*q->iter_begin());
	assert(queue_equals_string(*q, "")); 
	assert_no_leak(0); 
	
	print_test_title("Deque::splice"); 
	assert(q->is_empty());
	assert_no_leak(0);
	unique_ptr<Deque<char>>q2(new Deque<char>());
	assert_no_leak(0);
	for (int i = 0; i < 5; i++) {
		q->insert_back(letters[i]);
		q2->insert_back(letters[i]);
	}
	assert(q->size() == 5);
	assert(q2->size() == 5);
	assert_no_leak(10);
	q->meld(*q2);
	assert(q->size() == 10);
	assert(q2->is_empty());
	assert_no_leak(10);
	assert(queue_equals_string(*q, "ABCDEABCDE"));
	if (DO_PERFORMANCE_TEST) {
		print_test_title("Performance");
		q->clear();
		clock_t begin = clock();
		for (int i = 0; i < BIG_N; i++) {
			q->insert_front('A');
			if ((i % 1000) == 0)
				cout << '.';
		}
		clock_t end = clock();
		assert(q->size() == BIG_N);
		cout << endl
			<< "Elapsed time to insert_front " << BIG_N << " times:"
			<< double(end-begin)/CLOCKS_PER_SEC << " seconds" << endl;
	}
	return 0;
}