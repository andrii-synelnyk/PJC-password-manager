#pragma once

#include <vector>
#include <string>

class Password;

class Category {
public:
    explicit Category(const std::string& name);

    std::string getName() const;

    void addPassword(const Password& password);

    void removePassword(const std::string& passwordName);

private:
    std::string name;
    std::vector<Password> passwords;
};