#include<sstream> 	
#include <iostream>
#include<fstream>
#include<string>
#include "Schedule.h"


int main(int argc, char** argv){
	try{
		std::ifstream inFile1, inFile2;
		std::ofstream outFile1;
		inFile1.open(argv[1]);
		inFile2.open(argv[2]);
		std::string numK = argv[3];
		outFile1.open(argv[4], std::ios::app);
	

		std::stringstream temp(numK);
		int K;
		temp >>K;
		int numJobs, check;
		inFile1 >> numJobs;
		inFile2>>check;
		if(numJobs != check){
			std::cout<<"error: different number of Jobs given in the infiles\n";
			return 0;
		}
		Schedule* newSchedule = new Schedule(numJobs, K);
		(*newSchedule).createSchedule(numJobs, K,inFile1, inFile2, outFile1);
		outFile1.close();
		inFile1.close();
		inFile2.close();
	}
	catch(std::exception& e){
		std::cout<<e.what();
	}
	return 0;
}
