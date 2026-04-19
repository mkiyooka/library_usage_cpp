# シリアライゼーションライブラリ比較: msgpack-c / Cap'n Proto / FlatBuffers / cereal

## 概要

| 項目 | msgpack-c | Cap'n Proto | FlatBuffers | cereal |
| ---- | --------- | ----------- | ----------- | ------ |
| バージョン | 7.0.0 | 1.1.0 | v25.12.19 | 1.3.2 |
| ライセンス | BSL-1.0 | MIT | Apache-2.0 | BSD-3-Clause |
| フォーマット | MessagePack (バイナリ) | Cap'n Proto (バイナリ) | FlatBuffers (バイナリ) | JSON / バイナリ / XML |
| スキーマ定義 | 不要 (マクロ) | `.capnp` IDL | `.fbs` IDL | 不要 (テンプレート) |
| コード生成 | 不要 | `capnp compile` | `flatc` | 不要 |
| エンコード処理 | あり | **なし** (ゼロコピー) | **なし** (ゼロコピー) | あり |
| 言語間互換 | ◎ 多言語 | ○ | ○ | △ C++ 中心 |
| ヘッダーオンリー | ✅ | ❌ | ❌ | ✅ |
| 主な用途 | IPC・通信 | IPC・RPC | ゲーム・組み込み | C++ 内部保存 |
| サンプル | `src/msgpack_example.cpp` | `src/capnproto_example.cpp` | `src/flatbuffers_example.cpp` | `src/cereal_example.cpp` |

---

## スループットベンチマーク（100件シリアライズ＋デシリアライズ）

計測環境: macOS, Apple M2, Release ビルド, Celero benchmark (`benches/bench_serialization.cpp`)

| ライブラリ | us/iteration | 相対速度 (cereal=1.0) |
| --------- | ------------ | -------------------- |
| cereal (binary) | 5.69 | 1.00x (baseline) |
| msgpack | 4.28 | 1.33x |
| FlatBuffers | 3.18 | 1.79x |
| Cap'n Proto | 1.87 | **3.04x** |

Cap'n Proto が最速。デシリアライズがゼロコピーで完了するため、往復コストがほぼシリアライズのみになる。
FlatBuffers も `FlatBufferBuilder` のリセットコスト込みで cereal より 1.8 倍速い。

```bash
# ベンチマーク実行
./build/benches/bench_serialization
```

---

## msgpack-c

### 特徴

- **スキーマ不要**: 既存の `struct` に `MSGPACK_DEFINE` マクロを 1 行追加するだけ
- **多言語互換**: Python (`msgpack`)・Ruby・Rust・Go など多数の言語と相互運用可能
- **ヘッダーオンリー**: C++ モードはヘッダーのみで動作
- **シリアライズあり**: 書き込み時にエンコード、読み込み時にデコードを行う

### CMake 設定

スキーマファイルもコード生成ツールも不要。FetchContent で取得するだけで使える。

```cmake
# cmake/dependencies-app.cmake
set(MSGPACK_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(MSGPACK_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(MSGPACK_USE_BOOST OFF CACHE BOOL "" FORCE)       # Boost 不要
FetchContent_Declare(msgpack
    URL https://github.com/msgpack/msgpack-c/archive/refs/tags/cpp-7.0.0.tar.gz
    ...
)
FetchContent_MakeAvailable(msgpack)

# src/CMakeLists.txt
add_executable(msgpack_example msgpack_example.cpp)
target_link_libraries(msgpack_example PRIVATE msgpack-cxx)  # ターゲット名: msgpack-cxx
```

### 型定義

`MSGPACK_DEFINE` マクロでフィールドを列挙するだけでシリアライズ対応になる。
フィールドは **順序依存** なので、追加は末尾にのみ行うこと。

```cpp
struct Sensor {
    std::string name;
    double      value;
    int64_t     timestamp_ms;
    MSGPACK_DEFINE(name, value, timestamp_ms)  // シリアライズ対象フィールドを列挙
};
```

### 基本的な使い方

```cpp
#include <msgpack.hpp>

// --- シリアライズ ---
Sensor sensor{"temperature", 23.5, 1700000000000LL};
msgpack::sbuffer buf;                    // 可変長バイトバッファ
msgpack::pack(buf, sensor);              // sensor → バイト列

// --- デシリアライズ ---
auto obj = msgpack::unpack(buf.data(), buf.size());
Sensor restored;
obj.get().convert(restored);             // バイト列 → sensor
```

### 複数オブジェクトの逐次読み出し

1 つのバッファに複数オブジェクトを詰め込んで、`offset` で順に読み出せる。

