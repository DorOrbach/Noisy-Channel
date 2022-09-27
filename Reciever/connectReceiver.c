/*Module description:
* this function deals with setting up the communication between the client and the server:
* - initiate the socket.
* - connect to the server.
* - recieving messages.
* - perform the correct operation regarding each message.
*/
#include "connectReceiver.h"
#include <stdio.h>
#include "send_receive_tools_receiver.h"
#include "hamming.h"

#define MAX_LEN 20
#define MAX_MSG 200
#define LOG_LEN 33 
#define LOG_MSG 221
#define STRINGS_ARE_EQUAL( Str1, Str2 ) ( strcmp( (Str1), (Str2) ) == 0 )

#define MAX_FILE_NAME 500
#define MESSAGE_SIZE 249
#define BYTE_SIZE 8
#define RECEIVED_SIZE 31

char* ip = NULL;
int port = 0;
char* name = NULL;
char* file = NULL;

/*Description:
* After a succssive connection this function recieve the messages from the server
* And sends them to the serverMessegas function to proccesing
*/
void receiveFromChannel(char* server_ip, int server_port, char* file_name, SOCKET* recv_socket) {
	TransferResult_t res;
	char* recv_str = NULL;
	int corrected = 0, sent = 0;
	while (1) {
		res = ReceiveString(&recv_str, recv_socket);
		if (TRNS_DISCONNECTED == res || TRNS_FAILED == res) {
			break;
		}
		//perform hamming decryption and print to file:
		sent++;
		corrected+=hamming_decryption(recv_str,file_name);
		free(recv_str);
		recv_str = NULL;

	}
	printf("received: %d\n", sent * BYTE_SIZE* RECEIVED_SIZE);
	printf("corrected %d\n", corrected);
	return;
}


/*Description
* This function creates the socket and connecting to the channel
* recieve - ip, port, user name from the main function
*/
void connectChannel(char* server_ip, int server_port) {
	SOCKET recv_socket= INVALID_SOCKET; // comuunication socket
	char file_name[MAX_FILE_NAME];
	char rec_str[MESSAGE_SIZE];
	int token;
	ip = server_ip;
	port = server_port;
	WSADATA wsaData;
	//Call WSAStartup and check for errors.
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("Error at WSAStartup()\n");
	}
	//create socket:
	recv_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Check for errors to ensure that the socket is a valid socket.
	if (recv_socket == INVALID_SOCKET) {
		printf("Failed connecting to server on %s:%d. Exiting\n", server_ip, server_port);
		WSACleanup();
		return;
	}
	int server_port_int = 0;
	SOCKADDR_IN clientService;
	//Create a sockaddr_in object clientService and set values.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(server_ip); //Setting the IP address to connect to
	clientService.sin_port = htons(server_port); //Setting the port to connect to.

	//connect to server:
	if (connect(recv_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		//connection failed
		printf("Failed connecting to server on %s:%d\n", server_ip, server_port);
	}
	else {
		//connection succeed
		printf("please send file name to write:\n");
		//file_name[0] = '\0';
		scanf("%s", &file_name);
		if (!strcmp(file_name, "quit")) {
			exit(1);
		}
		//get message from channel:
		receiveFromChannel(server_ip, server_port, file_name, recv_socket);
		//finishing and closing socket
		closesocket(recv_socket);
		if (WSACleanup() == SOCKET_ERROR) {
			printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
			exit(1);
		}
		return;
	}
}

