
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

int loadSettings(settings_t *settings)
{
    FILE *file = openToReadFile("settings.dat");
    if (file == NULL) {
        perror("Failed to open settings file");
        return -1;
    }

    fread(settings, sizeof(settings_t), 1, file);
    fclose(file);

    return 1;
}

void readSettingsFile(FILE *file, settings_t *settings)
{
    if (file == NULL || settings == NULL) {
        fprintf(stderr, "Invalid file or settings pointer\n");
        return;
    }

    fread(settings, sizeof(settings_t), 1, file);
}
