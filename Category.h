#pragma once

#include <vector>
#include <string>
#include <algorithm>

class Password; // Declaration is needed to avoid circular dependency, as password relies on category and vice versa

/**
 * @class Category
 * @brief Class to hold data of category entry in program.
 */
class Category {
public:
    /**
    * @brief Constructs a Category object.
    * @param name The name of the category.
    */
    explicit Category(const std::string& name);

    /**
    * @brief Adds a Password to the Category.
    * @param password The Password to be added.
    */
    void addPassword(const Password& password);

    /**
    * @brief Removes a Password from the Category by its name.
    * @param passwordName The name of the Password to be removed.
    */
    void removePassword(const std::string& passwordName);

    // Getters
    std::string getName() const;

private:
    std::string name;
    std::vector<Password> passwords;
};