```cpp
msgpack::sbuffer buf;
msgpack::packer<msgpack::sbuffer> pk(buf);
pk.pack(std::string("hello"));
pk.pack(42);
pk.pack(std::vector<int>{1, 2, 3});

std::size_t off = 0;
auto r1 = msgpack::unpack(buf.data(), buf.size(), off);  // off が自動更新される
auto r2 = msgpack::unpack(buf.data(), buf.size(), off);
auto r3 = msgpack::unpack(buf.data(), buf.size(), off);

r1.get().as<std::string>();   // "hello"
r2.get().as<int>();           // 42
r3.get().as<std::vector<int>>(); // {1, 2, 3}
```

### 動作確認

```bash
./build/msgpack_example
# Packed basic types: 27 bytes
# str=hello  int=42  vec=[ 1 2 3 ]
# Sensor packed: 37 bytes
# Sensor restored: name=temperature value=23.5 ts=1700000000000
```

---

## Cap'n Proto

### 特徴

- **ゼロコピー**: シリアライズしたバイト列がそのままメモリ表現（デコード処理なし）
- **IDL 必須**: `.capnp` スキーマファイルを記述し、`capnp compile` でコード生成
- **フィールド番号管理**: 各フィールドに `@0`, `@1` ... の番号が必須（後方互換性の基盤）
- **Cap'n Proto RPC**: gRPC に相当する非同期 RPC フレームワークが内蔵

### スキーマ記述 (`proto/person.capnp`)

```capnp
@0xb4d4d4d4d4d4d4d4;    # ファイルID（capnp id コマンドで生成）

struct Person {
  id    @0 :UInt32;      # @N はフィールド番号（変更禁止、削除不可）
  name  @1 :Text;
  email @2 :Text;
  score @3 :Float64;
}

struct PersonList {
  people @0 :List(Person);
}
```

**フィールド番号のルール:**

- `@0` から始まり連番で付与する
- 一度付けた番号は**変更・削除不可**（後方互換性破壊のため）
- フィールドを廃止したい場合は `$deprecated` アノテーションを使い番号は保持する

### コード生成

```bash
# 手動でコード生成する場合
capnp compile -oc++ proto/person.capnp
# → person.capnp.h と person.capnp.c++ が生成される
```

**CMake での自動生成** (`src/CMakeLists.txt`):

```cmake
set(CAPNP_SCHEMA ${CMAKE_SOURCE_DIR}/proto/person.capnp)
set(CAPNP_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/capnp_gen)
file(MAKE_DIRECTORY ${CAPNP_OUT_DIR})

add_custom_command(
    OUTPUT ${CAPNP_OUT_DIR}/person.capnp.h ${CAPNP_OUT_DIR}/person.capnp.c++
    # capnpc-c++ プラグインが CMAKE_BINARY_DIR にあるので PATH に追加する
    COMMAND ${CMAKE_COMMAND} -E env "PATH=${CMAKE_BINARY_DIR}:$ENV{PATH}"
            $<TARGET_FILE:capnp_tool> compile
            -oc++:${CAPNP_OUT_DIR}
            --src-prefix=${CMAKE_SOURCE_DIR}/proto
            ${CAPNP_SCHEMA}
    DEPENDS ${CAPNP_SCHEMA} capnp_tool capnpc_cpp
)

add_executable(capnproto_example
    capnproto_example.cpp
    ${CAPNP_OUT_DIR}/person.capnp.c++   # 生成された .c++ をソースに追加
)
target_include_directories(capnproto_example PRIVATE ${CAPNP_OUT_DIR})
target_link_libraries(capnproto_example PRIVATE CapnProto::capnp)
```

> **注意**: FetchContent でビルドした `capnp` ツールは `CMAKE_BINARY_DIR` 直下に配置される。
> `capnp compile -oc++` は `capnpc-c++` プラグインを PATH から探すため、
> `cmake -E env "PATH=..."` でビルドディレクトリを PATH に追加する必要がある。

### CMake 設定の注意点

Cap'n Proto は GNU C++ 拡張機能を使うため、プロジェクトが `CMAKE_CXX_EXTENSIONS OFF` の場合は
FetchContent の前後で一時的に ON にする必要がある。

```cmake
set(_saved_cxx_ext ${CMAKE_CXX_EXTENSIONS})
set(CMAKE_CXX_EXTENSIONS ON)          # Cap'n Proto のビルドに必要
FetchContent_MakeAvailable(capnproto)
set(CMAKE_CXX_EXTENSIONS ${_saved_cxx_ext})  # 元に戻す
```

### 基本的な使い方

