#include "socket1.hpp"

ssize_t	ft_send(int connect_socket, const std::string &buffer) {
	ssize_t				ret;
	std::string			response;
	std::ostringstream	oss;

	oss << buffer.length() + 2;
	response += "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/plain\r\n";
	response += "Content-Length: ";
	response += oss.str() + "\r\n";
	response += "\r\n";
	response += buffer;
	response += "\r\n";

	// send(): クライアントにレスポンスメッセージを送信する
	ret = send(connect_socket, response.c_str(), response.length(), 0);
	if (ret < 0) {
		perror("send");
		return -1;
	} else if (ret == 0) {
		// 0が返ってきたら、クライアント側が一方的に接続を切ったということ
		std::cout << "send 接続が切れた: " << connect_socket << std::endl;
	} else {
		std::cout << ret << "bytes 送信しました" << std::endl;
	}
	return ret;
}
