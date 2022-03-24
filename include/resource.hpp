#pragma once

#include <iostream>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <map>
#include <utility>

class Resource {
    public:
        Resource(bool override_allowed) : _override(override_allowed) {}
        ~Resource() = default;

        static YAML::Node get_yaml(const std::string &path);

        void generate();
        void readFile(YAML::Node &);

        void check_directories();
        void checkFiles(std::filesystem::path, std::filesystem::path);

        void writeController();
        void writeModel();
        void writePolicy();
        void writeValidator();
        void writeRoutes();
        void writeSeeder();

        void setCrudName(std::string);
        void setTests(bool);
        void setPolicies(bool);
    private:
        bool _override;
        std::string _crud_name;
        std::string _crud_lower;
        std::map<std::string, std::string> _column;
        std::map<std::string, std::string> foreign_keys;
        std::vector<std::string> validator_optional;
        bool route_crud;
        std::map<std::string, std::string> additional_routes;
        bool _policies;
        bool _tests;
};