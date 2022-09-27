#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "hamming.h"
#include <math.h>

#define MESSAGE_SIZE 249
#define ORIGINAL_MESSAGE_LEN 27
#define ORIGINAL_MESSAGE_BITS 209
#define ASCII_OFFSET 48

char* convert_bin_for_print(const char* bin_str) {
	char str[ORIGINAL_MESSAGE_LEN];
	int value = 0, j = 0, i = 0, iter = 1;
	while (i < ORIGINAL_MESSAGE_LEN - 1) {
		value = 0;
		while (j < 8 * iter) {
			if (bin_str[j] == '1') {
				value = value + pow(2, (7 - (j % 8)));
			}
			j++;
		}
		str[i] = value +ASCII_OFFSET;
		i++;
		iter++;
	}
	str[i] = '\0';
	return str;
}

void print_to_output_file(char* str, const char* file_path) {
	FILE* ptr = NULL;
	//open file for writing:
	ptr = fopen(file_path, "a");
	if (ptr == NULL) {
		printf("Error: can't open output file, exiting program\n");
		exit(1);
	}
	char deleted_hamming_str[ORIGINAL_MESSAGE_BITS];
	strcpy(deleted_hamming_str, delete_parity_bits(str));
	char output_string[ORIGINAL_MESSAGE_LEN];
	strcpy(output_string, convert_bin_for_print(deleted_hamming_str));
	fprintf(ptr, "%s", output_string);
	fclose(ptr);
	return;
}




