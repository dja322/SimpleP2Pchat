#include <stdio.h>
#include "Networking.h"


int main(int argc, char *argv[])
{

    int choice = 0;

    printf("Peer-to-Peer Chat Menu:\n");
    printf("1. Connect to peer\n");
    printf("2. Login\n");
    printf("3. Register\n");
    printf("4. Settings\n");
    printf("5. Exit\n");
    printf("Enter your choice (1-5): ");

    while (1)
    {

        scanf("%d", &choice);
        clear_input_buffer();  // Clear the input buffer
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