#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

struct PluginConfig {
    std::string file;
    int number = 0;
};

struct DatabaseConfig {
    bool enabled = false;
    int value = 0;
    std::vector<int> ports;
};

struct ServerConfig {
    std::string ip;
    std::string dc;
};

struct Config {
    std::string title;
    std::vector<PluginConfig> plugins;
    DatabaseConfig database;
    std::map<std::string, ServerConfig> servers;
};

static Config LoadConfig(const std::string &file_path) {
    Config conf;
    YAML::Node root = YAML::LoadFile(file_path);

    conf.title = root["title"].as<std::string>("");

    if (root["plugin"]) {
        for (const auto &node : root["plugin"]) {
            PluginConfig plugin;
            plugin.file = node["file"].as<std::string>("");
            plugin.number = node["number"].as<int>(0);
            conf.plugins.push_back(plugin);
        }
    }

    if (root["database"]) {
        const auto &db = root["database"];
        conf.database.enabled = db["enabled"].as<bool>(false);
        conf.database.value = db["value"].as<int>(0);
        if (db["ports"]) {
            for (const auto &port : db["ports"]) {
                conf.database.ports.push_back(port.as<int>());
            }
        }
    }

    if (root["servers"]) {
        for (const auto &entry : root["servers"]) {
            ServerConfig server;
            server.ip = entry.second["ip"].as<std::string>("");
            server.dc = entry.second["dc"].as<std::string>("");
            conf.servers[entry.first.as<std::string>()] = server;
        }
    }

    return conf;
}

static void ShowConfig(const Config &conf) {
    std::cout << "title: " << conf.title << '\n';

    for (const auto &p : conf.plugins) {
        std::cout << "  plugin: file=" << p.file << ", number=" << p.number << '\n';
    }

    std::cout << "database: enabled=" << (conf.database.enabled ? "true" : "false") << ", value=" << conf.database.value
              << '\n';
    std::cout << "  ports: [";
    for (size_t i = 0; i < conf.database.ports.size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << conf.database.ports[i];
    }
    std::cout << "]\n";

    for (const auto &[name, server] : conf.servers) {
        std::cout << "  server " << name << ": ip=" << server.ip << ", dc=" << server.dc << '\n';
    }
}

static void EmitExample() {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "name" << YAML::Value << "yaml-cpp example";
    out << YAML::Key << "version" << YAML::Value << 1;
    out << YAML::Key << "tags" << YAML::Value << YAML::BeginSeq << "yaml" << "cpp" << "config" << YAML::EndSeq;
    out << YAML::EndMap;

    std::cout << "\n--- Emitter output ---\n" << out.c_str() << '\n';
}

int main() {
    const Config conf = LoadConfig("config_example/example.yaml");
    ShowConfig(conf);
    EmitExample();
    return 0;
}
