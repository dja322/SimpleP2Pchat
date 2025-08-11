#include "../UtilityFiles/UtilityFunctions.h"
#include "../UtilityFiles/StringUtils.h"
#include "UserSettings.h"

void userSettingsMenu() {
    
    printf("Settings: \n");
    printf("1. See settings\n");
    printf("2. Change Username\n");
    printf("3. Change Password\n");
    printf("4. Back to Main Menu\n");
    
    int choice = 0;

    settings_t userSettings;
    
    while (1)
    {
        choice = getSingleDigitNumericalInput();

        switch (choice) { 
            case 1:
                // See settings
                printf("Current settings:\n");
                if (loadSettings(&userSettings, "settings.dat") == 1) {
                    printf("Username: %s\n", userSettings.username);
                    printf("Password: %s\n", userSettings.password);
                }
                break;
            case 2:
                // Change Username
                
                break;
            case 3:
                // Change Password
                break;
            case 4:
                // Back to Main Menu
                printf("Returning to Main Menu...\n");
                return;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

int loadSettings(settings_t *settings, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open settings file");
        return 0;
    }

    char line[128];
    char value[64];

    // Initialize struct fields to empty/default
    settings->username[0] = '\0';
    settings->password[0] = '\0';
    settings->server_ip[0] = '\0';
    settings->server_port = 0;

    int LABEL_END = 10;

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        stripTrailingSpaces(line);

        // Username
        if (findSubString(line, "Username") == 0) {
            getSubString(value, line, LABEL_END, getStringLength(line) - 1);
            stripTrailingSpaces(value);
            copyString(settings->username, value);
        }
        // Password
        else if (findSubString(line, "Password") == 0) {
            getSubString(value, line, LABEL_END, getStringLength(line) - 1);
            stripTrailingSpaces(value);
            copyString(settings->password, value);
        }
        // IP
        else if (findSubString(line, "IP") == 0) {
            getSubString(value, line, LABEL_END, getStringLength(line) - 1);
            stripTrailingSpaces(value);
            copyString(settings->server_ip, value);
        }
        // PORT
        else if (findSubString(line, "PORT") == 0) {
            getSubString(value, line, LABEL_END, getStringLength(line) - 1);
            stripTrailingSpaces(value);
            settings->server_port = atoi(value);
        }
    }

    fclose(file);

    return 1;
}

int writeSettings(const settings_t *settings, const char *filename)
{
    // Open file in "w" mode to clear contents before writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open settings file");
        return 0;
    }

    fprintf(file, "Username: %s", settings->username);
    fprintf(file, "Password: %s", settings->password);
    fprintf(file, "IP      : %s", settings->server_ip);
    fprintf(file, "PORT    : %d", settings->server_port);

    fclose(file);

    return 1;
}
