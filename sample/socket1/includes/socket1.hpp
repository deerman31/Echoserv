#ifndef SOCKET1_HPP
# define SOCKET1_HPP

# include <cstring>
# include <iostream>
# include <sstream>

# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include <fcntl.h>

#define BUF_SIZE 1025
#define PORT_NUM 4242


int     ft_accept(int);
int     ft_listen(int);
ssize_t ft_recv(int, std::string &);
ssize_t ft_send(int, const std::string &);

#endif