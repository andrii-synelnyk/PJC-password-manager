#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <regex>

#include "Password.h"
#include "Encryptor.h"
#include "Category.h"

/**
 * @class PasswordManager
 * @brief Class to handle managing of passwords during program runtime.
 */
class PasswordManager {
public:

    /**
    * @brief Constructs a new Password Manager.
    * @param filePath The file path to load and save passwords.
    */
    explicit PasswordManager(const std::string& filePath);

    /**
    * @brief Adds a new password to the list of passwords and saves it.
    * @param password The password to add.
    */
    void addPassword(const Password& password);

    /**
    * @brief Edits a password by replacing a specified field with a new value.
    * @param input The input string containing the name of the password to edit, the field to change, and the new value.
    */
    void editPassword(const std::string& parameters);

    /**
    * @brief Deletes a password from the list of passwords and saves the changes.
    * @param name The name of the password to delete.
    * @param fromCategoryDeletion Optional flag indicating whether the deletion is part of a category deletion.
    */
    void deletePassword(const std::string& name, bool fromCategoryDeletion = false);

    /**
    * @brief Deletes a category from the list of categories and saves the changes.
    * @param name The name of the category to delete.
    */
    void deleteCategory(const std::string& name);

    /**
    * @brief Searches for passwords that match a specified pattern.
    * @param pattern The pattern to match.
    */
    void searchPasswords(const std::string& pattern);

    /**
    * @brief Sorts the passwords by specified fields.
    * @param input The input string containing the fields to sort by.
    */
    void sortPasswords(const std::string& input);

    /**
    * @brief Adds a category to the list of categories.
    * @param category The category to add.
    * @param print Optional flag indicating whether to print a success message.
    */
    void addCategory(const Category& category, bool print = true);

    /**
    * @brief Returns the list of categories.
    * @return The list of categories.
    */
    std::vector<Category> getCategories();

    /**
    * @brief Deletes all passwords in a specified category.
    * @param category The category to delete passwords from.
    */
    void deleteCategoryPasswords(const Category& category);

    /**
    * @brief Checks if a password is used in any entry.
    * @param password The password to check.
    */
    void checkIfPasswordUsed(const std::string& password);

private:
    std::string filePath;
    std::vector<Password> passwords;
    Encryptor encryptor;
    std::string correctPassword; // is obtained only during runtime
    std::vector<Category> categories;

    /**
    * @brief Loads passwords from an encrypted file.
    */
    void loadPasswords();

    /**
    * @brief Saves the current list of passwords to an encrypted file.
    */
    void savePasswords();

    /**
    * @brief Parses decrypted data and converts it to password and category objects.
    * @param decryptedData The decrypted data to parse.
    */
    void parseData(const std::string& decryptedData);

    /**
    * @brief Prompts the user for a password and returns it.
    * @return The password entered by the user.
    */
    std::string getPasswordInput();
};