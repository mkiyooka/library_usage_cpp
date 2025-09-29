#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>

struct PluginConfig {
    std::string file;
    std::uint64_t number;
};

struct DatabaseConfig {
    std::vector<int> ports;
    bool enabled;
    std::uint64_t value = 10;
};

struct ServerConfig {
    std::string ip;
    std::string dc;
};

struct Config {
    std::string title = "title";
    std::vector<struct PluginConfig> plugins;
    DatabaseConfig database;
    std::map<std::string, ServerConfig> servers;
};

void LoadConfig(const std::string &file_path, Config &conf);
void ShowConfig(const Config &conf);
