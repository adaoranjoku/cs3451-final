///*
//
//	Oliver Levy
//	CS77 - Technical Project
//	Created 14 May 2019
//
//*/
//#pragma once
//#include <vector>
//#include <iostream>
//
////template <class T> const_iterator;
//
//template <class T>
//struct node
//{
//	T obj;
//	node *next;
//
//	void node() {
//		obj = NULL;
//		next = NULL;
//	}
//	void node(T t) {
//		obj = t;
//		next = NULL;
//	}
//};
//
//
//template <class T>
//class std::list
//{
//	typedef T * iterator;
//	typedef const T * const_iterator;
//	/*    typedef std::list<T> iterator;
//		typedef ptrdiff_t difference_type;
//		typedef size_t size_type;
//		typedef T value_type;
//		typedef T * pointer;
//		typedef T & reference;
//	*/
//
//
//private:
//	node<T> *startNode;
//	node<T> *tailNode;
//	//      friend class const_iterator<T>;
//
//public:
//	//Constructor: takes data of type T
//	std::list() {
//		startNode = NULL;
//		tailNode = NULL;
//	}
//	// destructor?
//
//	/*Insert a node.
//		If pos is not specified, it will insert at end of list.
//		If pos is specified, insert at position pos in the list.
//	*/
//	void insertNode(T t, int pos = -1) {
//		node<T> *theNode = new node<T>;
//		theNode->obj = t;
//		theNode->next = NULL;
//		if (startNode == NULL) {
//			startNode = theNode;
//			tailNode = theNode;
//			theNode = NULL;
//		}
//		else if (pos == -1) {
//			tailNode->next = theNode;
//			tailNode = theNode;
//		}
//		else {
//			if (pos == 0) {
//				theNode->next = startNode;
//				startNode = theNode;
//			}
//			else {
//				int i = 1;
//				node<T> *pNode = new node<T>;
//				pNode = startNode;
//				while (i < pos) {
//					pNode = pNode->next;
//					i += 1;
//				}
//				theNode->next = pNode->next;
//				pNode->next = theNode;
//				pNode = NULL;
//				delete pNode;
//			}
//		}
//	}
//
//	//insert a vector of objects std::vector. Follows same rules as insertNode.
//	void insertVector(const std::vector<T> &Vector, int pos = -1) {
//		bool change = false;
//		if (pos != -1) { change = true; }
//		for (auto node : Vector) {
//			if (change == true) {
//				insertNode(node, pos);
//				pos += 1;
//			}
//			else {
//				insertNode(node);
//			}
//
//		}
//
//	}
//
//	//pop and return first node in list
//	T popFront() {
//		T toPop = startNode->obj;
//		if (startNode != tailNode) {
//			startNode = startNode->next;
//		}
//		else { startNode = NULL; tailNode = NULL; }
//		return toPop;
//	}
//
//
//	// pop and return last node in list
//	T pop() {
//		T toPop = tailNode->obj;
//		if (startNode != tailNode) {
//			node<T> *pNode = new node<T>;
//			pNode = startNode;
//			while (pNode->next != tailNode) {
//				pNode = pNode->next;
//			}
//			tailNode = pNode;
//			tailNode->next = NULL;
//			pNode = NULL;
//			delete pNode;
//		}
//		else {
//			startNode = NULL;
//			tailNode = NULL;
//		}
//
//		return toPop;
//
//	}
//
//	//return the size of the list
//	int size() {
//		int i = 0;
//
//		node<T> *pNode = new node<T>;
//		pNode = startNode;
//
//		while (pNode != NULL) {
//			i++;
//			pNode = pNode->next;
//		}
//		delete pNode;
//
//		return i;
//
//	}
//
//	//Delete a node containing the object: T t
//	void deleteNode(T t) {
//		node<T> *pNode = new node<T>;
//		node<T> *cNode = new node<T>;
//		pNode = startNode;
//
//		cNode = startNode;
//		while (pNode != NULL) {
//			if (pNode->obj == t && pNode == startNode) {
//				startNode = pNode->next;
//				pNode->next = NULL;
//
//			}
//			else if (pNode->obj == t && pNode == tailNode) {
//				cNode->next = NULL;
//				tailNode = cNode;
//			}
//			else if (pNode->obj == t) {
//				cNode->next = pNode->next;
//				pNode = pNode->next;
//			}
//			cNode = pNode;
//			pNode = pNode->next;
//		}
//		delete pNode;
//	}
//
//	//iterators- do not currently work
//	iterator begin() { return startNode; }
//	iterator end() { return tailNode; }
//
//	//Print each object in the list
//	void display() {
//		node<T> *pNode = new node<T>;
//		pNode = startNode;
//		while (pNode != NULL) {
//
//			std::cout << pNode->obj << " ";
//			pNode = pNode->next;
//		}
//		std::cout << std::endl;
//		delete pNode;
//	}
//
//
//};
//
//
////template <class T>
////class const_iterator{
//
//
////};
