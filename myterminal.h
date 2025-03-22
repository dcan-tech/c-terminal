#ifndef MYTERMINAL_H
#define MYTERMINAL_H

#define MAX_COMMAND_LENGTH 256
#define SHIFT 3

void intro();
void handleCommandLineArguments(int argc, char *argv[]);
void tokenizeCommandLine(const char *input);
void readUserInput(char *input);
int parseAndExecuteCommand(const char *input);
void executeSystemCommand(const char *command);
int encryptFile(const char *filename);
int decryptFile(const char *filename);
void printErrorMessage(const char *message);

#endif
