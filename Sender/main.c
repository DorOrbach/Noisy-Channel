/*Authors: Yoav Bruker:206020372 and Dor Orbach:204589790.
* main function of the sander program.
*/

//includes ---------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SocketSendRecvTools.h"
#include "connectClient.h"

//defines ----------------------------------------------
#define MAX_PORT_LEN 20
#define MAX_IP_LEN 15
#define MAX_FILENAME_LEN 500



int main(int argc, char* argv[]) {
	char server_ip[MAX_IP_LEN];
	char server_port[MAX_PORT_LEN];
	char file_name[MAX_FILENAME_LEN];
	strcpy(server_ip, argv[1]);
	strcpy(server_port, argv[2]);
	int port = atoi(server_port);
	int choice = 0;
	while (choice != 1) {
		choice = connectChannel(server_ip, port);
	}
	exit(0);
}