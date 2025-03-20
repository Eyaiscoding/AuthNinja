#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "sha.h"
#include "hex.h"
#include "osrng.h"

class Hashing
{
public:
    static std::string hashPassword(const std::string &password, const std::string &salt)
    {
        CroptoPP::SHA256 hash;
        unsigned char digest[CrypotoPP::SHA256::DIGESTSIZE];
        std::string saltedPassword = password + salt;
        hash.CalculateDigest(digest, reinterpret_cast<const unsigned char *>(saltedPassword.data()), saltedPassword.size());
        return std::string(reinterpret_cast<char *>(digest), CryptoPP::SHA256::DIGESTSIZE);
    }
    static std::string generateSalt()
    {
        CroptoPP::AutoSeededRamdomPool rng;
        unsigned char salt[16];
        rng.GenerateBlock(salt, sizeof(salt));
        return std::string(reinterpret_cast<char *>(salt), sizeof(salt));
    }
};

class UserRegistration
{
public:
    void registerUser(const std::string &username, const std::string &password)
    {
        if (username.empty() || password.empty())
        {
            throw std::invalid_argument("Username and password cannot be empty.");
        }
        if (!isPasswordStrong(password))
        {
            throw std::invalid_argument("Password is not strong enough.");
        }
        // std::string salt = Hashing::generateSalt();
        // std::string hashedPassword = Hashing::hashPassword(password, salt);
        // FileHandler::storeUserData(username, hashedPassword, salt);
    }

private:
    bool isPasswordStrong(const std::string &password)
    {
        // Password strength rules: at least 8 characters, 1 uppercase, 1 lowercase, 1 digit, 1 special character
        if (password.length() < 8)
            return false;
        bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
        for (char ch : password)
        {
            if (isupper(ch))
                hasUpper = true;
            if (islower(ch))
                hasLower = true;
            if (isdigit(ch))
                hasDigit = true;
            if (!isalnum(ch))
                hasSpecial = true;
        }
        return hasUpper && hasLower && hasDigit && hasSpecial;
    }
};

using namespace std;
class UserLogin
{
public:
    bool loginUser(const string &username, const string &password)
    {
        try
        {
            auto userData = FileHandler::retrieveUserData(username);
            string hashedPassword = Hashing::hashPassword(password, userData.salt);

            if (hashedPassword == userData.hashedPassword)
            {
                cout << "Login successful. Welcome, " << username << "!" << "\n";
                return true;
            }
            else
            {
                cout << "Incorrect password. Try again." << "\n";
                return false;
            }
        }
        catch (...)
        {
            cout << "Login failed. User not found." << "\n";
            return false;
        }
    }

    void resetPassword(const string &username, const string &newPassword)
    {
        if (newPassword.empty())
        {
            cout << "Password cannot be empty." << "\n";
            return;
        }

        string newSalt = Hashing::generateSalt();
        string newHashedPassword = Hashing::hashPassword(newPassword, newSalt);
        FileHandler::storeUserData(username, newHashedPassword, newSalt);

        cout << "Password reset successfully." << "\n";
    }
};
class PasswordVerification
{
public:
    bool verifyPassword(const std::string &username, const std::string &password)
    {
        auto userData = FileHandler::retrieveUserData(username);
        bool isPasswordValid = BCrypt::validatePassword(password, userData.hashedPassword);

        if (isPasswordValid)
        {
            std::cout << "Successful login attempt for username: " << username << std::endl;
        }
        else
        {
            std::cout << "Failed login attempt for username: " << username << std::endl;
        }
        return isPasswordValid;
    }

    bool resetPassword(const std::string &username, const std::string &newPassword)
    {
        auto userData = FileHandler::retrieveUserData(username);

        std::string newHashedPassword = BCrypt::generateHash(newPassword);
        userData.hashedPassword = newHashedPassword;
        FileHandler::saveUserData(username, userData);

        std::cout << "Password reset successfully for username: " << username << std::endl;
        return true;
    }
};

int main()
{

    UserRegistration registration;
    registration.registerUser("user1", "password123");

    PasswordVerification verification;
    if (verification.verifyPassword("user1", "password123"))
    {
        std::cout << "Password verification successful!" << std::endl;
    }
    else
    {
        std::cout << "Password verification failed!" << std::endl;
    }

    UserLogin login;
    if (login.loginUser("user1", "Password123"))
    {
        std::cout << "Login successful!" << std::endl;
    }
    else
    {
        std::cout << "Login failed!" << std::endl;
    }
    return 0
}
