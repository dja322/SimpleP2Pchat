
#include "Networking/Networking.h"
#include "Encryption/RSAOpenSSLEncryption.h"
#include "UtilityFiles/UtilityFunctions.h"
#include "DataTypes.h"
#include "UserActivity/UserSettings.h"
#include "Contacts/Contacts.h"

int main(int argc, char *argv[])
{
    int testEncryption = 0;

    if (testEncryption) {
        demo_encryption(argc, argv);
    }

    settings_t userSettings;

    loadSettings(&userSettings, "settings.dat");

    printf("User Settings Loaded:\n");

    int choice = 0;

    
    while (1)
    {
        printf("Peer-to-Peer Chat Menu:\n");
        printf("1. Connect to peer\n");
        printf("2. Contacts\n");
        printf("3. Settings\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");

        choice = getSingleDigitNumericalInput();

        switch (choice) 
        {
            case 1:
                printf("Connecting to peer...\n");
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