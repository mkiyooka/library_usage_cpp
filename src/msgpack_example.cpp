#include <iostream>
#include <string>
#include <vector>

#include <msgpack.hpp>

struct Person {
    uint32_t    id;
    std::string name;
    std::string email;
    double      score;
    MSGPACK_DEFINE(id, name, email, score)
};

struct PersonList {
    std::vector<Person> people;
    MSGPACK_DEFINE(people)
};

int main() {
    std::cout << "=== msgpack-c Example ===\n\n";

    // --- シリアライズ ---
    PersonList list;
    list.people = {
        {1, "Alice", "alice@example.com", 98.5},
        {2, "Bob",   "bob@example.com",   87.0},
        {3, "Carol", "carol@example.com", 92.3},
    };

    msgpack::sbuffer buf;
    msgpack::pack(buf, list);
    std::cout << "Serialized size: " << buf.size() << " bytes\n\n";

    // --- デシリアライズ ---
    auto obj = msgpack::unpack(buf.data(), buf.size());
    PersonList restored;
    obj.get().convert(restored);

    std::cout << "Deserialized " << restored.people.size() << " people:\n";
    for (const auto& p : restored.people) {
        std::cout << "  [" << p.id << "] "
                  << p.name << " <" << p.email << ">"
                  << "  score=" << p.score << "\n";
    }

    return 0;
}
