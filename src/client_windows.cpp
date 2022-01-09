#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include "main.hpp"

#pragma comment(lib, "Ws2_32.lib")
#define PORT "8088"
#define BUFFER_LEN 512
#define SRT_IP_MAX_LEN 16

int main()
{
    
    char* ip_str = (char*)malloc(SRT_IP_MAX_LEN);
    printf("Write IP (127.0.0.1) that you want to connect: ");
    scanf("%s", ip_str);
    WSADATA wsaData;

    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    addrinfo* result = NULL,
        * ptr = NULL,
        hints;

    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(ip_str, PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;

    ptr = result;

    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
        ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    char buffer[BUFFER_LEN] = { 0 };
    int valread;
    if ((valread = recv(ConnectSocket, data_len.buf, sizeof(data_len.buf), 0)) < 1)
    {
        perror("cannot read");
        exit(-5);
    }

    printf("There are %lu pids.\n", data_len.len);

    fflush(NULL);

    while (true)
    {
        valread = recv(ConnectSocket, buffer, BUFFER_LEN, 0);
        if (valread == -1)
        {
            printf("Error: %d", errno);
            break;
        }
        
        else if (valread == 0)
        {
            printf("Disconnected: %d", errno);
            break;
        }
        if(strstr(buffer, "EOF") != NULL) {
            fwrite(buffer, (strlen(buffer)-3), 1, stdout);
            memset(buffer, 0, BUFFER_LEN);

            fflush(NULL);
            break;
        }

        fwrite(buffer, BUFFER_LEN, 1, stdout);
        memset(buffer, 0, BUFFER_LEN);
        fflush(NULL);
    }
    
    unsigned long pid_to_close;
    printf("Type PID that you want to close: ");
    scanf("%lu", &pid_to_close);

    data_len.len = pid_to_close;

    send(ConnectSocket, data_len.buf, sizeof(data_len.buf), 0);

    valread = recv(ConnectSocket, buffer, BUFFER_LEN, 0);
    printf("%s", buffer);
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
