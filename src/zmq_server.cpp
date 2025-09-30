#include <iostream>
#include <string>

#include <zmq.hpp>

int main() {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");

    std::cout << "Server started on port 5555\n";
    std::cout << "Waiting for messages...\n";

    while (true) {
        zmq::message_t request;

        // メッセージ受信
        auto result = socket.recv(request, zmq::recv_flags::none);
        if (!result) {
            std::cerr << "Failed to receive message\n";
            continue;
        }
        std::string msg(static_cast<char *>(request.data()), request.size());
        std::cout << "Received: " << msg << "\n";

        // "exit"で終了
        if (msg == "exit") {
            std::string reply = "Server shutting down";
            socket.send(zmq::buffer(reply), zmq::send_flags::none);
            break;
        }

        // 応答送信
        std::string reply = "ACK: " + msg;
        socket.send(zmq::buffer(reply), zmq::send_flags::none);
    }

    std::cout << "Server stopped\n";
    return 0;
}
