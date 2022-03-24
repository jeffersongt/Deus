#!/usr/bin/env python3

import sys
import os
import platform

if os.system('cmake . -B build') != 0:
  print("Failed to generate build file, please make sure that CMake is installed", file=sys.stderr)
  exit(1)

if os.system('cmake --build build') != 0:
  print("Build failed", file=sys.stderr)
  exit(1)

actual_os = platform.system()
bin = ""
install_path = ""

if actual_os == "Darwin":
  bin = "deus"
  install_path = "/usr/local/bin"
elif actual_os == "Linux":
  bin = "deus"
  install_path = "/home/" + os.getenv("USERNAME") + "/.local/bin"
elif actual_os == "Windows":
  bin = "Debug/deus"
  install_path = "$env:UserProfile/AppData/Local/Microsoft/WindowsApps"
else:
  print("Unknown system, you will need to install Deus manually", file=sys.stderr)
  exit(1)

if os.system("cp " + bin + " " + install_path) != 0:
  print("Failed to install Deus at \"" + install_path + "\"", file=sys.stderr)
  exit(1)

print("Deus has been installed at \"" + install_path + "\"")