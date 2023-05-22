#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

int generateShift(const std::string& password) {
    // Generate a shift value from the password by summing the ASCII values of its characters
    int shift = 0;
    for (char c : password) {
        shift += static_cast<int>(c);
    }
    return shift % 256;  // Ensure the shift value is within the range of a byte
}

int computeChecksum(const std::string& data) {
    int checksum = 0;
    for (char c : data) {
        checksum += static_cast<int>(c);
    }
    return checksum;
}

int getCheckSumFromFile(const std::string& data){
//    std::istringstream iss(data); // Create an input string stream
//    std::string line;
//    std::string lastLine;
//    while (std::getline(iss, line))
//    {
//        lastLine = line; // Update the lastLine variable with each line read
//    }
//    // Convert the last line to an integer
//    return std::stoi(lastLine);

    // OR
    std::size_t lastNewlinePos = data.find_last_of('\n');
    std::string lastLine = data.substr(lastNewlinePos + 1);

    // Check if the last line consists only of digits
    if (lastLine.find_first_not_of("0123456789") != std::string::npos) {
        // Handle the case where the last line is not a valid integer
        throw std::runtime_error("Last line is not a valid integer.");
    }

    return std::stoi(lastLine);
}

bool isValidDecryption(const std::string& data, int storedChecksum) {
    return computeChecksum(data) == storedChecksum;
}

void shiftFile(const std::string& filePath, const std::string& password, bool decrypt) {
    std::ifstream inFile(filePath, std::ios::binary);
    std::string originalData((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    if (originalData.empty()) return;

    int shift = generateShift(password);
    int storedChecksum; // maybe should be declared as 0

    if (decrypt) {
        // Get checksum from the file for validation of decryption result
        try {
            storedChecksum = getCheckSumFromFile(originalData);
        }catch (const std::exception& ex){
            std::cout << "Cannot decrypt already decrypted file" << std::endl;
            return;
        }

        // If we're decrypting, reverse the shift
        shift = 256 - shift;

        // Remove checksum from the file
        std::size_t lastNewlinePos = originalData.find_last_of('\n');
        originalData = originalData.substr(0, lastNewlinePos);
    }else{
        // If we are encrypting, and it is possible to retrieve checkSum from the end of the file, it means the file is already encrypted
        try {
            getCheckSumFromFile(originalData);
            cout << "Cannot encrypt already encrypted file";
            return;
        }catch (const std::exception& ex){}
    }

    // Make a copy of original data to encrypt/decrypt it
    std::string tempData = originalData;

    // Encrypt temp data (shift each byte of the data)
    for (char& c : tempData) {
        c = (c + shift) % 256;
    }

    if (decrypt) {
        if (isValidDecryption(tempData, storedChecksum)) {
            originalData = tempData;
        } else {
            // Add record that there was an unsuccessful attempt to decrypt
            cout << "Wrong password. Unable to decrypt.";
            return;
        }
    }
    else {
        int checksum = computeChecksum(originalData);
        originalData = tempData; // append encrypted data to the file
        originalData += "\n" + to_string(checksum); // append checksum for decryption validity check
    }

    inFile.close();

    std::ofstream outFile(filePath, std::ios::binary);
    outFile.write(originalData.c_str(), originalData.size());
}

int main() {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path projectPath = currentPath.parent_path();
    std::filesystem::path filePath = projectPath / "Source files/test8.txt";

    std::string password = "password";

    //shiftFile(filePath, password, false);
    shiftFile(filePath, password, true);
}