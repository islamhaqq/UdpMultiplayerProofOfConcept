#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <array>

using boost::asio::ip::udp;

void listen_for_updates(udp::socket& socket) {
    std::array<char, 1024> recv_buf;
    udp::endpoint sender_endpoint;
    boost::system::error_code error;
    while (true) {
        size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint, 0, error);
        if (error && error != boost::asio::error::message_size) {
            throw boost::system::system_error(error);
        }
        std::cout.write(recv_buf.data(), len);
        std::cout << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_context io_context;

        udp::resolver resolver(io_context);
        udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "localhost", "12345").begin();

        udp::socket socket(io_context, udp::endpoint(udp::v4(), 0)); // Binding to 0 lets the OS choose the port

        std::thread listener_thread([&socket]() {
            try {
                listen_for_updates(socket);
            } catch (const std::exception& e) {
                std::cerr << "Listener thread error: " << e.what() << std::endl;
            }
        });

        std::cout << "Enter damage taken: ";

        // Changed from random damage to user input
        int damage;
        while (std::cin >> damage) {
            std::string message = "Damage: " + std::to_string(damage) + "\n";
            socket.send_to(boost::asio::buffer(message), receiver_endpoint);
            std::cout << "Enter next damage taken: ";
        }

        if(listener_thread.joinable()) {
            listener_thread.join();
        }
    } catch(const std::exception& e) {
        std::cerr << "Main thread error: " << e.what() << std::endl;
    }

    return 0;
}
