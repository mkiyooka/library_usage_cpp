#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "json_loader.hpp"

using Json = nlohmann::json;

void LoadConfig(const std::string &file_path, Config &conf) {
    if (file_path.empty()) {
        return;
    }

    std::ifstream file(file_path);
    if (!file.is_open()) {
        return;
    }

    Json json_config;
    file >> json_config;

    if (json_config.contains("title")) {
        conf.title = json_config["title"];
    }


    // plugin配列の読み取り
    if (json_config.contains("plugin") && json_config["plugin"].is_array()) {
        for (const auto &plugin_json : json_config["plugin"]) {
            PluginConfig plugin;
            plugin.file = plugin_json.value("file", "");
            plugin.number = plugin_json.value("number", 0);
            conf.plugins.push_back(plugin);
        }
    }

    // database設定の読み取り
    if (json_config.contains("database")) {
        const auto &db = json_config["database"];
        conf.database.enabled = db.value("enabled", false);
        conf.database.value = db.value("value", 0);

        if (db.contains("ports") && db["ports"].is_array()) {
            for (const auto &port : db["ports"]) {
                conf.database.ports.push_back(port);
            }
        }
    }

    // servers設定の読み取り
    if (json_config.contains("servers") && json_config["servers"].is_object()) {
        for (const auto &[name, server_json] : json_config["servers"].items()) {
            ServerConfig server;
            server.ip = server_json.value("ip", "");
            server.dc = server_json.value("dc", "");
            conf.servers[name] = server;
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
    LoadConfig("config_example/example.json", conf);
    ShowConfig(conf);
    return 0;
}
