/* The channel program:
* opens 2 sockets and listen for connection from both sender and receiver
*/

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include "send_receive_tools.h"
#include "noise.h"

#define RECIEVER_INDEX 1
#define SENDER_INDEX 0
#define SIZE_OF_MESSAGE 248
#define MESSAGE_LEN 32
#define BYTE_SIZE 8

int process_data(SOCKET* snd_socket, SOCKET* rec_socket, char* args[], int transmitted) {
	char* rec_buffer = NULL, noisy_message[SIZE_OF_MESSAGE+1];
	int errors = 0, rec_res = 0, send_res = 0;
	while (TRUE) {
		rec_res = ReceiveString(&rec_buffer, snd_socket);
		if (rec_res == TRNS_FAILED) {
			printf("Error: channel disconnected");
			break;
		}
		if (rec_res == TRNS_DISCONNECTED) {
			break;
		}
		//Generate noise:
		strcpy(noisy_message, generate_noise(rec_buffer, args, errors));
		//send the noisy message to the receiver:
		send_res = SendString(noisy_message, *rec_socket);
		if (send_res == TRNS_FAILED || rec_res == TRNS_DISCONNECTED) {
			printf("Error: channel disconnected - can not send message");
			free(rec_buffer);
			break;
		}
		transmitted = &transmitted + 1;;
		//free the string in order to get a new one:
		free(rec_buffer);
		rec_buffer = NULL;
	}

	//end of transmition:
	return 1;
}

void close_and_free(SOCKET* snd_Socket, SOCKET* rec_Socket) {
	closesocket(snd_Socket);
	closesocket(rec_Socket);
	if (WSACleanup() == SOCKET_ERROR) {
		printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
		exit(1);
	}
	return;
}

void open_socket(char *args[]) {
	char* host_name = NULL, *IP_buffer=NULL;
	SOCKET snd_Socket = INVALID_SOCKET, rec_Socket = INVALID_SOCKET;
	WSADATA wsaData;
	unsigned long Address;
	struct hostent* Host_name;
	SOCKADDR_IN snd_addr, rec_addr;
	
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("Error at WSAStartup()\n");
	}
	Host_name = gethostbyname(host_name);
	//in order to use inet_addr() we need to first use inet_ntoa - taken from docs.microsoft .
	IP_buffer = inet_ntoa(*((struct in_addr*)Host_name->h_addr_list[0]));
	Address = inet_addr(IP_buffer);
	if (Address == INADDR_NONE) {
		printf("Error while converting The string \"%s\" into an ip address. ending program.\n", IP_buffer);
		exit(1);
	}
	
	// Create a socket. 
	snd_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	rec_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (snd_Socket == INVALID_SOCKET|| rec_Socket == INVALID_SOCKET)
	{
		printf("Error at socket( ): %ld\n", WSAGetLastError());
		exit(1);
	}

	//bind to a socket:
	rec_addr.sin_family = AF_INET;
	rec_addr.sin_addr.s_addr = Address;
	rec_addr.sin_port = 0;
	snd_addr.sin_family = AF_INET;
	snd_addr.sin_addr.s_addr = Address;
	snd_addr.sin_port = 0;
	int snd_bind_res = bind(snd_Socket, (SOCKADDR*)&snd_addr, sizeof(snd_addr));
	int rec_bind_res = bind(rec_Socket, (SOCKADDR*)&rec_addr, sizeof(rec_addr));
	if (snd_bind_res == SOCKET_ERROR && rec_bind_res == SOCKET_ERROR) {
		printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
		exit(1);
	}

	//Listen on th Sockets:
	int snd_listen_res = listen(snd_Socket, SOMAXCONN);
	int rec_listen_res = listen(rec_Socket, SOMAXCONN);
	if (snd_listen_res == SOCKET_ERROR && rec_listen_res == SOCKET_ERROR) {
		printf("Failed listening on socket, error %ld.\n", WSAGetLastError());
		exit(1);
	}
	
	//waiting for connections from sender and receicer - needs to print the ip and ports of both:
	//printing senders socket ip and port:
	char snd_IP[16];
	socklen_t snd_addr_len = sizeof(snd_addr);
	getsockname(snd_Socket, (struct sockaddr*)&snd_addr, &snd_addr_len);
	inet_ntop(AF_INET, &snd_addr.sin_addr, snd_IP, sizeof(snd_IP));
	int snd_Port = ntohs(snd_addr.sin_port);
	printf("sender socket: IP address= %s port = %u\n", snd_IP, snd_Port);
	
	//printing receivers socket ip and port:
	char rec_IP[16];
	socklen_t rec_addr_len = sizeof(rec_addr);
	getsockname(rec_Socket, (struct sockaddr*)&rec_addr, &rec_addr_len);
	inet_ntop(AF_INET, &rec_addr.sin_addr, rec_IP, sizeof(rec_IP));
	int rec_Port = ntohs(rec_addr.sin_port);
	printf("receiver socket: IP address= %s port = %u\n", rec_IP, rec_Port);
	
	//accept new connections:
	SOCKET client_socket[2] = { INVALID_SOCKET };
	char* rec_message = NULL, choice[5];
	int errors = 0;
	int end_of_transmition = 0, transmitted = 0;
	while (TRUE) {
		client_socket[RECIEVER_INDEX] = accept(rec_Socket, NULL, NULL);
		client_socket[SENDER_INDEX] = accept(snd_Socket, NULL, NULL);
		if (client_socket[SENDER_INDEX]== INVALID_SOCKET || client_socket[RECIEVER_INDEX]== INVALID_SOCKET)
		{
			printf("ERROR while Accepting connection with client, error %ld\n", WSAGetLastError());
			exit(1);
		}

		//connected to both sender and receiver - get data from sender:
		process_data(client_socket[SENDER_INDEX], &client_socket[RECIEVER_INDEX], args, &transmitted);
		closesocket(client_socket[RECIEVER_INDEX]);
		closesocket(client_socket[SENDER_INDEX]);
		transmitted *= MESSAGE_LEN;
		printf("transmitted: %d bytes, flipped: %d bits\n", transmitted, errors);
		printf("continue? (yes/no)\n");
		scanf("%s", choice);
		if (strcmp(choice, "no") == 0) {
			//close all sockets:
			close_and_free(snd_Socket, rec_Socket);
			exit(0);
		}
	}
	return;
}

int main(int argc, char* argv[]) {
	open_socket(argv);
}