#include "core.hpp"

#include <filesystem>
#include <fstream>

void deus::core::config()
{
    if (!_override && std::filesystem::exists("deus-config.yml"))
        throw std::runtime_error("Config file \"deus-config.yml\" already exists");

    auto crud_name = "# Crud name\n# Format: string\ncrud-name: TypeHere\n\n";
    auto model = "# Database model\n# Format: - variable-name: variable-type\nmodels:\n  - email: string\n  - password: string\n  - username: string\n  - phone: number\n\n";
    auto foreign_keys = "# Foreign keys: if a variable value belongs to another table\n# Format: - variable-name: foreign-model-name\nforeign-keys:\n  - phone: Phone\n\n";
    auto swivel_tables = "# Swivel tables: if you wish to set a swivel table\n# Format: - name-of-the-field-(ex: users): name-of-the-swivel-table-(ex: User)\nswivel-tables:\n  - users: User\n\n";
    auto optional = "# Http request body Validator: should the field be optional\n# Format: [ variable-name, other-variable-name ]\nbody-validator-optional-values: [\n\tusername,\n\tusername,\n]\n\n";
    auto controller = "# Routes controller: crud, additional routes\n# Format: crud: yes = true, no = false\n# Format: additionals: - { routes: route-name, http: http-method }\ncontroller:\n  - crud: true\n  - additionals:\n    - { routes: TypeHereAnAdditionalRouteIfNecessary1, http: get }\n    - { routes: TypeHereAnAdditionalRouteIfNecessary2, http: post }\n    - { routes: TypeHereAnAdditionalRouteIfNecessary3, http: patch }\n    - { routes: TypeHereAnAdditionalRouteIfNecessary4, http: delete }\n\n";
    auto policies = "# Policies\n# Format: yes = true, no = false\npolicies: true\n\n";
    auto tests = "# Automated tests for the generated routes\n# Format: yes = true, no = false\nautomated-routes-testing: true\n";

    std::ofstream file("deus-config.yml");
    file << crud_name << model << foreign_keys << swivel_tables << optional << controller << policies << tests;
}