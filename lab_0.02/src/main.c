#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


void grep(FILE* inputStream, const char* substr) {
	char buffer[2048];
	while (!feof(inputStream)) {
		while (fgets(buffer, sizeof(buffer), inputStream) != NULL) {
			if (strstr(buffer, substr) != NULL) {
				fprintf(stdout, "%s", buffer);
			}
		}
	}
}

int main(int argc, char** argv) {
	if (argc > 1) {
		FILE* openFile = NULL;
		char c = getopt(argc, argv, "f:");
		switch (c) {
			case -1:
				grep(stdin, argv[1]);
				break;
			
			case 'f':
				if ((openFile = fopen(optarg, "r")) != NULL) {
					grep(openFile, argv[1]);
					fclose(openFile);
				}
				else {
					int error = errno;
					fprintf(stderr, "Error (%d): %s\n", error, strerror(errno));
					return 2;
				}
				break;
			
			default:
				break;
		}
	}
	else {
		fprintf(stderr, "Error, no substring specified\n");
		return 1;
	}

	return 0;
}