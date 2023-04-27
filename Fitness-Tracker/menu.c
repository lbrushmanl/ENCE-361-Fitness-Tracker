//Includes
//C Libs
#include <stdint.h>
#include <stdio.h>

//Our Libs
#include "menu.h"
#include "display.h"
#include "adcSelector.h"
#include "buttons4.h"

#define STEP_INCREMENT 100
#define KM_TO_MILE 6214
#define LONG_PUSH_THRESHOLD 70

//Test menu values
#define SUB_STEPS 500
#define ADD_STEPS 100
#define SUB_KMS 45
#define ADD_KMS 9

#define MAX_DOWN_PRESS_COUNT 140

//Methods
void clearStepInfo(void);
void stepPercent(void);
void updateValue(uint32_t, uint8_t);

//Global Variables
//Sets of strings used on the display
char screenNames[][15] = {"Step Count", "Total Distance", "Set Goal", "Goal Set", "[GOAL COMPLETE]"};
char units[][5] = {"raw", "%", "km", "mile"};
enum menuElements preivousMenu;

/**
 * Sets up the menus with there names and units
 */
void inltiliseMenu()
{
    element = SC;
    preivousMenu = SG;
    int screenIndex;

    menu[SG].values[1] = 1000;
    clearStepInfo();

    for (screenIndex = SC; screenIndex <= SG; screenIndex++) {
        menu[screenIndex].unitSelect = 0;
        menu[screenIndex].screenName = screenNames[screenIndex];

        if (screenIndex == SG) {
            menu[SG].units[0] = units[0];
            menu[SG].units[1] = units[0];
        } else {
            menu[screenIndex].units[0] = units[2*screenIndex];
            menu[screenIndex].units[1] = units[2*screenIndex + 1];
        }
    }
}


/**
 * A final state machine that is used for navigating the menu system as well as implmenting the test menu
 */
void menuSelect()
{
    static uint8_t count = 0;

    //Test mode
    if (checkButton (SW1) == PUSHED) {

        if (checkButton (UP) == PUSHED && menu[SC].values[0] < sizeof(uint32_t) - ADD_STEPS) {
            updateValue(menu[SC].values[0] + ADD_STEPS, SC);
            updateValue(menu[TD].values[0] + ADD_KMS, TD);
        } else if (checkButton (DOWN) == PUSHED && menu[SC].values[0] > SUB_STEPS) {
            updateValue(menu[TD].values[0] - SUB_KMS, TD);
            updateValue(menu[SC].values[0] - SUB_KMS, SC);
        }
    }

    //Normal mode
    else if (checkButton (DOWN) == PUSHED) {

        if (count != MAX_DOWN_PRESS_COUNT)
            count++;

        visualiseShortLongPush(count/10);

        //Long Push
        if ((count > LONG_PUSH_THRESHOLD) && element != SG) {
            clearStepInfo();
        }

    } else if(checkButton (UP) == PUSHED) {
        menu[element].unitSelect = !(menu[element].unitSelect);

    } else if (checkButton (DOWN) == RELEASED) {
        //Short Push
        clearScreen();
        if (element == SG && (count > 0 && count < LONG_PUSH_THRESHOLD)) {
            menu[element].unitSelect = 0;
            menu[SG].values[1] = menu[SG].values[0];
            stepPercent();
            element = SC;
         }
        count = 0;
    }

    //check Left press
    if (checkButton (RIGHT) == PUSHED) {
        element++;
        if (element > SG)
            element = SC;
    }

    //check Right press
    if (checkButton (LEFT) == PUSHED) {
        element--;
        if (element == OV)
            element = SG;
    }

    //update SG from pot
    if (element == SG) {
        menu[SG].values[0] = (getADCValue() / 40) * ADD_STEPS;
    }

}

void updateValue(uint32_t value, uint8_t element)
{
    menu[element].values[0] = value;

    // Sets the second values for there specific units
    switch (element) {
        case SC:
            menu[element].values[1] = (value*100)/menu[SG].values[1]; //raw to % of total goal
            break;
        case TD:
            menu[element].values[1] = (value * KM_TO_MILE)/10000; //km to miles
            break;
    }
}


/**
 * Clears the step info
 */
void clearStepInfo()
{
    menu[SC].values[0] = 0;
    menu[TD].values[0] = 0;
    menu[SC].values[1] = 0;
    menu[TD].values[1] = 0;
}

void stepPercent()
{
    menu[SC].values[1] = (menu[SC].values[0]*100)/menu[SG].values[1]; //raw to % of total goal
}

/**
 * Increases the step count by one
 */
void stepIncrease()
{
    static uint8_t count = 0;

    menu[SC].values[0]++;
    stepPercent();
    count ++;

    if (count > 10) {
        menu[TD].values[0] += 1;
        menu[TD].values[1] = (menu[TD].values[0] * KM_TO_MILE)/10000; //km to miles
        count = 0;
    }

}

void menuDisplay()
{

    static uint8_t completeFlag = 0;

    if (menu[SC].values[0] >= menu[SG].values[1]) {
            displayMenuTitle(screenNames[GC]);
            completeFlag = 1;
    } else if (menu[SC].values[0] < menu[SG].values[1] && completeFlag == 1) {
        preivousMenu += 1;
        completeFlag = 2;
    } else if (element == SG) {
        if (menu[element].unitSelect == 1)
            displayMenuTitle(screenNames[GS]);
        else
            completeFlag = 2;
    }

    if (preivousMenu != element || completeFlag == 2) {
        preivousMenu = element;
        displayMenuTitle(menu[element].screenName);
        completeFlag = 0;
    }


    displayMenu(menu[element].values[menu[element].unitSelect], menu[element].units[menu[element].unitSelect]);
}
