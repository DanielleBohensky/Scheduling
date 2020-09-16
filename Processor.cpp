#include "Processor.h"

	Processor::Processor(int ProcessingJob, int timeRemainingOnCurrentJob){
		this->ProcessingJob = ProcessingJob;
		this->timeRemainingOnCurrentJob = timeRemainingOnCurrentJob;
	}
