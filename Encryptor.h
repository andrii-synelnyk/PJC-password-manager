#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

class Encryptor {
public:
    // Constructor
    Encryptor();

    void encrypt(const std::string& filePath, const std::string& password, const std::string& data);
    std::string decrypt(const std::string& filePath, const std::string& password);
    bool isFileEmpty(const std::string& filePath);
    void clearFile(const std::string& filePath);

private:
    int generateShift(const std::string& password);
    std::string computeChecksum(const std::string& data);
};