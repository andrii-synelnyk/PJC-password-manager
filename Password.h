#pragma once

#include <string>

#include "Category.h"

/**
 * @class Password
 * @brief Class to hold data of password entry in program.
 */
class Password {
public:
    /**
    * @brief Constructs a Password object.
    * @param name The name of the password.
    * @param passwordText The actual password text.
    * @param category The category of the password.
    * @param website The website for which the password is used.
    * @param login The login used with this password.
    */
    Password(const std::string& name, const std::string& passwordText, const Category& category,
             const std::string& website = "", const std::string& login = "");

    /**
    * @brief Converts a Password object to string.
    * @return A string representing the Password object.
    */
    std::string to_string() const;

    // Getters
    std::string getName() const;
    std::string getPasswordText() const;
    std::string getWebsite() const;
    std::string getLogin() const;

    /**
    * @brief Gets the Category of the Password, which can be modified.
    * @return The modifiable Category of the Password.
    */
    Category& getCategoryCanModify();

    /**
    * @brief Gets the Category of the Password, which cannot be modified.
    * @return The unmodifiable Category of the Password.
    */
    const Category& getCategoryCannotModify() const;

    // Setters
    void setName(const std::string& name);
    void setPasswordText(const std::string& passwordText);
    void setCategory(const Category& category);
    void setWebsite(const std::string& website);
    void setLogin(const std::string& login);

private:
    std::string name;
    std::string passwordText;
    Category category;
    std::string website;
    std::string login;
};