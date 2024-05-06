#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int set_non_blocking(int fd) {
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
		perror("fcntl()");
		return -1;
	}
    return 0;
}

int main() {
    char        buf[1000];

    if (set_non_blocking(STDIN_FILENO) < 0) {
        return 1;
    }
    while(true) {
        ssize_t ret = read(STDIN_FILENO, buf, (sizeof(buf) -1));
        if (ret < 0) {
            std::cout << "ノンブロッキングにセットされているため、ブロッキングせずにエラーが返された\n";
            perror("read");
            return 1;
        }
        std::string input(buf, ret);
        std::cout << "入力 -> " << input <<  std::endl;
        std::memset(buf, 0, sizeof(buf));
    }
    return 0;
}