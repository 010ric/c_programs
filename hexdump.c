#include <stdio.h>

void hexdump(FILE *output, char *buffer, int length) {
	for (int i = 0; i < length; i += 0x10) {
		fprintf(output, "%06x : ", i & 0xFFFFFF);
		for (int j = 0; j < 0x10; j++) {
			int index = i + j;
			if (index < length) {
				fprintf(output, "%02x ", buffer[index] & 0xFF);
			}
			else {
				fprintf(output, "   ");
			}
		}
		fprintf(output, "  "); 
		for (int j = 0; j < 0x10; j++) {
			int index = i + j;
			if (index < length) {
				fprintf(output, "%c", (buffer[index] < 0x20 || buffer[index] > 0x7e) ? '.' : buffer[index]);
			}
		}
		fprintf(output, "\n");
	}
	fprintf(output, "\n");
}

