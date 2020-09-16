# Scheduling

## Scheduling Algorithm Project for CS323:
### Project Description: This algorithm returns a schedule when given a list of jobs, the length of time required to complete each individual job, the dependencies existing between the jobs, and a predetermined number of processors to complete those jobs.


### Example Input
``` 
Input 1: A File containing the number of jobs, and then lists the parent child relationships:
Ex:	7		7 is the total number of jobs  
	3 4 		3 is the parent/prerequisite to 4
 	5 19		5 is the parent/prerequisite to 19
Input 2: A File containing the number of jobs and the time for each job
Ex:	7		7 is the total number of jobs
	1 23		Job 1 takes 23 units of time
	2 5		Job 2 take 5 units of time
Input 3: A string stating the number of total processors 
Input 4: An empty text file – prints the final completed schedule

```

### Design Process
```
Step 0:	Initialization: 
	G -> graph (from input file 1)
	Proc -> number of processors given by input 3
	Time = 0
Step 1: job -> get an unmarked “orphan node”/job from graph G
Step 2: Mark job
Step 3: Insert job into the linked-list, descending order based on the number of jobs dependent on it
Step 4: Repeat steps 1-4 until all unmarked orphan nodes are inserted into the linked-list
Step 5: Find an available processor
		If(no processors are available)  wait for the next time slice
Step 6: Remove the first job in the linked-list (the one with the most dependents) and place it into the available processor, setting the processor’s “in use until” time to be the amount of time to finish the new job it was given.
Step 7: Repeat steps 5 and 6 until the linked-list is empty or there are no more processors available.
Step 8: time++
Step 9: Job -> find a finished job (will be in an idle processor)
Step 10: Delete Job and all its outgoing edges (child dependencies) from graph G
Step 11: Repeat step 9 and 10 until there are no more idle processors
Step 12: Repeat steps 1-11 until graph G is empty.
```
	

