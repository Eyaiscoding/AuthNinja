# AuthNinja

<p align="center">
  <img src="./assets/authninja-logo.webp" width="200" />
</p>

AuthNinja is a secure authentication system implemented in C++. The project follows **SOLID principles** to ensure a clean and maintainable codebase.

## How It Works

### Registration:
1. The user provides a username and password.
2. The system checks if the password is strong:
   - At least 8 characters.
   - Includes uppercase, lowercase, digits, and special characters.
3. A unique salt is generated using `CryptoPP::AutoSeededRandomPool`.
4. The password is combined with the salt and hashed using `CryptoPP::SHA256`.
5. The username, hashed password (in hex), and salt (in hex) are stored in `users.txt`.

### Login:
1. The user provides their username and password.
2. The system retrieves the stored salt and hashed password from `users.txt`.
3. The entered password is combined with the salt and hashed using `CryptoPP::SHA256`.
4. The new hash is compared to the stored hash.
5. If they match, the login is successful.

### Password Reset:
1. The user provides a new password.
2. A new salt is generated using `CryptoPP::AutoSeededRandomPool`.
3. The new password is combined with the new salt and hashed using `CryptoPP::SHA256`.
4. The updated hash and salt (in hex) are stored in `users.txt`.

## Security Practices

### Salting:
- Each password is hashed with a unique salt, ensuring that even identical passwords produce different hashes.
- This prevents rainbow table attacks.

### Hashing:
- Passwords are hashed using SHA-256, a secure and widely trusted algorithm.
- Hashing makes it computationally infeasible to reverse-engineer the original password.

### Hex Encoding:
- Hashes and salts are stored in hexadecimal format, ensuring accurate and readable storage in text files.

### Crypto++ Library:
The program uses the **Crypto++** library for:
- Generating secure random salts (`CryptoPP::AutoSeededRandomPool`).
- Hashing passwords with SHA-256 (`CryptoPP::SHA256`).
- Encoding and decoding data in hexadecimal format (`CryptoPP::HexEncoder` and `CryptoPP::HexDecoder`).
- Crypto++ ensures the program adheres to industry-standard cryptographic practices.

## How the Code Respects SOLID Principles

1. **Single Responsibility Principle (SRP)**:
   - Each class has a single responsibility:
     - `Hashing`: Handles password hashing and salt generation.
     - `FileHandler`: Manages file operations (storing and retrieving user data).
     - `UserRegistration`: Handles user registration logic.
     - `UserLogin`: Handles user login logic.
     - `PasswordVerification`: Handles password verification logic.
   - This ensures that each class is focused on one task, making the code easier to maintain and extend.

2. **Open/Closed Principle (OCP)**:
   - The system is open for extension but closed for modification:
     - New hashing algorithms can be added to the `Hashing` class without modifying existing code.
     - New file storage formats can be added to the `FileHandler` class without affecting other modules.
   - This allows the system to evolve without breaking existing functionality.

3. **Liskov Substitution Principle (LSP)**:
   - Derived classes (e.g., `Hashing`) can be substituted for their base classes without altering the correctness of the program:
     - If a new hashing algorithm is implemented, it can replace the existing one without affecting other modules.
   - This ensures that the system remains robust and flexible.

4. **Interface Segregation Principle (ISP)**:
   - Interfaces are segregated to ensure that classes only implement methods they need:
     - `Hashing` only provides methods for hashing and salt generation.
     - `FileHandler` only provides methods for file operations.
   - This prevents classes from being forced to implement unnecessary methods.

5. **Dependency Inversion Principle (DIP)**:
   - High-level modules (e.g., `UserRegistration`, `UserLogin`, `PasswordVerification`) depend on abstractions (e.g., `Hashing`, `FileHandler`) rather than concrete implementations:
     - This allows the system to be more flexible and easier to test, as dependencies can be mocked or replaced.

## How to Run

### Requirements
This code is designed to run on **Microsoft Visual Studio**. Follow these steps to set up and run the application:

1. Clone the repository.
2. Clone the Crypto++ library:
   ```sh
   git clone https://github.com/weidai11/cryptopp
   ```
3. Open the `cryptlib.vcxproj` file in **Microsoft Visual Studio**.
4. Select the `cryptlib` solution with the **Release** option and build it.
5. Once the build is successful, locate the `cryptlib.lib` file under `cryptopp\x64\Output\Release`.
6. Copy the **AuthNinja** code into a new **Microsoft Visual Studio C++ project**.
7. Open **Solution Explorer**, right-click on the project, and open **Properties**.
8. Configure the project as follows:
   - Select **All Configurations** and **All Platforms** at the top bar.
   - Under **C/C++ > General**, set **Additional Include Directories** to the path of the `cryptopp` directory.
   - Under **Linker > General**, set **Additional Library Directories** to `cryptopp\x64\Output\Release`.
   - Under **Linker > Input**, add `cryptlib.lib` to **Additional Dependencies**.
   - Under **C/C++ > Code Generation**, set **Runtime Library** to **Multi-threaded**.
9. Click **Apply**, then **OK**.
10. Select the **Release** option and run the application.

## GitHub Actions

The project includes a GitHub Actions workflow to automate testing on every push or pull request to the `main` branch.

## Contributors

- Eya Abidi
- Ines Jabri
- Ahmed Dhia Dridi
- Amina Jebari
- Mohamed Yessine Aifa

