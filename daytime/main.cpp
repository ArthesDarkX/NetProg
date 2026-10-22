#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdexcept>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>

int main()
{
    try
    {
        int client_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if (client_fd == -1)
        {
            throw std::runtime_error("Произошла ошибка при создании сокета");
        }
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(13);
        inet_pton(AF_INET, "172.16.40.1", &server.sin_addr);
        socklen_t server_len = sizeof(server);

        ssize_t send_bytes = sendto(client_fd, "", 1, 0, (sockaddr*)&server, server_len);
        if (send_bytes == -1)
        {
            throw std::runtime_error("Произошла ошибка при отправке данных!");
        }

        char buff[1024];
        ssize_t recv_bytes = recvfrom(client_fd, buff, sizeof(buff), 0, (sockaddr*)&server, &server_len);
        if (recv_bytes == -1)
        {
            throw std::runtime_error("Произошла ошибка при приёме данных!");
        }
        buff[recv_bytes] = '\0';
        std::cout << buff;
    }
    catch (std::runtime_error& e)
    {
        std::cerr << "Произошла ошибка: " << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
