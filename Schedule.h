#ifndef SCHEDULE_H
#define SCHEDULE_H
#include "linkedList.h"
#include "Processor.h"


class Schedule
{
	public:
		Schedule(int numberOfJobs, int totalNumberOfProcessors1);
		void createSchedule(int numJobs, int numProc, std::ifstream& inFile1, std::ifstream& inFile2, std::ofstream& outFile1);
		~Schedule();
		
	protected:
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
		
		
		void constructJobArray(std::ifstream& inFile2);
		void loadAdjacencyMatrix(std::ifstream& inFile1);
		void computeParentCount();
		void computeDependantCountArray();
		bool graphIsEmpty();
		Jobs* findOrphan();
		void loadOrphanJobsIntoLinkedList();
		void putJobOnTable(Processor* inserting, int avaliableProcessor);
		void loadProcessorArray();
		void updateProcessorTime();
		bool processorComplete(int i);
		bool allProcessorsComplete();
		bool hasCycle();
		void printScheduleTable(std::ofstream& outFile1);
		void removeNodeFromGraph(int ID);
		void deleteFinishedNodes();
		void processorStatus();

};

#endif
