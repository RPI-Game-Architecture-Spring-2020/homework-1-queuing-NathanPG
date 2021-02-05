#pragma once

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

/*
** Thread-safe queue.
** https://www.research.ibm.com/people/m/michael/podc-1996.pdf
*/

class ga_queue
{
public:
#pragma once
	class Node {
	public:
		Node() {
			next = nullptr;
			data = nullptr;
		}
		//Node(const Node& orig);
		//virtual ~Node();
		bool hasNext() {
			if (next) {
				return true;
			}
			return false;
		}

		Node* getNext() {
			return next;
		}

		void setNext(Node* newNext) {
			next = newNext;
		}

		void* getData() {
			return data;
		}
		void setValue(void* newData) {
			data = newData;
		}

	private:
		Node* next;
		void* data;
	};

	ga_queue(int node_count);
	~ga_queue();

	void push(void* data);
	bool pop(void** data);
	bool compareNode(Node* oldNode, Node* newNode);
	bool CAS(Node* addr, Node* oldNode, Node* newNode);
	int get_count() const;

private:
	int _node_count = 0;
	int max_count = 0;
	Node* Head;
	Node* Tail;
};


