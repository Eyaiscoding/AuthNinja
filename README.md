#include <iostream>
#include <limits>
#include "UserDatabase.h"  
#include "Authenticator.h"
#include "HashingUtility.h" 

void registerUser(UserDatabase& db) {
    std::string username, password;
    std::cout << "\n=== User Registration ===\n";
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    std::string salt = generateSalt();
    std::string hashedPassword = hashPassword(password, salt);

    if (db.addUser(username, salt, hashedPassword)) {
        std::cout << "Registration successful!\n";
    } else {
        std::cout << "User already exists.\n";
    }
}

void loginUser(Authenticator& auth) {
    std::string username, password;
    std::cout << "\n=== User Login ===\n";
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (auth.login(username, password)) {
        std::cout << "Login successful! Welcome, " << username << "!\n";
    } else {
        std::cout << "Login failed. Incorrect username or password.\n";
    }
}

void displayMenu() {
    std::cout << "\n===== Authentication System =====\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    UserDatabase db;
    Authenticator auth(db);

    int choice;
    while (true) {
        displayMenu();
        while (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number: ";
        }

        switch (choice) {
            case 1:
                registerUser(db);
                break;
            case 2:
                loginUser(auth);
                break;
            case 3:
                std::cout << "Exiting the program. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }
}
 ***