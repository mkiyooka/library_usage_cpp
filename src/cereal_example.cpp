#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

struct Measurement {
    std::string sensor_id;
    double timestamp = 0.0;
    std::vector<double> values;

    template <class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(sensor_id), CEREAL_NVP(timestamp), CEREAL_NVP(values));
    }
};

static void JsonSerialization() {
    std::cout << "=== JSON Serialization ===\n";

    Measurement m{
        "sensor_01", 1713400000.0, {1.23, 4.56, 7.89, 0.12}
    };

    {
        std::ofstream ofs("measurement.json");
        cereal::JSONOutputArchive ar(ofs);
        ar(CEREAL_NVP(m));
    }

    Measurement m2;
    {
        std::ifstream ifs("measurement.json");
        cereal::JSONInputArchive ar(ifs);
        ar(m2);
    }

    std::cout << "sensor_id: " << m2.sensor_id << "\n";
    std::cout << "timestamp: " << m2.timestamp << "\n";
    std::cout << "values: ";
    for (double v : m2.values) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}

static void BinarySerialization() {
    std::cout << "\n=== Binary Serialization ===\n";

    std::vector<Measurement> dataset;
    for (int i = 0; i < 3; ++i) {
        dataset.push_back({
            "sensor_0" + std::to_string(i),
            static_cast<double>(i) * 0.1,
            {static_cast<double>(i), static_cast<double>(i) * 2.0}
        });
    }

    {
        std::ofstream ofs("dataset.bin", std::ios::binary);
        cereal::BinaryOutputArchive ar(ofs);
        ar(dataset);
    }

    std::vector<Measurement> loaded;
    {
        std::ifstream ifs("dataset.bin", std::ios::binary);
        cereal::BinaryInputArchive ar(ifs);
        ar(loaded);
    }

    std::cout << "Loaded " << loaded.size() << " measurements:\n";
    for (const auto &d : loaded) {
        std::cout << "  " << d.sensor_id << " t=" << d.timestamp << " vals=[" << d.values[0] << ", " << d.values[1]
                  << "]\n";
    }
}

int main() {
    JsonSerialization();
    BinarySerialization();
    return 0;
}
