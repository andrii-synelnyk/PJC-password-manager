#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

class Encryptor {
public:
    // Constructor
    Encryptor();

    // Getters
    void shiftFile(const std::string& filePath, const std::string& password, bool decrypt);

private:
    int generateShift(const std::string& password);
    int computeChecksum(const std::string& data);
    int getCheckSumFromFile(const std::string& data);
    bool isValidDecryption(const std::string& data, int storedChecksum);
};