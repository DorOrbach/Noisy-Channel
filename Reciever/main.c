/*Authors: Yoav Bruker:206020372 and Dor Orbach:204589790.
* main function of the receiver program.
*/

//includes ---------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "connectReceiver.h"

//defines ----------------------------------------------
#define MAX_PORT_LEN 20
#define MAX_IP_LEN 15
#define MAX_FILENAME_LEN 500
//#define STRINGS_ARE_EQUAL( Str1, Str2 ) ( strcmp( (Str1), (Str2) ) == 0 )
#define SIZE_OF_MESSAGE 206

int main(int argc, char* argv[]) {
	char server_ip[MAX_IP_LEN];
	char server_port[MAX_PORT_LEN];
	strcpy(server_ip, argv[1]);
	strcpy(server_port, argv[2]);
	int port = atoi(server_port);
	
	while (TRUE) {
		connectChannel(server_ip, port);
	}
}