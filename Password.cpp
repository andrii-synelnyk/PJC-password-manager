#include "Password.h"

// Constructor
Password::Password(const std::string& name, const std::string& passwordText, const Category& category,
         const std::string& website, const std::string& login)
        : name(name), passwordText(passwordText), category(category), website(website), login(login) {}


std::string Password::to_string() const { // const here means that the function is read only and will not modify any fields of Password objects
    return name + ' ' + passwordText + ' ' + category.getName() + ' ' + website + ' ' + login;
}

// Getters
std::string Password::getName() const {
    return name;
}

std::string Password::getPasswordText() const {
    return passwordText;
}

std::string Password::getWebsite() const {
    return website;
}

std::string Password::getLogin() const {
    return login;
}

Category& Password::getCategoryCanModify() { return category; }

const Category& Password::getCategoryCannotModify() const { return category; } // guaranties that neither Password object nor Category object returned can be modified

// Setters
void Password::setName(const std::string& name) {
    this->name = name;
}

void Password::setPasswordText(const std::string& passwordText) {
    this->passwordText = passwordText;
}

void Password::setWebsite(const std::string& website) {
    this->website = website;
}

void Password::setLogin(const std::string& login) {
    this->login = login;
}

void Password::setCategory(const Category& category) {
    this->category = category;
}