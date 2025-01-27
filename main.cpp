#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "PasswordManager.h"

std::string chooseFile();
void userAddPassword(PasswordManager& manager);
std::string generatePassword(int length, bool includeUppercase, bool includeSpecialChars);
Category userAddCategory(PasswordManager& manager);
void userDeletePassword(PasswordManager& manager);
void userDeleteCategory(PasswordManager& manager);
void evaluatePasswordStrength(const std::string& password);
void userSearch(PasswordManager& manager);
void userSort(PasswordManager& manager);
void userEditPassword(PasswordManager& manager);

int main() {
    // Getting file from user input
    std::string filePath = chooseFile();
    PasswordManager passwordManager(filePath);

    // Proceeding with program
    std::string command;
    std::cout << "Welcome to the Password Manager. Type 'help' for a list of commands." << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "quit") {
            break;
        } else if (command == "help") {
            std::cout << "Available commands:" << std::endl;
            std::cout << "quit - Quit the program" << std::endl;
            std::cout << "help - Display this help message" << std::endl;
            std::cout << "search - Search for passwords" << std::endl;
            std::cout << "sort - Sort passwords" << std::endl;
            std::cout << "add - Add a new password" << std::endl;
            std::cout << "edit - Edit an existing password" << std::endl;
            std::cout << "delete - Delete a password" << std::endl;
            std::cout << "add_category - Add a new category" << std::endl;
            std::cout << "delete_category - Delete a category" << std::endl;
        } else if (command == "search") {
            userSearch(passwordManager);
        } else if (command == "sort") {
            userSort(passwordManager);
        } else if (command == "add") {
            userAddPassword(passwordManager);
        } else if (command == "edit") {
            userEditPassword(passwordManager);
        } else if (command == "delete") {
            userDeletePassword(passwordManager);
        } else if (command == "add_category") {
            userAddCategory(passwordManager);
        } else if (command == "delete_category") {
            userDeleteCategory(passwordManager);
        } else {
            std::cout << "[ERROR] Unknown command. Type 'help' for a list of commands." << std::endl;
        }
    }

    return 0;
}

/**
 * @brief Prompts the user to select a file
 * @details This function lists the files in the current directory and prompts the user to select a file by entering its number or a direct path.
 * @return The path of the selected file
 */
std::string chooseFile() {
    std::vector<std::filesystem::path> files;
    int fileIndex = 1;

    // List all files in the current directory
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path projectPath = currentPath.parent_path();
    std::filesystem::path filesDirectory = projectPath / "Encrypted files/";

    while (true) {
        for (const auto& entry : std::filesystem::directory_iterator(filesDirectory)) {
            if (entry.is_regular_file()) {
                std::cout << fileIndex << ": " << entry.path().filename() << std::endl;
                files.push_back(entry.path());
                fileIndex++;
            }
        }

        std::string input;
        std::cout << "Please select a file by entering its number, or enter a direct path: ";
        std::getline(std::cin, input);

        std::filesystem::path selectedFilePath;

        try {
            // If the user entered a number, use it as an index into the list of files
            fileIndex = std::stoi(input) - 1;
            if (fileIndex < 0 || fileIndex >= files.size()) {
                std::cout << "[ERROR] Invalid file number." << std::endl;
                files.clear();
                fileIndex = 1;
                continue; // Start the loop again to let the user choose a valid file
            }
            selectedFilePath = files[fileIndex];
        } catch (std::invalid_argument& e) {
            // If the user didn't enter a number, treat the input as a file path
            selectedFilePath = input;
        }

        // Check if the file exists
        if (!std::filesystem::exists(selectedFilePath)) {
            std::cout << "[ERROR] File does not exist: " << selectedFilePath << std::endl;
            files.clear();
            fileIndex = 1;
            continue; // Start the loop again to let the user choose a valid file
        }

        std::cout << "Selected file: " << selectedFilePath.filename() << std::endl;

        return selectedFilePath;
    }
}

