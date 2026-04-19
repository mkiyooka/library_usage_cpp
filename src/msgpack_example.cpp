#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <msgpack.hpp>

// カスタム型（MSGPACK_DEFINE マクロで自動変換）
struct Sensor {
    std::string name;
    double value;
    int64_t timestamp_ms;
    MSGPACK_DEFINE(name, value, timestamp_ms)
};

int main() {
    std::cout << "=== msgpack-c Example ===\n\n";

    // --- 基本型のシリアライズ ---
    msgpack::sbuffer buf;
    msgpack::packer<msgpack::sbuffer> pk(buf);
    pk.pack(std::string("hello"));
    pk.pack(42);
    pk.pack(std::vector<int>{1, 2, 3, 4, 5});
    pk.pack(
        std::map<std::string, int>{
            {"a", 1},
            {"b", 2}
    }
    );

    std::cout << "Packed basic types: " << buf.size() << " bytes\n";

    // デシリアライズ（offset逐次読み出し）
    std::size_t off = 0;
    auto result1 = msgpack::unpack(buf.data(), buf.size(), off);
    auto result2 = msgpack::unpack(buf.data(), buf.size(), off);
    auto result3 = msgpack::unpack(buf.data(), buf.size(), off);
    auto result4 = msgpack::unpack(buf.data(), buf.size(), off);

    std::cout << "  str=" << result1.get().as<std::string>() << "\n";
    std::cout << "  int=" << result2.get().as<int>() << "\n";
    auto v = result3.get().as<std::vector<int>>();
    std::cout << "  vec=[ ";
    for (int x : v)
        std::cout << x << " ";
    std::cout << "]\n";
    auto m = result4.get().as<std::map<std::string, int>>();
    std::cout << "  map={a=" << m["a"] << ", b=" << m["b"] << "}\n\n";

    // --- カスタム型 ---
    Sensor sensor{"temperature", 23.5, 1700000000000LL};
    msgpack::sbuffer sensor_buf;
    msgpack::pack(sensor_buf, sensor);
    std::cout << "Sensor packed: " << sensor_buf.size() << " bytes\n";

    auto sensor_obj = msgpack::unpack(sensor_buf.data(), sensor_buf.size());
    Sensor restored;
    sensor_obj.get().convert(restored);
    std::cout << "Sensor restored: name=" << restored.name << " value=" << restored.value
              << " ts=" << restored.timestamp_ms << "\n\n";

    // --- 複数センサーのバッチ送信 ---
    std::vector<Sensor> readings = {
        {    "temp",    23.5, 1000},
        {"humidity",    65.2, 1001},
        {"pressure", 1013.25, 1002}
    };
    msgpack::sbuffer batch_buf;
    msgpack::pack(batch_buf, readings);
    std::cout << "Batch (" << readings.size() << " sensors): " << batch_buf.size() << " bytes\n";

    auto batch_obj = msgpack::unpack(batch_buf.data(), batch_buf.size());
    std::vector<Sensor> restored_batch;
    batch_obj.get().convert(restored_batch);
    for (const auto &s : restored_batch) {
        std::cout << "  " << s.name << "=" << s.value << "\n";
    }

    return 0;
}
