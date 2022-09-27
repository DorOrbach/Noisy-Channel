//This module deals with all the functions that incharge of performing the hamming code.

#define _CRT_SECURE_NO_WARNINGS
#include "encryption.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
		zero_padded_str[i+1] = str[i];
		i--;
	}
	zero_padded_str[0] = '0';
	return zero_padded_str;
}

/*Description: convert a string to its binary represenation.
* Input: and array which is a string.
* output: a string containing the binary representation of the input string.
*/
char* convert_string_to_binary(const char *str) {
	char binary_str[LINE_LEN];
	char buff[BYTE_SIZE+1];
	int i = 0, k = 0;
	while (i < ORIGINAL_LEN) {
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

//this function check if a number is a power of 2 or not:
//it is taken from "geeksforgeeks.org"
int is_power_2(int num) {
		return (ceil(log2(num)) == floor(log2(num)));
}

/*Description:
* this functions perform the hamming code on each 26 bits according to the start index given.
* input: bin_message - 26 bits of the original message in binary representation, 
*        start_index - the index from which to start counting 26 bits.
*output: a string which contains the hamming code for the 26 bits given.
*/
char* perform_hamming(char* bin_message, int start_index) {
	char hamminged_str[HAMMING_LEN];
	int i = 0, k = 0, j = 0;
	//copy the data bits to the str:
	while (i < HAMMING_LEN) {
		k = i + 1;
		if (!is_power_2(k)) {
			hamminged_str[i] = bin_message[j+ start_index];
			j++;
		}
		i++;
	}
	//now perform the calculation for each "check" bit:
	i = 0;
	j = 0;
	k = 0;
	int xor_res = 0b00000000, value = 0;;
	while (i < HAMMING_LEN-1) {
		k = i + 1;
		if (!is_power_2(k)) {
			i++;
			continue;
		}
		//this is a check bit:
		j = k + 1;
		while (j < HAMMING_LEN-1) {
			if ((j & k) == k) {
				xor_res = xor_res^(hamminged_str[j - 1]- ASCII_OFFSET);
			}
			j++;
		}
		hamminged_str[i] = xor_res + ASCII_OFFSET;
		i++;
	}
	hamminged_str[HAMMING_LEN-1] = '\0';
	return hamminged_str;
}


/*Description: perform the hamming code on the entire message.
* input: bin_message - binary representation of the message.
* output: encrypted_str - binary representation of the code after hamming.
*/
char* message_encryption(char* bin_message) {
	char encrypted_str[ENCRYPTED_LINE_LEN];
	int start_index = 0, copy_index = 0;
	while (start_index < LINE_LEN-2) {
		strcpy(encrypted_str + copy_index, perform_hamming(bin_message, start_index));
		start_index += ORIGINAL_LEN;
		copy_index += (HAMMING_LEN-1);
	}
	return encrypted_str;
}

/*Description:
* convert the binary representation of the string (after performing hamming code)
* to a string. every 8 bits is sumed to a single char.
* input: bin_str.
* output: str.
*/
char* convert_bin_to_string(char* bin_str) {
	char str[HAMMING_LEN];
	int value = 0, j = 0, i = 0, iter = 1;
	while (i < HAMMING_LEN-1) {
		value = 0;
		while (j < 8*iter) {
			if (bin_str[j] == '1') {
				value = value + pow(2, (7 - (j%8)));
			}
			j++;
		}
		str[i] = value;
		i++;
		iter++;
	}
	str[i] = '\0';
	return str;	
}