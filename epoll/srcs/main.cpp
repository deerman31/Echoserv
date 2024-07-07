#include "epoll.hpp"

int	main(int argc, const char **argv) {
	std::vector<Listen>		listens;
	std::vector<Connection>	connections;
	std::vector<int> ports;

	if (!set_port(argc, argv, ports)) {
		std::cerr << "例: ./echoserv 8080 4242" << std::endl;
		return 1;
	}
    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_createl");
		return 1;
    }

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

	for (std::vector<int>::const_iterator it = ports.begin(); it != ports.end(); ++it) {
		Listen	listen;
		listen.port_num = *it;
		listen.fd = ft_listen(listen.port_num);
        if (listen.fd < 0) {
            server_close(EXIT_FAILURE, listens, connections, epoll_fd);
        }

        event.data.fd = listen.fd;
        event.events = EPOLLIN;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen.fd, &event) < 0) {
            perror("epoll_ctl");
            close(listen.fd);
            server_close(EXIT_FAILURE, listens, connections, epoll_fd);
        }
		listens.push_back(listen);
	}

	while (true) {
        int ret = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (ret < 0) {
            perror("epoll_wait");
            server_close(EXIT_FAILURE, listens, connections, epoll_fd);
        }
        for (int i = 0; i < ret; i += 1) {
            if (events[i].events & EPOLLIN) {
                for (std::vector<Listen>::iterator l = listens.begin(); l != listens.end(); ++l) {
                    if (events[i].data.fd == l->fd) {
                        Connection c;
                        c.port_num = l->port_num;
                        c.fd = ft_accept(l->fd);
                        if (c.fd < 0) {
                            server_close(EXIT_FAILURE, listens, connections, epoll_fd);
                        }
                        event.data.fd = c.fd;
                        event.events = EPOLLIN | EPOLLOUT | EPOLLERR;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, c.fd, &event) < 0) {
                            perror("epoll_ctl");
                            close(c.fd);
                            server_close(EXIT_FAILURE, listens, connections, epoll_fd);
                        }
                        connections.push_back(c);
                    }
                }
            }
            for (std::vector<Connection>::iterator c = connections.begin(); c != connections.end();) {
                if (events[i].data.fd == c->fd) {
                    if (events[i].events & EPOLLIN) {
                        ssize_t ret = ft_recv(*c);
                        if (ret < 0) {
                            server_close(EXIT_FAILURE, listens, connections, epoll_fd);
                        } else if (ret == 0) {
                            close(c->fd);
                            c = connections.erase(c);
                            continue;
                        }
                    } else if (events[i].events & EPOLLOUT) {
                        ssize_t ret = ft_send(*c);
                        if (ret < 0) {
                            server_close(EXIT_FAILURE, listens, connections, epoll_fd);
                        } else if (ret == 0) {
                            close(c->fd);
                            c = connections.erase(c);
                            continue;
                        }
                    }
                    ++c;
                }
            }
        }
	}
    server_close(EXIT_SUCCESS, listens, connections, epoll_fd);
	return 0;
}