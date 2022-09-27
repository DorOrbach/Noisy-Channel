/*Module description:
* this module deals with setting up the communication between the client and the server:
* - initiate the socket.
* - connect to the server.
* - recieving messages.
* - perform the correct operation regarding each message.
* - sending the message via the channel.
*/
#include "connectClient.h"
#include <stdio.h>
#include "SocketSendRecvTools.h"
#include "FileHandler.h"
#include "encryption.h"

#define MAX_MSG 27
#define ENCRYPTED_MESSAGE 32

char *ip=NULL;
int port=0;
char *name=NULL;
char *file=NULL;
SOCKET m_socket; // comuunication socket



/*Description
* This function creates the socket and connects to the server
* recieve - ip, port, user name from the main function
*/
int connectChannel(char* server_ip, int server_port) {
	char file_name[MAX_FILE_NAME];
	int token;
	int exit_choice = 0;
	ip= server_ip;
	port = server_port;
	WSADATA wsaData;
	//Call WSAStartup and check for errors.
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("Error at WSAStartup()\n");
	}
	//create socket:
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Check for errors to ensure that the socket is a valid socket.
	if (m_socket == INVALID_SOCKET) {
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
	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		//connection failed
		printf("Failed connecting to server, check ip and port and retry\n");
	}
	else {
		//connection succeed
		//ask for file name - if "quit" then the program will close.
		strcpy(file_name, (get_file_name()));
		if (strcmp(file_name, "quit") == 0) {
			exit_choice = 1;
			return exit_choice;
		}
		//perform hamming and send to channel:
		sendToChannel(server_ip, server_port, file_name, &m_socket);
		//finishing and closing socket
		closesocket(m_socket);
		if (WSACleanup() == SOCKET_ERROR) {
			printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
			exit(1);
		}
		return 0;
	}
}



/*Description:
* After a connection is made, this function send the messages to the channel.
* before sending the message it will be sent to "message encryption" which will perform the hamming code.
*/
void sendToChannel(char* server_ip, int server_port, char* file_name, SOCKET* m_socket) {
	char send_str[HAMMING_LEN], encrypted_str[ENCRYPTED_LINE_LEN], bin_message[LINE_LEN];
	char buff[MAX_MSG];
	//open file:
	FILE* f_ptr = open_file(file_name);
	//read the file 26 bytes at a time:
	int sent = 0;
	TransferResult_t send_res = 0;
	while (fscanf(f_ptr, "%026s", buff) !=EOF) {
		//encrypted using hamming code:
		strcpy(bin_message, convert_string_to_binary(buff));
		strcpy(encrypted_str, message_encryption(bin_message));
		strcpy(send_str, convert_bin_to_string(encrypted_str));
		send_res = SendString(encrypted_str, *m_socket);
		if (TRNS_SUCCEEDED != send_res) {
			//connection failed
			printf("Failed connecting to channel on %s:%d\n", server_ip, server_port);
			exit(1);
		}
		sent++;		
	}
	//finished iterating over the entire file:
	//print the file size and the bytes that was sent:
	printf("file length: %d\n", sent * (MAX_MSG-1));
	printf("sent: %d\n", sent * (HAMMING_LEN-1));
	fclose(f_ptr);
	return;
}
