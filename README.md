# AuthNinja

<img src="./assets/authninja-logo.webp" width="200" />

AuthNinja is a robust authentication system implemented in C++ following SOLID principles. It provides secure and efficient authentication using file-based storage, password hashing with SHA-256, and salt for added security.

## Features
- User registration with salted and hashed passwords
- User authentication with SHA-256 hashed password verification
- Follows SOLID design principles
- Simple console-based user interface
- Automated testing with GitHub Actions

## Installation
1. Clone the repository:
   ```sh
   git clone https://github.com/your-repo/AuthNinja.git
   ```
2. Build the project:
   ```sh
   make build
   ```
3. Run tests:
   ```sh
   make test
   ```
## Usage
1. Choose an option from the menu:
   - `1`: Register a new user
   - `2`: Log in with an existing user
   - `3`: Exit the program
2. Enter a username and password when prompted.
3. If registration is successful, the credentials are saved securely in `users.txt`.
4. If logging in, the system will verify the username and password before granting access.
   
## Contributors
- Eya Abidi
- Ines Jabri
- Ahmed Dhia Dridi
- Amina Jebari
- Mohamed Yessine Aifa
