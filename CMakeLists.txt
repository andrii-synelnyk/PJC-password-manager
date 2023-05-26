cmake_minimum_required(VERSION 3.24)
project(Password_Generator)

set(CMAKE_CXX_STANDARD 20)

add_executable(Password_Generator main.cpp Password.h Password.cpp Encryptor.cpp Encryptor.h PasswordManager.cpp PasswordManager.h)
