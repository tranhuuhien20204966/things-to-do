#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to store account status
struct Account {
    char username[100];
    int status;
};

// Function to check if an account exists and is active
int checkAccount(struct Account accounts[], int count, char *username) {
	int i ; 
    for (i = 0; i < count; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            if (accounts[i].status == 1) {
                return 1; // Account exists and is active
            } else {
                return 0; // Account exists but is locked
            }
        }
    }
    return -1; // Account does not exist
}

// Function to log activity to a file
void logToFile(FILE *logFile, char *action, char *value, char *result) {
    time_t t;
    struct tm *tm_info;
    char buffer[26];

    time(&t);
    tm_info = localtime(&t);

    strftime(buffer, 26, "[%d/%m/%Y %H:%M:%S] $ ", tm_info);
    fprintf(logFile, "%s%s $ %s $ %s\n", buffer, action, value, result);
}

int main() {
    struct Account accounts[100];
    int accountCount = 0;
    char currentUsername[100] = "";
    char input[100];
    char logFileName[100];

    // Open the account.txt file and read account information
    FILE *accountFile = fopen("account.txt", "r");
    if (accountFile == NULL) {
        printf("Could not open the account.txt file.\n");
        return 1;
    }

    while (fscanf(accountFile, "%s %d", accounts[accountCount].username, &accounts[accountCount].status) != EOF) {
        accountCount++;
    }

    fclose(accountFile);

    // Open the log file
    printf("Enter the log file name (e.g., log_20201234.txt): ");
    scanf("%s", logFileName);

    FILE *logFile = fopen(logFileName, "w");
    if (logFile == NULL) {
        printf("Could not create the log file.\n");
        return 1;
    }

    // Display the menu
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Log in\n");
        printf("2. Post message\n");
        printf("3. Logout\n");
        printf("4. Exit\n");
        printf("Select an option: ");
        scanf("%d", &choice);

        char action[10];
        char result[10];

        switch (choice) {
            case 1:
                if (strlen(currentUsername) == 0) {
                    printf("Username: ");
                    scanf("%s", input);

                    int accountStatus = checkAccount(accounts, accountCount, input);

                    if (accountStatus == 1) {
                        strcpy(currentUsername, input);
                        strcpy(action, "1");
                        strcpy(result, "+OK");
                    } else if (accountStatus == 0) {
                        strcpy(action, "1");
                        strcpy(result, "-ERR");
                    } else {
                        strcpy(action, "1");
                        strcpy(result, "-ERR");
                    }
                } else {
                    strcpy(action, "1");
                    strcpy(result, "-ERR");
                }
                break;
            case 2:
                if (strlen(currentUsername) > 0) {
                    printf("Post message: ");
                    scanf(" %[^\n]", input);

                    strcpy(action, "2");
                    strcpy(result, "+OK");
                } else {
                    strcpy(action, "2");
                    strcpy(result, "-ERR");
                }
                break;
            case 3:
                if (strlen(currentUsername) > 0) {
                    strcpy(currentUsername, "");
                    strcpy(action, "3");
                    strcpy(result, "+OK");
                } else {
                    strcpy(action, "3");
                    strcpy(result, "-ERR");
                }
                break;
            case 4:
                fclose(logFile);
                return 0;
            default:
                strcpy(action, "0");
                strcpy(result, "-ERR");
                break;
        }

        // Log the activity
        logToFile(logFile, action, input, result);

        printf("Result: %s\n", result);
    } while (1);

    return 0;
}

