#define _CRT_SECURE_NO_WARNINGS
#include "noise.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_MESSAGE 248
#define MESSAGE_LEN 32
#define BYTE_SIZE 8
#define LINE_LEN 249
#define ORIGINAL_LEN 32


/*Description:
* this function gets an array with 6 slots, which represent a char, and add zeros at the start
* in order to get a 8 bit representation.
* input: str - string with binary representaion of a char.
* output: zero_padded_str - the same represenation only in 8 bit.
*/
char* zero_padding(char* str) {
	char zero_padded_str[BYTE_SIZE + 1];
	int i = 7;
	while (i > -1) {
		zero_padded_str[i + 1] = str[i];
		i--;
	}
	zero_padded_str[0] = '0';
	return zero_padded_str;
}

/*Description: convert a string to its binary represenation.
* Input: and array which is a string.
* output: a string containing the binary representation of the input string.
*/
char* convert_string_to_binary(const char* str) {
	char binary_str[LINE_LEN];
	char buff[BYTE_SIZE + 1];
	int i = 0, k = 0;
	while (i < ORIGINAL_LEN-1) {
		_itoa(str[i], buff, 2);
		//this will give us 7 bits and we need to add 1 more zero at the start:
		strcpy(buff, zero_padding(buff));
		//copy the binary representation of the string to an array:
		strcpy(binary_str + k, buff);
		k = k + BYTE_SIZE;
		i++;
	}
	return binary_str;
}

char* generate_noise(char* rec_message, char* args[], int* errors) {
	int ssc_res = 0;
	//insert noise:
	if (strcmp(args[1], "-d") == 0) {
		//reverese every "nth" bit
		int next_bit_to_reverse = 0;
		ssc_res = sscanf(args[2], "%d", &next_bit_to_reverse);
		errors = reverse_bits(rec_message, next_bit_to_reverse);
		return rec_message;

	}
	if (strcmp(args[1], "-r") == 0) {
		//random noise
		int bit_reverse_prob = 0, random_seed = 0;
		ssc_res = sscanf(args[2], "%d", &bit_reverse_prob);
		ssc_res = sscanf(args[3], "%d", &random_seed);
		create_random_noise(rec_message, bit_reverse_prob, random_seed, errors);
		return rec_message;
	}
}

void create_random_noise(char* data, int probabillity, int seed, int* errors) {
	double  random = 0.0;
	int mask = 1;
	srand(seed);
	int i = 0;
	while (i < SIZE_OF_MESSAGE) {
		double f = (double)rand() / RAND_MAX;
		random = f * (65536);
		if (random > probabillity) {
			if (data[i] == '1') {
				data[i] = '0';
				errors++;
			}
			else {
				data[i] = '1';
				errors++;
			}
		}
		i++;
	}
}

int reverse_bits(char* data, int next_bit_to_reverse) {
	int i = 0;
	int n = next_bit_to_reverse - 1;
	int errors = 0;
	if (n == 0) {
		return 0;
	}
	while (i < LINE_LEN - 1) {
		if (i == n) {
			if (data[i] == '1') {
				data[i] = '0';
				errors++;
				n += n;
			}
			else {
				data[i] = '1';
				n += n;
				errors++;
			}
		}
		i++;
	}
	return errors;

}