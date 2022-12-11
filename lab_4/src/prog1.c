#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define SHM_NAME "shmem_file"
#define SHM_SIZE 64

// TODO if there is already a process with this program
// exit with error message

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	struct timespec ts;
	struct sembuf sops;
	
	key_t key = ftok(SHM_NAME, 1);
	
	int semid = semget(key, 1, IPC_CREAT | 0666);
	if (semid == -1) {
		fprintf(stderr, "semget failed: %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	
	int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
	if (shmid == -1) {
		fprintf(stderr, "shmget failed: %s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	printf("[first] key: %d\n[first] mem_id: %d\n[first] sem_id: %d\n", key, shmid, semid);

	char* shm_ptr = shmat(shmid, NULL, 0);
	printf("[first] shm_ptr: %p\n", shm_ptr);

	semctl(semid, 0, SETVAL, (int)0); // init semaphore value with 0
	sops.sem_num = 0;
	sops.sem_flg = 0;

	while(1) {
		char str[64];
		clock_gettime(CLOCK_REALTIME, &ts);
		struct tm* curr = localtime(&ts.tv_sec);
		double sec_ns = (double)curr->tm_sec + ((double)ts.tv_nsec / 1000000000.);
		sprintf(str, "{%2d:%2d:%.3lf} pid = %d", curr->tm_hour, curr->tm_min, sec_ns, getpid());
		
		/* 
			at this point semaphore is open for prog1 process (semaphore value = 0), 
			so we can access shared memory
		*/
		strcpy(shm_ptr, str);
		sleep(10);
		
		/*
			allowing prog2 process to access critical section
			by setting sem_op = 2
		*/
		sops.sem_op = 2;
		semop(semid, &sops, 1);

		/* 
			waiting for semaphore to be opened 
			(when semaphore value will be reset to 0) 
			for prog1 process for next iteration
		*/
		sops.sem_op = 0;
		semop(semid, &sops, 1);
	}

	
	
	/*
	shmdt(shm_ptr);

	shmctl(shmid, IPC_RMID, NULL);
	*/
	
	return 0;
}