#include <stdio.h>
#include <string.h>
#include "Networking.h"
#include "UtilityFunctions.h"

int main(int argc, char *argv[])
{

    int choice = 0;

    printf("Peer-to-Peer Chat Menu:\n");
    printf("1. Connect to peer\n");
    printf("2. Login\n");
    printf("3. Register\n");
    printf("4. Settings\n");
    printf("5. Exit\n");
    
    while (1)
    {
        printf("Enter your choice (1-5): ");

        choice = getSingleDigitNumericalInput();
        // char input[16];

        // //get input
        // if (!fgets(input, sizeof(input), stdin)) {
        //     printf("Input error. Response to possibly to large\n");
        //     continue;
        // }

        // // Clear remaining input if too large
        // if (strchr(input, '\n') == NULL) {
        //     int ch;
        //     while ((ch = getchar()) != '\n' && ch != EOF);
        // }

        // // Validate input
        // if (sscanf(input, "%d", &choice) != 1 || input[strspn(input, "0123456789\n")] != '\0') {
        //     printf("Invalid input. Please enter a single number.\n");
        //     continue;
        // }

        switch (choice) 
        {
            case 1:
                printf("Connecting to peer...\n");
                break;
            case 2:
                printf("Logging in...\n");
                break;
            case 3:
                printf("Registering...\n");
                break;
            case 4:
                printf("Opening settings...\n");
                break;
            case 5:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please select a valid option.\n");
                break;
        }
            
            
    }




    return 0;
}