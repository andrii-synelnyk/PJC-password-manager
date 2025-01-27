#include "PasswordManager.h"

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

void PasswordManager::editPassword(const std::string& input) {
    // Read and separate user input
    std::istringstream lineStream(input);
    std::string parameter;
    std::vector<std::string> parameters;

    // Split each line into fields using ' ' as the delimiter
    while (std::getline(lineStream, parameter, ' ')) {
        parameters.push_back(parameter);
    }

    // Check if user provided exactly 3 parameters
    if (parameters.size() != 3) {
        std::cout << "[ERROR] Wrong number of parameters. Expected 3." << std::endl;
        return;
    }

    std::string name = parameters[0];
    std::string field = parameters[1];
    std::string newValue = parameters[2];
    bool foundPassword = false;

    for (Password& pass: passwords) {
        if (pass.getName() == name) {
            if (field == "name") {
                pass.setName(newValue);
            } else if (field == "passwordText") {
                pass.setPasswordText(newValue);
            } else if (field == "category") {
                // Create category object, if category with such name doesn't exist
                Category category("");
                bool foundCategory = false;
                for (const auto& cat : categories) {
                    if (cat.getName() == newValue) {
                        foundCategory = true;
                        category = cat;
                        break;
                    }
                }
                if (!foundCategory){
                    category = Category(newValue);
                    addCategory(category);
                }

                pass.setCategory(category);
                category.addPassword(pass);
            } else if (field == "website") {
                pass.setWebsite(newValue);
            } else if (field == "login") {
                pass.setLogin(newValue);
            }
            foundPassword = true;
        }
    }
    if(!foundPassword) std::cout << "[ERROR] There are no passwords with such name." << std::endl;

    savePasswords();
}

void PasswordManager::deletePassword(const std::string& name, bool fromCategoryDeletion) {
    auto it = std::find_if(passwords.begin(), passwords.end(), [&name](const Password& password) {
        return password.getName() == name;
    });

    if (it != passwords.end()) {
        Category& category = (*it).getCategoryCanModify();
        category.removePassword((*it).getName());

        passwords.erase(it);
        if (!fromCategoryDeletion) savePasswords();
    } else {
        std::cout << "[ERROR] No password with the name: " << name << std::endl;
    }
}

void PasswordManager::deleteCategory(const std::string& name){
    auto it = std::find_if(categories.begin(), categories.end(), [&name](const Category& category) {
        return category.getName() == name;
    });

    if (it != categories.end()) {
        deleteCategoryPasswords(*it);
        categories.erase(it);
        savePasswords();
    } else {
        std::cout << "[ERROR] No category with the name: " << name << std::endl;
    }
}

void PasswordManager::searchPasswords(const std::string& pattern) {
    std::regex e(".*" + pattern + ".*");  // a regex that will match any string containing the input pattern
    bool found = false;

    for (const auto& pas : passwords){
        if (std::regex_match(pas.getName(), e) ||
            std::regex_match(pas.getPasswordText(), e) ||
            std::regex_match(pas.getWebsite(), e) ||
            std::regex_match(pas.getLogin(), e) ||
            std::regex_match(pas.getCategoryCannotModify().getName(), e)) {
            std::cout << pas.to_string() << std::endl;
            found = true;
        }
    }

    if (!found) std::cout << "[ERROR] There are no elements in passwords matching provided pattern." << std::endl;
}

void PasswordManager::sortPasswords(const std::string& input) {
    // Read and separate user input
    std::istringstream lineStream(input);
    std::string parameter;
    std::vector<std::string> parameters;

    // Split each line into fields using ' ' as the delimiter
    while (std::getline(lineStream, parameter, ' ')) {
        parameters.push_back(parameter);
    }

    // Initialize flags based on user input
    bool name, password, category, website, login;

    for (const std::string& par : parameters){
        if (par == "name") name = true;
        else if (par == "password") password = true;
        else if (par == "category") category = true;
        else if (par == "website") website = true;
        else if (par == "login") login = true;
    }

    // Make a copy of passwords to sort them
    std::vector<Password> copiedPasswords = passwords;

    // Sort passwords
    std::sort(copiedPasswords.begin(), copiedPasswords.end(),
              [&name, &password, &category, &website, &login](const Password& a, const Password& b) {

        if (name) {
            if (a.getName() != b.getName()) {
                return a.getName() < b.getName();
            }
        }

        if (password){
            if (a.getPasswordText().size() != b.getPasswordText().size()) {
                return a.getPasswordText().size() < b.getPasswordText().size();
            } else if(a.getPasswordText() != b.getPasswordText()){
                return a.getPasswordText() < b.getPasswordText();
            }
        }

        if (category){
            if (a.getCategoryCannotModify().getName() != b.getCategoryCannotModify().getName()) {
                return a.getCategoryCannotModify().getName() < b.getCategoryCannotModify().getName();
            }
        }

        if (website){
            if (a.getWebsite() != b.getWebsite()) {
                return a.getWebsite() < b.getWebsite();
            }
        }

        if (login){
            if (a.getLogin() != b.getLogin()) {
                return a.getLogin() < b.getLogin();
            }
        }

        // Fallback to name if all fields are the same
        return a.getName() < b.getName();
    });

    // Print the sorted passwords
    for (const auto& pas : copiedPasswords) {
        std::cout << pas.to_string() << std::endl;
    }
}

