# Echoserv

Echoserv is a sample Echo server implemented in C++98, designed to demonstrate the basics of creating an HTTP server similar to nginx. This project was created as a reference for a school assignment.

## Features

- Implements a basic Echo server that returns the received HTTP request message
- Uses poll() for asynchronous I/O
- Utilizes non-blocking sockets
- Provides a foundation for understanding HTTP server implementation

## Prerequisites

- C++ compiler with C++98 support
- Make

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/echoserv.git
   cd echoserv
   ```

2. Navigate to the code directory:
   ```
   cd code
   ```

3. Compile the project:
   ```
   make
   ```

## Usage

1. Run the server with desired port numbers:
   ```
   ./echoserv 8080 4242
   ```

2. Send requests using curl:
   ```
   curl localhost:8080
   ```

3. Or use a web browser:
   ```
   http://localhost:8080/
   ```

## Learning Resources

This sample code is designed to provide a basic understanding of HTTP server implementation. While it includes minimal comments, it's recommended to use AI tools like ChatGPT or Claude for a deeper understanding of the code.

## Related Project

For those interested in seeing a more complete HTTP server implementation, check out my full project:
[http_server_webserv](https://github.com/deerman31/http_server_webserv)

## Contributing

Contributions, issues, and feature requests are welcome. Feel free to check [issues page](https://github.com/yourusername/echoserv/issues) if you want to contribute.

## License

[MIT](https://choosealicense.com/licenses/mit/)

## Contact

Your Name - [@yourtwitterhandle](https://twitter.com/yourtwitterhandle) - email@example.com

Project Link: [https://github.com/yourusername/echoserv](https://github.com/yourusername/echoserv)
