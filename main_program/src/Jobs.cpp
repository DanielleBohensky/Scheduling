#include "Jobs.h"

		Jobs::Jobs(){
		this->jobTime = -9999;
		this->ProcessorUsing = 0;
		this->onLineWaitingToBeProcessed = true;
		this->parentCount = 0;
		this->dependantCount = 9999;
		this->jobID = 0;		
	}
	
	Jobs::Jobs(int jobTime, int ProcessorUsing, int parentCount, int dependantCount, int jobID){
		this->jobTime = jobTime;
		this->ProcessorUsing = ProcessorUsing;
		this->onLineWaitingToBeProcessed = false;
		this->parentCount = parentCount;
		this->dependantCount = dependantCount;
		this->jobID = jobID;
	}
	
	void Jobs::printJob(std::ofstream& outFile){
		outFile << jobID <<  " " <<jobTime << " " <<parentCount <<" " <<dependantCount;
	}
	
	Jobs::~Jobs(){}
