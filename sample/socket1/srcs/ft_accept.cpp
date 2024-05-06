#include "socket1.hpp"

int	ft_accept(int listen_fd) {
    int                 fd;
    struct sockaddr_in  address;
    int addrlen = sizeof(address);

    // accept(): クライアントからの接続を受け入れ、新たなsocketfdを作成します。
    fd = accept(listen_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (fd < 0) {
        perror("accept");
        return -1;
    }
    return fd;
}
