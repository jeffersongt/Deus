#include "config.hpp"

Config::Config() {
    _filename = "deus-config.yml";
    _dir = "deus-config";
}

int Config::core() {
    if (createDir() != 0) {
        return -1;
    }
    if (writeFile() != 0) {
        return -1;
    }
    return 0;
}

int Config::createDir() {
    if (!std::filesystem::is_directory(_dir)) {
        std::filesystem::create_directory(_dir);
        if (!std::filesystem::is_directory(_dir)) {
            return -1;
        }
    }
    return 0;
}

int Config::checkFile() {
    std::filesystem::current_path(_dir);
    if (std::filesystem::exists(_filename)) {
        return -1;
    }
    return 0;
}

void Config::resetPath() {
    std::filesystem::current_path("../");
    return;
}

int Config::writeFile() {
    try {
        std::string crud_name = "# Crud name\n# Format: string\ncrud-name: TypeHere\n\n";
        std::string model = "# Database model\n# Format: - variable-name: variable-type\nmodels:\n  - email: string\n  - password: string\n  - username: string\n  - phone: number\n\n";
        std::string foreign_keys = "# Foreign keys: if a variable value belongs to another table\n# Format: - variable-name: foreign-model-name\nforeign-keys:\n  - phone: Phone\n\n";
        std::string optional = "# Http request body Validator: should the field be optional\n# Format: [ variable-name, other-variable-name ]\nbody-validator-optional-values: [\n\tusername,\n\tusername,\n]\n\n";
        std::string controller = "# Routes controller: crud, additional routes\n# Format: crud: yes = true, no = false\n# Format: additionals: - { routes: route-name, http: http-method }\ncontroller:\n  - crud: true\n  - additionals:\n    - { routes: TypeHereAnAdditionalRouteIfNecessary1, http: get }\n    - { routes: TypeHereAnAdditionalRouteIfNecessary2, http: post }\n    - { routes: TypeHereAnAdditionalRouteIfNecessary3, http: patch }\n    - { routes: TypeHereAnAdditionalRouteIfNecessary4, http: delete }\n\n";
        std::string policies = "# Policies\n# Format: yes = true, no = false\npolicies: true\n\n";
        std::string tests = "# Automated tests for the generated routes\n# Format: yes = true, no = false\nautomated-routes-testing: true\n";

        if (checkFile() != 0) {
            throw "Config file already exists";
        }
        std::ofstream file(_filename);
        file << crud_name << model << foreign_keys << optional << controller << policies << tests;
        file.close();
        resetPath();
        return 0;
    } catch(const char* error) {
        std::cerr << error << std::endl;
        return -1;
    }
}