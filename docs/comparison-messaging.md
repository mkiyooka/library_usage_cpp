# メッセージング・通信ライブラリ比較: ZeroMQ vs NNG

## 概要

| 項目 | ZeroMQ (cppzmq) | NNG |
|------|----------------|-----|
| バージョン | cppzmq 4.11.0 / libzmq 4.3.5 | v1.11 |
| ライセンス | MIT / MPL-2.0 | MIT |
| 言語 | C++ ラッパー (cppzmq) | C API (+ 非公式 C++ ラッパー) |
| 関係 | ZeroMQ の公式 C++ バインディング | ZeroMQ の後継実装 (nanomsg → NNG) |
| ヘッダーオンリー | ❌ | ❌ |

## アーキテクチャの違い

**ZeroMQ** は libzmq (C) + cppzmq (C++ ヘッダー) の組み合わせ。  
**NNG** は ZeroMQ 創始者の Garrett D'Amore が設計した後継実装。ZeroMQ の問題点 (スレッドモデル, API の一貫性) を再設計。

## サポートパターン

| パターン | ZeroMQ | NNG |
|---------|--------|-----|
| Req/Rep | ✅ | ✅ |
| Pub/Sub | ✅ | ✅ |
| Push/Pull | ✅ | ✅ |
| Pair | ✅ | ✅ |
| Bus | ❌ | ✅ |
| Survey | ❌ | ✅ |

## ユーザーコード比較

### ZeroMQ (cppzmq) - Req/Rep

```cpp
// サーバー
zmq::context_t ctx;
zmq::socket_t sock(ctx, zmq::socket_type::rep);
sock.bind("tcp://*:5555");
zmq::message_t req;
sock.recv(req);
std::string body(static_cast<char*>(req.data()), req.size());
sock.send(zmq::message_t("pong", 4), zmq::send_flags::none);

// クライアント
zmq::socket_t sock(ctx, zmq::socket_type::req);
sock.connect("tcp://localhost:5555");
sock.send(zmq::message_t("ping", 4), zmq::send_flags::none);
zmq::message_t reply;
sock.recv(reply);
```

### NNG - Req/Rep

```cpp
// サーバー
nng_socket rep_sock;
nng_rep0_open(&rep_sock);
nng_listen(rep_sock, "tcp://*:5555", nullptr, 0);
nng_msg* msg;
nng_recvmsg(rep_sock, &msg, 0);
std::string body(static_cast<char*>(nng_msg_body(msg)), nng_msg_len(msg));
nng_msg_free(msg);
nng_msg* rmsg; nng_msg_alloc(&rmsg, 0);
nng_msg_append(rmsg, "pong", 4);
nng_sendmsg(rep_sock, rmsg, 0);

// クライアント
nng_socket req_sock;
nng_req0_open(&req_sock);
nng_dial(req_sock, "tcp://localhost:5555", nullptr, 0);
nng_msg* smsg; nng_msg_alloc(&smsg, 0);
nng_msg_append(smsg, "ping", 4);
nng_sendmsg(req_sock, smsg, 0);
nng_msg* reply; nng_recvmsg(req_sock, &reply, 0);
```

## 評価

| 観点 | ZeroMQ | NNG |
|------|--------|-----|
| C++ API の使いやすさ | ◎ RAII ラッパーが充実 | △ C API のみ（nngpp など別途必要） |
| エコシステム | ◎ 実績多数・バインディング豊富 | △ 比較的新しい |
| スレッドモデル | 明示的なコンテキスト管理 | 内部管理（シンプル） |
| TLS サポート | ✅ (CURVE 暗号化) | ✅ (mbedTLS/wolfSSL) |
| Windows 対応 | ✅ | ✅ |
| アクティブ開発 | △ メンテ中 | ✅ |
| ドキュメント | ◎ 豊富 | ○ 十分 |

## 推奨

- **既存プロジェクトや豊富なエコシステムが必要**: **ZeroMQ (cppzmq)**
- **新規プロジェクトで後継の設計を採用したい**: **NNG**
- **C++ らしい API が必要**: ZeroMQ (cppzmq のラッパーが優秀)

NNG は ZeroMQ の設計上の問題点を修正した後継だが、C++ バインディングが標準装備でないため、現時点では cppzmq の方が使いやすい。
