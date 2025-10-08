#!/usr/bin/env python3
"""gRPC client example in Python."""

import sys
import grpc
import greeter_pb2
import greeter_pb2_grpc


def run():
    """Run the gRPC client."""
    # サーバーアドレス
    server_address = 'localhost:50051'

    # コマンドライン引数からユーザー名を取得、なければ "World"
    name = sys.argv[1] if len(sys.argv) > 1 else "World"

    # チャネルを作成（認証なし）
    with grpc.insecure_channel(server_address) as channel:
        stub = greeter_pb2_grpc.GreeterStub(channel)

        print(f"Sending request to {server_address} with name: {name}")

        # RPCを呼び出し
        response = stub.SayHello(greeter_pb2.HelloRequest(name=name))

        print(f"Greeter received: {response.message}")


if __name__ == '__main__':
    run()
