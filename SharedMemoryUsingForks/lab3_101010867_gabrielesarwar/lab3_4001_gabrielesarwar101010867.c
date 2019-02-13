#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#include "shm_com.h"

int main()
{


	//we create shared mem segment with a call to shget with IPC_creat bit specified
	int running = 1;
	void *shared_memory = (void *)0;
	struct shared_use_st *shared_stuff;
	int shmid;
	srand((unsigned int)getpid());    
	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	
	

	//we now make shared memory accessible to the program.
	//this attathes memory to both parent and child
	shared_memory = shmat(shmid, (void *)0, 0);
	if (shared_memory== (void *)-1) {
	    fprintf(stderr, "shmat failed\n");
	    exit(EXIT_FAILURE);
	}

	printf("Memory attatched at %X\n", (int)shared_memory);

	pid_t pid;
	char *message;
	int n;

	pid = fork(); //create a child using fork()

	shared_stuff = (struct shared_random_st *)shared_memory;
	
	switch(pid) 
	{
	case -1:
		perror("fork fail");
		exit(1);
	case 0:
		//child
	while(running) {
		
		//CHILD-want to generate a random number and put it the shared memory within the 			child process
		shared_stuff->randomnumber = rand() % 20;;
		if (shared_stuff-> randomnumber >= 5) {
			running = 0;
			shared_stuff -> flagon =1; //sends signal to the parent
		
		}
	}
		break;
	default:
		//this is parent
		shared_stuff->flagon = 0;
		while(running){
			if(shared_stuff->flagon) {
				printf("number is: %i", shared_stuff->randomnumber);
				sleep( rand() % 4 ); /* make the other process wait for us ! */
				shared_stuff -> flagon = 0;
				running=0;	
			}

		}
	break;
   	

	
   }
	if (shmdt(shared_memory) == -1) {
            
	    exit(EXIT_FAILURE);
	}
	
	if (shmctl(shmid, IPC_RMID, 0) == -1) {
	   
	   exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
