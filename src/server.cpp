#define PORT 8088
#define BUFFER_LEN 512

#include "main.hpp"

int main()
{
    int server_fd, new_socket, valread;
    sockaddr_in addresss;
    int opt = 1;
    int addrlen = sizeof(addresss);
    char buffer[BUFFER_LEN] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(-1);
    }

    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(-2);
    }
    addresss.sin_family = AF_INET;
    addresss.sin_addr.s_addr = INADDR_ANY;
    addresss.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr *)&addresss, sizeof(addresss)) < 0)
    {
        perror("bind failed");
        exit(-3);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(-3);
    }

    if ((new_socket = accept(server_fd, (sockaddr *)&addresss,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(-3);
    }

    std::vector<Proc_Info> pid_name = get_pid_and_name();
    unsigned long size = pid_name.size();
    data_len.len = size;
    send(new_socket, data_len.buf, sizeof(data_len.buf), 0);

    for (unsigned long i = 0; i < size; i++)
    {
        memset(buffer, 0, BUFFER_LEN);
        sprintf(buffer, "Number: %lu, PID: %lu - %s\n", (i + 1), pid_name.at(i).pid, pid_name.at(i).name);
        if (send(new_socket, buffer, strlen(buffer), 0) < 0)
        {
            perror("not send");
        }
        if (i == (size - 1))
        {
            send(new_socket, "EOF", sizeof("EOF"), 0);
        }
    }

    unsigned long pid;

    if((valread = recv(new_socket, data_len.buf, sizeof(data_len.buf), 0)) < 1)
    {
        perror("Not recieved");
    }

    pid = data_len.len;

    if(kill_proc_by_pid(pid) >= 0){
        send(new_socket, "Success in closing process\n", sizeof("Success in closing process\n"), 0);
    }
    else
    {
        send(new_socket, "Failure in closing process\n", sizeof("Failure in closing process\n"), 0);
    }

    printf("%lu\n", data_len.len);

    if (shutdown(server_fd, SHUT_RDWR) != 0)
    {
        exit(-4);
    }
    close(server_fd);

    return 0;
}