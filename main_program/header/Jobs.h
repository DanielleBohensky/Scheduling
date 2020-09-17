#ifndef JOBS_H
#define JOBS_H
#include<fstream>
#include<iostream>
#include<string>

class Jobs
{
	friend class Schedule;
	friend class node;
	friend class linkedList;	
	public:
		Jobs();
		Jobs(int jobTime, int ProcessorUsing, int parentCount, int dependantCount, int jobID);
		void printJob(std::ofstream& outFile);
		~Jobs();
		
	private:
		int jobTime;
		int ProcessorUsing;
		bool onLineWaitingToBeProcessed; 
		int parentCount;
		int dependantCount;
		int jobID;
};

#endif
