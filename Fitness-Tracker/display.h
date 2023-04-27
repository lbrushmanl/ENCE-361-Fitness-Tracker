#ifndef DISPLAY_H_
#define DISPLAY_H_

void initDisplay (void);
void displayMenuTitle(char *screenName);
void visualiseShortLongPush(uint8_t timeDown);
void clearScreen();
void displayMenu(uint32_t value, char *units);

#endif
