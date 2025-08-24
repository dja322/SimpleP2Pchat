
#include "Networking/Networking.h"
#include "Encryption/RSAEncryption.h"
#include "UtilityFiles/UtilityFunctions.h"
#include "DataTypes.h"
#include "UserActivity/UserSettings.h"
#include "Contacts/Contacts.h"

int main(int argc, char *argv[])
{
    //test encryption if enabled
    if (OUTPUT_ENCRYPT_LOGS) {
        demo_encryption(argc, argv);
    }

    //initialize settings
    settings_t userSettings;

    //load settings, or create settings if file does not exist
    loadSettings(&userSettings, "settings.dat");
    
    int choice = 0;
    
    system(CLEAR);
    
    while (1)
    {
        //output main menu and get user choice
        printf("User Settings Loaded\n");
        printf("Peer-to-Peer Chat Menu:\n");
        printf("1. Connect to peer\n");
        printf("2. Contacts\n");
        printf("3. Settings\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");

        choice = getSingleDigitNumericalInput();

        fflush(stdout);
        
        system(CLEAR);

        switch (choice) 
        {
            case 1:
                printf("Setting up networking...\n");
                runConnection();
                break;
            case 2:
                printf("Viewing contacts...\n");
                contactMenu();
                break;
            case 3:
                printf("Opening settings...\n");
                userSettingsMenu();
                break;
            case 4:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please select a valid option.\n");
                break;
        }
            
            
    }




    return 0;
}