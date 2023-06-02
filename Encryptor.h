#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <random>

/**
 * @class Encryptor
 * @brief Class to handle encryption and decryption of data.
 */
class Encryptor {
public:

    /**
    * @brief Encrypts data and writes it to a file.
    * @param filePath The path of the file to write the encrypted data to.
    * @param password The password to use for encryption.
    * @param data The data to be encrypted.
    */
    void encrypt(const std::string& filePath, const std::string& password, const std::string& data);

    /**
    * @brief Decrypts data from a file.
    * @param filePath The path of the file to decrypt the data from.
    * @param password The password to use for decryption.
    * @return The decrypted data if the password is correct, otherwise an error message.
    */
    std::string decrypt(const std::string& filePath, const std::string& password);

    /**
    * @brief Checks if a file is empty.
    * @param filePath The path of the file to check.
    * @return True if the file is empty, false otherwise.
    */
    bool isFileEmpty(const std::string& filePath);

    /**
    * @brief Clears the contents of a file.
    * @param filePath The path of the file to clear.
    */
    void clearFile(const std::string& filePath);

private:
    /**
    * @brief Generates a shift value based on a given password.
    * @param password The password to generate the shift from.
    * @return The generated shift value.
    */
    int generateShift(const std::string& password);

    /**
    * @brief Computes a checksum of a given data string.
    * @param data The string to compute the checksum from.
    * @return The computed checksum as a string.
    */
    std::string computeChecksum(const std::string& data);
};