/**
 * @brief Facilitates the addition of a new password by the user
 * @details This function prompts the user to enter necessary details for a password. If no password is provided, it generates one.
 * @param manager The PasswordManager instance for managing passwords
 */
void userAddPassword(PasswordManager& manager) {
    std::string name, passwordText, website, login;
    Category category("");

    std::cout << "Enter the name for this password entry: ";
    std::getline(std::cin, name); // get line for reading spaces

    std::cout << "Enter the password or press enter to generate a password: ";
    std::getline(std::cin, passwordText);

    // If the user didn't enter a password, generate one for them
    if (passwordText.empty()) {
        int length;
        char includeUppercase, includeSpecialChars;
        std::cout << "Enter the number of characters for the password: ";
        std::cin >> length;
        std::cin.ignore(1, '\n'); // clear buffer from newline character (because cin leaves newline) for any getLine that might be encountered further in program

        std::cout << "Include uppercase letters? (y/n): ";
        std::cin >> includeUppercase;
        std::cin.ignore(1, '\n');

        std::cout << "Include special characters? (y/n): ";
        std::cin >> includeSpecialChars;
        std::cin.ignore(1, '\n');

        passwordText = generatePassword(length, includeUppercase == 'y', includeSpecialChars == 'y');
    } else {
        evaluatePasswordStrength(passwordText);
        manager.checkIfPasswordUsed(passwordText);
    }

    // Inputting category
    if (manager.getCategories().empty()) {
        std::cout << "There are no categories yet. ";
        category = userAddCategory(manager);
    } else {
        while (true){
            int categoryIndex = 1;
            std::vector<Category> categories;

            for (const Category& cat : manager.getCategories()) {
                std::cout << categoryIndex << ": " << cat.getName() << std::endl;
                categories.push_back(cat);
                categoryIndex++;
            }

            std::string input;
            std::cout << "Please select an existing category from the list above, or enter the name for the new category: ";
            std::getline(std::cin, input);

            try {
                // If the user entered a number, use it as an index into the list of categories
                categoryIndex = std::stoi(input) - 1;
                if (categoryIndex < 0 || categoryIndex >= categories.size()) {
                    std::cout << "[ERROR] Invalid category number." << std::endl;
                    categories.clear();
                    continue; // Start the loop again to let the user choose a valid category
                }
                category = categories[categoryIndex]; // creating copy not actual category

                std::cout << "Selected category: " << category.getName() << std::endl;
            } catch (std::invalid_argument& e) {
                // If the user didn't enter a number, treat the input as a category name
                if (!input.empty()) {
                    category = Category(input);
                    manager.addCategory(category);
                } else continue;
            }

            break;
        }

    }

    std::cout << "Enter the website for this password (optional): ";
    std::getline(std::cin, website);

    std::cout << "Enter the login for this password (optional): ";
    std::getline(std::cin, login);

    Password password(name, passwordText, category, website, login);
    manager.addPassword(password);

    category.addPassword(password);
}

/**
 * @brief Generates a password according to user specifications
 * @details This function generates a password of a given length and can include uppercase letters and special characters based on user input.
 * @param length Length of the password to generate
 * @param includeUppercase Determines whether the password should include uppercase letters
 * @param includeSpecialChars Determines whether the password should include special characters
 * @return The generated password
 */
std::string generatePassword(int length, bool includeUppercase, bool includeSpecialChars) {
    std::string lowercaseLetters = "abcdefghijklmnopqrstuvwxyz";
    std::string uppercaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string specialChars = "!@#$%^&*()";
    std::string digits = "0123456789";

    std::string charset = lowercaseLetters + digits;
    if (includeUppercase) {
        charset += uppercaseLetters;
    }
    if (includeSpecialChars) {
        charset += specialChars;
    }

    // Initialize the random number generator with the current time (otherwise each time program is run the same numbers will be generated)
    std::srand(std::time(nullptr));  // Seed the random number generator with the current time

    std::string password;
    for (int i = 0; i < length; i++) {
        password += charset[std::rand() % charset.size()];
    }

    std::cout << "Generated password: " << password << std::endl;

    return password;
}

