/*
 * MyTerminal - Custom C-Based Shell
 * Author: Dylan Canfield
 * Description: A terminal emulator with command parsing and Caesar cipher encryption.
 * Originally developed for SD247 coursework.
 */


#include "myterminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 256  // Define maximum length for user input commands
#define SHIFT 3  // Define shift value for Caesar cipher encryption and decryption

// Declare functions for various functionalities within application
void intro();  // Displays introductory message or help information to the user
void handleCommandLineArguments(int argc, char *argv[]);  // Processes command-line arguments provided at program start
void tokenizeCommandLine(const char *input);  // Splits input into tokens for commands with arguments
void readUserInput(char *input);  // Reads user input from terminal
int parseAndExecuteCommand(const char *input);  // Parses input command and decides on execution path
void executeSystemCommand(const char *command);  // Executes system commands passed as input
int encryptFile(const char *filename);  // Encrypts content of specified file using Caesar cipher
int decryptFile(const char *filename);  // Decrypts content of specified file using Caesar cipher
void printErrorMessage(const char *message);  // Outputs error messages to user

// main function, supporting command line arguments
int main(int argc, char *argv[]) {
    // Process command line arguments if provided
    if (argc > 1) {
        handleCommandLineArguments(argc, argv);
        return 0; // Successful termination after handling arguments
    }
    char command[MAX_COMMAND_LENGTH]; // Buffer to hold user input

    intro(); // Display introductory message or help

    // Main loop for interactive command input
    while (1) {
        readUserInput(command); // Prompt and read input from user
        // Attempt to execute the command and check for termination signal
        if (parseAndExecuteCommand(command) == 0) {
            break; // Command indicated to exit, break out of loop
        }
    }

    printf("Exiting...\n"); // Final message-program termination
    return 0; // Successful termination
} // End of main


void intro() { // Introduction to program
    printf("Welcome to MyTerminal!\n\n");
    printf("This custom terminal application mimics the basic functionalities of standard terminals found in operating systems.\n");
    printf("It allows you to execute system commands like 'ls' and 'pwd', as well as to encrypt and decrypt text files using a Caesar Cipher.\n\n");
    printf("Usage:\n");
    printf("  - To execute system commands, simply type the command (e.g., 'ls', 'pwd') and press Enter.\n");
    printf("  - To encrypt a file, use the command: secure_file -f <filename> -e\n");
    printf("  - To decrypt a file, use the command: secure_file -f <filename> -d\n");
    printf("  - Type 'exit' or 'quit' to terminate the application.\n");
    printf("  - Use '?' to list this information again.\n\n");
    printf("Please note: This application is designed for educational purposes and uses 'system()' for command execution, which is not secure for production systems.\n\n");
    printf("Please be aware:\n");
    printf("- Encrypting a file that has already been encrypted will further modify its contents, potentially making it unrecoverable.\n");
    printf("- Similarly, decrypting a file that was not encrypted, or was encrypted with a different key, will also alter its contents.\n\n");
    printf("Enjoy using MyTerminal!\n");
} // End of intro

// Handles command line arguments for encryption or decryption
void handleCommandLineArguments(int argc, char *argv[]) {
    // Ensure minimum required arguments are provided
    if (argc < 3) {
        // Display usage syntax if not enough arguments are given
        printf("Usage: %s -f <filename> [-e | -d] (Encrypt/Decrypt)\n", argv[0]);
        return;
    }

    char *filename = NULL; // Initialize filename pointer as NULL
    char action = '\0'; // Initialize action character as null character

    // Iterate through provided arguments
    for (int i = 1; i < argc; i++) {
        // Check for '-f' flag indicating filename follows
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i]; // Assign next argument as filename
        } else if (strcmp(argv[i], "-e") == 0) { // Check for encryption flag
            action = 'e'; // Set action to encrypt
        } else if (strcmp(argv[i], "-d") == 0) { // Check for decryption flag
            action = 'd'; // Set action to decrypt
        }
    }

    // Verify that both required arguments (filename and action) were provided
    if (filename == NULL || action == '\0') {
        printf("Error: Missing required arguments.\n");
        printf("Usage: %s -f <filename> [-e | -d] (Encrypt/Decrypt)\n", argv[0]);

        return; // Exit if any required argument is missing
    }

    // Initialize result variable to track success or failure
    // from encryptFile and decryptFile
    int result = -1;
    // Perform encryption or decryption based on flag
    if (action == 'e') {
        result = encryptFile(filename); // Encrypt the specified file
        if (result == 0) { // Check if encryption was successful
            printf("File encrypted: %s\n", filename); // Confirm encryption success
        }
    } else if (action == 'd') {
        result = decryptFile(filename); // Decrypt the specified file
        if (result == 0) { // Check if decryption was successful
            printf("File decrypted: %s\n", filename); // Confirm decryption success
        }
    }
} // End of handleCommandLineArguments



