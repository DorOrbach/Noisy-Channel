#ifndef SEND_RECEIVE_TOOLS_H
#define SEND_RECEIVE_TOOLS_H

//This module is mostly taken from the tirgul of the course "Introduction to Systems Programming".

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

typedef enum { TRNS_FAILED, TRNS_DISCONNECTED, TRNS_SUCCEEDED } TransferResult_t;
TransferResult_t SendBuffer(const char* Buffer, int BytesToSend, SOCKET sd);
TransferResult_t SendString(const char* Str, SOCKET sd);
TransferResult_t ReceiveBuffer(char* OutputBuffer, int RemainingBytesToReceive, SOCKET sd);
TransferResult_t ReceiveString(char** OutputStrPtr, SOCKET sd);

#endif // SOCKET_SEND_RECV_TOOLS_H



