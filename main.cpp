#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "PasswordManager.h"

std::string chooseFile();
void userAddPassword(PasswordManager& manager);
std::string generatePassword(int length, bool includeUppercase, bool includeSpecialChars);

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
            // TODO: Implement password search functionality
        } else if (command == "sort") {
            // TODO: Implement password sorting functionality
        } else if (command == "add") {
            userAddPassword(passwordManager);
        } else if (command == "edit") {
            // TODO: Implement edit password functionality
        } else if (command == "delete") {
            // TODO: Implement delete password functionality
        } else if (command == "add_category") {
            // TODO: Implement add category functionality
        } else if (command == "delete_category") {
            // TODO: Implement delete category functionality
        } else {
            std::cout << "Unknown command. Type 'help' for a list of commands." << std::endl;
        }
    }

    return 0;
}

std::string chooseFile() {
    std::vector<std::filesystem::path> files;
    int fileIndex = 1;

    // List all files in the current directory
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path projectPath = currentPath.parent_path();
    std::filesystem::path filesDirectory = projectPath / "Source files/";

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
                std::cout << "Invalid file number." << std::endl;
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
            std::cout << "File does not exist: " << selectedFilePath << std::endl;
            files.clear();
            fileIndex = 1;
            continue; // Start the loop again to let the user choose a valid file
        }

        std::cout << "Selected file: " << selectedFilePath.filename() << std::endl;

        return selectedFilePath;
    }
}

void userAddPassword(PasswordManager& manager) {
    std::string name, passwordText, category, website, login;

    std::cout << "Enter the name for this password entry: ";
    std::getline(std::cin, name);

    std::cout << "Enter the password or press enter to generate a password: ";
    std::getline(std::cin, passwordText);

    // If the user didn't enter a password, generate one for them
    if (passwordText.empty()) {
        int length;
        char includeUppercase, includeSpecialChars;
        std::cout << "Enter the number of characters for the password: ";
        std::cin >> length;

        std::cout << "Include uppercase letters? (y/n): ";
        std::cin >> includeUppercase;

        std::cout << "Include special characters? (y/n): ";
        std::cin >> includeSpecialChars;

        passwordText = generatePassword(length, includeUppercase == 'y', includeSpecialChars == 'y');
    }

    std::cout << "Enter the category for this password: ";
    std::getline(std::cin, category);

    std::cout << "Enter the website for this password (optional): ";
    std::getline(std::cin, website);

    std::cout << "Enter the login for this password (optional): ";
    std::getline(std::cin, login);

    Password password(name, passwordText, category, website, login);
    manager.addPassword(password);
}

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