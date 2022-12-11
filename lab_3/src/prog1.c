#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

#define SHM_NAME "shmem_file"
#define SHM_SIZE 64

int g_shmid = 0;

void signalHandler() {
	if (g_shmid) {
		printf("Destroying shared memory segment\n");
		shmctl(g_shmid, IPC_RMID, NULL);
	}
	exit(0);
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);

	struct timespec ts;
	struct stat buffer;
	FILE* file;
	
	if (stat(SHM_NAME, &buffer) != 0) {
		if (!(file = fopen(SHM_NAME, "w"))) {
			fprintf(stderr, "Shared memory file cannot be created: %s(%d)\n", strerror(errno), errno);
			exit(1);
		}
		fclose(file);
	}

	key_t shm_key = ftok(SHM_NAME, 1);
	g_shmid = shmget(shm_key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
	if (g_shmid == -1 && errno == EEXIST) {
		fprintf(stderr, "The program is already running: %s(%d)", strerror(errno), errno);
		exit(1);
	}
	if (g_shmid == -1) {
		fprintf(stderr, "%s(%d)\n", strerror(errno), errno);
		exit(1);
	}
	printf("[first] key: %d\n[first] mem_id: %d\n", shm_key, g_shmid);

	char* shm_ptr = shmat(g_shmid, NULL, 0);
	printf("[first] shm_ptr: %p\n", shm_ptr);

	while(1) {
		char str[64];
		clock_gettime(CLOCK_REALTIME, &ts);
		struct tm* curr = localtime(&ts.tv_sec);
		double sec_ns = (double)curr->tm_sec + ((double)ts.tv_nsec / 1000000000.);
		sprintf(str, "{%2d:%2d:%.3lf} pid = %d", curr->tm_hour, curr->tm_min, sec_ns, getpid());
		strcpy(shm_ptr, str);
		sleep(5);
	}
	
	
	return 0;
}