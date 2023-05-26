#include "Password.h"

// Constructor
Password::Password(const std::string& name, const std::string& passwordText, const std::string& category,
                   const std::string& website, const std::string& login)
        : name(name), passwordText(passwordText), category(category), website(website), login(login) {}

// Getters
std::string Password::getName() const {
    return name;
}

std::string Password::getPasswordText() const {
    return passwordText;
}

std::string Password::getCategory() const {
    return category;
}

std::string Password::getWebsite() const {
    return website;
}

std::string Password::getLogin() const {
    return login;
}

// Setters
void Password::setName(const std::string& name) {
    this->name = name;
}

void Password::setPasswordText(const std::string& passwordText) {
    this->passwordText = passwordText;
}

void Password::setCategory(const std::string& category) {
    this->category = category;
}

void Password::setWebsite(const std::string& website) {
    this->website = website;
}

void Password::setLogin(const std::string& login) {
    this->login = login;
}