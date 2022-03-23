#include "resource.hpp"
#include "config.hpp"

void help() {
    std::cout << "---" << std::endl;
    std::cout << "Deus help:" << std::endl;
    std::cout << "Argument 1 could be:" << std::endl;
    std::cout << "- help     : print the help section" << std::endl;
    std::cout << "- config   : generate a config file for Deus program" << std::endl;
    std::cout << "- resource : generate a resource based on your config file" << std::endl;
    std::cout << "---" << std::endl;
    return;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Wrong arguments provided" << std::endl;
        EXIT_FAILURE;
    }
    if (std::string(argv[1]) == "help") {
        help();
    }
    if (std::string(argv[1]) == "resource") {
        Resource resource;
        if (resource.core() != 0) {
            EXIT_FAILURE;
        }
    }
    if (std::string(argv[1]) == "config") {
        Config config;
        if (config.core() != 0) {
            EXIT_FAILURE;
        }
    }
    return 0;
}