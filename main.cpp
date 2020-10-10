#include <iostream>
#include <stdio.h>
#include <string>

#ifdef _WIN32
#define WINVER 0x0A00
#endif

#define ASIO_STANDALONE
#define UTC (0)

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <time.h>

int main() {
  bool connected = false;
  while (!connected) {
    asio::error_code error;
    asio::io_context context;
    asio::socket_base::keep_alive ka(true);

    asio::ip::tcp::endpoint endpoint(
        asio::ip::make_address("192.168.0.100", error), 8081);
    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, error);
    socket.set_option(ka);
    time_t ertime = time(NULL);
    if (!error) {
      std::cout << "Connected to " << endpoint.address() << std::endl;

      std::string request = "GET / HTTP1.1\r\n"
                            "Host: 192.168.0.100\r\n"
                            "Connection: close\r\n\r\n";

      socket.write_some(asio::buffer(request.data(), request.size()), error);

      size_t bytes = socket.available();
      if (bytes > 0) {
        std::vector<char> vBuffer(bytes);
        socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), error);

        for (auto c : vBuffer) {
          std::cout << c;
        }
      } else {
        std::cout << "No bytes to be read" << std::endl;
      }
      connected = true;
    } else {
      std::cout << "Failed to connect\n"
                << "ERROR:\t" << strtok(ctime(&ertime), "\n") << "\t"
                << error.message() << std::endl;
    }
  }
  return 0;
}
