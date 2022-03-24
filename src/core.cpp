#include "core.hpp"
#include "config.hpp"
#include "resource.hpp"
#include <string_view>
#include <iostream>

void deus::core::help(int exit_status) {
    std::cout << "USAGE:" << std::endl;
    std::cout << "\tdeus [ACTION]\n" << std::endl;
    std::cout << "ACTIONS:" << std::endl;
    std::cout << "\thelp       print this help section" << std::endl;
    std::cout << "\tconfig     generate a deus config file" << std::endl;
    std::cout << "\tresource   generate resources based on your config file\n" << std::endl;
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "\t--override allow overriding existing files" << std::endl;
    exit(exit_status);
}

void deus::core::parsing(int ac, char **av)
{
    if (ac == 1)
        help(EXIT_FAILURE);
    std::string_view arg = av[1];
    if (arg == "help" || arg == "--help" || arg == "-h")
        help(EXIT_SUCCESS);
    if (!(arg == "config" || arg == "resource"))
        help(EXIT_FAILURE);
    for (int i = 2; i < ac; i++) {
        if (av[i] == std::string_view("--override"))
            m_override = true;
    }
}

deus::core::core(int ac, char **av)
{
    parsing(ac, av);
    std::string_view arg = av[1];
    if (arg == "config") {
        Config config;
        if (config.core() != 0) {
            throw std::runtime_error("error occured");
        }
    }
    else if (arg == "resource") {
        Resource resource;
        if (resource.core() != 0) {
            throw std::runtime_error("error occured");
        }
    }
}