/**
 * @brief Evaluates the strength of a password
 * @details This function evaluates the strength of a given password based on its length and the types of characters it includes.
 * @param password The password to evaluate
 */
void evaluatePasswordStrength(const std::string& password) {
    int length = password.size();
    bool hasLowercase = false, hasUppercase = false, hasDigit = false, hasSpecialChar = false;

    for (char c : password) {
        if (std::islower(c))
            hasLowercase = true;
        else if (std::isupper(c))
            hasUppercase = true;
        else if (std::isdigit(c))
            hasDigit = true;
        else
            hasSpecialChar = true;
    }

    std::string strength;

    if (length >= 8 && hasLowercase && hasUppercase && hasDigit && hasSpecialChar)
        strength = "very strong";
    else if (length >= 8 && hasLowercase && hasUppercase && hasDigit)
        strength = "strong";
    else if (length >= 8 && (hasLowercase || hasUppercase) && hasDigit)
        strength = "medium";
    else if (length >= 8)
        strength = "weak";
    else
        strength = "very weak";

    std::cout << "The password strength is " << strength << std::endl;
}

/**
 * @brief Facilitates the addition of a new category by the user
 * @details This function prompts the user to enter a name for a new category and adds it to the PasswordManager.
 * @param manager The PasswordManager instance for managing categories
 * @return The created Category object
 */
Category userAddCategory(PasswordManager& manager){
    std::string name;

    while (true) {
        std::cout << "Enter the name for new category: ";
        std::getline(std::cin, name);

        if (!name.empty()) {
            Category category(name);
            manager.addCategory(category);

            return category;
        } else continue;
    }
}

/**
 * @brief Facilitates the deletion of a password by the user
 * @details This function prompts the user to enter the name of a password and deletes it from the PasswordManager.
 * @param manager The PasswordManager instance for managing passwords
 */
void userDeletePassword(PasswordManager& manager){
    std::string name;

    std::cout << "Enter the name of the password you want to delete: ";
    std::getline(std::cin, name);

    manager.deletePassword(name);
}

/**
 * @brief Facilitates the deletion of a category by the user
 * @details This function prompts the user to enter the name of a category and deletes it from the PasswordManager.
 * @param manager The PasswordManager instance for managing categories
 */
void userDeleteCategory(PasswordManager& manager){
    std::string name;

    std::cout << "Enter the name of the category you want to delete: ";
    std::getline(std::cin, name);

    manager.deleteCategory(name);
}

/**
 * @brief Facilitates the searching of passwords by the user
 * @details This function prompts the user to enter a search pattern and displays the matching passwords.
 * @param manager The PasswordManager instance for managing passwords
 */
void userSearch(PasswordManager& manager){
    std::string pattern;

    std::cout << "Enter pattern which you want to search in passwords (default: press Enter to show all passwords): ";
    std::getline(std::cin, pattern);

    manager.searchPasswords(pattern);
}


/**
 * @brief Facilitates the sorting of passwords by the user
 * @details This function prompts the user to enter sorting parameters and sorts the passwords accordingly.
 * @param manager The PasswordManager instance for managing passwords
 */
void userSort(PasswordManager& manager){
    std::string parameters;

    std::cout << "Enter parameters for sorting separated by spaces (default: press Enter to sort by name): ";
    std::getline(std::cin, parameters);

    manager.sortPasswords(parameters);
}

/**
 * @brief Facilitates the editing of a password by the user
 * @details This function prompts the user to enter parameters for editing a password and applies the changes.
 * @param manager The PasswordManager instance for managing passwords
 */
void userEditPassword(PasswordManager& manager){
    std::string parameters;

    std::cout << "Enter parameters for editing password in format <name> <field name> <new value>: ";
    std::getline(std::cin, parameters);

    manager.editPassword(parameters);
}