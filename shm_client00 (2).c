/*shm-client - client program to demonstrate shared memory.*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define SHMSZ 64   /* Shared memory size */

int main(){
	int shmid;
	key_t key;
	int *shm;
	int balance,t;
    /* Segment named by the serve "5678" */
	key = 5678;
    /* Locate the segment.  */
	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
		perror("shmget");
		return 1;
	}
    /* Attach the segment to data space.*/
	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		return 1;
	}
    /* Read credit from the server. */
	balance = *shm;
	printf("Client's balance = %d\nAmount = ",balance);
	scanf("%d",&t);
	*shm = balance - t;
	printf("Client's new balance = %d\n",*shm);
    /* Change the first character of the segment to '*', indicating we have read 
       the segment. */
	
	return 0;
}
