#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	FILE* fd;
	char c;

	// arguments starts at argv[1]
	for (int i = 1; i <= argc; ++i) {
		if (argc > 1) {
			if (i == argc) {
				break;
			}

			if ((fd = fopen(argv[i], "r")) == NULL) {
				printf("Can't open file: %s\n", argv[i - 1]);
				exit(1);
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