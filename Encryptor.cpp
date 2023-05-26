#include "Encryptor.h"

Encryptor::Encryptor() = default;

int Encryptor::generateShift(const std::string& password) {
    int shift = 0;
    for (char c : password) {
        shift += static_cast<int>(c);
    }
    return shift % 256;
}

int Encryptor::computeChecksum(const std::string& data) {
    int checksum = 0;
    for (char c : data) {
        checksum += static_cast<int>(c);
    }
    return checksum;
}

void Encryptor::encrypt(const std::string& filePath, const std::string& password, const std::string& data) {
    int shift = generateShift(password);

    std::string tempData = data;

    for (char& c : tempData) {
        c = (c + shift) % 256;
    }

    int checksum = computeChecksum(data);
    tempData += "\n" + std::to_string(checksum);

    std::ofstream outFile(filePath, std::ios::binary);
    outFile.write(tempData.c_str(), tempData.size());
}

std::string Encryptor::decrypt(const std::string& filePath, const std::string& password) {
    std::ifstream inFile(filePath, std::ios::binary);
    std::string encryptedData((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();

    int shift = generateShift(password);
    shift = 256 - shift;

    std::size_t lastNewlinePos = encryptedData.find_last_of('\n');
    std::string lastLine = encryptedData.substr(lastNewlinePos + 1);
    encryptedData = encryptedData.substr(0, lastNewlinePos);

    int storedChecksum = std::stoi(lastLine);

    for (char& c : encryptedData) {
        c = (c + shift) % 256;
    }

    if(computeChecksum(encryptedData) != storedChecksum) {
        return "Wrong password. Unable to decrypt.";
    }

    return encryptedData;
}

bool Encryptor::isFileEmpty(const std::string& filePath){
    std::ifstream inFile(filePath, std::ios::binary);
    std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    if (data.empty()) return true;
    return false;
}

//int main() {
//    std::filesystem::path currentPath = std::filesystem::current_path();
//    std::filesystem::path projectPath = currentPath.parent_path();
//    std::filesystem::path filePath = projectPath / "Source files/test8.txt";
//
//    std::string password = "password";
//
//    //shiftFile(filePath, password, false);
//    //shiftFile(filePath, password, true);
//}