// Reads user input from standard input and stores it in provided buffer
void readUserInput(char *input) {
    printf("\n$ "); // Display prompt for user input
    fflush(stdout); // Ensure prompt is displayed immediately by flushing stdout buffer
    fgets(input, MAX_COMMAND_LENGTH, stdin); // Capture user input, including newline, up to maximum length
} // End of readUserInput

// Frees dynamically allocated memory for command line arguments
void freeArgv(char **argv, int argc) {
    for (int i = 0; i < argc; i++) {
        free(argv[i]); // Release memory allocated for each argument string
    }
    free(argv); // Release memory allocated for pointers to argument strings
} // End of freeArgv



// Splits a command line string into separate arguments and processes them
void tokenizeCommandLine(const char *input) {
    // Create a modifiable copy of the input to prevent altering the original string
    char *inputCopy = strdup(input);
    // Check for successful memory allocation for the copy
    if (!inputCopy) {
        printf("Error: Memory allocation failed.\n");
        return; // Exit if memory allocation fails
    }

    // Prepare to collect and parse command line arguments
    int size = 5, argc = 0; // Initial size for argv array and argument counter
    // Allocate initial array for storing arguments
    char **argv = calloc(size, sizeof(char *));
    // Check for successful memory allocation for argv
    if (!argv) {
        printf("Error: Memory allocation failed.\n");
        free(inputCopy); // Ensure inputCopy is freed on failure
        return; // Exit if memory allocation fails
    }

    // Tokenize the input string by whitespace and newline characters
    char *token = strtok(inputCopy, " \n");
    while (token) {
        // Check if array needs to be resized to accommodate more arguments
        if (argc >= size - 1) {
            size *= 2; // Double the size of argv
            char **newArgv = realloc(argv, size * sizeof(char *));
            // Check for successful memory reallocation
            if (!newArgv) {
                printf("Error: Memory allocation failed.\n");
                freeArgv(argv, argc); // Free allocated arguments so far
                free(inputCopy); // Free the input copy
                return; // Exit if memory reallocation fails
            }
            argv = newArgv; // Update argv to point to the resized array
        }
        // Duplicate the current token and store it in argv
        argv[argc] = strdup(token);
        // Check for successful memory allocation for the token
        if (!argv[argc]) {
            printf("Error: Memory allocation failed for argument.\n");
            freeArgv(argv, argc); // Free allocated arguments so far
            free(inputCopy); // Free the input copy
            return; // Exit if memory allocation for token fails
        }
        argc++; // Increment argument count
        token = strtok(NULL, " \n"); // Continue tokenizing
    }
    argv[argc] = NULL; // Ensure argv is NULL-terminated

    // Further process tokenized arguments
    if (argc > 1 && strcmp(argv[0], "secure_file") == 0) {
        handleCommandLineArguments(argc, argv); // Handle specific command
    } else {
        // Provide feedback for unrecognized command
        printf("Command Incomplete. Please see usage below.\n");
        printf("Usage: secure_file -f <filename> [-e | -d] (Encrypt/Decrypt)\n");
    }

    freeArgv(argv, argc); // Free memory allocated for arguments
    free(inputCopy); // Free the duplicated input string
} // End of tokenizeCommandLine

// Parses input command and decides appropriate action
int parseAndExecuteCommand(const char *input) {
    // Check for the help command and display introductory information
    if (strcmp(input, "?\n") == 0) {
        intro();
        // Check for exit or quit commands to terminate the program
    } else if (strncmp(input, "exit\n", 5) == 0 || strncmp(input, "quit\n", 5) == 0) {
        return 0; // Return 0 to signal program termination
        // Handle secure_file command by tokenizing command line arguments
    } else if (strncmp(input, "secure_file", 11) == 0) {
        tokenizeCommandLine(input);
        // For all other inputs, attempt to execute them as system commands
    } else {
        executeSystemCommand(input);
    }
    return 1; // Return 1 to continue the program's execution loop
} // End of parseAndExecuteCommand

