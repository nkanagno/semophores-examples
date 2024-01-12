#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/sem.h>
#include <unistd.h>

#define SHMSZ 64  /* Shared memory size */

void s_wait();
void s_signal();
struct sembuf sop;

int main(){
	int shmid, semid;
	key_t key_shm,key_sem;
	int *shm;
	int balance = 500;
   	key_shm = 5678;	 /*Name of shared memory segment "5678".*/
    /* Create the segment. */
	if ((shmid = shmget(key_shm, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		return 1;
	}
    /* Attach the segment to data space. */
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		return 1;
	}
    /* generate key for the semaphore */
	if ((key_sem = ftok("shm_server01.c", 'Q')) == -1){ 
		perror("ftok");	
	}
	semid=semget(key_sem,1,IPC_CREAT|0666);     /* Create semaphore  */
	semctl(semid,0,SETVAL,1);		    /*Set value to 1 */
  
	*shm = balance;
	
    /* Wait until the other process changes the credit. */
	for(int i = 0;i<5;i++){
		while (*shm == balance) sleep(1);
		printf("Balance = %d\n",*shm);
		balance = *shm;	
	}
    /* Release share memory */
	if(shmctl(shmid,IPC_RMID,NULL) == -1)	{
		perror("shmctl :");
	}
    /* Release semaphore */
	semctl(semid,0,IPC_RMID);
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
