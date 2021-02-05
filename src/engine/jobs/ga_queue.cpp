/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_queue.h"
#include <mutex>
using namespace std;

mutex Head_Mutex;
mutex Tail_Mutex;


ga_queue::ga_queue(int node_count)
{
	// TODO:
	// Initialize the queue.
	// For extra credit, preallocate 'node_count' elements (instead of
	// allocating on push).
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf

	max_count = node_count;
	_node_count = 0;
	Node* node = new Node();
	node->setNext(NULL);
	this->Head = node;
	this->Tail = node;
}

ga_queue::~ga_queue()
{
	// TODO:
	// Free any resources held by the queue.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf
	if (!Head->hasNext()) {
		return;
	}
	Node* currentNode = Head;
	Head_Mutex.lock();
	while (currentNode->hasNext()) {
		currentNode = currentNode->getNext();
		free(Head);
		Head = currentNode;
	}
	free(Head);
	free(Tail);
	Head_Mutex.unlock();
}

//Basic Requirements
void ga_queue::push(void* data)
{
	// TODO:
	// Push 'data' onto the queue in a thread-safe manner.
	// If you preallocated 'node_count' elements, and if the queue is full when
	// this function is called, you must block until another thread pops an
	// element off the queue.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf
	Node* node = new Node();
	node->setValue(data);
	node->setNext(NULL);
	Tail_Mutex.lock();

	Tail->setNext(node);
	Tail = node;
	_node_count++;

	Tail_Mutex.unlock();
}

bool ga_queue::pop(void** data)
{
	// TODO:
	// Pop one element off the queue in a thread-safe manner and place it in
	// the memory pointed to by 'data'.
	// If the queue is empty when this function is called, return false.
	// Otherwise return true.
	// See https://www.research.ibm.com/people/m/michael/podc-1996.pdf
	Head_Mutex.lock();

	Node* node = Head;
	Node* new_head = node->getNext();
	if (new_head == nullptr) {
		Head_Mutex.unlock();
		return false;
	}
	*data = new_head->getData();
	Head = new_head;
	_node_count--;

	Head_Mutex.unlock();

	free(node);
	return true;
}

int ga_queue::get_count() const
{
	// TODO:
	// Get the number of elements currently in the queue.
	return _node_count;
}


//Extra credit - threading bug...
/*
bool ga_queue::CAS(Node** addr, Node* oldNode, Node* newNode) {
	if (*addr != oldNode) {
		return false;
	}
	*addr = newNode;
	if (oldNode != NULL) {
		oldNode->setNext(*addr);
	}
	return true;
}

bool ga_queue::pop(void** data)
{
	Node* head;
	while (true)
	{
		head = Head;
		Node* tail = Tail;
		Node* next = head->getNext();
		if (head == Head) {
			if (head == tail) {
				if (next == NULL) {
					return false;
				}
				CAS(&Tail, tail, next);
			}
			else {
				*data = next->getData();
				if (CAS(&Head, head, next)) {
					break;
				}
			}
		}
	}
	_node_count--;
	free(head);
	return true;
}

void ga_queue::push(void* data)
{
	Node* node = new Node();
	node->setValue(data);
	node->setNext(NULL);
	Node* tail;
	while (true)
	{
		tail = Tail;
		Node* next = tail->getNext();
		if (tail == Tail) {
			if (next == NULL) {
				Node* tmp = tail->getNext();
				if (CAS(&tmp, next, node)) {
					break;
				}
			}
			else {
				CAS(&Tail, tail, next);
			}
		}
	}
	CAS(&Tail, tail, node);
	_node_count++;
}
*/
