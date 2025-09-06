// #include "Client.h"
// #include <boost/asio.hpp>
// #include <iostream>
// #include <string>
// #include <string_view>

// const boost::asio::ip::udp::endpoint SERVER_ENDPOINT(boost::asio::ip::make_address("127.0.0.1"), 5000);

// Client::Client() {
//     using namespace boost::asio;

//     io_context ctx;

//     ip::udp::socket clientSocket(ctx, ip::udp::endpoint(ip::udp::v4(), 0));

//     std::string message = "Hello World!";
//     clientSocket.async_send_to(buffer(message.data(), message.size()),
//                                SERVER_ENDPOINT,
//                                [](std::error_code ec, size_t sendBytes) {
//                                    if (!ec && sendBytes > 0) {
//                                        std::cout << sendBytes << " Bytes send successfully!" << std::endl;
//                                    } else {
//                                        std::cout << "ERROR! " << ec.message() << std::endl;
//                                    }
//                                });

//     ctx.run();
// }