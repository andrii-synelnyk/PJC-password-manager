#include "Category.h"
#include <algorithm>

#include "Password.h"

Category::Category(const std::string& name) : name(name) {}

std::string Category::getName() const { return name; }

void Category::addPassword(const Password& password) {
    passwords.push_back(password);
}

void Category::removePassword(const std::string& passwordName) {
    passwords.erase(std::remove_if(passwords.begin(), passwords.end(),
                                   [&passwordName](const Password& password) {
                                       return password.getName() == passwordName;
                                   }),
                    passwords.end());
}