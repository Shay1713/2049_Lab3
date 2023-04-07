#include <msp430.h>
#include "peripherals.h"
#include <stdio.h>
#include <stdlib.h>


// Declare globals here
unsigned char currKey=0;
unsigned int length = 32;
unsigned int currentLength = 4;
unsigned int currentIndex = 0;
int arrayIndex = 0;
char note1;
char note2;
int count = 0;;
int i = 0;
//unsigned char dispSz = 0;

// Functions
void swDelay2(char numLoops);
void sendSimon(int sequence, int length);
int loadSequence(int sequence, int count);
void printNum(char input);
void printNum(char input);
void countDown(void);
void time2(int numLoops2, int c);
int ledDecimal(int binaryInput);
void S1toS4toLEDs(void);


// TIMER FUNCTIONS
void swDelay(char numLoops);
void startingA2Timer(void);
void stoppingA2Timer(void);
void displayingA2Timer(unsigned long curr_time);
void timerA2Function();
void timerA2CountDown(void);
void tellTheNote(char input);
volatile unsigned long timer = 0;
char button_state;
unsigned long elapsed_time;
char timer_on;
unsigned long start_time;
unsigned long current_time;
int sec;
int min;
int total_dec;
int total_sec;

unsigned long start_time = 0;
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void)
{
    timer++;
    if ((timer % 50) == 0) { // blinking LED
        P1OUT ^= BIT0;
    }
}

char sequence[36] = {'d', 'b', 'b', 'c', 'b', 'b', 'a', 'a', 'd', 'd', 'a', 'a', 'a', 'b', 'a', 'a', 'g', 'g', 'd', 'b', 'b', 'b', 'c', 'b', 'a', 'a', 'd', 'd', 'a', 'a', 'a', 'b', 'a', 'a', 'g', 'g'};


enum GAME_STATE {WELCOME, STARTING, SEQUENCE, SONGCHOICE, GAMEOVER, RESTART, WIN, TEST};

// Main
void main(void)

