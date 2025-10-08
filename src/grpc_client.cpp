#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "greeter.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using greeter::Greeter;
using greeter::HelloReply;
using greeter::HelloRequest;

class GreeterClient {
public:
    GreeterClient(std::shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {}

    // サーバーにリクエストを送信
    std::string SayHello(const std::string &user) {
        // リクエストを準備
        HelloRequest request;
        request.set_name(user);

        // レスポンス用のコンテナ
        HelloReply reply;

        // コンテキストは、RPCの設定を変更するために使用可能
        ClientContext context;

        // 実際のRPC呼び出し
        Status status = stub_->SayHello(&context, request, &reply);

        // 返り値とレスポンスをチェック
        if (status.ok()) {
            return reply.message();
        }
        std::cerr << "RPC failed: " << status.error_code() << ": " << status.error_message() << "\n";
        return "RPC failed";
    }

private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char *argv[]) {
    // サーバーアドレス
    std::string target_str = "localhost:50051";

    // コマンドライン引数からユーザー名を取得、なければ "World"
    std::string user = (argc > 1) ? argv[1] : "World";

    // チャネルを作成（認証なし）
    GreeterClient greeter(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

    std::cout << "Sending request to " << target_str << " with name: " << user << "\n";

    // RPCを呼び出し
    std::string reply = greeter.SayHello(user);
    std::cout << "Greeter received: " << reply << "\n";

    return 0;
}