void PasswordManager::addCategory(const Category& category, bool print) {
    categories.push_back(category);

    if (print) std::cout << "Added category: " << category.getName() << std::endl;
}

void PasswordManager::loadPasswords() {

    if (!encryptor.isFileEmpty(filePath)) {
        correctPassword = getPasswordInput();

        // Decrypt data from file
        std::string decryptedData = encryptor.decrypt(filePath, correctPassword);

        // Convert decrypted data to password objects
        parseData(decryptedData);
        std::cout << "Loaded passwords from file" << std::endl;
    } else {
        std::cout << "File is empty" << std::endl;
        correctPassword = getPasswordInput();
    }
}

void PasswordManager::savePasswords() {

    if (passwords.empty()) {
        encryptor.clearFile(filePath);
        std::cout << "No passwords left in the file. File is empty now." << std::endl;
        return;
    }

    // Convert all Password objects to strings
    std::string data;
    for (const Password& password : passwords) {
        data += password.to_string() + '\n';
    }

    // Encrypt and write the data to the file
    encryptor.encrypt(filePath, correctPassword, data);

    std::cout << "Saved changes" << std::endl;
}

void PasswordManager::parseData(const std::string& decryptedData) {
    std::istringstream iss(decryptedData);
    std::string line;

    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string field;
        std::vector<std::string> fields;

        // Split each line into fields using ' ' as the delimiter
        while (std::getline(lineStream, field, ' ')) {
            fields.push_back(field);
        }

        // Construct a Password object from the fields and add it to the vector
        if (fields.size() >= 3) {
            std::string name = fields[0];
            std::string passwordText = fields[1];
            std::string categoryName = fields[2];

            // Check if category with such name already exists, if it doesn't create new one
            // Not to create two separate category objects from decrypted passwords text which have the same category
            Category category("");
            bool found = false;
            for (const auto& cat : categories) {
                if (cat.getName() == categoryName) {
                    found = true;
                    category = cat; // creating copy not actual category
                    break;
                }
            }
            if (!found){
                category = Category(categoryName);
                addCategory(category, false);
            }

            std::string website = fields.size() > 3 ? fields[3] : "";
            std::string login = fields.size() > 4 ? fields[4] : "";

            Password password(name, passwordText, category, website, login);
            passwords.push_back(password);

            category.addPassword(password);
        } else {
            // Handle the error: each line must have at least 3 fields
            std::cout << "[ERROR] Invalid format for password data: " << line << std::endl;
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

void PasswordManager::deleteCategoryPasswords(const Category& category) {
    // First, find all the passwords that need to be deleted
    std::vector<std::string> passwordsToDelete;
    for (const Password& password : passwords) {
        if (password.getCategoryCannotModify().getName() == category.getName()) {
            passwordsToDelete.push_back(password.getName());
        }
    }

    // Then, delete them
    for (const std::string& passwordName : passwordsToDelete) {
        deletePassword(passwordName, true);
    }
}

void PasswordManager::checkIfPasswordUsed(const std::string& password) {
    std::vector<Password> usedPasswords;

    for (const auto& pas : passwords){
        if (pas.getPasswordText() == password){
            usedPasswords.push_back(pas);
        }
    }

    if (!usedPasswords.empty()) {
        std::cout << "This password has been used in the following entries:" << std::endl;
        for (const auto& pas : usedPasswords) {
            std::cout << pas.to_string() << std::endl;
        }
    } else {
        std::cout << "This password has not been used before." << std::endl;
    }
}