{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
                                     // You can then configure it properly, if desired

    // Useful code starts here
    initLeds();
    configDisplay();
    configKeypad();

    Graphics_clearDisplay(&g_sContext); // Clear the display
    Graphics_flushBuffer(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, "HERO", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT); // instructing player
    Graphics_drawStringCentered(&g_sContext, "Press '*'", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT); // instructing player
    Graphics_flushBuffer(&g_sContext);



    // Configure LED P1.0
    P1SEL &= ~BIT0;
    P1DIR |= BIT0;
    startingA2Timer();
    _enable_interrupts(); // Enables global interrupts


    enum GAME_STATE state = WELCOME;

    while (1)
    {
        unsigned long current_time = timer;
        elapsed_time = current_time - start_time;

        // Check if any keys have been pressed on the 3x4 keypad
        currKey = getKey();

        if (currKey == '#') { // to restart game at any time
            state = RESTART;
        }

        switch (state) {

            case TEST:
                S1toS4toLEDs();
            break;

            case WELCOME:
                BuzzerOff();
                setLeds(0);
                currKey = getKey();
                Graphics_drawStringCentered(&g_sContext, "HERO", AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT); // instructing player
                Graphics_drawStringCentered(&g_sContext, "Press '*'", AUTO_STRING_LENGTH, 48, 25, TRANSPARENT_TEXT); // instructing player
                Graphics_flushBuffer(&g_sContext);
                if (currKey == '*') {
                    state = SONGCHOICE;
                }
            break;

            case SONGCHOICE:
                Graphics_clearDisplay(&g_sContext);
                Graphics_flushBuffer(&g_sContext);
                BuzzerOn();
                Graphics_drawStringCentered(&g_sContext, "Song Choices", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "Today Are ...", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext);
                swDelay2(5);
                BuzzerOff();
                Graphics_clearDisplay(&g_sContext); // Clear the display
                Graphics_flushBuffer(&g_sContext);
                BuzzerOn();
                Graphics_drawStringCentered(&g_sContext, "Hush Little", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT); // instructing player
                Graphics_drawStringCentered(&g_sContext, "Baby", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT); // instructing player
                Graphics_flushBuffer(&g_sContext);
                swDelay2(4);
                BuzzerOff();
                Graphics_clearDisplay(&g_sContext); // Clear the display
                Graphics_flushBuffer(&g_sContext);
                start_time = timer;
                state = STARTING;
            break;

            case STARTING:
                printf("Count Down Begins\n");
                countDown();
                Graphics_clearDisplay(&g_sContext); // Clear the display
                Graphics_flushBuffer(&g_sContext);
                arrayIndex = 36; // resetting for the sequence state
                count = 0;
                state = SEQUENCE;

            break;

            case SEQUENCE:
                printf("Made It To Sequence\n");
                for (i = 0; i < arrayIndex; i++) {
                    Graphics_clearDisplay(&g_sContext); // Clear the display
                    Graphics_flushBuffer(&g_sContext);
                    setLeds(0);
                    timer = 0;
                    timer_on = 0;
                    while(1){
                        timer_on = 1;
                        if (timer_on) {
                            if ((timer % 10) == 0) {
                                displayingA2Timer(timer);
                            }
                            if ((sec == 1) && (total_dec == 10)){ // playing the note
                                tellTheNote(i);
                                S1toS4toLEDs();
                                printf("note1 is %c and note2 is %c\n", note1, note2);
                                if ((note1 == '1' | '2' | '3' | '4') && (note2 == '1' | '2' | '3' | '4')){
                                    break;
                                }
                            }
                        }
                    } // end while loop 1
                    timer = 0;
                    timer_on = 0;
                    while(1){
                        timer_on = 1;
                        if (timer_on) {
                            if ((timer % 10) == 0) {
                                displayingA2Timer(timer);
                            }
                            if (((sec == 0) && (total_dec == 0)) || ((sec == 0) && (total_dec == 10))){ // playing the note
                                if (note1 != note2){
                                     count = 12;
                                     printf("BAD ROUND\n");
                                     break;
                                 } else if (note1 == note2){
                                     count = 0;
                                     printf("good round\n");
                                     break;
                                 }
                            }
                        }
                    } // end of while loop2
                    if (count == 0){
                        printf("good round\n");
                    } else if (count == 12){
                        printf("GAMEOVER");
                        break;
                    }

                } // end of for loop

                if (count == 0){
                    printf("YOU WON\n");
                    state = WIN;
                } else if (count == 12){
                    printf("YOU LOSE");
                    state = GAMEOVER;
                } // the user pressed all buttons correctly


            break;

            case GAMEOVER:
                Graphics_clearDisplay(&g_sContext);
                Graphics_flushBuffer(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "GAME OVER", AUTO_STRING_LENGTH, 50, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "YOU LOSE", AUTO_STRING_LENGTH, 50, 45, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext); // refresh
                swDelay2(5);
                Graphics_clearDisplay(&g_sContext);
                Graphics_flushBuffer(&g_sContext);
                state = WELCOME;
            break;

            case RESTART:
                Graphics_clearDisplay(&g_sContext);
                Graphics_flushBuffer(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "RESTART", AUTO_STRING_LENGTH, 50, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "PROCESSING", AUTO_STRING_LENGTH, 50, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "...", AUTO_STRING_LENGTH, 50, 45, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext); // refresh
                setLeds(0);
                swDelay2(4);
                Graphics_clearDisplay(&g_sContext);
                Graphics_flushBuffer(&g_sContext);
                state = WELCOME;
           break;

           case WIN:
                Graphics_clearDisplay(&g_sContext);
                Graphics_flushBuffer(&g_sContext);
                Graphics_drawStringCentered(&g_sContext, "CONGRATS", AUTO_STRING_LENGTH, 50, 25, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "YOU WON", AUTO_STRING_LENGTH, 50, 35, TRANSPARENT_TEXT);
                Graphics_drawStringCentered(&g_sContext, "HERO", AUTO_STRING_LENGTH, 50, 45, TRANSPARENT_TEXT);
                Graphics_flushBuffer(&g_sContext); // refresh
                for (count = 9000; count > 0; count--){
                    setLeds('8' - 0x30);
                    setLeds('4' - 0x30);
                    setLeds('2' - 0x30);
                    setLeds('1' - 0x30);
                }
                setLeds(0);
                Graphics_clearDisplay(&g_sContext);
                Graphics_flushBuffer(&g_sContext);
                state = WELCOME;
           break;
        }


    }  // end while (1)
}


// FUNCTIONS START HERE


void countDown() {
    timerA2CountDown();
    BuzzerOff();
    setLeds(0);
}

