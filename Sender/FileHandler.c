/*This module contains the functions that deals with the file which the program reads.
* We did not add description o the functions because theire names are self explainatory.
*/

#define _CRT_SECURE_NO_WARNINGS
#include "FileHandler.h"
#define MAX_FILE_NAME 500

FILE* open_file(const char *path) {
	FILE* ptr = NULL;
	ptr = fopen(path, "r");
	if (ptr == NULL) {
		printf("Error: file do not exist, or can not open file, exiting program\n");
		exit(1);
	}
	return ptr;
}

char* get_file_name() {
	char file_name[MAX_FILE_NAME];
	printf("Enter file name:\n");
	//file_name[0] = '\0';
	scanf("%s", &file_name);
	if (!strcmp(file_name, "quit")) {
		exit(1);
	}
	return file_name;
}