#include "UtilityFunctions.h"


int getSingleDigitNumericalInput()
{
    int choice = -1;
    char input[16];

    //get input
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Input error. Response to possibly to large\n");
        choice = -1;
    }
    
    // Clear remaining input if too large
    if (strchr(input, '\n') == NULL) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        choice = -1;
    }
    
    // Validate input
    if (sscanf(input, "%d", &choice) != 1 || input[strspn(input, "0123456789\n")] != '\0') {
        printf("Invalid input. Please enter a single number.\n");
        choice = -1;
    }

    return choice;
}

FILE* openToReadFile(const char* filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        // File does not exist
        return NULL;
    }
    return file;
}