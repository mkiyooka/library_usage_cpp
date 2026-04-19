#!/usr/bin/env python3
"""NNG (nngpp) client example in Python using pynng."""

import sys
import pynng


def run():
    """Run the NNG Req/Rep client."""
    server_address = "tcp://localhost:5556"
    message = sys.argv[1] if len(sys.argv) > 1 else "Hello"

    with pynng.Req0() as sock:
        sock.dial(server_address)
        print("Connected to NNG server")

        sock.send(message.encode())
        print(f"Sent: {message}")

        reply = sock.recv().decode()
        print(f"Received: {reply}")


if __name__ == "__main__":
    run()
