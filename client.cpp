#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <array>
#include <random>
#include <chrono>

using boost::asio::ip::udp;

std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> damage_distribution(1, 10); // Damage range between 1 and 10
std::uniform_int_distribution<int> sleep_distribution(1, 5); // Sleep between 1 and 5 seconds

void listen_for_updates(udp::socket& socket) {
    std::array<char, 1024> recv_buf;
    udp::endpoint sender_endpoint;
    boost::system::error_code error;
    size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint, 0, error);
    if (error && error != boost::asio::error::message_size) {
        throw boost::system::system_error(error);
    }
    std::cout.write(recv_buf.data(), len);
    std::cout << std::endl;
}

int main() {
    try {
        boost::asio::io_context io_context;

        udp::resolver resolver(io_context);
        udp::endpoint receiver_endpoint = *resolver.resolve(udp::v4(), "localhost", "12345").begin();

        udp::socket socket(io_context, udp::endpoint(udp::v4(), 0)); // Binding to 0 lets the OS choose the port

        std::thread listener_thread([&socket]() {
            try {
                while (true) {
                    listen_for_updates(socket);
                }
            } catch (const std::exception& e) {
                std::cerr << "Listener thread error: " << e.what() << std::endl;
            }
        });

        // Simulate taking random damage at random intervals
        while (true) {
            int damage = damage_distribution(generator);
            std::string message = "Damage: " + std::to_string(damage);
            socket.send_to(boost::asio::buffer(message), receiver_endpoint);

            int sleep_time = sleep_distribution(generator);
            std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
        }

        if(listener_thread.joinable()) {
            listener_thread.join();
        }
    } catch(const std::exception& e) {
        std::cerr << "Main thread error: " << e.what() << std::endl;
    }

    return 0;
}
