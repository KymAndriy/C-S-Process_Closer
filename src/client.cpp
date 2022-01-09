#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include "main.hpp"

#define PORT 8088
#define BUFFER_LEN 512
#define SRT_IP_MAX_LEN 16


int main()
{
    char* ip_str = (char*)malloc(SRT_IP_MAX_LEN);
    printf("Write IP (127.0.0.1) that you want to connect: ");
    scanf("%s", ip_str);

    int sock = 0, valread;
    sockaddr_in serv_addr;

    char buffer[BUFFER_LEN] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation error");
        exit(-1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, ip_str, &serv_addr.sin_addr) <= 0)
    {
        perror("invalid addr");
        exit(-2);
    }

    if (connect(sock, (sockaddr *)&serv_addr,
                sizeof(serv_addr)) < 0)
    {
        perror("connection failed");
        exit(-3);
    }

    if ((valread = recv(sock, data_len.buf, sizeof(data_len.buf), 0)) < 1)
    {
        perror("cannot read");
        exit(-5);
    }

    printf("There are %lu pids.\n", data_len.len);

    fflush(NULL);

    while (true)
    {
        valread = recv(sock, buffer, BUFFER_LEN, 0);
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

    send(sock, data_len.buf, sizeof(data_len.buf), 0);

    valread = recv(sock, buffer, BUFFER_LEN, 0);
    printf("%s", buffer);
    if (shutdown(sock, SHUT_RDWR) != 0)
    {
        exit(-4);
    }
    close(sock);
    return 0;
}
