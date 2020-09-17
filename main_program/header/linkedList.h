#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "node.h"


class linkedList
{
	friend class Schedule;
	
	public:
		node * listHead;
		linkedList();
		void listInsert(node* givenJob);	
		bool JobsWaitingQueIsEmpty();
		int removeFirst();
		node* findSpot(node* givenJob);
		void printList(std::ofstream& outFile);
		void deleteLL();
};

#endif
