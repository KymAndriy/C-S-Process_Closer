#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#include "main.hpp"

#pragma comment(lib, "Ws2_32.lib")

#define PORT "8088"
#define BUFFER_LEN 512


int main()
{
    WSADATA wsaData;

    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    addrinfo* result = NULL, * ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    SOCKET ListenSocket = INVALID_SOCKET;

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    SOCKET ClientSocket = INVALID_SOCKET;
    
    ClientSocket = accept(ListenSocket, nullptr, nullptr);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    char buffer[512] = {0};

    int valread;

    std::vector<Proc_Info> pid_name = get_pid_and_name();
    unsigned long size = pid_name.size();
    data_len.len = size;
    send(ClientSocket, data_len.buf, sizeof(data_len.buf), 0);

    for (unsigned long i = 0; i < size; i++)
    {
        memset(buffer, 0, BUFFER_LEN);
        sprintf(buffer, "Number: %lu, PID: %lu - %s\n", (i + 1), pid_name.at(i).pid, pid_name.at(i).name);
        if (send(ClientSocket, buffer, strlen(buffer), 0) < 0)
        {
            perror("not send");
        }
        if (i == (size - 1))
        {
            send(ClientSocket, "EOF", sizeof("EOF"), 0);
        }
    }

    unsigned long pid;

    if((valread = recv(ClientSocket, data_len.buf, sizeof(data_len.buf), 0)) < 1)
    {
        perror("Not recieved");
    }

    pid = data_len.len;

    if(kill_proc_by_pid(pid) >= 0){
        send(ClientSocket, "Success in closing process\n", sizeof("Success in closing process\n"), 0);
    }
    else
    {
        send(ClientSocket, "Failure in closing process\n", sizeof("Failure in closing process\n"), 0);
    }

    printf("%lu\n", data_len.len);

    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}