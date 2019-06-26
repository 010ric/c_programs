#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IN 256

int main(int argc, char **argv) {
	char *buf = malloc(MAX_IN);
	while(1) {
		buf = fgets(buf, MAX_IN, stdin);
		if (strncmp(buf, "exit", 4) == 0)
			goto end;
		system(buf);
	}
	end:
	free(buf);
	return 0;
}
