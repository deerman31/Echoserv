#include "socket1.hpp"

ssize_t	ft_recv(int connect_socket, std::string &buffer) {
    char    buf[BUF_SIZE];
    ssize_t ret;

    // recv(): クライアントからのリクエストメッセージを受信し、bufに格納する
    ret = recv(connect_socket, buf, (BUF_SIZE - 1), 0);
    if (ret < 0) {
        perror("recv");
        return -1;
    } else if (ret == 0) {
        // 0が返ってきたら、クライアント側が一方的に接続を切ったということ
        std::cout << "recv 接続が切れた: " << connect_socket << std::endl;
    } else {
        buffer = std::string(buf, ret);
        std::cout << ret << "bytes 受信しました" << std::endl;
        std::cout << "-----------リクエストメッセージ-----------" << std::endl;
        std::cout << buffer << std::endl;
        std::cout << "---------------------------------------" << std::endl;
    }
    return ret;
}