// Executes a system command, handling empty or invalid inputs
void executeSystemCommand(const char *command) {
    // Duplicate command to modify and check for errors
    char *cmdCopy = strdup(command);
    if (cmdCopy == NULL) {
        // Notify user of memory allocation issues
        printErrorMessage("Memory allocation failed when trying to execute a system command.");
        return;
    }
    // Search for and remove newline character
    char *newline = strchr(cmdCopy, '\n');
    if (newline) *newline = 0; // Null-terminate command string before newline

    // Ensure command isn't whitespace; if so, prompt user for valid input
    if (strlen(cmdCopy) == 0 || strspn(cmdCopy, " \t") == strlen(cmdCopy)) {
        printf("Unknown command, unable to execute. Type '?' to see the introduction.\n");
    } else {
        // Proceed with system command execution and check for errors
        int status = system(cmdCopy);
        if (status == -1) {
            // Inform user if system command execution fails
            printErrorMessage("Failed to execute system command.");
        } else if (status != 0) {
            printErrorMessage("System command did not execute successfully. Possible Unknown Command.");
            printf("Type '?' to see the introduction again.\n");
        }
    }
    // Cleanup: free dynamically allocated memory for command copy
    free(cmdCopy);
} // End of executeSystemCommand

// Encrypts the content of file using a Caesar cipher
int encryptFile(const char* filename) {
    // Attempt to open file for reading and writing
    FILE *fp = fopen(filename, "r+");
    if (fp == NULL) {
        // Inform user if file cannot be opened
        printf("Error opening file: %s\n", filename);
        printf("No such file or directory.\n\n");
        return -1; // Indicate failure due to file not being accessible
    }

    int ch; // Variable to hold each character read from file
    // Read through each character of file
    while ((ch = fgetc(fp)) != EOF) {
        // Apply Caesar cipher encryption to lowercase letters
        if ('a' <= ch && ch <= 'z') {
            ch = 'a' + (ch - 'a' + SHIFT) % 26;
            // Apply Caesar cipher encryption to uppercase letters
        } else if ('A' <= ch && ch <= 'Z') {
            ch = 'A' + (ch - 'A' + SHIFT) % 26;
        } else if ('0' <= ch && ch <= '9') {
            ch = '0' + (ch - '0' + SHIFT) % 10; // Shift numbers
        }
        // Move back one character to overwrite encrypted character
        fseek(fp, -1, SEEK_CUR);
        // Write encrypted character
        fputc(ch, fp);
        // Flush write operation before next read
        fseek(fp, 0, SEEK_CUR);
    }

    // Close the file after encryption is complete
    fclose(fp);
    return 0; // Indicate success
} // End of encryptFile

// Decrypts content of file using a Caesar cipher
int decryptFile(const char* filename) {
    // Attempt to open file for reading and writing
    FILE *fp = fopen(filename, "r+");
    if (fp == NULL) {
        // Inform user if file cannot be opened
        printf("Error opening file: %s\n", filename);
        printf("No such file or directory.\n\n");
        return -1; // Indicate failure due to file not being accessible
    }

    int ch; // Variable to hold each character read from file
    // Read through each character of file
    while ((ch = fgetc(fp)) != EOF) {
        // Apply Caesar cipher decryption to lowercase letters
        if ('a' <= ch && ch <= 'z') {
            ch = 'a' + (ch - 'a' - SHIFT + 26) % 26;
            // Apply Caesar cipher decryption to uppercase letters
        } else if ('A' <= ch && ch <= 'Z') {
            ch = 'A' + (ch - 'A' - SHIFT + 26) % 26;
        } else if ('0' <= ch && ch <= '9') {
            ch = '0' + (ch - '0' - SHIFT + 10) % 10; // Shift numbers back
        }
        // Move back one character to overwrite decrypted character
        fseek(fp, -1, SEEK_CUR);
        // Write the decrypted character
        fputc(ch, fp);
        // Flush write operation before next read
        fseek(fp, 0, SEEK_CUR);
    }
    // Close file after decryption is complete
    fclose(fp);
    return 0; // Indicate success
} // End of decryptFile

// Outputs an error message to standard output
void printErrorMessage(const char *message) {
    printf("%s\n", message); // Display the provided message
} // End of printErrorMessage
// EOF




