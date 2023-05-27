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
    void editPassword(const std::string& name, const Password& newPassword);
    void deletePassword(const std::string& name, bool fromCategoryDeletion = false);
    void deleteCategory(const std::string& name);

    std::vector<Password> searchPasswords(const std::string& searchParameter);
    std::vector<Password> sortPasswords(const std::string& sortParameter);

    void addCategory(const Category& category);
    std::vector<Category> getCategories();
    void deleteCategoryPasswords(Category category);

private:
    std::string filePath;
    std::vector<Password> passwords;
    Encryptor encryptor;

    std::vector<Category> categories;

    void loadPasswords();
    void savePasswords();

    void parseData(const std::string& decryptedData);
    std::string getPasswordInput();
};