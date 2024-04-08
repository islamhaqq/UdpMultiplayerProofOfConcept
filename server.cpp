#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::udp;

int main() {
    try {
        boost::asio::io_context io_context;
        udp::socket socket(io_context, udp::endpoint(udp::v4(), 12345));

        while (true) {
            char data[1024];
            udp::endpoint sender_endpoint;
            size_t length = socket.receive_from(boost::asio::buffer(data, 1024), sender_endpoint);
            std::cout << "Received: " << std::string(data, length) << std::endl;

            // Echo back the received message
            socket.send_to(boost::asio::buffer(data, length), sender_endpoint);
        }
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
