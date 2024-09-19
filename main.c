/************** ECE2049 DEMO CODE ******************/
/**************  25 August 2021   ******************/
/***************************************************/

#include <msp430.h>
#include <stdlib.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"

// Function Prototypes
void swDelay(char numLoops);

// Declare globals here

// Main
void main(void)

{
    unsigned char currKey=0, dispSz = 3;
    unsigned char dispThree[3];



    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                 // You can then configure it properly, if desired

    // Useful code starts here
    initLeds();

    configDisplay();
    configKeypad();

    // *** Intro Screen ***
    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Write some text to the display
    Graphics_drawStringCentered(&g_sContext, "WELCOME", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "TO", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "SPACE INVADERS", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);

    dispThree[1] = 51;
    unsigned int j = 0;
    unsigned int killCheck = 0;
    unsigned int updateSpeed = 1;
    unsigned int ylocation = 10;
    unsigned char aliens[5];
    unsigned int state = 0;
    unsigned long int count = 0;
    unsigned long int countdown = 0;
    dispThree[0] = ' ';
    dispThree[2] = ' ';
    while(1){
        switch(state){
        case 0: //Welcome Screen
            updateSpeed = 1;
            Graphics_drawStringCentered(&g_sContext, "WELCOME", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "TO", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "SPACE INVADERS", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            currKey = getKey();
            if(currKey == '*'){
                state = 1;
            }
            break;
        case 1: //Countdown
            if(countdown == 0){
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
            }
            if(countdown == 20000){
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                        }
            if(countdown == 40000){
                Graphics_clearDisplay(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                                    }
            if(countdown == 60000){
                Graphics_clearDisplay(&g_sContext);
                state = 2;
                countdown = 0;
            }
            countdown++;
            break;

        case 2: //Generate Aliens
            srand(count);
            ylocation = 10;
            int i = 1;
            while(i < 6){
                aliens[i - 1] = (rand() % 10) + 48;
                dispThree[1] = aliens[i - 1];
                Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 16 * i, ylocation, OPAQUE_TEXT);
                i++;
            }
            Graphics_flushBuffer(&g_sContext);
            count = 0;
            state = 3;
            break;

        case 3: //Game
            if( count == (10000 / updateSpeed)){ //Update aliens
                ylocation = ylocation + 10;
                Graphics_clearDisplay(&g_sContext);
                i = 1;
                while(i < 6){ //Disp aliens
                    dispThree[1] = aliens[i - 1];
                    Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 16 * i, ylocation, OPAQUE_TEXT);
                    i++;
                }
                Graphics_flushBuffer(&g_sContext);
                count = 0;
            }//end update aliens
                currKey = getKey();

                if(currKey){ //checking if key was pressed
                    i = 1;
                    while(i < 6){ //see if an alien has that key
                        if(currKey == aliens[i - 1]){
                            aliens[i - 1] = 10; //kill command
                        }
                        i++;
                    }
                    i = 1;
                    Graphics_clearDisplay(&g_sContext);
                    killCheck = 0;
                    while(i < 6){ //Disp aliens
                        dispThree[1] = aliens[i - 1];
                        Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 16 * i, ylocation, OPAQUE_TEXT);
                        killCheck = killCheck + aliens[i - 1];
                        i++;
                    }
                    Graphics_flushBuffer(&g_sContext);
                    if(killCheck == 50){ //check if all are dead
                        Graphics_clearDisplay(&g_sContext);
                        state = 2; //reset aliens
                        updateSpeed = updateSpeed + 1; //faster fall speed
                    }
                }

                if(ylocation == 100){
                    Graphics_clearDisplay(&g_sContext);
                    state = 4;
                    count = 0;
                }

            count++;
            break;



        case 4://lose the game, reset
                if(count == 1){
                    Graphics_drawStringCentered(&g_sContext, "YOU LOSE", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
                    Graphics_drawStringCentered(&g_sContext, "TRY AGAIN", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT);
                    Graphics_flushBuffer(&g_sContext);
                }
                count++;
                if(count >= 50000){
                    state = 0;
                    Graphics_clearDisplay(&g_sContext);
                }
        break;



        }
    }
}
