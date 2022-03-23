#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

class Config {
    public:
      Config();
      int core();

      int createDir();
      int checkFile();
      int writeFile();

      void resetPath();
    private:
      std::string _filename;
      std::string _dir;
};