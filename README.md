# CLI-Application
## Project Overview
Negar Shell is a comprehensive command-line interface (CLI) application developed in C programming language. It provides a robust and user-friendly environment for performing various file management operations, user management, and system administration tasks.

The key features of Negar Shell include:

1. **User Management:** The application supports user creation, authentication, and permission management. It allows administrators to create and manage user accounts with varying levels of access and privileges.
2. **File Operations:** Negar Shell offers a wide range of file management commands, such as creating directories, copying, moving, and deleting files and directories, and more. It also includes utilities like cat, wc, and diff for file manipulation and analysis.
3. **Security Features:** The application incorporates password security measures, time-based user account restrictions, and user-level permission controls to ensure a secure and controlled environment.
4. **Extensibility:** The modular design of Negar Shell allows for easy expansion and integration of additional features and functionalities in the future.

   
## Getting Started
To get started with Negar Shell, follow these steps:

1. Clone the repository to your local machine:
```console
git clone https://github.com/Negar-Fazel/CLI-Application.git
```

2. Navigate to the project directory:
```console
cd CLI-Application
```

3. Compile the source code:
```console
gcc -o negar-shell main.cpp
```

4. Run the Negar Shell application:
```console
./negar-shell
```

## Usage
Negar Shell provides a variety of commands for managing users, files, and system operations. Upon launching the application, you will be prompted to log in with a valid username and password.

Some of the key commands available in Negar Shell include:

- create_user: Creates a new user account
- su: Switches to a different user account
- passwd: Changes the password of the current user
- pwd: Displays the current working directory
- cd: Changes the current working directory
- ls: Lists the contents of the current directory
- mkdir: Creates a new directory
- cat: Displays the contents of a file
- rm: Deletes a file or directory
- cp: Copies a file or directory
- mv: Moves a file or directory

  
For a complete list of available commands and their usage, please refer to the project's documentation.

## Contact
Negar Fazel: negarfazel.business@gmail.com

## License
This project is licensed under the MIT License.
