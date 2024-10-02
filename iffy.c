#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE_LENGTH 256
#define MAX_COMMAND_LENGTH 128
#define MAX_PARAM_LENGTH 256
#define DEFAULT_SCRIPT_PATH "./scripts/"  // Default script path
#define SCRIPT_EXTENSION ".sh"            // Define the script file extension

int execute_command(const char *command, const char *parameter, const char *script_path) {
    char full_script_path[MAX_COMMAND_LENGTH + 50];
    snprintf(full_script_path, sizeof(full_script_path), "%s%s%s", script_path, command, SCRIPT_EXTENSION);

    printf("Executing command: %s with parameter: %s\n", full_script_path, parameter);

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        char *args[] = {strdup(full_script_path), strdup(parameter), NULL};  // Create argument array with full path to script
        execvp(args[0], args);
        perror("execvp failed");  // Print error if exec fails
        exit(127);  // Return a specific exit code if execvp fails
    } else if (pid < 0) {
        perror("fork failed");  // Print error if fork fails
        return 1;
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);  // Wait for the child to finish
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);  // Get the exit status of the child
            printf("Command exited with status: %d\n", exit_code);
            return exit_code;  // Return the actual exit status
        } else {
            printf("Command did not exit normally.\n");
            return 1;  // Return error if the child did not exit normally
        }
    }
}

void parse_and_execute_script(const char *filename, const char *script_path) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char *command1 = NULL;
        char *param1 = NULL;
        char *command2 = NULL;
        char *param2 = NULL;

        char *token = strtok(line, ";");
        if (token != NULL) {
            char *equals_sign = strchr(token, '=');
            if (equals_sign != NULL) {
                command1 = strndup(token, equals_sign - token);  // Store the first command
                param1 = strdup(equals_sign + 1);  // Store the first parameter
                param1[strcspn(param1, "\n")] = '\0';  // Remove newline if any

                // Get second command and parameter
                token = strtok(NULL, ";");
                if (token != NULL) {
                    equals_sign = strchr(token, '=');
                    if (equals_sign != NULL) {
                        command2 = strndup(token, equals_sign - token);  // Store the second command
                        param2 = strdup(equals_sign + 1);  // Store the second parameter
                        param2[strcspn(param2, "\n")] = '\0';  // Remove newline

                        // Now that all parameters are saved, execute them
                        if (execute_command(command1, param1, script_path) == 0) {
                            printf("First command succeeded, executing second command.\n");
                            execute_command(command2, param2, script_path);
                        } else {
                            printf("First command failed, skipping second command.\n");
                        }

                        // Free allocated memory for second command and param
                        free(command2);
                        free(param2);
                    }
                }
            }
            // Free allocated memory for first command and param
            free(command1);
            free(param1);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <script_file>\n", argv[0]);
        return 1;
    }

    // Get the script path from the environment variable or use the default path
    const char *script_path = getenv("SCRIPT_PATH");
    if (!script_path) {
        script_path = DEFAULT_SCRIPT_PATH;
    }

    parse_and_execute_script(argv[1], script_path);
    return 0;
}

