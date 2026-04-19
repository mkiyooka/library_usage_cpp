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
