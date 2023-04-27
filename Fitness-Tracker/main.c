//Includes
//C Libs
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

//Arm Libs
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "inc/hw_ints.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"

//Our Libs
#include "buttons4.h"
#include "readAccelerometer.h"
#include "display.h"
#include "adcSelector.h"
#include "menu.h"

//Systick configuration
#define SAMPLE_RATE_HZ  100
#define ACC_UPDATE_RATE 5
#define SCREEN_UPDATE_RATE 20

#define UPPER_THRESHOLD 35
#define LOWER_THRESHOLD 5

//Methods
void initClock (void);
void initSysTick(void);
void initTimerInterupts(void);
void fillBuffers(void);
void checkStep(void);

//Global Variables
uint8_t getAcclFlag = 0;
uint8_t displayUpdate = 0;
uint8_t pollButton = 0;
vector3_t acceleration;
int32_t meanMagnitude = 0;

/**
 * main.c
 */
int main(void)
{
    // Initialise
    initClock();
    initSysTick();
    //initTimerInterupts();
    initDisplay();
    inltiliseMenu();
    initADC();

    // Initialise Accl
    initAccl();

    //initialise button
    initButtons();

    fillBuffers();

    // Enable interrupts to the processor
    IntMasterEnable();

    //Main loop
    while (1)
    {
        if (getAcclFlag) {
            getAcclFlag = 0;
            checkStep();
        }

        if (pollButton) {
            pollButton = 0;
            updateButtons();
            menuSelect();
        }

        if (displayUpdate) {
            displayUpdate = 0;
            menuDisplay();
        }
    }

    return 0;
}




void fillBuffers()
{
    uint8_t i = 0;
    for (i = 0; i < 10; i++) {
        getAcclData(&acceleration);
    }

    meanMagnitude = sqrt((acceleration.x * acceleration.x) + (acceleration.y * acceleration.y) + (acceleration.z * acceleration.z));
}

void checkStep()
{
    static uint8_t passThreshold = 0;
    static uint32_t magnitude = 0;
    static int32_t magDelta = 0;

    getAcclData(&acceleration);
    magnitude = sqrt((acceleration.x * acceleration.x) + (acceleration.y * acceleration.y) + (acceleration.z * acceleration.z));

    magDelta = magnitude - meanMagnitude;

    if (magDelta > UPPER_THRESHOLD)
        passThreshold = 1;
    else if (magDelta < LOWER_THRESHOLD && passThreshold == 1) {
        stepIncrease();
        passThreshold = 0;
    } else if (magDelta < 5)
        passThreshold = 0;
}

// Interup for determning when to do tasks
void SysTickIntHandler(void)
{
    static uint8_t displayUpdateFreq = 0;
    static uint8_t accUpdateFreq = 0;

    pollButton = 1;


    if (++accUpdateFreq == ACC_UPDATE_RATE) {
        getAcclFlag = 1;
        accUpdateFreq = 0;
    }

    if (++displayUpdateFreq == SCREEN_UPDATE_RATE) {
        displayUpdate = 1;
        displayUpdateFreq = 0;
    }

    if (element == SG)
        ADCProcessorTrigger(ADC0_BASE, 3);
}

void initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}

void initSysTick (void)
{
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}
