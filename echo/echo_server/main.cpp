#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>

int main()
{
    try
    {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1)
        {
            throw std::runtime_error("Не удалось создать сокет!");
        }

        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons(7777);

        if (bind(server_fd, (sockaddr*)&server, sizeof(server)) == -1)
        {
            close(server_fd);
            throw std::runtime_error("Произошла ошибка при присоединении порта к сокету");
        }
        if (listen(server_fd, 1) == -1)
        {
            close(server_fd);
            throw std::runtime_error("Произошла ошибка при прослушке порта: " + std::to_string(ntohs(server.sin_port)));
        }

        while (true)
        {
            struct sockaddr_in client;
            memset(&client, 0, sizeof(client));
            socklen_t len_client = sizeof(client);

            int client_fd = accept(server_fd, (sockaddr*)&client, &len_client);
            if (client_fd == -1)
            {
                close(server_fd);
                throw std::runtime_error("Произошла ошибка при установке соединения с клиентом!");
            }
            while (true)
            {
                char buff[1024];
                ssize_t recv_bytes = recv(client_fd, buff, sizeof(buff), 0);
                if (recv_bytes == 0)
                {
                    std::cout << "Клиент закрыл подключение\n";
                    break;
                }
                else if (recv_bytes == -1)
                {
                    std::cerr << "Произошла ошибка при приёме данных!\n";
                    break;
                }

                ssize_t total_send = 0;
                bool send_failed = false;
                while (total_send < recv_bytes)
                {
                    ssize_t send_bytes = send(client_fd, buff + total_send, recv_bytes - total_send, 0);
                    if (send_bytes == -1)
                    {
                        std::cerr << "Произошла ошибка при отправке данных!\n";
                        send_failed = true;
                        break;
                    }
                    total_send += send_bytes;
                }
                if (send_failed)
                {
                    break;
                }
            }
            close(client_fd);
        }
    }
    catch (std::runtime_error& ex)
    {
        std::cout << ex.what() << '\n';
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}