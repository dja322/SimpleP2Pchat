#include "../DataTypes.h"
#include "../UtilityFiles/UtilityFunctions.h"
#include "Contacts.h"
#include "../StandardConstants.h"

void contactMenu()
{
    contact_t contacts[MAX_CONTACTS];
    int contact_count = 0;

    int choice = 3;
    char name[100];
    char address[100];

    const int successCode = loadContactsFile("contacts.dat", contacts, &contact_count);

    if (!successCode)
    {
        printf("No existing contacts found. Starting with an empty contact list.\n");
        FILE* file = fopen("contacts.dat", "w");
        if (file) {
            fclose(file);
        }
    }

    while (1) {
        printf("Contact Menu:\n");
        printf("1. Add Contact\n");
        printf("2. Remove Contact\n");
        printf("3. List Contacts\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");
        choice = getSingleDigitNumericalInput();

        switch (choice) {
            case 1:
                printf("Enter name: ");
                scanf("%s", name);
                printf("Enter address: ");
                scanf("%s", address);
                add_contact(name, address, contacts, &contact_count);
                break;
            case 2:
                printf("Enter name of contact to remove: ");
                scanf("%s", name);
                remove_contact(name, contacts, &contact_count);
                break;
            case 3:
                list_contacts(contacts, contact_count);
                break;
            case 4:
                saveContactsFile("contacts.dat", contacts, contact_count);
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

}

void add_contact(const char* name, const char* address, contact_t* contacts, int* contact_count) {
    // Implementation for adding a contact
    if (*contact_count < MAX_CONTACTS) {
        strncpy(contacts[*contact_count].name, name, sizeof(contacts[*contact_count].name) - 1);
        strncpy(contacts[*contact_count].address, address, sizeof(contacts[*contact_count].address) - 1);
        contacts[*contact_count].name[sizeof(contacts[*contact_count].name) - 1] = '\0';
        contacts[*contact_count].address[sizeof(contacts[*contact_count].address) - 1] = '\0';
        (*contact_count)++;
    } else {
        printf("Contact list is full. Cannot add more contacts.\n");
    }
}

void remove_contact(const char* name, contact_t* contacts, int* contact_count) {
    // Implementation for removing a contact
    for (int i = 0; i < *contact_count; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            // Shift remaining contacts down
            for (int j = i; j < *contact_count - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            (*contact_count)--;
            printf("Contact '%s' removed successfully.\n", name);
            return;
        }
    }
    printf("Contact '%s' not found.\n", name);
}

void list_contacts(contact_t* contacts, int contact_count) {
    // Implementation for listing all contacts
    for (int i = 0; i < contact_count; i++) {
        printf("Contact %d:\n", i + 1);
        printf("Name: %s\n", contacts[i].name);
        printf("Address: %s\n", contacts[i].address);
    }
}

int loadContactsFile(const char* filename, contact_t* contacts, int* contact_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    while (*contact_count < MAX_CONTACTS && fscanf(file, "%99[^,],%99[^\n]\n",
               contacts[*contact_count].name, contacts[*contact_count].address) == 2) {
        contacts[*contact_count].name[99] = '\0';
        contacts[*contact_count].address[99] = '\0';
        (*contact_count)++;
    }

    fclose(file);

    return 1;
}

void saveContactsFile(const char* filename, contact_t* contacts, int contact_count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    for (int i = 0; i < contact_count; i++) {
        fprintf(file, "%s,%s\n", contacts[i].name, contacts[i].address);
    }

    fclose(file);
}
