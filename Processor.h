#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor
{
	friend class Schedule;
	
	public:
		Processor(int ProcessingJob, int timeRemainingOnCurrentJob);
		
	protected:
		int ProcessingJob; //will be initialized to zero when avaliable
		int timeRemainingOnCurrentJob;
};

#endif
