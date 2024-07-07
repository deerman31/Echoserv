#include <climits>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>

#define BUF_SIZE 1025

struct Listen {
	// port番号
	int		port_num;
	// fdやeventの種類と発生を管理するpollfdの構造体
	pollfd	pfd;
};

struct Connection {
	// port番号
	int			port_num;
	// fdやeventの種類と発生を管理するpollfdの構造体
	pollfd		pfd;
	// 受信したリクエストメッセージをクライアントにそのまま送り返すために保存するbuffer
	std::string	buffer;
};

void	server_close(int n, std::vector<Listen> &listens, std::vector<Connection> &connections) {
	for (std::vector<Listen>::iterator it = listens.begin(); it != listens.end(); ++it) {
		if (it->pfd.fd != -1) {
			close(it->pfd.fd);
			it->pfd.fd = -1;
		}
	}
	for (std::vector<Connection>::iterator it = connections.begin(); it != connections.end(); ++it) {
		if (it->pfd.fd != -1) {
			close(it->pfd.fd);
			it->pfd.fd = -1;
		}
	}
	if (n == EXIT_FAILURE) {
		std::exit(EXIT_FAILURE);
	}
}

static bool	is_num(const std::string &s) {
	for (size_t i = 0; i < s.length(); i += 1) {
		if (std::isdigit(s.at(i)) == 0) {
			std::cerr << "ERROR: 引数は整数である必要があります" << std::endl;
			return false;
		}
	}
	return true;
}

static bool	is_port(const std::string &s) {
	if (s.length() < 4 || s.length() > 5) {
		std::cerr << "ERROR: 整数の範囲は1024~49151です" << std::endl;
		return false;
	}
	int port = std::atoi(s.c_str());
	if (port < 1024 || port > 49151) {
		std::cerr << "ERROR: 整数の範囲は1024~49151です" << std::endl;
		return false;
	}
	return true;
}

bool	set_port(int argc, const char **argv ,std::vector<int> &ports) {
	if (argc == 1) {
		std::cerr << "ERROR: 引数が必要です" << std::endl;
		return false;
	}
	for (int i = 1; i < argc; i += 1) {
		if (!is_num(std::string(argv[i])) || !is_port(std::string(argv[i])))
			return false;
		ports.push_back(std::atoi(argv[i]));
	}
	return true;
}

int	ft_accept(int listen_fd) {
	int					fd;
	struct sockaddr_in	address;
	int addrlen = sizeof(address);

	// accept(): クライアントからの接続を受け入れ、新たなsocketfdを作成します。
	fd = accept(listen_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if (fd < 0) {
		perror("accept");
		return -1;
	}
	// fcntl(): fdをノンブロッキングに設定
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
		perror("fcntl()");
		return -1;
	}
	return fd;
}
int	ft_listen(int port_num) {
	int					fd;
	struct sockaddr_in	address;
	int					opt;

	// AF_INET: IPv4 インターネットプロトコルを使用することを指定します
	// SOCK_STREAM: 信頼性の高い、接続指向のバイトストリームを提供します
	// socket(): 接続を待ち受けるsocketのFDを作成します
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("socket()");
		return -1;
	}

	opt = 1;
	// setsockopt(): ソケットオプションの設定
    // SO_REUSEADDR: 同じポートに対して早く再バインドできるようにするオプションを設定します
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt()");
		close(fd);
		return -1;
	}

	// AF_INET: アドレスファミリをIPv4に設定します
	address.sin_family = AF_INET;
	// INADDR_ANY: ポート番号をネットワークバイトオーダーに変換して設定します
	address.sin_addr.s_addr = INADDR_ANY;
	// htons(): ポート番号をネットワークバイトオーダーに変換して設定します
	address.sin_port = htons(port_num);

	// bind(): 指定されたポートとアドレスをソケットに関連付けます
	if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("bind()");
		close(fd);
		return -1;
	}

	// listen(): サーバが接続要求を待ち受けるように設定します。第2引数はバックログ（保留中の接続のキューの最大長）です
	if (listen(fd, SOMAXCONN) < 0) {
		perror("listen()");
		close(fd);
		return -1;
	}
	// fcntl(): fdをノンブロッキングに設定
	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
		perror("fcntl()");
		close(fd);
		return -1;
	}
	return fd;
}

