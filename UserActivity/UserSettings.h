
#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

void userSettingsMenu();
int loadSettings(settings_t *settings, const char *filename);
int writeSettings(const settings_t *settings, const char *filename);
void initializeSettings(settings_t *settings);


#endif // USER_SETTINGS_H
