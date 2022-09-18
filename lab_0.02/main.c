#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// TODO reading from pipeline

int main(int argc, char** argv) {
	char c;

	if (argc > 1) {
		FILE* open_file = NULL;
		c = getopt(argc, argv, "f:");
		
		if (c == -1) {
			fprintf(stderr, "Error, no option\n");
			return 3;
		}
		
		switch (c) {
			case 'f':
				if ((open_file = fopen(optarg, "r")) != NULL) {
					char buffer[2048];
					while (fgets(buffer, sizeof(buffer), open_file) != NULL) {
						if (strstr(buffer, argv[1]) != NULL) {
							fprintf(stdout, "%s", buffer);
						}
					}
					fclose(open_file);
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