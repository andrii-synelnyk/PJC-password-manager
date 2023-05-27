#include <random>
#include "Encryptor.h"

Encryptor::Encryptor() = default;

int Encryptor::generateShift(const std::string& password) {
    int shift = 0;
    for (char c : password) {
        shift += static_cast<int>(c);
    }
    return shift % 256;
}

std::string Encryptor::computeChecksum(const std::string& data) {
    long long checksum = 0;
    for (char c : data) {
        checksum += static_cast<int>(c);
    }
    std::string stringSum = std::to_string(checksum);
    return stringSum;
}

void Encryptor::encrypt(const std::string& filePath, const std::string& password, const std::string& data) {
    int shift = generateShift(password);
    std::string tempData = data;

    for (char& c : tempData) {
        c = (c + shift) % 256;
    }

    // Generate a random 6-digit number
    std::srand(std::time(nullptr));  // Seed the random number generator with the current time
    int randomNumber = 0;
    for (int i = 0; i < 6; ++i) {
        randomNumber = randomNumber * 10 + (std::rand() % 10);
    }

    std::string checksum = computeChecksum(data);
    tempData += "\n" + std::to_string(randomNumber) + checksum;

    std::ofstream outFile(filePath, std::ios::binary);
    outFile.write(tempData.c_str(), tempData.size());
    outFile.close(); // not obligatory
}

std::string Encryptor::decrypt(const std::string& filePath, const std::string& password) {
    std::fstream file(filePath, std::ios::binary | std::ios::in | std::ios::out);
    std::string encryptedData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    int shift = generateShift(password);
    shift = 256 - shift;

    std::size_t lastNewlinePos = encryptedData.find_last_of('\n');
    std::string lastLine = encryptedData.substr(lastNewlinePos + 1);
    encryptedData = encryptedData.substr(0, lastNewlinePos);

    // Get check sum by removing first 6 chars from last line
    std::string storedChecksum = lastLine.substr(6);

    // Decrypt data
    for (char& c : encryptedData) {
        c = (c + shift) % 256;
    }
    std::string decryptedData = encryptedData;

    if(computeChecksum(decryptedData) != storedChecksum) {

        // Record the current time
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm *now_tm = std::localtime(&now_time);

        // Format the time as hhmmss
        char timestamp[7];
        std::strftime(timestamp, sizeof(timestamp), "%H%M%S", now_tm);

        // Replace the first 6 characters of the timestamp line
        lastLine.replace(0, 6, timestamp);
        std::string newLastLine = "\n" + lastLine;

        // Seek to the position of the last newline character and write the new last line
        file.seekp(lastNewlinePos, std::ios::beg);
        file.write(newLastLine.c_str(), newLastLine.size());

        file.close();

        return "Wrong password. Unable to decrypt.";
    }

    file.close();
    return decryptedData;
}

bool Encryptor::isFileEmpty(const std::string& filePath){
    std::ifstream inFile(filePath, std::ios::binary);
    std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    inFile.close();
    if (data.empty()) return true;
    return false;
}
