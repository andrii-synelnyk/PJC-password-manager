#pragma once

#include <string>

#include "Category.h"

class Password {
public:
    // Constructor
    Password(const std::string& name, const std::string& passwordText, const Category& category,
             const std::string& website = "", const std::string& login = "");

    // Getters
    std::string getName() const;
    std::string getPasswordText() const;
    Category& getCategoryCanModify();
    const Category& getCategoryCannotModify() const;
    std::string getWebsite() const;
    std::string getLogin() const;

    // Setters
    void setName(const std::string& name);
    void setPasswordText(const std::string& passwordText);
    void setCategory(const Category& category);
    void setWebsite(const std::string& website);
    void setLogin(const std::string& login);

    // To string
    std::string to_string() const;

private:
    std::string name;
    std::string passwordText;
    Category category;
    std::string website;
    std::string login;
};