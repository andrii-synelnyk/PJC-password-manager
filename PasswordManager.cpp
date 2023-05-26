#include "PasswordManager.h"
#include <fstream>
#include <iostream>


PasswordManager::PasswordManager(const std::string& filePath)
        : filePath(filePath) {
    loadPasswords();
}

void PasswordManager::addPassword(const Password& password) {
    // Add the new password to the vector
    passwords.push_back(password);

    // Save all passwords to the encrypted file
    savePasswords();
}

void PasswordManager::editPassword(const std::string& name, const Password& newPassword) {
    // TODO: implement
}

void PasswordManager::deletePassword(const std::string& name) {
    // TODO: implement
}

std::vector<Password> PasswordManager::searchPasswords(const std::string& searchParameter) {
    // TODO: implement
    return std::vector<Password>();
}

std::vector<Password> PasswordManager::sortPasswords(const std::string& sortParameter) {
    // TODO: implement
    return std::vector<Password>();
}

void PasswordManager::loadPasswords() {

    if (!encryptor.isFileEmpty(filePath)) {
        std::string passwordForFile = getPasswordInput();

        std::string decryptedData = encryptor.decrypt(filePath, passwordForFile);
        //std::cout << decryptedData << std::endl; // DEBUG

        parseData(decryptedData);
    } else {
        std::cout << "File is empty" << std::endl; // DEBUG
    }
}

void PasswordManager::savePasswords() {
    // Convert all Password objects to strings
    std::string data;
    for (const Password& password : passwords) {
        data += password.to_string() + "\n";
    }

    // Encrypt and write the data to the file
    std::string passwordForFile = getPasswordInput();

    encryptor.encrypt(filePath, passwordForFile, data);
}

void PasswordManager::parseData(const std::string& decryptedData) {
    std::istringstream iss(decryptedData);
    std::string line;

    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string field;
        std::vector<std::string> fields;

        // Split each line into fields using ';' as the delimiter
        while (std::getline(lineStream, field, ';')) {
            fields.push_back(field);
        }

        // Construct a Password object from the fields and add it to the vector
        if (fields.size() >= 3) {
            std::string name = fields[0];
            std::string passwordText = fields[1];
            std::string category = fields[2];
            std::string website = fields.size() > 3 ? fields[3] : "";
            std::string login = fields.size() > 4 ? fields[4] : "";

            Password password(name, passwordText, category, website, login);
            passwords.push_back(password);
        } else {
            // Handle the error: each line must have at least 3 fields
            std::cerr << "Error: Invalid format for password data: " << line << std::endl;
        }
    }
}

std::string PasswordManager::getPasswordInput(){
    std::string passwordForFile;

    if (!encryptor.isFileEmpty(filePath)) {
        while (encryptor.decrypt(filePath, passwordForFile) == "Wrong password. Unable to decrypt.") {
            std::cout << "Please enter a password for encrypted file: ";
            std::getline(std::cin, passwordForFile);
        }
    } else {
        std::cout << "Please enter new password for a file: ";
        std::getline(std::cin, passwordForFile);
    }

    return passwordForFile;
}