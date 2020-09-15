#include<sstream> 	
#include <iostream>
#include<fstream>
#include<string>
using namespace std;


class Jobs;
class Processors;
class Schedule;
class linkedList;

class Jobs{
friend class Schedule;
friend class node;
friend class linkedList;
	int jobTime;
	int ProcessorUsing;
	bool onLineWaitingToBeProcessed; 
	int parentCount;
	int dependantCount;
	int jobID;
 
	
public: 

	Jobs(){
		this->jobTime = -9999;
		this->ProcessorUsing = 0;
		this->onLineWaitingToBeProcessed = true;
		this->parentCount = 0;
		this->dependantCount = 9999;
		this->jobID = 0;		
	}
	
	Jobs(int jobTime, int ProcessorUsing, int parentCount, int dependantCount, int jobID){
		this->jobTime = jobTime;
		this->ProcessorUsing = ProcessorUsing;
		this->onLineWaitingToBeProcessed = false;
		this->parentCount = parentCount;
		this->dependantCount = dependantCount;
		this->jobID = jobID;
	}
	
	printJob(ofstream& outFile){
		outFile << jobID <<  " " <<jobTime << " " <<parentCount <<" " <<dependantCount;
	}
	
};


class node{
friend class linkedList;
friend class Schedule;
	Jobs* theJob;
	bool listHead;
	node* next;
public:
	node(){
		this->listHead = true;
		this->theJob = new Jobs();
		this->next = NULL;
	}

	node(Jobs* theJob){
		this->theJob = theJob;
		this->listHead = false;
		this->next = NULL;
	}

	node(Jobs* theJob, node* b){
		this->theJob = theJob;
		this->listHead = false;
		this->next = b;
	}
	
	~node(){
		delete theJob;
		delete next;
	}

	
	void printNode(ofstream& outFile){
			(*theJob).printJob(outFile);
			if(next == NULL) 
				outFile<<", NULL";
	}
};



class linkedList{
friend class Schedule;
public:
	node * listHead;
	linkedList(){
		this->listHead = new node();
	}


