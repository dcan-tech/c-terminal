# MyTerminal — C-Based Custom Shell with File Encryption

A lightweight, command-line terminal emulator built in C, featuring basic system command execution and Caesar cipher file encryption/decryption. Designed as an educational tool to explore command parsing, memory management, and file I/O operations.

## Features

- **System Command Execution**  
  Execute standard shell commands like `ls`, `pwd`, etc., directly from the program.

- **File Encryption & Decryption**  
  Use `secure_file` with Caesar Cipher (shift of 3) to encrypt/decrypt text files.

- **Command-Line Argument Support**  
  Run encryption/decryption directly from the terminal:  
  `./MyTerminal -f filename.txt -e` or `-d`

- **Robust Error Handling**  
  Includes detailed input validation, feedback, and memory safety practices.

## Usage Examples

Interactive mode:
```bash
$ ./MyTerminal
$ ls
$ secure_file -f notes.txt -e
$ secure_file -f notes.txt -d
```

Direct execution:
```
$ ./MyTerminal -f notes.txt -e
```
Command Syntax

    secure_file -f <filename> -e – Encrypt the file
    secure_file -f <filename> -d – Decrypt the file
    exit or quit – Exit the program
    ? – Show help

⚠️ Note: This is for educational use only. The system() call is not secure in production environments.

Key Concepts Demonstrated

    Command-line parsing and tokenization
    File I/O and Caesar cipher logic
    Dynamic memory management
    Defensive programming and input validation
    Custom shell-like interface

 Build Instructions
```
gcc -o MyTerminal main.c
./MyTerminal
```
Author
Dylan Canfield

This project was created as part of coursework for SD247 (Systems Development).
