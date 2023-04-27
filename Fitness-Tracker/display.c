#include <stdint.h>

#include "menu.h"
#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#define TEXT_BUFFER_SIZE 17
#define CLEAR_DISPLAY "                "

// init Display
void initDisplay (void)
{
    // Initialise the Orbit OLED display
    OLEDInitialise ();
}

void displayMenuTitle(char *screenName)
{
    OLEDStringDraw (CLEAR_DISPLAY, 0, 0);
    char title_text_buffer[17];
    usnprintf(title_text_buffer, sizeof(title_text_buffer), "%s", screenName);
    OLEDStringDraw (title_text_buffer, 0, 0);
}

void visualiseShortLongPush(uint8_t timeDown)
{
    char text_buffer[TEXT_BUFFER_SIZE] = CLEAR_DISPLAY;
    uint8_t index;

    for (index = 0; index <= timeDown; index++)
        text_buffer[index] = '#';

    OLEDStringDraw (text_buffer, 0, 2);
    OLEDStringDraw ("SHORT | LONG", 0, 3);
}

void clearScreen()
{
    OLEDStringDraw (CLEAR_DISPLAY, 0, 2);
    OLEDStringDraw (CLEAR_DISPLAY, 0, 3);
}

void displayMenu(uint32_t value, char *units)
{
    char text_buffer[TEXT_BUFFER_SIZE];           //Display fits 16 characters wide.

    OLEDStringDraw (CLEAR_DISPLAY, 0, 1);

    // Display as decimal value or whole
    if (element == TD) {
        uint32_t integer = 0;
        uint16_t fraction = 0;

        integer = value / 100;
        fraction = value % 100;

        usnprintf(text_buffer, sizeof(text_buffer), "%u.%02u %s", integer, fraction, units);
    } else
        usnprintf(text_buffer, sizeof(text_buffer), "%u %s", value, units);

    // Form a new string for the line.  The maximum width specified for the
    //  number field ensures it is displayed right justified.
    // Update line on display.
    OLEDStringDraw (text_buffer, 0, 1);
}
