#include<p18f4550.h>
#include "servos.h"
#include <delays.h>

#pragma config PLLDIV = 5
#pragma config CPUDIV = OSC1_PLL2
#pragma config FOSC = HS            
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config XINST = OFF

int N_SERVOS = 8;
int servoPos[8];
int i = 0;
int posMin = 0;
int posMax = 0;

const char displayCatodo[] = {0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111};

int ExibeServoDisplay(int i) {
    PORTD = displayCatodo[i];
}

void main(void) {

    TRISB = 0;
    TRISC = 1;
    TRISD = 0;
    PORTD = 0;

    SetClockBase(CLOCK_20MHz);
    SetPulseWidth(1000, 2000);
    SetServoScale(0, 180);
    posMin = GetMinPosition();
    posMax = GetMaxPosition();

    InitServos(4, (char) &PORTB);

    MoveAllServos(180);
    Delay10KTCYx(150);
    MoveServoAsync(6, 5);
    Delay10KTCYx(150);
    MoveServoAsync(6, 5);

}
