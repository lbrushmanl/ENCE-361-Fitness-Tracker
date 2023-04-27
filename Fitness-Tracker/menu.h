#ifndef MENU_H_
#define MENU_H_

//Includes
//C Libs
#include <stdint.h>

//The set of menu screens
enum menuElements{SC, //Step Count
    TD, //Total Distance
    SG,//Set Goal
    GS,//Goal Set
    GC,//Goal Compleat
    OV = 255}; //OVER FLOW

typedef struct {
    uint8_t unitSelect;
    char *units[2];
    char *screenName;
    uint32_t values[2];
} menuScreenInfo;

menuScreenInfo menu[3];

enum menuElements element;

void inltiliseMenu();
void menuSelect();
void stepIncrease();
void updateValue(uint32_t value, uint8_t element);
void menuDisplay();

#endif
