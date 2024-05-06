#include "socket1.hpp"

int main() {
    int         listen_socket;
    int         connect_socket;
    ssize_t     ret;
    std::string buffer;

    listen_socket = ft_listen(PORT_NUM);
    if (listen_socket < 0) {
        return 1;
    }
    while (true) {
        std::cout << "接続を待っています [" << PORT_NUM << "] ..." << std::endl;
        connect_socket = ft_accept(listen_socket);
        if (connect_socket < 0) {
            return 1;
        }
        std::cout << "接続に成功 [" << PORT_NUM << "] ..." << std::endl;
        ret = ft_recv(connect_socket, buffer);
        if (ret < 0) {
            return 1;
        }
        ret = ft_send(connect_socket, buffer);
        if (ret < 0) {
            return 1;
        }
        close(connect_socket);
    }
    close(listen_socket);
    return 0;
}