	//inserting Job objects in descending order based on dependantJobCount
	void listInsert(node* givenJob){
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

	bool JobsWaitingQueIsEmpty(){
		bool outP = ((*listHead).next == NULL);
		return outP;
	}


	//returns the first node, if not, returns the listHead
	int removeFirst(){
		node* temp = (*listHead).next;
		if((*listHead).next != NULL ){
			(*listHead).next = (*temp).next;
			return (*(*temp).theJob).jobID;
		}
		return 0;	
	}

	//nodes are given priority in the job list based on their number of dependents.
	node* findSpot(node* givenJob){
		node* ll = listHead;
		
		while(((*ll).next!=NULL)&&(((*(*((*ll).next)).theJob).dependantCount)>=(*(*givenJob).theJob).dependantCount))
		{
			ll = (*ll).next;
		}		
		return ll;
	}


	void printList(ofstream& outFile){
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


	//Print to Command Line
/*	void printListCL(){
		node* myIt = this->listHead;
		while((*myIt).next != NULL){
			cout << "(";
			(*myIt).printNodeCL();
			cout <<")"<<"-->";
			// ending the infinite loop
			myIt = (*myIt).next;
		}
		cout << "(";
		(*myIt).printNodeCL();
		cout <<")"<< "--> NULL\n";

	} */
	
	void deleteLL(){
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
};



class Processor{
friend class Schedule;
	int ProcessingJob; //will be initialized to zero when avaliable
	int timeRemainingOnCurrentJob;
	
public:
	Processor(int ProcessingJob, int timeRemainingOnCurrentJob){
		this->ProcessingJob = ProcessingJob;
		this->timeRemainingOnCurrentJob = timeRemainingOnCurrentJob;
	}

};

class Schedule{
	int currentTime;
	int numberOfJobs;
	int totalNumberOfProcessors;
	int numberProcessorsCurrentlyUsing;	
	Jobs ** jobArray;
	Processor ** processorArray;
	linkedList* openJobs;
	int ** adjacencyMatrix; // the diagonal will hold whether the node is on the graph.
	int totalJobTime;
	int ** scheduleTable;
	bool changeMadeInTable;
	
public:
	
	//schedule constructor
	Schedule(int numberOfJobs, int totalNumberOfProcessors1){
		this->changeMadeInTable = false;
		this->currentTime = 0;
		this->numberOfJobs = numberOfJobs;
		if(totalNumberOfProcessors1>numberOfJobs) totalNumberOfProcessors1 = numberOfJobs;
		this->totalNumberOfProcessors = totalNumberOfProcessors1;
		this->numberProcessorsCurrentlyUsing = 0;
		this->openJobs = new linkedList(); 
		this->adjacencyMatrix = new int*[numberOfJobs +1];
		for(int i = 1; i <= numberOfJobs; i++){
			adjacencyMatrix[i]= new int[numberOfJobs +1];
			for(int j = 1; j<= numberOfJobs; j++){
				if(i==j){
					adjacencyMatrix[i][j]=1;//the diagonal represents that the node is on the graph. (since we do not allow cycles, a node will never call itself)
				}
				else adjacencyMatrix[i][j]= 0;
			}
		}
		this->jobArray = new Jobs*[numberOfJobs +1];
		this->processorArray = new Processor*[totalNumberOfProcessors +1];
		for(int i = 1; i< totalNumberOfProcessors +1; i++){
			processorArray[i] = new Processor(0,0);
		}
		this->scheduleTable = new int*[totalNumberOfProcessors1 +1];
		this->totalJobTime = -9999;
	}	
	
	
	//creates the job array and the schedule table
	void constructJobArray( ifstream& inFile2){ 
		int  job, time;
		int timeTotal = 0;
		while(inFile2>>job && inFile2>>time){
			timeTotal += time;
			Jobs* newJob = new Jobs(time, -1, -9999, -9999,job);
			jobArray[job]= newJob;				
		}
		totalJobTime = timeTotal;	
		for(int i = 0; i<=totalNumberOfProcessors; i++){
				scheduleTable[i] = new int[timeTotal];
				for(int j = 0; j<timeTotal; j++){
					scheduleTable[i][j]=0;
				}
			}
	}
	
	//reads in 2 jobs at a time, the first representing the parent job and the second representing the child job
	void loadAdjacencyMatrix(ifstream& inFile1){
		int parent, child;
		while(inFile1>>parent && inFile1>>child){
			adjacencyMatrix[parent][child] = 1;
		}
	}

	//adjacencyMatrix[parent][child] = 1, ,means that a parent-child relationship exists.
	//if adjacencyMatrix[parent][child] = 0, then the job, "child", is not dependant on the job "parent"   	
	void computeParentCount(){//call each node in adjacencyMatrix
		int tempCount = 0;
		for(int i = 1; i<=numberOfJobs; i++){
			for(int j = 1; j<=numberOfJobs; j++){
				if(adjacencyMatrix[j][i] == 1 &&  i!=j) tempCount++;
			}
			(*jobArray[i]).parentCount = tempCount;
			tempCount = 0;
		}
	}
	
	//adjacencyMatrix[parent][child] = 1, ,means that a parent-child relationship exists.
	//if adjacencyMatrix[parent][child] = 0, then the job, "child", is not dependant on the job "parent"                      
	void computeDependantCountArray(){//call each node in adjacencyMatrix
		int tempCount = 0;
		for(int i = 1; i<=numberOfJobs; i++){
			for(int j = 1; j<=numberOfJobs; j++){
				if(adjacencyMatrix[i][j] == 1  && i!=j) tempCount++;
			}
			(*jobArray[i]).dependantCount = tempCount;
			tempCount = 0;
		}		
	}
	
	void printJobArray(){
		cout<<endl<<endl<<"JobArray\n";
		for(int i = 1; i<=numberOfJobs; i++){
			cout<<(*jobArray[i]).jobID<< " "<<(*jobArray[i]).parentCount<<" "<<(*jobArray[i]).dependantCount<<endl;
		}
	}
	
	void printAdjMatrix(){
		cout<<endl<<endl<<"Adjacency Matrix\n";
		for(int i = 1; i<=numberOfJobs; i++){
			for(int j = 1; j<=numberOfJobs; j++){
				cout<< 	adjacencyMatrix[i][j]<< " ";
			}
			cout << endl;
		}
	}


	//checks the diagonal of the 2D array (adjacencyMatrix[i][i]), all non zero elements mean that the job still exists in the graph - ie has not been added to the schedule.
	bool graphIsEmpty(){
		for(int i = 1; i<=numberOfJobs; i++ ){
			if(adjacencyMatrix[i][i]!=0) {
				return false;	
			}
		}
		return true;
	}
	
	//finds an orphan job (no pre-requisite job) that is not already on the waiting list to be processed.
	//a job is defined as an orphan if it has no "Parent"/Prerequisite jobs
	//If there are no available jobs - it will return nullptr		
	Jobs* findOrphan(){
		for(int i = 1; i<=numberOfJobs; i++){
			if((*jobArray[i]).parentCount<=0   &&   !((*jobArray[i]).onLineWaitingToBeProcessed)){
				return jobArray[i];
			}
		}
		return nullptr;
		//return jobArray[1];// returns the first job - will check if it has been processed
	}
	
	
	
	void loadOrphanJobsIntoLinkedList(){
		while(true){
 
			Jobs* orphan = findOrphan();
			//if no orphan was returned or an orphan that is already on the "waiting to be processed" line ->there are no available jobs to be added to processors.
			if((orphan == nullptr)||(*orphan).onLineWaitingToBeProcessed == true){
				break;
			}
			//if there exists an orphan, it is added to the "waiting to be processed" line
			//and it is removed from the graph.
			else{
				(*orphan).onLineWaitingToBeProcessed = true;
				adjacencyMatrix[(*orphan).jobID][(*orphan).jobID]=0;
				node* insertingOrphan = new node(orphan);
				(*openJobs).listInsert(insertingOrphan);	
			}
		}
	}	
	
	//adding a job onto the time table, and filling up the required time slots it will 
	//need from the processor to complete its task.
	putJobOnTable(Processor* inserting, int avaliableProcessor){
		this->changeMadeInTable = true;
		int jobTIME = (*inserting).ProcessingJob;
		for(int i = currentTime; i< (*jobArray[jobTIME]).jobTime +currentTime ; i++){
			scheduleTable[avaliableProcessor][i] = (*inserting).ProcessingJob;
		}
	}
	
	//iterates through the processor array
	//when it finds an avaliable processor it loads the first available job onto it.
	//Runs, loading jobs onto processors until it runs out of available processors or open jobs.
	void loadProcessorArray(){
		int avaliableProcessor = 0;
		bool endOfProcessorArray = false;
		while (!endOfProcessorArray){
			endOfProcessorArray = true;
			for(int i = avaliableProcessor+1; i< totalNumberOfProcessors +1; i++ ){
				if (processorComplete(i) ){
					avaliableProcessor = i;
					endOfProcessorArray = false;
					break;
				}
			}
			if(endOfProcessorArray) break;
			Processor* openProcessor =  processorArray[avaliableProcessor];	
			int tempJob = (*openJobs).removeFirst();
			if(tempJob == 0){
				break;
			}
			(*openProcessor).ProcessingJob = tempJob;
			(*openProcessor).timeRemainingOnCurrentJob = (* jobArray[tempJob]).jobTime;// fix to array
			putJobOnTable(openProcessor, avaliableProcessor);		
		}

	}
	
	//reduces the time remaining for all processors to complete their current jobs by 1.
	void updateProcessorTime(){
		for(int i = 1; i< totalNumberOfProcessors+1; i++ ){
			(*processorArray[i]).timeRemainingOnCurrentJob--;
		}
	}
	
	//checks to see if there is time remaining for the processor to complete its current job
	bool processorComplete(int i){
		if((*processorArray[i]).timeRemainingOnCurrentJob > 0  ) return false;
		else return true;
	}
	
	
	bool allProcessorsComplete(){
		for(int i = 1; i< totalNumberOfProcessors+1; i++ ){
			if(! processorComplete(i) ) return false;
		}
		return true;		
	}
	
	//cycle detection (scheduling algorithms only work with acyclic graphs)
	//will detect that there exists a cycle when 
	//A:all processors are completed
	//B:there are no jobs waiting to be processed(ie "orphan jobs")
	//C:the graph is not empty.  
	bool hasCycle(){
		if (  ((*(*openJobs).listHead).next == NULL) &&   !graphIsEmpty()  &&  allProcessorsComplete()  ){
			return true;
		}
		return false;
	}
	
	void printScheduleTable(ofstream& outFile1){
		for(int i = 0; i<=totalNumberOfProcessors; i++){
			if(i == 0){
				outFile1<<"------";//4 spaces
			}
			else outFile1<<"P("<<i<<") |";
			for(int j = 0; j<totalJobTime; j++){
				if(i==0){
					if(j>=10){
						outFile1<<"-"<<j <<"--";
					}
					else outFile1<<"--"<<j <<"--";
				}				
			
				else if(scheduleTable[i][j] ==0) {
					outFile1 <<"----|";	
				}

				else if(scheduleTable[i][j] >=10){
					outFile1<<" "<<scheduleTable[i][j] <<" |";
				}
				else outFile1 <<"  "<<scheduleTable[i][j] <<" |";
			}
			outFile1 <<endl;		
			if(i!=0){
				for(int j = 0; j<=totalJobTime; j++){
					outFile1<<"-----";
				}
				outFile1 <<endl;	
			}		
		}
		outFile1 <<endl;
	}
	
	//removing both the parent and child dependancies contained within the graph that depend on that node.
	void removeNodeFromGraph(int ID){
		//removing the parent and child relationships off of the graph. 
		if(ID == 0) return;
		for(int i = 1; i<=numberOfJobs; i++ ){
			if(adjacencyMatrix[i][ID]!=0 || adjacencyMatrix[ID][i]!=0 ){
				adjacencyMatrix[ID][i]=0;		
				(*jobArray[i]).parentCount --;
			}	
		}
	}

	//If the job was completed by the processor, it is removed from the graph, freeing up its children
	void deleteFinishedNodes(){
		for(int i = 1; i< totalNumberOfProcessors+1; i++ ){
			if(processorComplete(i)){
				removeNodeFromGraph((*processorArray[i]).ProcessingJob);
				//make processor available- done automatically
			}
		}

	}



	void processorStatus(){
		cout<<"\n\n Processor List: ";
		for(int i = 1; i <=totalNumberOfProcessors; i++){
			cout<<" Processor: "<<i<<" JobID: "<< (*processorArray[i]).ProcessingJob<< "current time: "<< currentTime <<" time remaining: "<<(*processorArray[i]).timeRemainingOnCurrentJob<<endl;
		}
		cout<< endl<< endl;
	}
	
	
	void createSchedule(int numJobs, int numProc, ifstream& inFile1, ifstream& inFile2, ofstream& outFile1, ofstream& outFile2){
		bool cycleDetection = false;
		constructJobArray(inFile2);
		loadAdjacencyMatrix(inFile1);
		computeParentCount();
		computeDependantCountArray();
		int it = 0;
		while(! graphIsEmpty()    ||   !allProcessorsComplete()  ){
			loadOrphanJobsIntoLinkedList();
			(*openJobs).printList(outFile2);
			loadProcessorArray();
			cycleDetection = hasCycle();
			if(cycleDetection) {
				outFile1<<"\nCYCLE DETECTED, NO SCHEDULE POSSIBLE\n";
				return;
			}
			if(changeMadeInTable){
				printScheduleTable(outFile1);	
			}
			changeMadeInTable = false;
			currentTime++;
			updateProcessorTime();
			deleteFinishedNodes();	
			it++;
			//processorStatus();		
		}
		outFile1<<"\n\n\n\nFinal Table: \n";
		printScheduleTable(outFile1);
		
	}
~Schedule(){
		for(int i = 1; i<=numberOfJobs; i++){
			delete jobArray[i];
		}
		delete jobArray;
	
		for(int i =1; i< totalNumberOfProcessors +1; i++ ){
			delete processorArray[i];
		}
		delete processorArray;

		for(int i = 1; i <= numberOfJobs; i++){
			delete adjacencyMatrix[i];
		}
		delete adjacencyMatrix;


		for(int i = 0; i<=totalNumberOfProcessors; i++){
			delete scheduleTable[i];
		}
		delete scheduleTable;
	
	}
	
};


int main(int argc, char** argv){
	try{
		ifstream inFile1, inFile2;
		ofstream outFile1, outFile2;
		inFile1.open(argv[1]);
		inFile2.open(argv[2]);
		string numK = argv[3];
		outFile1.open(argv[4], ios::app);
		outFile2.open(argv[5], ios::app);

		stringstream temp(numK);
		int K;
		temp >>K;
		int numJobs, check;
		inFile1 >> numJobs;
		inFile2>>check;
		if(numJobs != check){
			cout<<"error: different number of Jobs given in the infiles\n";
			return 0;
		}
		Schedule* newSchedule = new Schedule(numJobs, K);
		(*newSchedule).createSchedule(numJobs, K,inFile1, inFile2, outFile1, outFile2);
		outFile1.close();
		outFile2.close();
		inFile1.close();
		inFile2.close();
	}
	catch(exception& e){
		cout<<e.what();
	}
	return 0;
}
