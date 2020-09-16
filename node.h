#ifndef NODE_H
#define NODE_H
#include "Jobs.h"


class node
{
	friend class linkedList;
	friend class Schedule;
	
	public:
		node();
		node(Jobs* theJob);
		node(Jobs* theJob, node* b);
		~node();
		void printNode(std::ofstream& outFile);
		
	protected:
		Jobs* theJob;
		bool listHead;
		node* next;
};

#endif