void printNum(char input) {
    unsigned char dispThree[3];
    unsigned char dispSz = 3;
    dispThree[0] = ' ';
    dispThree[2] = ' ';

    if ((input >= '1') && (input <= '4')) {
        dispThree[1] = input;
    }
    if (input == '1') {
        Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 12, 55, OPAQUE_TEXT);

    } else if (input == '2') {
        Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 36, 55, OPAQUE_TEXT);

    } else if (input == '3') {
        Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 60, 55, OPAQUE_TEXT);

    } else if (input == '4') {
        Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 84, 55, OPAQUE_TEXT);
    }
    Graphics_flushBuffer(&g_sContext);

}


void swDelay2(char numLoops)
{
    volatile unsigned int i,j;  // volatile to prevent removal in optimization
                                // by compiler. Functionally this is useless code
    for (j=0; j<numLoops; j++)
    {
        i = 50000 ;                 // SW Delay
        while (i > 0)               // could also have used while (i)
           i--;
    }
}

int ledDecimal(int binaryInput){
    if (binaryInput == 1) {
        return 8;
    }
    if (binaryInput == 2){
        return 4;
    }
    if (binaryInput == 3){
        return 2;
    }
    if (binaryInput == 4){
        return 1;
    } return 0;
}

// A2 TIMER FUNCTIONS

void startingA2Timer(void){
    TA2CTL = TASSEL_1 | ID_0 | MC_1; // ACLK, Divide by 1, Up mode
    TA2CCR0 = 160;                   // 160 + 1 ACLK ticks = ~0.0049s
    TA2CCTL0 = CCIE;                 // Enable capture/compare interrupt
}
void stoppingA2Timer(void){
    TA2CTL = MC_0;     // Stop mode
    TA2CCTL0 &= ~CCIE; // Disable interrupts
}

void displayingA2Timer(unsigned long curr_time){
    // First, get each portion of the display using integer math
    total_sec =  curr_time / 100; // Seconds (integer part)
    total_dec = curr_time % 100; // Hundredths (fractional part)
    min = total_sec / 60; // Minutes
    sec = total_sec % 60; // Seconds
    //unsigned char str[9]; // mm:ss.hh = 8 chars + null terminator
    //printf(str, 9, "%02d:%02d.%02d", min, sec, total_dec);
    printf("minutes %d seconds %d total_dec %d\n", min, sec, total_dec);
    //Graphics_drawStringCentered(&g_sContext, str, 9, 48, 15, OPAQUE_TEXT);
}

void timerA2Function(){
    timer_on = 0; // for A2 timer
    start_time = 0; // for A2 timer
    currKey = getKey();
    if (currKey == '1') {
        timer_on = 1;
        start_time = timer; // Record timer value at start
    }
    if (currKey == '2') {
        timer_on = 0;
    }
    if (timer_on) {
        current_time = timer;
        elapsed_time = current_time - start_time;
        if ((timer % 10) == 0) {
            displayingA2Timer(elapsed_time);
            Graphics_flushBuffer(&g_sContext);
        }
    }
}


