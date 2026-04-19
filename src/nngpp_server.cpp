#include <iostream>
#include <string>

#include <nngpp/nngpp.h>
#include <nngpp/protocol/rep0.h>

int main() {
    try {
        auto sock = nng::rep::open();
        sock.listen("tcp://*:5556");

        std::cout << "NNG server started on port 5556\n";
        std::cout << "Waiting for messages...\n";

        while (true) {
            auto buf = sock.recv();
            std::string msg(static_cast<const char *>(buf.data()), buf.size());
            std::cout << "Received: " << msg << "\n";

            if (msg == "exit") {
                std::string reply = "Server shutting down";
                sock.send(nng::view(reply.data(), reply.size()));
                break;
            }

            std::string reply = "ACK: " + msg;
            sock.send(nng::view(reply.data(), reply.size()));
        }

        std::cout << "Server stopped\n";
    } catch (const nng::exception &e) {
        std::cerr << "nng error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