ssize_t	ft_send(Connection &connection) {
	ssize_t				ret;
	std::string			response;
	std::ostringstream	oss;

	oss << connection.buffer.length() + 2;
	response += "HTTP/1.1 200 OK\r\n";
	response += "Content-Type: text/plain\r\n";
	response += "Content-Length: ";
	response += oss.str() + "\r\n";
	response += "\r\n";
	response += connection.buffer;
	response += "\r\n";

	// send(): クライアントにレスポンスメッセージを送信する
	ret = send(connection.pfd.fd, response.c_str(), response.length(), 0);
	if (ret < 0) {
		perror("send");
		return -1;
	} else if (ret == 0) {
		// 0が返ってきたら、クライアント側が一方的に接続を切ったということ
		std::cout << "send 接続が切れた: " << connection.pfd.fd << std::endl;
		return ret;
	}
	return ret;
}


ssize_t	ft_recv(Connection &connection) {
	char	buf[BUF_SIZE];
	ssize_t	ret;

	// recv(): クライアントからのリクエストメッセージを受信し、bufに格納する
	ret = recv(connection.pfd.fd, buf, (BUF_SIZE - 1), 0);
	if (ret < 0) {
		perror("recv");
		return -1;
	} else if (ret == 0) {
		// 0が返ってきたら、クライアント側が一方的に接続を切ったということ
		std::cout << "recv 接続が切れた: " << connection.pfd.fd << std::endl;
		return ret;
	}
	// 構造体のbufferにリクエストメッセージを格納
	connection.buffer = std::string(buf, ret);
	std::cout << "-----------リクエストメッセージ-----------" << std::endl;
	std::cout << connection.buffer << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	return ret;
}

int	ft_poll(std::vector<Listen>	&listen_fds, std::vector<Connection> &connection_fds) {
	int					ret;
	std::vector<pollfd>	tmp;

	// poll()は一つのリストでなければ使えないため、tmpに二つのvectorを追加する
	// listen_fdsの各々のpollfdをtmpに追加する
	for (std::vector<Listen>::const_iterator it = listen_fds.begin(); it != listen_fds.end(); ++it) {
		tmp.push_back(it->pfd);
	}
	// connection_fdsの各々のpollfdをtmpに追加する
	for (std::vector<Connection>::const_iterator it = connection_fds.begin(); it != connection_fds.end(); ++it) {
		tmp.push_back(it->pfd);
	}
	// 第３引数はブロックする時間 -1をセットすると永遠にブロックし続ける　0の場合はブロックせずにすぐに0を返す
	ret = poll(tmp.data(), tmp.size(), 1000);
	if (ret < 0) {
		perror("poll");
		return -1;
	} else if (ret == 0) {
		// 戻り値が0ということはイベントが発生していない
		std::cout << "TIMEOUT!" << std::endl;
		return 0;
	}
	// pollを通過するとイベントが発生したpollfdのreventsフィールドにビットが立つので各vectorのreventsを更新する
	std::vector<pollfd>::const_iterator t = tmp.begin();
	for (std::vector<Listen>::iterator it = listen_fds.begin(); it != listen_fds.end(); ++it) {
		it->pfd.revents = t->revents;
		++t;
	}
	for (std::vector<Connection>::iterator it = connection_fds.begin(); it != connection_fds.end(); ++it) {
		it->pfd.revents = t->revents;
		++t;
	}
	return ret;
}

