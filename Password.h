#pragma once

#include <string>

class Password {
public:
    // Constructor
    Password(const std::string& name, const std::string& passwordText, const std::string& category,
             const std::string& website = "", const std::string& login = "");

    // Getters
    std::string getName() const;
    std::string getPasswordText() const;
    std::string getCategory() const;
    std::string getWebsite() const;
    std::string getLogin() const;

    // Setters
    void setName(const std::string& name);
    void setPasswordText(const std::string& passwordText);
    void setCategory(const std::string& category);
    void setWebsite(const std::string& website);
    void setLogin(const std::string& login);

    // To string
    std::string to_string() const;

private:
    std::string name;
    std::string passwordText;
    std::string category;
    std::string website;
    std::string login;
};