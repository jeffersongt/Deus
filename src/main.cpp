#include "core.hpp"
#include <iostream>

int main(int ac, char** av)
{
    try {
        deus::core(ac, av);
        return EXIT_SUCCESS;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}