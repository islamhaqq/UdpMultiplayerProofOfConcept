#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::udp;

int main() {
    try {
        boost::asio::io_context io_context;

        udp::resolver resolver(io_context);
        udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "localhost", "12345").begin();

        udp::socket socket(io_context);
        socket.open(udp::v4());

        std::string message = "Hello from Client";
        socket.send_to(boost::asio::buffer(message), receiver_endpoint);

        char reply[1024];
        udp::endpoint sender_endpoint;
        size_t reply_length = socket.receive_from(boost::asio::buffer(reply, 1024), sender_endpoint);
        std::cout << "Server replied: " << std::string(reply, reply_length) << std::endl;
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
