#include <iostream>
#include <stdio.h>
#include <string>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#ifdef _WIN32
#define WINVER 0x0A00
#endif

#define ASIO_STANDALONE
#define UTC (0)

#include <time.h>
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <boost/bind/bind.hpp>

int main()
{
    bool connected = false;
    while (!connected){
        asio::error_code error;
        asio::io_context context;

        asio::ip::tcp::endpoint endpoint(asio::ip::make_address("192.168.0.100", error), 8081);
        asio::ip::tcp::socket socket(context);

        socket.connect(endpoint, error);
        time_t ertime = time(NULL);
        char recieved[1024];
        if(!error){
            std::cout << "Connected" << std::endl;
            std::cout << socket.read_some(asio::buffer(recieved), error) << std::endl;
            connected = true;
        }
        else{
            std::cout << "Failed to connect\n"
                      << "ERROR:\t"
                      << strtok(ctime(&ertime), "\n")
                      << "\t"
                      << error.message()
                      << std::endl;
        }
    }
    return 0;
}