int	main(int argc, const char **argv) {
	// listens: クライアントからの新規接続を待ち受けるSocketのvector
	std::vector<Listen>		listens;
	// connections: クライアントとの通信(IO操作)を行うSocketのvector
	std::vector<Connection>	connections;

	std::vector<int> ports;
	if (!set_port(argc, argv, ports)) {
		std::cerr << "例: ./echoserv 8080 4242" << std::endl;
		return 1;
	}
	//ここでコマンドライン引数から受け取ったPORT番号をもとにListenソケット作成し、Listensに追加
	for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end(); ++it) {
		Listen	listen;
		listen.port_num = *it;
		// ft_listens()でport_numをもとに新規接続を待ち受けるsocketのfdを作成
		listen.pfd.fd = ft_listen(listen.port_num);
		// 新規接続イベントは読み込みイベントなので、eventsフィールドにPOLLINをセットする
		listen.pfd.events = POLLIN;
		// vectorに追加
		listens.push_back(listen);
		if (listen.pfd.fd < 0) {
			server_close(EXIT_FAILURE, listens, connections);
		}
	}

	while (true) {
		// ft_poll()でイベントの発生を監視する
		// イベントが発生していれば、pollfdのreventsフィールドにビットが立つ
		int ret = ft_poll(listens, connections);
		if (ret < 0) {
			server_close(EXIT_FAILURE, listens, connections);
		} else if (ret == 0) {
			// 0ということはイベントが発生していないのでループの先頭に戻す
			continue;
		}
		for (std::vector<Listen>::iterator l = listens.begin(); l != listens.end(); ++l) {
			// (l->pfd.revents & POLLIN)のように書くとreventsフィールドにビットが立っているか判別できる
			if (l->pfd.revents & POLLIN) {
				Connection c;
				c.port_num = l->port_num;
				// ft_accept()で通信用のsocketfdを作成する
				c.pfd.fd = ft_accept(l->pfd.fd);
				if (c.pfd.fd < 0) {
					server_close(EXIT_FAILURE, listens, connections);
				}
				// connectionのsocketが最初にやるのは読み込みイベントのため、eventsフィールドにPOLLINをセットする
				c.pfd.events = POLLIN;
				c.pfd.revents = 0;
				// vectorに追加
				connections.push_back(c);
			}
		}
		for (std::vector<Connection>::iterator c = connections.begin(); c != connections.end();) {
			if (c->pfd.revents & POLLIN) {
				// ft_recv()でクライアントからのリクエストメッセージを受信し、コマンドラインに出力し、構造体のbufferに保存する
				ssize_t ret = ft_recv(*c);
				if (ret < 0) {
					server_close(EXIT_FAILURE, listens, connections);
				} else if (ret == 0) {
					// 0が返ってきたら、クライアント側が一方的に接続を切ったということなので
					// fdをcloseし、vectorから構造体を削除する
					close(c->pfd.fd);
					c = connections.erase(c);
					continue;
				}
				// 読み込みイベントが終わったら、次は書き込みイベントなのでeventsフィールドにPOLLOUTにセットする
				c->pfd.events = POLLOUT;
			} else if (c->pfd.revents & POLLOUT) {
				// ft_send()でクライアントにft_recv()で構造体のbufferに保存したリクエストメッセージをそのままレスポンスメッセージとして、送信する
				ssize_t ret = ft_send(*c);
				if (ret < 0) {
					server_close(EXIT_FAILURE, listens, connections);
				} else if (ret == 0) {
					// 0が返ってきたら、クライアント側が一方的に接続を切ったということなので
					// fdをcloseし、vectorから構造体を削除する
					close(c->pfd.fd);
					c = connections.erase(c);
					continue;
				}
				// 書き込みイベントが終わったら、次のリクエストメッセージの受信を待つためにeventsフィールドにPOLLINをセットする
				c->pfd.events = POLLIN;
			}
			++c;
		}
	}

	server_close(EXIT_SUCCESS, listens, connections);
	return 0;
}