#define _CRT_SECURE_NO_WARNINGS

#include"hamming.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"FileHandler.h"
#include <math.h>

#define MESSAGE_SIZE 249
#define ASCII_OFFSET 48
#define MESSAGE_IN_BYTES 31

#define ORIGINAL_MESSAGE_LEN 27
#define ORIGINAL_MESSAGE_BITS 209

//this function check if a number is a power of 2 or not:
//it is taken from "geeksforgeeks.org"
int is_power_2(int num) {
	if (num == 1 || num==2) {
		return 1;
	}
	else {
		int i = 2, val = 0;
		while (val<num) {
			val = pow(2, i);
			if (val == num) {
				return 1;
			}
			i++;
		}
	}
	return 0;
}

/*Description:
* this functions perform the hamming code on each 26 bits according to the start index given.
* input: bin_message - 26 bits of the original message in binary representation,
*        start_index - the index from which to start counting 26 bits.
*output: a string which contains the hamming code for the 26 bits given.
*/
int check_hamming_parity(char* bin_message) {
	int i = 0, k = 0, j = 0;
	int xor_res = 0b00000000, value = 0;
	int false_index = 0;
	while (i < MESSAGE_IN_BYTES - 1) {
		k = i + 1;
		if (i!=0 && i!=1 && i!=3 && i!=7 && i!=15) {//skip over data bits
			i++;
			continue;
		}
		//this is a check bit: check if parity is correct.
		j = k + 1;
		while (j < MESSAGE_IN_BYTES - 1) {
			if ((j & k) == k) {
				xor_res = xor_res ^ (bin_message[j - 1] - ASCII_OFFSET);
			}
			j++;
		}
		if (bin_message[i] != xor_res + ASCII_OFFSET) {
			false_index = false_index+ i;
		}
		i++;
	}
	return false_index;
}


int hamming_decryption(char *str, const char *output_path) {
	//check for each bit if it's parity is correct:
	int i = 0, false_index = 0, corrected = 0;
	while (i < MESSAGE_SIZE - 1) {
		false_index = check_hamming_parity(str + i);
		if (false_index != 0) {
			//flip the bits.
			if (str[false_index] == '0') {
				str[false_index] = '1';
			}
			else {
				str[false_index] = '0';
			}
			corrected++;
		}
		i = i + 8;
	}
	//print the corrected data to output file:
	print_to_output_file(str, output_path);
	return corrected;
}


char* delete_parity(char* str) {
	int i = 0, j=0;
	char deleted_hamming_str[ORIGINAL_MESSAGE_LEN];
	while (i < MESSAGE_IN_BYTES) {
		if (i==0 || i==1 || i==3 || i==7 || i==15) {
			i++;
			continue;
		}
		deleted_hamming_str[j] = str[i];
		i++;
		j++;
	}
	deleted_hamming_str[j] = '\0';
	return deleted_hamming_str;
}

char* delete_parity_bits(char* str) {
	char deleted_hamming_str[ORIGINAL_MESSAGE_BITS+1];
	int i = 0, k = 0;
	while (i < MESSAGE_SIZE - 30) {
		strcpy(deleted_hamming_str+k,delete_parity(str + i));
		i = i + (MESSAGE_IN_BYTES-1);
		k = k + (ORIGINAL_MESSAGE_LEN-1);
	}
	deleted_hamming_str[ORIGINAL_MESSAGE_BITS-1] = '\0';
	return deleted_hamming_str;
}

