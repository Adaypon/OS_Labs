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


int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	struct timespec ts;
	struct sembuf sops;
	struct stat buffer;

	if (stat(SHM_NAME, &buffer) != 0) {
		fprintf(stderr, "Cannot find shared memory file. Please run prog1 executable file first.\n");
		exit(1);
	}
		
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
	printf("[second] key: %d\n[second] mem_id: %d\n[second] sem_id: %d\n", key, shmid, semid);

	char* shm_ptr = shmat(shmid, NULL, SHM_RDONLY);
	printf("[second] shm_ptr: %p\n", shm_ptr);
	sops.sem_num = 0;
	sops.sem_flg = 0;

	printf("[second] Waiting...\n");
	sops.sem_op = -1;
	semop(semid, &sops, 1);
	/*
		initially, semaphore value is 0,
		so the prog2 process is blocked for now (0 + (-1) = -1 < 0)

		at the moment when sem_op is 2 in prog1, semaphore value += 2, so 0 + 2 = 2 >= 0
		
		checking sum: 2 + (-1) = 1 >= 0, unlock prog2 process, set semaphore value = 1
	*/

	clock_gettime(CLOCK_REALTIME, &ts);
	struct tm* curr = localtime(&ts.tv_sec);
	double sec_ns = (double)curr->tm_sec + ((double)ts.tv_nsec / 1000000000.);
	printf("{%2d:%2d:%.3lf} pid = %d reading shmem: %s\n", curr->tm_hour, curr->tm_min, sec_ns, getpid(), shm_ptr);
		
	shmdt(shm_ptr);
	sops.sem_op = -1;
	semop(semid, &sops, 1);
	/*
		semaphore value += (-1), so 1 - 1 = 0
		and prog1 process can be unlocked now 
	*/

	return 0;
}