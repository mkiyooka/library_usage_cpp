#include <iostream>
#include <sstream>

#include <capnp/message.h>
#include <capnp/serialize.h>
#include <kj/std/iostream.h>

#include "person.capnp.h"

int main() {
    std::cout << "=== Cap'n Proto Example ===\n\n";

    // --- シリアライズ ---
    capnp::MallocMessageBuilder message;
    auto person_list = message.initRoot<PersonList>();
    auto people = person_list.initPeople(3);

    people[0].setId(1);
    people[0].setName("Alice");
    people[0].setEmail("alice@example.com");
    people[0].setScore(98.5);

    people[1].setId(2);
    people[1].setName("Bob");
    people[1].setEmail("bob@example.com");
    people[1].setScore(87.0);

    people[2].setId(3);
    people[2].setName("Carol");
    people[2].setEmail("carol@example.com");
    people[2].setScore(92.3);

    // バッファへシリアライズ
    std::ostringstream oss;
    kj::std::StdOutputStream kj_out(oss);
    capnp::writeMessage(kj_out, message);
    std::string bytes = oss.str();
    std::cout << "Serialized size: " << bytes.size() << " bytes\n\n";

    // --- デシリアライズ ---
    std::istringstream iss(bytes);
    kj::std::StdInputStream kj_in(iss);
    capnp::InputStreamMessageReader reader(kj_in);

    auto pl = reader.getRoot<PersonList>();
    std::cout << "Deserialized " << pl.getPeople().size() << " people:\n";
    for (auto p : pl.getPeople()) {
        std::cout << "  [" << p.getId() << "] " << p.getName().cStr() << " <"
                  << p.getEmail().cStr() << "> score=" << p.getScore() << "\n";
    }

    return 0;
}
