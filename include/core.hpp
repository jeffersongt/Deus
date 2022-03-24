#pragma once

namespace deus {
    class core {
    private:
        bool _override = false;
    public:
        core(int ac, char **av);
        ~core() = default;

        static void help(int exit_status);
        void parsing(int ac, char **av);
        void config();
    };
}