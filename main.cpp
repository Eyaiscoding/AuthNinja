#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cryptopp/sha.h> // Crypto++ library for SHA-256
#include <cryptopp/hex.h> // Crypto++ library for hex encoding
#include <cryptopp/osrng.h> // Crypto++ library for random number generation

class UserRegistration {
    public:
        void registerUser(const std::string& username, const std::string& password) {
            if (username.empty() || password.empty()) {
                throw std::invalid_argument("Username and password cannot be empty.");
            }
            if (!isPasswordStrong(password)) {
                throw std::invalid_argument("Password is not strong enough.");
            }
            //std::string salt = Hashing::generateSalt();
            //std::string hashedPassword = Hashing::hashPassword(password, salt);
            //FileHandler::storeUserData(username, hashedPassword, salt);
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
    