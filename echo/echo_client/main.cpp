#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    try
    {
        int client_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (client_fd == -1)
        {
            throw std::runtime_error("Произошла ошибка при создании сокета!");
        }

        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(7);
        if (inet_pton(AF_INET, "172.16.40.1", &server.sin_addr) != 1)
        {
            close(client_fd);
            throw std::runtime_error("Неверный ip-адрес!");
        }

        int connect_result = connect(client_fd, (sockaddr*)&server, sizeof(server));
        if (connect_result == -1)
        {
            close(client_fd);
            throw std::runtime_error("Произошла ошибка при подключении к серверу!");
        }

        std::string msg;
        std::getline(std::cin, msg);


        ssize_t total_send_bytes = 0;
        while (total_send_bytes < msg.length())
        {
            ssize_t send_bytes = send(client_fd, msg.c_str() + total_send_bytes, msg.length() - total_send_bytes, 0);
            if (send_bytes == -1)
            {
                close(client_fd);
                throw std::runtime_error("Произошла ошибка при отправке данных!");
            }
            total_send_bytes += send_bytes;
        }

        std::string buff(total_send_bytes, '\0');
        ssize_t total_recv_bytes = 0;
        while (total_recv_bytes < total_send_bytes)
        {
            ssize_t recv_bytes = recv(client_fd, buff.data() + total_recv_bytes, buff.length() - total_recv_bytes, 0);
            switch (recv_bytes)
            {
            case 0:
                close(client_fd);
                throw std::runtime_error("Сервер закрыл соединение!");
            case -1:
                close(client_fd);
                throw std::runtime_error("Произошла ошибка при принятии данных!");
            }       
            total_recv_bytes += recv_bytes;
        }
        std::cout << buff << '\n';
        close(client_fd);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}