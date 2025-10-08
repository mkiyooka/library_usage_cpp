#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "greeter.grpc.pb.h"

using greeter::Greeter;
using greeter::HelloReply;
using greeter::HelloRequest;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

// Greeterサービスの実装
class GreeterServiceImpl final : public Greeter::Service {
    Status SayHello(ServerContext *context, const HelloRequest *request, HelloReply *reply) override {
        std::string prefix("Hello ");
        reply->set_message(prefix + request->name());
        std::cout << "Received request from: " << request->name() << "\n";
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    GreeterServiceImpl service;

    ServerBuilder builder;
    // リッスンするアドレスを追加（認証なし）
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // サービスを登録
    builder.RegisterService(&service);
    // サーバーの組み立て
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << "\n";

    // サーバーをシャットダウンするまで待機
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}
