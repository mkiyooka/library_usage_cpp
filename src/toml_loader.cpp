#include <iostream>
#include <string>

#include <toml++/toml.hpp>

#include "toml_loader.hpp"

void LoadConfig(const std::string &file_path, Config &conf) {
    if (file_path.empty()) {
        return;
    }
    auto toml_config = toml::parse_file(file_path);
    conf.title = toml_config["title"].value_or("");

    // plugin配列の読み取り
    if (auto *arr = toml_config["plugin"].as_array()) {
        for (auto &&el : *arr) {
            if (auto *table = el.as_table()) {
                PluginConfig plugin;
                plugin.file = (*table)["file"].value_or("");
                plugin.number = (*table)["number"].value_or(0);
                conf.plugins.push_back(plugin);
            }
        }
    }

    // database設定の読み取り
    if (auto *db_table = toml_config["database"].as_table()) {
        conf.database.enabled = (*db_table)["enabled"].value_or(false);
        conf.database.value = (*db_table)["value"].value_or(0);

        if (auto *ports_arr = (*db_table)["ports"].as_array()) {
            for (auto &&port : *ports_arr) {
                conf.database.ports.push_back(port.value_or(0));
            }
        }
    }

    // servers設定の読み取り
    if (auto *servers_table = toml_config["servers"].as_table()) {
        for (auto &&[key, value] : *servers_table) {
            if (auto *server_table = value.as_table()) {
                ServerConfig server;
                server.ip = (*server_table)["ip"].value_or("");
                server.dc = (*server_table)["dc"].value_or("");
                conf.servers[std::string(key.str())] = server;
            }
        }
    }
}

void ShowConfig(const Config &conf) {
    if (!conf.title.empty() && conf.title != "title") {
        std::cout << "title: " << conf.title << '\n';

        for (const auto &p : conf.plugins) {
            std::cout << "  plugin: file=" << p.file << ", number=" << p.number << '\n';
        }

        // database情報の出力
        std::cout << "database: enabled=" << (conf.database.enabled ? "true" : "false")
                  << ", value=" << conf.database.value << '\n';
        std::cout << "  ports: [";
        for (size_t i = 0; i < conf.database.ports.size(); ++i) {
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << conf.database.ports[i];
        }
        std::cout << "]\n";

        // servers情報の出力
        for (const auto &[name, server] : conf.servers) {
            std::cout << "  server " << name << ": ip=" << server.ip << ", dc=" << server.dc << '\n';
        }
    }
}

int main() {
    Config conf;
    LoadConfig("config_example/example.toml", conf);
    ShowConfig(conf);
    return 0;
}
