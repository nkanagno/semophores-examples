/* client program to demonstrate shared memory with semaphores.*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHMSZ 64   /* Shared memory size */

void s_wait(int);
void s_signal(int);
struct sembuf sop;

int main(){
	int shmid, semid;
	key_t key_shm, key_sem;
	int *shm;
	int balance,t;
    /* Segment named by the serve "5678" */
	key_shm = 5678;
    /* Locate the segment.  */
	if ((shmid = shmget(key_shm, SHMSZ, 0666)) < 0) {
		perror("shmget");
		return 1;
	}
    /* Attach the segment to data space.*/
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		return 1;
	}
	/* generate key for the semaphore */
	if ((key_sem = ftok("shm_server01.c", 'Q')) == -1){ 
		perror("ftok");	
	}
	semid=semget(key_sem,1,IPC_CREAT|0666);     /* Connect to the semaphore  */

    /* Read credit from the server. */
	s_wait(semid);
	balance = *shm;
	printf("Client's balance = %d\nAmount = ",balance);
	scanf("%d",&t);
	*shm = balance - t;
	s_signal(semid);
	printf("Client's new balance = %d\n",*shm);
    	
	return 0;
}

void s_wait(int semid){
	sop.sem_num=0;
	sop.sem_op=-1;
	sop.sem_flg=0;
	if(semop(semid, &sop, 1) != 0){
		perror("Locking:");
	}
	else{
		printf("Semaphore Lock \n");
	}
}
void s_signal(int semid){
	sop.sem_num=0;
	sop.sem_op=1;
	sop.sem_flg=0;
	if(semop(semid, &sop, 1) != 0){
		perror("Unlocking:");
	}
	else{
		printf("Semaphore Unlock\n");
	}
} 
