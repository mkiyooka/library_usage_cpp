#!/usr/bin/env python3
"""ZeroMQ client example in Python."""

import sys
import zmq


def run():
    """Run the ZeroMQ client."""
    # サーバーアドレス
    server_address = 'tcp://localhost:5555'

    # コマンドライン引数からメッセージを取得、なければ "Hello"
    message = sys.argv[1] if len(sys.argv) > 1 else "Hello"

    # ZeroMQコンテキストとソケットを作成
    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect(server_address)

    print(f"Connected to server")

    # メッセージ送信
    socket.send_string(message)
    print(f"Sent: {message}")

    # 応答受信
    reply = socket.recv_string()
    print(f"Received: {reply}")


if __name__ == '__main__':
    run()
