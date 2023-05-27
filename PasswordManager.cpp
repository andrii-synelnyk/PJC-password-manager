#include "PasswordManager.h"
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

void PasswordManager::addCategory(const Category& category) {
    categories.push_back(category);

    std::cout << "Added category: " << category.getName() << std::endl;
}

void PasswordManager::loadPasswords() {

    if (!encryptor.isFileEmpty(filePath)) {
        std::string passwordForFile = getPasswordInput();

        // Decrypt data from file
        std::string decryptedData = encryptor.decrypt(filePath, passwordForFile);

        // Convert decrypted data to password objects
        parseData(decryptedData);
        std::cout << "Loaded passwords from file" << std::endl;
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

    // Get password for encryption
    std::string passwordForFile = getPasswordInput();

    // Encrypt and write the data to the file
    encryptor.encrypt(filePath, passwordForFile, data);

    std::cout << "Saved changes" << std::endl;
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
            std::string categoryName = fields[2];

            Category category("");
            bool found = false;
            for (const auto& cat : categories) {
                if (cat.getName() == categoryName) {
                    found = true;
                    category = cat;
                    break;
                }
            }
            if (!found){
                category = Category(categoryName);
                addCategory(category);
            }

            std::string website = fields.size() > 3 ? fields[3] : "";
            std::string login = fields.size() > 4 ? fields[4] : "";

            Password password(name, passwordText, category, website, login);
            passwords.push_back(password);

            category.addPassword(password);
        } else {
            // Handle the error: each line must have at least 3 fields
            std::cerr << "Error: Invalid format for password data: " << line << std::endl;
        }
    }
}

std::string PasswordManager::getPasswordInput(){
    std::string passwordForFile;

    if (!encryptor.isFileEmpty(filePath)) {
        do {
            std::cout << "Please enter a password for encrypted file: ";
            std::getline(std::cin, passwordForFile);
        } while (encryptor.decrypt(filePath, passwordForFile) == "Wrong password. Unable to decrypt.");
    } else {
        std::cout << "Please enter new password for a file: ";
        std::getline(std::cin, passwordForFile);
    }

    return passwordForFile;
}

std::vector<Category> PasswordManager::getCategories(){
    return categories;
}