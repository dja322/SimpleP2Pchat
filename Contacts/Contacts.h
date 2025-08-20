#ifndef CONTACTS_H
#define CONTACTS_H

void contactMenu();
void add_contact(const char* name, const char* address, contact_t* contacts, int* contact_count);
void remove_contact(const char* name, contact_t* contacts, int* contact_count);
void list_contacts(contact_t* contacts, int contact_count);
int loadContactsFile(const char* filename, contact_t* contacts, int* contact_count);
void saveContactsFile(const char* filename, contact_t* contacts, int contact_count);

#endif // CONTACTS_H
