#include "node.h"

	node::node(){
		this->listHead = true;
		this->theJob = new Jobs();
		this->next = NULL;
	}

	node::node(Jobs* theJob){
		this->theJob = theJob;
		this->listHead = false;
		this->next = NULL;
	}

	node::node(Jobs* theJob, node* b){
		this->theJob = theJob;
		this->listHead = false;
		this->next = b;
	}
	
	node::~node(){
		delete theJob;
		delete next;
	}

	
	void node::printNode(std::ofstream& outFile){
			(*theJob).printJob(outFile);
			if(next == NULL) 
				outFile<<", NULL";
	}
