#include <iostream>
#include <string>

#include <zmq.hpp>

int main(int argc, char *argv[]) {
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect("tcp://localhost:5555");

    std::cout << "Connected to server\n";

    // コマンドライン引数からメッセージを取得、なければ "Hello"
    std::string msg = (argc > 1) ? argv[1] : "Hello";

    // メッセージ送信
    socket.send(zmq::buffer(msg), zmq::send_flags::none);
    std::cout << "Sent: " << msg << "\n";

    // 応答受信
    zmq::message_t reply;
    auto result = socket.recv(reply, zmq::recv_flags::none);
    if (!result) {
        std::cerr << "Failed to receive reply\n";
        return 1;
    }
    std::string reply_str(static_cast<char *>(reply.data()), reply.size());
    std::cout << "Received: " << reply_str << "\n";

    return 0;
}
