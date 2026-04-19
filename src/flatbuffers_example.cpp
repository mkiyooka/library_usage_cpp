#include <iostream>
#include <vector>

#include "person_generated.h" // flatc が生成

using namespace fbs;

int main() {
    std::cout << "=== FlatBuffers Example ===\n\n";

    // --- シリアライズ ---
    flatbuffers::FlatBufferBuilder builder(1024);

    // Person を3人作成
    std::vector<flatbuffers::Offset<Person>> people;

    auto name1 = builder.CreateString("Alice");
    auto email1 = builder.CreateString("alice@example.com");
    people.push_back(CreatePerson(builder, 1, name1, email1, 98.5));

    auto name2 = builder.CreateString("Bob");
    auto email2 = builder.CreateString("bob@example.com");
    people.push_back(CreatePerson(builder, 2, name2, email2, 87.0));

    auto name3 = builder.CreateString("Carol");
    auto email3 = builder.CreateString("carol@example.com");
    people.push_back(CreatePerson(builder, 3, name3, email3, 92.3));

    auto people_vec = builder.CreateVector(people);
    auto list = CreatePersonList(builder, people_vec);
    builder.Finish(list);

    std::cout << "Serialized size: " << builder.GetSize() << " bytes\n\n";

    // --- デシリアライズ（ゼロコピー）---
    const uint8_t *buf = builder.GetBufferPointer();
    const PersonList *pl = GetPersonList(buf);

    std::cout << "Deserialized " << pl->people()->size() << " people:\n";
    for (const auto *p : *pl->people()) {
        std::cout << "  [" << p->id() << "] " << p->name()->str() << " <" << p->email()->str() << ">"
                  << "  score=" << p->score() << "\n";
    }

    // バイナリ検証
    flatbuffers::Verifier verifier(buf, builder.GetSize());
    std::cout << "\nVerification: " << (VerifyPersonListBuffer(verifier) ? "OK" : "FAIL") << "\n";

    return 0;
}