```cpp
#include <capnp/message.h>
#include <capnp/serialize.h>
#include <kj/std/iostream.h>
#include "person.capnp.h"             // capnp compile が生成したヘッダー

// --- シリアライズ ---
capnp::MallocMessageBuilder message;  // メッセージのメモリ管理
auto person_list = message.initRoot<PersonList>();
auto people = person_list.initPeople(3);  // 3人分のスロットを確保

people[0].setId(1);
people[0].setName("Alice");
people[0].setEmail("alice@example.com");
people[0].setScore(98.5);
// ... (people[1], people[2] も同様)

// ストリームに書き出す
std::ostringstream oss;
kj::std::StdOutputStream kj_out(oss);
capnp::writeMessage(kj_out, message);
std::string bytes = oss.str();
// → bytes.size() = 216 bytes (100人なら ~3200 bytes)

// --- デシリアライズ (ゼロコピー) ---
std::istringstream iss(bytes);
kj::std::StdInputStream kj_in(iss);
capnp::InputStreamMessageReader reader(kj_in);

auto pl = reader.getRoot<PersonList>();
for (auto p : pl.getPeople()) {
    // フィールドアクセス = バイト列へのポインタを返すだけ（デコードなし）
    p.getId();           // uint32_t
    p.getName().cStr();  // const char* (コピーなし)
    p.getScore();        // double
}
```

### 動作確認

```bash
./build/capnproto_example
# Serialized size: 216 bytes
# Deserialized 3 people:
#   [1] Alice <alice@example.com> score=98.5
#   [2] Bob <bob@example.com> score=87
#   [3] Carol <carol@example.com> score=92.3
```

---

## FlatBuffers

### 特徴

- **ゼロコピー**: バッファを直接参照するためデシリアライズ処理なし
- **IDL 必須**: `.fbs` スキーマファイルを記述し、`flatc` でヘッダーを生成
- **ヘッダーのみ生成**: `.h` ファイル 1 つだけ生成（`.cpp` は不要）
- **`table` vs `struct`**: `table` はヒープ参照（フィールド省略可）、`struct` はインライン（固定サイズ）
- **ビルトイン検証**: `Verifier` で受信データの整合性を O(n) でチェックできる

### スキーマ記述 (`proto/person.fbs`)

```flatbuffers
namespace fbs;           // C++ の namespace に対応

table Person {           // table: フィールドが省略可能なヒープ型
  id:    uint32;
  name:  string;
  email: string;
  score: double;
}

table PersonList {
  people: [Person];      // Person の配列
}

root_type PersonList;    // バッファのルート型を指定（必須）
```

**`table` と `struct` の使い分け:**

```flatbuffers
// struct: 固定サイズ、インライン配置、フィールド省略不可（座標などに最適）
struct Vec3 { x: float; y: float; z: float; }

// table: 可変サイズ、フィールド省略可、後方互換性あり（通常はこちらを使う）
table Entity { pos: Vec3; name: string; hp: int = 100; }
```

### コード生成

```bash
# 手動でヘッダーを生成する場合
flatc --cpp --scoped-enums -o ./output proto/person.fbs
# → output/person_generated.h が生成される（.cpp は不要）
```

`--scoped-enums`: C++11 の `enum class` を使用（名前衝突を防ぐ）

**CMake での自動生成** (`src/CMakeLists.txt`):

```cmake
set(FBS_SCHEMA ${CMAKE_SOURCE_DIR}/proto/person.fbs)
set(FBS_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/fbs_gen)
file(MAKE_DIRECTORY ${FBS_OUT_DIR})

add_custom_command(
    OUTPUT ${FBS_OUT_DIR}/person_generated.h
    COMMAND $<TARGET_FILE:flatc>   # FetchContent でビルドした flatc
            --cpp --scoped-enums
            -o ${FBS_OUT_DIR}
            ${FBS_SCHEMA}
    DEPENDS ${FBS_SCHEMA} flatc    # flatc ターゲットへの依存を宣言
)

add_executable(flatbuffers_example
    flatbuffers_example.cpp
    ${FBS_OUT_DIR}/person_generated.h  # ヘッダーを依存に追加（生成トリガー）
)
target_include_directories(flatbuffers_example PRIVATE ${FBS_OUT_DIR})
target_link_libraries(flatbuffers_example PRIVATE flatbuffers)
```

> **注意**: Cap'n Proto と異なり、生成物は `.h` のみ。`add_executable` のソースに
> `person_generated.h` を追加することで `add_custom_command` の依存関係が確立される。

### CMake 設定

```cmake
set(FLATBUFFERS_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(FLATBUFFERS_BUILD_FLATC ON CACHE BOOL "" FORCE)      # flatc コンパイラをビルド
set(FLATBUFFERS_BUILD_FLATHASH OFF CACHE BOOL "" FORCE)
set(FLATBUFFERS_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)
set(FLATBUFFERS_BUILD_SHAREDLIB OFF CACHE BOOL "" FORCE)
FetchContent_Declare(flatbuffers URL ...)
FetchContent_MakeAvailable(flatbuffers)
# ターゲット名: flatbuffers (静的ライブラリ), flatc (コンパイラ実行ファイル)
```

