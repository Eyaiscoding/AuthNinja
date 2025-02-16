#include <iostream>
#include <unordered_map>
#include <string>

class User {
    public:
        std::string username;
        std::string passwordHash;
    
        User(const std::string& u, const std::string& p) : username(u), passwordHash(p) {}
    };