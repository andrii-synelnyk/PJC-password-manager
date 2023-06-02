#pragma once

#include <vector>
#include <string>
#include "Password.h"

#include "Encryptor.h"

#include "Category.h"

class PasswordManager {
public:
    PasswordManager(const std::string& filePath);

    void addPassword(const Password& password);
    void editPassword(const std::string& parameters);
    void deletePassword(const std::string& name, bool fromCategoryDeletion = false);
    void deleteCategory(const std::string& name);

    void addCategory(const Category& category);
    std::vector<Category> getCategories();
    void deleteCategoryPasswords(const Category& category);

    void checkIfPasswordUsed(const std::string& password);
    void searchPasswords(const std::string& pattern);
    void sortPasswords(const std::string& input);

private:
    std::string filePath;
    std::vector<Password> passwords;
    Encryptor encryptor;

    std::string correctPassword; // is obtained only during runtime

    std::vector<Category> categories;

    void loadPasswords();
    void savePasswords();

    void parseData(const std::string& decryptedData);
    std::string getPasswordInput();
};