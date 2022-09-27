#ifndef CONNECT_RECEIVER_H
#define CONNECT_RECEIVER_H

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
#include <string.h>
#include "send_receive_tools_receiver.h"

void connectChannel(char* server_ip, int server_port);
void receiveFromChannel(char* server_ip, int server_port, char* file_name, SOCKET* recv_socket);
#endif