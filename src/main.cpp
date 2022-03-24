#include "resource.hpp"
#include "config.hpp"

#include <string_view>

void help(int exit_status) {
    std::cout << "USAGE:" << std::endl;
    std::cout << "\tdeus [ARGUMENT]\n" << std::endl;
    std::cout << "ARGUMENT:" << std::endl;
    std::cout << "\thelp       print this help section" << std::endl;
    std::cout << "\tconfig     generate a deus config file" << std::endl;
    std::cout << "\tresource   generate resources based on your config file" << std::endl;
    exit(exit_status);
}

int main(int ac, char** av)
{
    std::string_view arg = av[1];
    if (ac != 2)
        help(EXIT_FAILURE);

    if (arg == "help" or arg == "--help" or arg == "-h")
        help(EXIT_SUCCESS);

    else if (arg == "resource") {
        Resource resource;
        if (resource.core() != 0) {
            return EXIT_FAILURE;
        }
    }
    else if (arg == "config") {
        Config config;
        if (config.core() != 0) {
            return EXIT_FAILURE;
        }
    }
    else
        help(EXIT_FAILURE);
    return EXIT_SUCCESS;
}