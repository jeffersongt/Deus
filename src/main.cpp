#include "resource.hpp"
#include "config.hpp"

#include <string_view>

void help(int exit_status) {
    std::cout << "USAGE:" << std::endl;
    std::cout << "\tdeus [ARGUMENT]\n" << std::endl;
    std::cout << "ARGUMENT:" << std::endl;
    std::cout << "\thelp     : print this help section" << std::endl;
    std::cout << "\tconfig   : generate a Deus config file" << std::endl;
    std::cout << "\tresource : generate resources based on your config file" << std::endl;
    exit(exit_status);
}

int main(int argc, char** argv)
{
    if (argc != 2)
        help(EXIT_FAILURE);

    if (std::string_view(argv[1]) == "help")
        help(EXIT_SUCCESS);

    else if (std::string_view(argv[1]) == "resource") {
        Resource resource;
        if (resource.core() != 0) {
            return EXIT_FAILURE;
        }
    }
    else if (std::string_view(argv[1]) == "config") {
        Config config;
        if (config.core() != 0) {
            return EXIT_FAILURE;
        }
    }
    else
        help(EXIT_FAILURE);
    return EXIT_SUCCESS;
}