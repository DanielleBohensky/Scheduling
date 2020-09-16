#include "Schedule.h"

	Schedule::Schedule(int numberOfJobs, int totalNumberOfProcessors1){
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
	void Schedule::constructJobArray( std::ifstream& inFile2){ 
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
	void Schedule::loadAdjacencyMatrix(std::ifstream& inFile1){
		int parent, child;
		while(inFile1>>parent && inFile1>>child){
			adjacencyMatrix[parent][child] = 1;
		}
	}

	//adjacencyMatrix[parent][child] = 1, ,means that a parent-child relationship exists.
	//if adjacencyMatrix[parent][child] = 0, then the job, "child", is not dependant on the job "parent"   	
	void Schedule::computeParentCount(){//call each node in adjacencyMatrix
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
	void Schedule::computeDependantCountArray(){//call each node in adjacencyMatrix
		int tempCount = 0;
		for(int i = 1; i<=numberOfJobs; i++){
			for(int j = 1; j<=numberOfJobs; j++){
				if(adjacencyMatrix[i][j] == 1  && i!=j) tempCount++;
			}
			(*jobArray[i]).dependantCount = tempCount;
			tempCount = 0;
		}		
	}
	


	//checks the diagonal of the 2D array (adjacencyMatrix[i][i]), all non zero elements mean that the job still exists in the graph - ie has not been added to the schedule.
	bool Schedule::graphIsEmpty(){
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
	Jobs* Schedule::findOrphan(){
		for(int i = 1; i<=numberOfJobs; i++){
			if((*jobArray[i]).parentCount<=0   &&   !((*jobArray[i]).onLineWaitingToBeProcessed)){
				return jobArray[i];
			}
		}
		return nullptr;
		//return jobArray[1];// returns the first job - will check if it has been processed
	}
	
	
	
	void Schedule::loadOrphanJobsIntoLinkedList(){
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
	void Schedule::putJobOnTable(Processor* inserting, int avaliableProcessor){
		this->changeMadeInTable = true;
		int jobTIME = (*inserting).ProcessingJob;
		for(int i = currentTime; i< (*jobArray[jobTIME]).jobTime +currentTime ; i++){
			scheduleTable[avaliableProcessor][i] = (*inserting).ProcessingJob;
		}
	}
	
	//iterates through the processor array
	//when it finds an avaliable processor it loads the first available job onto it.
	//Runs, loading jobs onto processors until it runs out of available processors or open jobs.
	void Schedule::loadProcessorArray(){
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
	void Schedule::updateProcessorTime(){
		for(int i = 1; i< totalNumberOfProcessors+1; i++ ){
			(*processorArray[i]).timeRemainingOnCurrentJob--;
		}
	}
	
	//checks to see if there is time remaining for the processor to complete its current job
	bool Schedule::processorComplete(int i){
		if((*processorArray[i]).timeRemainingOnCurrentJob > 0  ) return false;
		else return true;
	}
	
	
	bool Schedule::allProcessorsComplete(){
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
	bool Schedule::hasCycle(){
		if (  ((*(*openJobs).listHead).next == NULL) &&   !graphIsEmpty()  &&  allProcessorsComplete()  ){
			return true;
		}
		return false;
	}
	
	void Schedule::printScheduleTable(std::ofstream& outFile1){
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
			outFile1 <<std::endl;		
			if(i!=0){
				for(int j = 0; j<=totalJobTime; j++){
					outFile1<<"-----";
				}
				outFile1 <<std::endl;	
			}		
		}
		outFile1 <<std::endl;
	}
	
	//removing both the parent and child dependancies contained within the graph that depend on that node.
	void Schedule::removeNodeFromGraph(int ID){
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
	void Schedule::deleteFinishedNodes(){
		for(int i = 1; i< totalNumberOfProcessors+1; i++ ){
			if(processorComplete(i)){
				removeNodeFromGraph((*processorArray[i]).ProcessingJob);
				//make processor available- done automatically
			}
		}

	}



	void Schedule::processorStatus(){
		std::cout<<"\n\n Processor List: ";
		for(int i = 1; i <=totalNumberOfProcessors; i++){
			std::cout<<" Processor: "<<i<<" JobID: "<< (*processorArray[i]).ProcessingJob<< "current time: "<< currentTime <<" time remaining: "<<(*processorArray[i]).timeRemainingOnCurrentJob<<std::endl;
		}
		std::cout<< std::endl<< std::endl;
	}
	
	
	void Schedule::createSchedule(int numJobs, int numProc, std::ifstream& inFile1, std::ifstream& inFile2, std::ofstream& outFile1){
		bool cycleDetection = false;
		constructJobArray(inFile2);
		loadAdjacencyMatrix(inFile1);
		computeParentCount();
		computeDependantCountArray();
		int it = 0;
		while(! graphIsEmpty()    ||   !allProcessorsComplete()  ){
			loadOrphanJobsIntoLinkedList();
		//	(*openJobs).printList(outFile2);
			loadProcessorArray();
			cycleDetection = hasCycle();
			if(cycleDetection) {
				outFile1<<"\nCYCLE DETECTED, NO SCHEDULE POSSIBLE\n";
				return;
			}
		/*	if(changeMadeInTable){
				printScheduleTable(outFile1);	
			}
		*/
			changeMadeInTable = false;
			currentTime++;
			updateProcessorTime();
			deleteFinishedNodes();	
			it++;
			//processorStatus();		
		}
		printScheduleTable(outFile1);
		
	}
Schedule::~Schedule(){
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
