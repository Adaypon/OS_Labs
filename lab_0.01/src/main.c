#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv) {
	FILE* fd = NULL;
	char c;

	// arguments starts at argv[1]
	for (int i = 1; i <= argc; ++i) {
		if (argc > 1) {
			if (i == argc) {
				break;
			}

			if ((fd = fopen(argv[i], "r")) == NULL) {
				int error = errno;
				fprintf(stderr, "Error (%d): %s: %s\n", error, argv[i], strerror(errno));
				continue;
			}
		}
		else {
			fd = stdin;
		}

		do {
			c = fgetc(fd);
			printf("%c", c);
		} while (c != EOF);

		if (argc > 1) {
			fclose(fd);
		}
	}
	return 0;
}