### 基本的な使い方

```cpp
#include <flatbuffers/flatbuffers.h>
#include "person_generated.h"   // flatc が生成したヘッダー

using namespace fbs;

// --- シリアライズ ---
// FlatBufferBuilder はバッファを後ろから前へ構築する（文字列・配列から先に作る）
flatbuffers::FlatBufferBuilder builder(1024);  // 初期容量 (自動拡張あり)

// 文字列・ネストオブジェクトは先に作る必要がある
auto name1  = builder.CreateString("Alice");
auto email1 = builder.CreateString("alice@example.com");
auto p1 = CreatePerson(builder, 1, name1, email1, 98.5);

// 配列も事前に作成
std::vector<flatbuffers::Offset<Person>> people_offsets = {p1, p2, p3};
auto people_vec = builder.CreateVector(people_offsets);

// ルートオブジェクトを最後に作成して Finish する
auto list = CreatePersonList(builder, people_vec);
builder.Finish(list);

// シリアライズ済みバッファへのポインタとサイズ
const uint8_t* buf  = builder.GetBufferPointer();
size_t         size = builder.GetSize();
// → size = 240 bytes (3人分)

// --- デシリアライズ (ゼロコピー) ---
// buf をそのまま参照するだけ（コピー・デコードなし）
const PersonList* pl = GetPersonList(buf);

for (const auto* p : *pl->people()) {
    p->id();            // uint32_t
    p->name()->str();   // std::string へのコピー変換
    p->name()->c_str(); // const char* (ゼロコピー)
    p->score();         // double
}

// --- 検証 (受信データが壊れていないか確認) ---
flatbuffers::Verifier verifier(buf, size);
bool ok = VerifyPersonListBuffer(verifier);  // O(n) で整合性チェック
```

**シリアライズの構築順序に注意:**

FlatBuffers はバッファを後ろから前に向かって書き込む。そのため、
ネストされたオブジェクト（文字列・配列・テーブル）は **親より先に** 作らなければならない。

```cpp
// NG: PersonList を作ってから Person を追加しようとすると失敗する
// OK: 末端（文字列）→ Person → PersonList の順で作る
auto name  = builder.CreateString("Alice");  // 1. 文字列
auto p     = CreatePerson(builder, 1, name, email, 98.5);  // 2. Person
auto vec   = builder.CreateVector({p});      // 3. 配列
auto list  = CreatePersonList(builder, vec); // 4. ルート
builder.Finish(list);                        // 5. 完了
```

### 動作確認

```bash
./build/flatbuffers_example
# Serialized size: 240 bytes
# Deserialized 3 people:
#   [1] Alice <alice@example.com>  score=98.5
#   [2] Bob <bob@example.com>  score=87
#   [3] Carol <carol@example.com>  score=92.3
# Verification: OK
```

---

## 選択指針

| ユースケース | 推奨 |
| ---------- | --- |
| C++ 内部データ保存（JSON/バイナリを切り替えたい） | **cereal** |
| 軽量 IPC・多言語間通信（Python 等と連携） | **msgpack-c** |
| 高性能 RPC・プロセス間大量データ転送 | **Cap'n Proto** |
| ゲーム・組み込み・ゼロコピーのバイナリ配布 | **FlatBuffers** |
| スキーマ定義なしに素早く実装したい | **msgpack-c** または **cereal** |
| gRPC の代替 RPC フレームワークが必要 | **Cap'n Proto RPC** |

## CMake 統合の複雑さ

| ライブラリ | CMake 統合 | コード生成ツール | 生成物 |
| --------- | --------- | -------------- | ----- |
| cereal | `FetchContent` のみ | 不要 | — |
| msgpack | `FetchContent` のみ | 不要 | — |
| Cap'n Proto | `FetchContent` + `CMAKE_CXX_EXTENSIONS ON` | `capnp compile -oc++` | `.h` + `.c++` |
| FlatBuffers | `FetchContent` + `flatc` ターゲット | `flatc --cpp` | `.h` のみ |

## 後方互換性

| ライブラリ | フィールド追加 | フィールド削除 | フィールド型変更 |
| --------- | ------------ | ------------ | -------------- |
| cereal | △ (バイナリは困難) | ❌ | ❌ |
| msgpack | △ (末尾追加のみ) | ❌ | ❌ |
| Cap'n Proto | ✅ (番号で管理) | ✅ (`$deprecated` で保持) | ❌ |
| FlatBuffers | ✅ (`table` は optional) | ✅ (deprecated フィールド) | ❌ |
