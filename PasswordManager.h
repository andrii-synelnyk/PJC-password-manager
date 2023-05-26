#pragma once

#include <vector>
#include <string>
#include "Password.h"

#include "Encryptor.h"

class PasswordManager {
public:
    PasswordManager(const std::string& filePath);

    void addPassword(const Password& password);
    void editPassword(const std::string& name, const Password& newPassword);
    void deletePassword(const std::string& name);

    std::vector<Password> searchPasswords(const std::string& searchParameter);
    std::vector<Password> sortPasswords(const std::string& sortParameter);

private:
    std::string filePath;
    std::vector<Password> passwords;
    Encryptor encryptor;

    void loadPasswords();
    void savePasswords();

    void parseData(const std::string& decryptedData);
    std::string getPasswordInput();
};