#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include "semun.h"

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);
static int sem_id;

// Lab4 SYSC 4001 Gabriele Sarwar, 101010867

int main(int argc, char *argv[])
{
	int i;
	pid_t pid;


	srand((unsigned int)getpid());
	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
	if (argc > 1) {
	if (!set_semvalue()) {
		fprintf(stderr, "failed to initialize semaphore " );
		exit(EXIT_FAILURE);
	}

	sleep(2);
}
	
	pid = fork();
	
	switch(pid){
	case -1:
		perror("fork failed");
		exit(1);
	case 0:
		        
	if (!semaphore_p()) exit(EXIT_FAILURE); //start wait process 
		printf("this is child: this out put is printed after parent is done due to wait()"); 
	semaphore_v();
		break;
	default:
		if (!semaphore_p()) exit(EXIT_FAILURE);
		printf("this is the parent: semaphore waits for this output first");
		semaphore_v();
		break;
	}

    
	printf("\n%d - finished\n", getpid());
	if (argc > 1) {    
		sleep(10);
		del_semvalue();
	}
	exit(EXIT_SUCCESS);
	}


	static int set_semvalue(void)
	{
	union semun sem_union;
	sem_union.val = 1;
	if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
	 return(0);
	return(1);
	}

	static void del_semvalue(void)
	{
	union semun sem_union;
	if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
		fprintf(stderr, "Failed to delete semaphore\n" ) ;
	}

	static int semaphore_p(void)
	{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1; /* P() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_p failed\n" ) ;
	return(0);
	}
	return(1);
	}

	static int semaphore_v(void)
	{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1; /* V() */
	sem_b.sem_flg = SEM_UNDO;
	if (semop(sem_id, &sem_b, 1) == -1) {
		fprintf(stderr, "semaphore_v failed\n");
	return(0);
	}
	return(1);
	}









