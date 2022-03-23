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
        Resource();
        int core();

        YAML::Node openFile();
        int readFile(YAML::Node);

        int checkDirectory(std::string);
        int checkFiles(std::string, std::string);
        void resetPath();

        int writeController();
        int writeModel();
        int writePolicy();
        int writeValidator();
        int writeRoutes();

        void setCrudName(std::string);
        void setTests(bool);
        void setPolicies(bool);
    private:
        std::vector<std::string> dir_to_check;
        std::string _crud_name;
        std::map<std::string, std::string> _column;
        std::map<std::string, std::string> foreign_keys;
        std::vector<std::string> validator_optional;
        bool route_crud;
        std::map<std::string, std::string> additional_routes;
        bool _policies;
        bool _tests;
};