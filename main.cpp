#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
// Include other necessary header files here

std::string chooseFile();

int main() {
    // Getting file from user input
    std::string filePath = chooseFile();

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
            // TODO: Implement add password functionality
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