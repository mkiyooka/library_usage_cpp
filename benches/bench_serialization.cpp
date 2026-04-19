#include <sstream>
#include <string>
#include <vector>

#include <celero/Celero.h>

// --- cereal ---
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

// --- msgpack ---
#include <msgpack.hpp>

// --- Cap'n Proto ---
#include <capnp/message.h>
#include <capnp/serialize.h>
#include <kj/std/iostream.h>
#include "person.capnp.h"

// --- FlatBuffers ---
#include "person_generated.h"

CELERO_MAIN

// ベンチマーク対象データ: 100人のリスト
static const int N = 100;

struct PersonData {
    uint32_t id;
    std::string name;
    std::string email;
    double score;

    template <class Archive>
    void serialize(Archive& ar) {
        ar(id, name, email, score);
    }
    MSGPACK_DEFINE(id, name, email, score)
};

static std::vector<PersonData> make_people() {
    std::vector<PersonData> v;
    v.reserve(N);
    for (int i = 0; i < N; ++i) {
        v.push_back({static_cast<uint32_t>(i), "Person" + std::to_string(i),
                     "person" + std::to_string(i) + "@example.com", static_cast<double>(i) * 0.5});
    }
    return v;
}

static const std::vector<PersonData> kPeople = make_people();

// ===== cereal (binary) =====
BASELINE(Serialization, cereal_binary, 10, 1000) {
    std::ostringstream oss;
    {
        cereal::BinaryOutputArchive ar(oss);
        ar(kPeople);
    }
    std::string bytes = oss.str();
    std::vector<PersonData> out;
    {
        std::istringstream iss(bytes);
        cereal::BinaryInputArchive ar(iss);
        ar(out);
    }
    celero::DoNotOptimizeAway(out.size());
}

// ===== msgpack =====
BENCHMARK(Serialization, msgpack, 10, 1000) {
    msgpack::sbuffer buf;
    msgpack::pack(buf, kPeople);
    auto obj = msgpack::unpack(buf.data(), buf.size());
    std::vector<PersonData> out;
    obj.get().convert(out);
    celero::DoNotOptimizeAway(out.size());
}

// ===== Cap'n Proto =====
BENCHMARK(Serialization, capnproto, 10, 1000) {
    // serialize
    capnp::MallocMessageBuilder message;
    auto pl = message.initRoot<PersonList>();
    auto people = pl.initPeople(N);
    for (int i = 0; i < N; ++i) {
        people[i].setId(kPeople[i].id);
        people[i].setName(kPeople[i].name);
        people[i].setEmail(kPeople[i].email);
        people[i].setScore(kPeople[i].score);
    }
    std::ostringstream oss;
    kj::std::StdOutputStream kj_out(oss);
    capnp::writeMessage(kj_out, message);
    std::string bytes = oss.str();

    // deserialize
    std::istringstream iss(bytes);
    kj::std::StdInputStream kj_in(iss);
    capnp::InputStreamMessageReader reader(kj_in);
    auto result = reader.getRoot<PersonList>();
    celero::DoNotOptimizeAway(result.getPeople().size());
}

// ===== FlatBuffers =====
BENCHMARK(Serialization, flatbuffers, 10, 1000) {
    // serialize
    flatbuffers::FlatBufferBuilder builder(4096);
    std::vector<flatbuffers::Offset<fbs::Person>> offsets;
    offsets.reserve(N);
    for (const auto& p : kPeople) {
        offsets.push_back(fbs::CreatePerson(builder, p.id, builder.CreateString(p.name),
                                            builder.CreateString(p.email), p.score));
    }
    auto vec = builder.CreateVector(offsets);
    auto list = fbs::CreatePersonList(builder, vec);
    builder.Finish(list);

    // deserialize (zero-copy)
    const fbs::PersonList* pl = fbs::GetPersonList(builder.GetBufferPointer());
    celero::DoNotOptimizeAway(pl->people()->size());
}
