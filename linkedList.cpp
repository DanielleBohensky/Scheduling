#include "linkedList.h"


	linkedList::linkedList(){
		this->listHead = new node();
	}

	//inserting Job objects in descending order based on dependantJobCount
	void linkedList::listInsert(node* givenJob){
		node* ll = listHead;
		node* spot;
		if((*listHead).next == NULL ) spot = listHead;
		else {
			spot = findSpot(givenJob);	
		}
		(*givenJob).next = (*spot).next;
		(*spot).next = givenJob;
		//cout<< (*a).data << " "<< (*a).next<< endl<< (*spot).data << " "<< (*spot).next<< endl;
	}

	bool linkedList::JobsWaitingQueIsEmpty(){
		bool outP = ((*listHead).next == NULL);
		return outP;
	}


	//returns the first node, if not, returns the listHead
	int linkedList::removeFirst(){
		node* temp = (*listHead).next;
		if((*listHead).next != NULL ){
			(*listHead).next = (*temp).next;
			return (*(*temp).theJob).jobID;
		}
		return 0;	
	}

	//nodes are given priority in the job list based on their number of dependents.
	node* linkedList::findSpot(node* givenJob){
		node* ll = listHead;
		
		while(((*ll).next!=NULL)&&(((*(*((*ll).next)).theJob).dependantCount)>=(*(*givenJob).theJob).dependantCount))
		{
			ll = (*ll).next;
		}		
		return ll;
	}


	void linkedList::printList(std::ofstream& outFile){
		node* myIterator = this->listHead;
		outFile << "List Head --> ";
		
		if((*myIterator).next == NULL) {
			outFile<<" NULL\n";
			return;
		}
			
		myIterator = (*myIterator).next;
		while((*myIterator).next != NULL){
			outFile << "(";
			(*myIterator).printNode(outFile);
			outFile <<")"<<"-->";
			myIterator = (*myIterator).next;
		}
		outFile << "(";
		(*myIterator).printNode(outFile);
		outFile<<")"<< "--> NULL\n";
	}



	void linkedList::deleteLL(){
		node* itPointer = this->listHead;
		node* itPointer2 = itPointer;
		if((*itPointer).next != NULL)
			itPointer2 = (*itPointer2).next;
		else{
			delete (itPointer);
			return;
		}
		while( (*itPointer2).next != NULL ) {
			delete (itPointer);
			itPointer = itPointer2;
			itPointer2 = (*itPointer2).next;
		}
		return;
	}
