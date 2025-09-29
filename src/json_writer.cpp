#include <iostream>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

int main() {
    // 一括代入のサンプル: 単位別に構築
    Json config;
    config["title"] = "JSON Example";

    // database部分を一括代入
    Json database_section = {
        {  "ports", {8000, 8001}},
        {"enabled",       true},
        {  "value",         42}
    };
    config["database"] = database_section;

    // servers部分を一括代入
    Json servers_section = {
        {"alpha", {{"ip", "10.0.0.1"}, {"dc", "eqdc10"}}},
        { "beta", {{"ip", "10.0.0.2"}, {"dc", "eqdc10"}}}
    };
    config["servers"] = servers_section;

    // plugin部分を一括代入
    Json plugin_section = {
        {{"file", "fileA.json"}, {"number", 10}},
        {{"file", "fileB.json"}, {"number", 15}}
    };
    config["plugin"] = plugin_section;
    std::cout << config.dump(4) << "\n";

    // 更新時のサンプル: 部分的な変更
    Json database_update = {
        {  "ports", {9000, 9001}},
        {"enabled",        false}
        // dobはそのまま保持される（部分更新）
    };
    config["database"].update(database_update);

    std::cout << "\n\nAfter partial update of database:\n";
    std::cout << config.dump(4) << "\n";

    return 0;
}
