#!/usr/bin/env python3
import os
import platform

os.system('cmake . -B build')
os.system('cmake --build build')

actual_os = platform.system()

if (actual_os == "Darwin"):
  os.system('echo $PATH')

if (actual_os == "Linux"):
  os.system('echo $PATH')

if (actual_os == "Windows"):
  os.system('path')
