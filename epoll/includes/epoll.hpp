#ifndef ECHOSERV_HPP
# define ECHOSERV_HPP

# include <climits>
# include <cstdlib>
# include <cstdio>
# include <cstring>
# include <iostream>
# include <vector>
# include <sstream>

# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <fcntl.h>

# define BUF_SIZE 1025
# define MAX_EVENTS 100

struct Listen {
	// port番号
	int		port_num;
	int     fd;
};

struct Connection {
	// port番号
	int			port_num;
    int         fd;
	// 受信したリクエストメッセージをクライアントにそのまま送り返すために保存するbuffer
	std::string	buffer;
};

bool	set_port(int, const char**, std::vector<int>&);
int		ft_accept(int);
int		ft_listen(int);
int		ft_poll(std::vector<Listen>&, std::vector<Connection>&);
ssize_t	ft_recv(Connection&);
ssize_t	ft_send(Connection&);
void	server_close(int, std::vector<Listen>&, std::vector<Connection>&, int);

#endif