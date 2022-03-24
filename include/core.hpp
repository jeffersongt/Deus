#pragma once

namespace deus {
    class core {
    private:
        bool m_override = false;
    public:
        core(int ac, char **av);
        ~core() = default;

        void parsing(int ac, char **av);
        static void help(int exit_status);
    };
}