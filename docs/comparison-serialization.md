# シリアライゼーションライブラリ比較: msgpack / Cap'n Proto / FlatBuffers / cereal

## 概要

| 項目 | msgpack-c | Cap'n Proto | FlatBuffers | cereal |
|------|-----------|-------------|-------------|--------|
| バージョン | 7.0.0 | 1.1.0 | v25.12.19 | 1.3.2 |
| ライセンス | BSL-1.0 | MIT | Apache-2.0 | BSD-3-Clause |
| フォーマット | MessagePack (バイナリ) | Cap'n Proto (バイナリ) | FlatBuffers (バイナリ) | JSON / バイナリ / XML |
| IDL / スキーマ | 不要 (マクロ定義) | `.capnp` ファイル | `.fbs` ファイル | 不要 (テンプレート) |
| エンコード不要 | ❌ | ✅ (ゼロコピー) | ✅ (ゼロコピー) | ❌ |
| 言語間互換 | ◎ 多言語対応 | ○ | ○ | △ C++ 中心 |
| ヘッダーオンリー | ✅ (C++) | ❌ | ❌ | ✅ |
| 用途 | IPC・通信 | IPC・RPC | ゲーム・組み込み | C++ 内部保存 |

## スループットベンチマーク（100件シリアライズ＋デシリアライズ）

計測環境: macOS, Apple M2, Release ビルド, Celero benchmark

| ライブラリ | us/iteration | 相対速度 (cereal=1.0) |
|-----------|-------------|----------------------|
| cereal (binary) | 5.69 | 1.00x (baseline) |
| msgpack | 4.28 | 1.33x |
| FlatBuffers | 3.18 | 1.79x |
| Cap'n Proto | 1.87 | 3.04x |

> Cap'n Proto が最速。FlatBuffers は FlatBufferBuilder のリセットコスト込みでも cereal より高速。

## スキーマ記述

### Cap'n Proto (.capnp)

```
@0xb4d4d4d4d4d4d4d4;
struct Person {
  id    @0 :UInt32;
  name  @1 :Text;
  email @2 :Text;
  score @3 :Float64;
}
struct PersonList {
  people @0 :List(Person);
}
```

**特徴**: フィールド番号 (`@0`, `@1` ...) が必須。後方互換性を番号で管理。

### FlatBuffers (.fbs)

```
namespace fbs;
table Person {
  id:    uint32;
  name:  string;
  email: string;
  score: double;
}
table PersonList {
  people: [Person];
}
root_type PersonList;
```

**特徴**: `table` vs `struct` の区別あり。`table` はヒープ、`struct` はインライン（固定サイズ用）。

### msgpack-c (マクロ)

```cpp
struct Person {
    uint32_t    id;
    std::string name;
    std::string email;
    double      score;
    MSGPACK_DEFINE(id, name, email, score)
};
```

**特徴**: スキーマファイル不要。既存の struct にマクロを追加するだけ。

### cereal (テンプレート)

```cpp
struct Person {
    uint32_t    id;
    std::string name;
    std::string email;
    double      score;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(CEREAL_NVP(id), CEREAL_NVP(name), CEREAL_NVP(email), CEREAL_NVP(score));
    }
};
```

**特徴**: スキーマ不要。JSON/XML/バイナリをアーカイブ型で切り替え可能。

## ユーザーコード比較: シリアライズ・デシリアライズ

### msgpack

```cpp
// シリアライズ
msgpack::sbuffer buf;
msgpack::pack(buf, person);

// デシリアライズ
auto obj = msgpack::unpack(buf.data(), buf.size());
Person p; obj.get().convert(p);
```

### Cap'n Proto

```cpp
// シリアライズ
capnp::MallocMessageBuilder message;
auto p = message.initRoot<Person>();
p.setName("Alice"); p.setScore(98.5);
std::ostringstream oss;
kj::std::StdOutputStream out(oss);
capnp::writeMessage(out, message);

// デシリアライズ (ゼロコピー: フィールドアクセスはデコードなし)
std::istringstream iss(bytes);
kj::std::StdInputStream in(iss);
capnp::InputStreamMessageReader reader(in);
auto p = reader.getRoot<Person>();
p.getName().cStr(); // ポインタを返すだけ
```

### FlatBuffers

```cpp
// シリアライズ
flatbuffers::FlatBufferBuilder builder(1024);
auto name = builder.CreateString("Alice");
auto person = CreatePerson(builder, 1, name, email, 98.5);
builder.Finish(person);

// デシリアライズ (ゼロコピー: バッファを直接参照)
const Person* p = GetPerson(builder.GetBufferPointer());
p->name()->str(); // std::string への変換
```

### cereal (binary)

```cpp
// シリアライズ
std::ostringstream oss;
{ cereal::BinaryOutputArchive ar(oss); ar(person); }

// デシリアライズ
std::istringstream iss(oss.str());
{ cereal::BinaryInputArchive ar(iss); ar(person2); }
```

## CMake 統合の複雑さ

| ライブラリ | CMake 統合 | コード生成 |
|-----------|-----------|----------|
| cereal | `FetchContent` のみ | 不要 |
| msgpack | `FetchContent` のみ | 不要 |
| Cap'n Proto | `FetchContent` + `CMAKE_CXX_EXTENSIONS ON` | `capnp compile -oc++` (PATH設定要) |
| FlatBuffers | `FetchContent` + `flatc` ターゲット | `flatc --cpp` |

## 後方互換性

| ライブラリ | フィールド追加 | フィールド削除 | フィールド型変更 |
|-----------|------------|------------|--------------|
| cereal | △ (バイナリは困難) | ❌ | ❌ |
| msgpack | △ (位置依存) | ❌ | ❌ |
| Cap'n Proto | ✅ (番号で管理) | ✅ (廃止フィールド保持) | ❌ |
| FlatBuffers | ✅ (table は optional) | ✅ | ❌ |

## 選択指針

| ユースケース | 推奨 |
|------------|-----|
| C++ 内部データ保存（人間可読 JSON も欲しい） | **cereal** |
| 軽量 IPC・多言語間通信 | **msgpack-c** |
| 高性能 RPC・プロセス間大量データ転送 | **Cap'n Proto** |
| ゲーム・組み込み・ゼロコピーのバイナリ配布 | **FlatBuffers** |
| スキーマ定義なしに素早く実装したい | **msgpack-c** または **cereal** |
| gRPC の代替 RPC フレームワークが欲しい | **Cap'n Proto RPC** |
