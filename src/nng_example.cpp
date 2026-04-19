#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/protocol/reqrep0/req.h>

static void check(int rv, const char *op) {
    if (rv != 0) {
        std::cerr << op << ": " << nng_strerror(rv) << "\n";
        std::exit(1);
    }
}

// Req/Rep パターン
void demo_reqrep() {
    std::cout << "-- Req/Rep pattern --\n";
    constexpr auto url = "inproc://reqrep";

    nng_socket rep_sock, req_sock;
    check(nng_rep0_open(&rep_sock), "nng_rep0_open");
    check(nng_req0_open(&req_sock), "nng_req0_open");
    check(nng_listen(rep_sock, url, nullptr, 0), "nng_listen");
    check(nng_dial(req_sock, url, nullptr, 0), "nng_dial");

    // サーバースレッド: 1回応答して終了
    std::thread server([&] {
        nng_msg *msg = nullptr;
        nng_recvmsg(rep_sock, &msg, 0);
        std::string req(static_cast<char *>(nng_msg_body(msg)), nng_msg_len(msg));
        std::cout << "  server got: " << req << "\n";
        nng_msg_free(msg);

        std::string reply = "pong";
        nng_msg *rmsg = nullptr;
        nng_msg_alloc(&rmsg, 0);
        nng_msg_append(rmsg, reply.data(), reply.size());
        nng_sendmsg(rep_sock, rmsg, 0);
    });

    // クライアント
    std::string payload = "ping";
    nng_msg *smsg = nullptr;
    nng_msg_alloc(&smsg, 0);
    nng_msg_append(smsg, payload.data(), payload.size());
    check(nng_sendmsg(req_sock, smsg, 0), "nng_sendmsg");

    nng_msg *rmsg = nullptr;
    check(nng_recvmsg(req_sock, &rmsg, 0), "nng_recvmsg");
    std::string reply(static_cast<char *>(nng_msg_body(rmsg)), nng_msg_len(rmsg));
    std::cout << "  client got: " << reply << "\n";
    nng_msg_free(rmsg);

    server.join();
    nng_close(req_sock);
    nng_close(rep_sock);
}

// Pub/Sub パターン
void demo_pubsub() {
    std::cout << "\n-- Pub/Sub pattern --\n";
    constexpr auto url = "inproc://pubsub";

    nng_socket pub_sock, sub_sock;
    check(nng_pub0_open(&pub_sock), "nng_pub0_open");
    check(nng_sub0_open(&sub_sock), "nng_sub0_open");
    // 空文字列 = 全てのトピックを購読
    check(nng_setopt(sub_sock, NNG_OPT_SUB_SUBSCRIBE, "", 0), "subscribe");
    check(nng_listen(pub_sock, url, nullptr, 0), "nng_listen");
    check(nng_dial(sub_sock, url, nullptr, 0), "nng_dial");

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::thread pub_thread([&] {
        for (int i = 0; i < 3; ++i) {
            std::string msg = "msg-" + std::to_string(i);
            nng_msg *m = nullptr;
            nng_msg_alloc(&m, 0);
            nng_msg_append(m, msg.data(), msg.size());
            nng_sendmsg(pub_sock, m, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });

    pub_thread.join();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    nng_msg *m = nullptr;
    while (nng_recvmsg(sub_sock, &m, NNG_FLAG_NONBLOCK) == 0) {
        std::string s(static_cast<char *>(nng_msg_body(m)), nng_msg_len(m));
        std::cout << "  subscriber got: " << s << "\n";
        nng_msg_free(m);
    }

    nng_close(sub_sock);
    nng_close(pub_sock);
}

int main() {
    std::cout << "=== NNG (nanomsg-next-gen) Example ===\n\n";
    demo_reqrep();
    demo_pubsub();
    return 0;
}
