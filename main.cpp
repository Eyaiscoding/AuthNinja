#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "sha.h"
#include "hex.h"
#include "osrng.h"

class Hashing {
public:
    static std::string hashPassword(const std::string& password, const std::string& salt) {
        CryptoPP::SHA256 hash;
        unsigned char digest[CryptoPP::SHA256::DIGESTSIZE];
        std::string saltedPassword = password + salt;
        hash.CalculateDigest(digest, reinterpret_cast<const unsigned char*>(saltedPassword.data()), saltedPassword.size());
        return std::string(reinterpret_cast<char*>(digest), CryptoPP::SHA256::DIGESTSIZE);
    }
    static std::string generateSalt() {
        CryptoPP::AutoSeededRandomPool rng;
        unsigned char salt[16];
        rng.GenerateBlock(salt, sizeof(salt));
        return std::string(reinterpret_cast<char*>(salt), sizeof(salt));
    }
};


class FileHandler {
public:
    struct UserData {
        std::string hashedPassword;
        std::string salt;
    };

    static void StoreUserData(const std::string& username, const std::string& hashedPassword, const std::string& salt) {
        std::ofstream file("users.txt", std::ios::app);
        if (!file) {
            throw std::runtime_error("Failed to open users file.");
        }
        std::string hashedPasswordHex, saltHex;
        CryptoPP::StringSource(hashedPassword, true,
            new CryptoPP::HexEncoder(new CryptoPP::StringSink(hashedPasswordHex)));

        // Convert salt to hex
        CryptoPP::StringSource(salt, true,
            new CryptoPP::HexEncoder(new CryptoPP::StringSink(saltHex)));

        // Write to file
        file << username << "," << hashedPasswordHex << "," << saltHex << "\n";
    }

    static UserData retrieveUserData(const std::string& username) {
        std::ifstream file("users.txt");
        if (!file) {
            throw std::runtime_error("Failed to open users file.");
        }
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string storedUsername, hashedPasswordHex, saltHex;
            if (std::getline(iss, storedUsername, ',') && std::getline(iss, hashedPasswordHex, ',') && std::getline(iss, saltHex)) {
                if (storedUsername == username) {
                    std::string hashedPassword, salt;
                    CryptoPP::StringSource(hashedPasswordHex, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(hashedPassword)));
                    CryptoPP::StringSource(saltHex, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(salt)));
                    return { hashedPassword, salt };
                }
            }
        }
        throw std::runtime_error("User not found.");
    }

    static void saveUserData(const std::string& username, const UserData& userData) {
        std::ifstream inFile("users.txt");
        std::ofstream outFile("temp.txt");
        if (!inFile || !outFile) {
            throw std::runtime_error("Failed to open users file.");
        }
        std::string line;
        bool userFound = false;
        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::string storedUsername, hashedPasswordHex, saltHex;
            if (std::getline(iss, storedUsername, ',') && std::getline(iss, hashedPasswordHex, ',') && std::getline(iss, saltHex)) {
                if (storedUsername == username) {
                    // Update the user's data
                    std::string newHashedPasswordHex, newSaltHex;
                    CryptoPP::StringSource(userData.hashedPassword, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(newHashedPasswordHex)));
                    CryptoPP::StringSource(userData.salt, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(newSaltHex)));
                    outFile << username << "," << newHashedPasswordHex << "," << newSaltHex << "\n";
                    userFound = true;
                }
                else {
                    outFile << line << "\n";
                }
            }
        }
        if (!userFound) {
            throw std::runtime_error("User not found.");
        }
        inFile.close();
        outFile.close();
        std::remove("users.txt");
        std::rename("temp.txt", "users.txt");
    }
};


class UserRegistration {
public:
    void registerUser(const std::string& username, const std::string& password) {
        if (username.empty() || password.empty()) {
            throw std::invalid_argument("Username and password cannot be empty.");
        }
        if (!isPasswordStrong(password)) {
            throw std::invalid_argument("Password is not strong enough.");
        }
        std::string salt = Hashing::generateSalt();
        std::string hashedPassword = Hashing::hashPassword(password, salt);
        FileHandler::StoreUserData(username, hashedPassword, salt);
    }

    std::string getPasswordFromUser() {
        std::string password;
        while (true) {
            std::cout << "Enter a password: ";
            std::cin >> password;

            if (isPasswordStrong(password)) {
                return password;
            }
            else {
                std::cout << "Password is not strong enough. Please follow these rules:\n";
                std::cout << "- At least 8 characters\n";
                std::cout << "- At least 1 uppercase letter\n";
                std::cout << "- At least 1 lowercase letter\n";
                std::cout << "- At least 1 digit\n";
                std::cout << "- At least 1 special character\n";
                std::cout << "Please try again.\n";
            }
        }
    }

private:
    bool isPasswordStrong(const std::string& password) {
        // Password strength rules: at least 8 characters, 1 uppercase, 1 lowercase, 1 digit, 1 special character
        if (password.length() < 8) return false;
        bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
        for (char ch : password) {
            if (isupper(ch)) hasUpper = true;
            if (islower(ch)) hasLower = true;
            if (isdigit(ch)) hasDigit = true;
            if (!isalnum(ch)) hasSpecial = true;
        }
        return hasUpper && hasLower && hasDigit && hasSpecial;
    }
};


class UserLogin {
public:
    bool loginUser(const std::string& username, const std::string& password) {
        auto userData = FileHandler::retrieveUserData(username);
        std::string hashedPassword = Hashing::hashPassword(password, userData.salt);
        return hashedPassword == userData.hashedPassword;
    }
};


class PasswordVerification {
public:
    bool verifyPassword(const std::string& username, const std::string& password) {
        auto userData = FileHandler::retrieveUserData(username);
        std::string hashedPassword = Hashing::hashPassword(password, userData.salt);
        bool isPasswordValid = (hashedPassword == userData.hashedPassword);

        if (isPasswordValid) {
            std::cout << "Successful login attempt for username: " << username << std::endl;
        }
        else {
            std::cout << "Failed login attempt for username: " << username << std::endl;
        }
        return isPasswordValid;
    }

    bool resetPassword(const std::string& username, const std::string& newPassword) {
        auto userData = FileHandler::retrieveUserData(username);

        std::string newSalt = Hashing::generateSalt();
        std::string newHashedPassword = Hashing::hashPassword(newPassword, newSalt);
        userData.hashedPassword = newHashedPassword;
        userData.salt = newSalt;
        FileHandler::saveUserData(username, userData);

        std::cout << "Password reset successfully for username: " << username << std::endl;
        return true;
    }
};


int main() {
    UserRegistration registration;
    UserLogin login;
    PasswordVerification verification;

    std::string username, password;
    int choice;

    while (true) {
        std::cout << "1. Register\n2. Login\n3. Reset Password\n4. Exit\nEnter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::cout << "Enter username: ";
            std::cin >> username;
            try {
                password = registration.getPasswordFromUser();
                registration.registerUser(username, password);
                std::cout << "User registered successfully!\n";
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 2: {
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter password: ";
            std::cin >> password;
            try {
                if (login.loginUser(username, password)) {
                    std::cout << "Login successful!\n";
                }
                else {
                    std::cout << "Login failed!\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 3: {
            std::cout << "Enter username: ";
            std::cin >> username;
            std::cout << "Enter new password: ";
            std::cin >> password;
            try {
                if (verification.resetPassword(username, password)) {
                    std::cout << "Password reset successful!\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 4: {
            std::cout << "Exiting...\n";
            return 0;
        }
        default: {
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
        }
    }

    return 0;
}