void timerA2CountDown(void){
    timer = 0;
    timer_on = 0;
    while(1){
        timer_on = 1;
        start_time = timer;
        if (timer_on) {
            current_time = timer;
            elapsed_time = current_time - start_time;
            if ((timer % 10) == 0) {
                displayingA2Timer(current_time);
            }
        }
        if((sec == 0) && (total_dec == 0)){
            setLeds('8' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "3", AUTO_STRING_LENGTH, 20, 55, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
        }
        if((sec == 1) && (total_dec == 40)){
            setLeds('4' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "2", AUTO_STRING_LENGTH, 40, 55, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            break;
         }
    }
    timer = 0;
    timer_on = 0;
    while(1){
            timer_on = 1;
            start_time = timer;
            if (timer_on) {
                current_time = timer;
                elapsed_time = current_time - start_time;
                if ((timer % 10) == 0) {
                    displayingA2Timer(current_time);
                }
            }
            if((sec == 0) && (total_dec == 90)){
               setLeds('2' - 0x30);
               Graphics_drawStringCentered(&g_sContext, "1", AUTO_STRING_LENGTH, 60, 55, TRANSPARENT_TEXT);
               Graphics_flushBuffer(&g_sContext);
               break;
            }
    }
    timer = 0;
        timer_on = 0;
        while(1){
                timer_on = 1;
                start_time = timer;
                if (timer_on) {
                    current_time = timer;
                    elapsed_time = current_time - start_time;
                    if ((timer % 10) == 0) {
                        displayingA2Timer(current_time);
                    }
                if((sec == 1) && (total_dec == 40)){
                   setLeds('1' - 0x30);
                   Graphics_drawStringCentered(&g_sContext, "GO", AUTO_STRING_LENGTH, 80, 55, TRANSPARENT_TEXT);
                   Graphics_flushBuffer(&g_sContext);
                   break;
                }
                if((sec == 2) && (total_dec == 40)){
                    setLeds(0);
                    break;
                }
        }
        }
}

void tellTheNote(char input) {
    while(1){
        if (sequence[input] == 'a') {
            BuzzerOnControl(74);
            setLeds('8' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "a", AUTO_STRING_LENGTH, 20, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '1'; // for me the note is 1
            break;
        } if (sequence[input] == 'B') { // b flat
            BuzzerOnControl(70);
            setLeds('8' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "B", AUTO_STRING_LENGTH, 60, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '1'; // for me the note is 1
            break;
        } if (sequence[input] == 'b') {
            BuzzerOnControl(66);
            setLeds('8' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "b", AUTO_STRING_LENGTH, 20, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '1'; // for me the note is 1
            break;
        } if (sequence[input] == 'c') {
            BuzzerOnControl(63);
            setLeds('4' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "c", AUTO_STRING_LENGTH, 40, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '2'; // for me the note is 2
            break;
        } if (sequence[input] == 'C') {
            BuzzerOnControl(59);
            setLeds('4' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "C", AUTO_STRING_LENGTH, 40, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '2'; // for me the note is 2
            break;
        } if (sequence[input] == 'd') {
            BuzzerOnControl(56);
            setLeds('4' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "d", AUTO_STRING_LENGTH, 40, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '2'; // for me the note is 2
            break;
        } if (sequence[input] == 'e') {
            BuzzerOnControl(53);
            setLeds('2' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "e", AUTO_STRING_LENGTH, 60, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '3'; // for me the note is 3
            break;
        } if (sequence[input] == 'F') { // f sharp
            BuzzerOnControl(44);
            setLeds('2' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "F", AUTO_STRING_LENGTH, 60, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '3'; // for me the note is 3
            break;
        } if (sequence[input] == 'f') {
            BuzzerOnControl(47);
            setLeds('2' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "f", AUTO_STRING_LENGTH, 60, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '3'; // for me the note is 3
            break;
        } if (sequence[input] == 'g') {
            BuzzerOnControl(42);
            setLeds('1' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "g", AUTO_STRING_LENGTH, 80, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '4'; // for me the note is 4
            break;
        } if (sequence[input] == 'Z') { // A flat
            BuzzerOnControl(39);
            setLeds('1' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "A", AUTO_STRING_LENGTH, 80, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '4'; // for me the note is 4
            break;
        }   if (sequence[input] == 'A') { // A2
            BuzzerOnControl(37);
            setLeds('1' - 0x30);
            Graphics_drawStringCentered(&g_sContext, "A", AUTO_STRING_LENGTH, 80, 50, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            note1 = '4'; // for me the note is 4
            break;
        }
    }
}


void S1toS4toLEDs(void) // Me setting the buttons to light the LEDs
{
    while(1){
        currKey = getKey();
            if (currKey == '1') { // S1
                printf("button %c pressed and note2 is %c\n", currKey, note2);
                setLeds('8' - 0x30);
                note2 = '1';
                break;
            }
            if (currKey == '2') { // S2
                printf("button %c pressed and note2 is %c\n", currKey, note2);
                setLeds('4' - 0x30);
                note2 = '2';
                break;
            }
            if (currKey == '3') { // S3 pressed
                printf("button %c pressed and note2 is %c\n", currKey, note2);
                setLeds('2' - 0x30);
                note2 = '3';
                break;
            }
            if (currKey == '4') { // S4 pressed
                printf("button %c pressed and note2 is %c\n", currKey, note2);
                setLeds('1' - 0x30);
                note2 = '4';
                break;
            }
            if (currKey == '7') {
                printf("button %c pressed\n", currKey);
                setLeds(0);
                break;
            }
    }
}
