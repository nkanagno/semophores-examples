#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ 64  /* Shared memory size */

int main(){
	char c;
	int shmid;
	key_t key;
	int *shm;
	int balance = 500;
   	key = 5678;	 /*Name of shared memory segment "5678".*/
    /* Create the segment. */
	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		return 1;
	}
    /* Attach the segment to data space. */
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		return 1;
	}

    /* Put some things into the memory for the other process to read. */
	*shm = balance;
	
    /* Wait until the other process changes the credit. */
	for(int i = 0;i<10;i++){
		while (*shm == balance) sleep(1);
		printf("Balance = %d\n",*shm);
		balance = *shm;	
	}
    /* Release share memory */
	if(shmctl(shmid,IPC_RMID,NULL) == -1)	{
		perror("shmctl :");
	}
    return 0;
}
