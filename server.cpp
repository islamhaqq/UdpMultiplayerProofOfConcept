#include <boost/asio.hpp>
#include <iostream>
#include <unordered_map>
#include <string>

using boost::asio::ip::udp;

std::string message;

class Server {
public:
    explicit Server(boost::asio::io_context& io_context) : socket_(io_context, udp::endpoint(udp::v4(), 12345)) {
        startReceive();
    }

private:
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<char, 1024> recv_buffer_{};
    std::unordered_map<std::string, int> client_health_;

    void startReceive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            [this](const boost::system::error_code& ec, std::size_t /*bytes_transferred*/) {
                if (!ec) processRequest();
                startReceive(); // wait for the next packet
            });
    }

    void broadcastHealthToAllClients() {
        for (const auto& pair : client_health_) {
            udp::resolver resolver(socket_.get_executor());
            auto endpoints = resolver.resolve(udp::v4(), pair.first.substr(0, pair.first.find(':')), pair.first.substr(pair.first.find(':') + 1));
            for (auto& endpoint : endpoints) {
                socket_.async_send_to(boost::asio::buffer(message), endpoint,
                                      [](boost::system::error_code /*ec*/, std::size_t /*bytes_transferred*/) {});
            }
        }
    }

    void processRequest() {
        const std::string received(recv_buffer_.data());
        const auto separator_pos = received.find(':');
        if (separator_pos == std::string::npos) {
            std::cerr << "Received message with incorrect format: " << received << std::endl;
            return;
        }
        const int damage = std::stoi(received.substr(separator_pos + 1));

        const std::string client_id = remote_endpoint_.address().to_string() + ":" + std::to_string(remote_endpoint_.port());
        if (!client_health_.contains(client_id)) client_health_[client_id] = 100;

        int& health = client_health_[client_id];
        health = std::max(0, health - damage);

        message = "Client " + client_id + " Health: " + std::to_string(health);
        std::cout << message << std::endl;

        broadcastHealthToAllClients();
    }
};

int main() {
    try {
        boost::asio::io_context io_context;
        Server server(io_context);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}
