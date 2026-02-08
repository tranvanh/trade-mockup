#pragma once
// #include "UtilsLib/NetworkComponent.h"
#include "UtilsLib/Common.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <asio.hpp>

TRANVANH_NAMESPACE_BEGIN

class Server; // forward declaration

class Session : public std::enable_shared_from_this<Session> {
    asio::ip::tcp::socket mSocket;
    Server& mServer;

public:
    Session(asio::ip::tcp::socket socket, Server& server) : mSocket(std::move(socket)), mServer(server) {}


    void start() {
        read();
    }

    void deliver(const std::string& msg) {

        //queue->push(message)
        // one socket, multithreaded
        //

        // std::lock_guard<std::mutex> lock(write_mutex_);
        // bool noWrite = write_msgs_.empty(); // false -> true -> make it guard only on the flag
        // write_msgs_.push_back(msg);
        //
        // std::lock_guard<std::mutex> lock(writeMutex);
        // if (!isWriting) {
                //isWriting
        //     write();
        // }
    }


    const asio::ip::tcp::socket& get_socket() const { return mSocket; }

private:
    void read() {
        [[maybe_unused]] auto self(shared_from_this());
        asio::async_read_until(mSocket, asio::dynamic_buffer(read_buffer_), '\n',
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string message = read_buffer_.substr(0, length - 1);  // Remove \n
                    read_buffer_.erase(0, length);
                    handle_message(message);
                    read();
                } else {
                    std::cout << "Client disconnected" << std::endl;
                }
            });
    }

    void handle_message(const std::string& message);  // Defined after Server class

    void write() {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(write_msgs_.front()),
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    // nested lock
                    std::lock_guard<std::mutex> lock(write_mutex_);
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty()) {
                        write();
                    }
                }
            });
    }
};


/// Current implementation acts as a TCP receiver where we expect to read size of message first and then the
/// content later We poll events allowing high number of clients to be connected
// class Server {
//     struct ClientConnection {
//         enum class State {
//             WAITING,
//             SIZE_RECEIVED,
//         };
//         State  state  = State::WAITING;
//         size_t msgLen = 0;
//     };
//
//     std::vector<char> mReceiveBuffer = std::vector<char>(BUFSIZ);
//
// public:
//     enum class AddressType {
//         ANY,
//         LOCAL,
//         PUBLIC,
//     };
//     AddressType                               mAddressType = AddressType::ANY;
//     std::string                               mAddress;
//     std::unordered_map<int, ClientConnection> mClientConnections;
//
//     Server(const AddressType addressType, const std::string& address = "");
//     Server(const Server&) = delete;
//     bool startListen(const int                                         socketId,
//                      const int                                         port,
//                      std::function<void(std::vector<char>, const int)> onReceive);
//
// private:
//     bool poll(const int                                         socketId,
//               sockaddr_in                                       socketAddress,
//               socklen_t                                         socketLen,
//               std::function<void(std::vector<char>, const int)> onReceive);
//     bool setNonBlockingSocket(const int socketId);
//
//     // Server receiving follows a rule of receiving message size first and content after
//     bool receive(const int clientSocket, std::function<void(std::vector<char>, const int)> onReceive);
//     bool receiveSize(const int clientSocket, size_t& out);
//     bool receiveContent(const int clientSocket, const size_t msgLen);
// };
class Server {
    asio::io_context&                    mContext;
    asio::ip::tcp::acceptor              mAcceptor;
    struct {
        std::unordered_set<std::shared_ptr<Session>> data;
        mutable std::mutex                           lock;
    } mActiveSessions;


public:
    Server(asio::io_context& io_context, short port)
        : mContext(io_context)
        , mAcceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) {}

    void run() {
        mContext.run();
        startAccepting();
    }
    //
    // void broadcast(const std::string& topic, const std::string& message) {
    //     std::lock_guard<std::mutex> lock(clients_mutex_);
    //
    //     std::string full_msg = "BROADCAST " + topic + " " + message + "\n";
    //     int         count    = 0;
    //
    //     for (auto& client : clients_) {
    //         if (client->is_subscribed(topic)) {
    //             client->deliver(full_msg);
    //             count++;
    //         }
    //     }
    //
    //     std::cout << "Broadcasted to " << count << " subscribers on topic: " << topic << std::endl;
    // }
    //
    // void handle_command(const std::string& cmd, const std::string& args) {
    //     std::cout << "Server command: " << cmd << " " << args << std::endl;
    //
    //     // Execute server-level commands
    //     if (cmd == "STATUS") {
    //         std::lock_guard<std::mutex> lock(clients_mutex_);
    //         std::cout << "Connected clients: " << clients_.size() << std::endl;
    //     } else if (cmd == "SHUTDOWN") {
    //         std::cout << "Server shutting down..." << std::endl;
    //         std::exit(0);
    //     }
    //     // Add more server commands as needed
    // }

    size_t numberOfSessions() const {
        std::lock_guard<std::mutex> lock(mActiveSessions.lock);
        return mActiveSessions.data.size();
    }
    //
    void remove_client(std::shared_ptr<Session> session) {
        std::lock_guard<std::mutex> lock(mActiveSessions.lock);
        mActiveSessions.data.erase(session);
    }

private:
    void startAccepting() {
        mAcceptor.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                auto session = std::make_shared<Session>(std::move(socket), this); // \todo fix constructor
                {
                    std::lock_guard<std::mutex> lock(mActiveSessions.lock);
                    mActiveSessions.data.insert(session);
                    std::cout << "New client connected! Total: " << mActiveSessions.data.size() << std::endl;
                }
                session->start(); // \todo activate
                session->deliver("OK Connected to server\n");
            }
            startAccepting(); // \todo makes no sense
        });
    }
};
TRANVANH_NAMESPACE_END
