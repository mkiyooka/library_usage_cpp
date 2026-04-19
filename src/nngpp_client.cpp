#include <iostream>
#include <string>

#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>

int main(int argc, char *argv[]) {
    try {
        auto sock = nng::req::open();
        sock.dial("tcp://localhost:5556");

        std::cout << "Connected to NNG server\n";

        std::string msg = (argc > 1) ? argv[1] : "Hello";

        sock.send(nng::view(msg.data(), msg.size()));
        std::cout << "Sent: " << msg << "\n";

        auto buf = sock.recv();
        std::string reply(static_cast<const char *>(buf.data()), buf.size());
        std::cout << "Received: " << reply << "\n";

    } catch (const nng::exception &e) {
        std::cerr << "nng error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
