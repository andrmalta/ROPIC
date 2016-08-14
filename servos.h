/*******************************************************************************
 *                              SERVOS CONTROL
 *******************************************************************************
 * Authors:         André Malta
 * File:            servos.h
 * Dependences:     PIC18x.h
 * Processor:       PIC18
 * Compiler:        MCC18
 * OSCCON:          16Mhz,tcy=0.25us
 * Created on 23 de Junho de 2016
 ******************************************************************************/
#ifndef SERVOS_H
#define	SERVOS_H

/*******************************************************************************
 *                          CLOCKS(MIN TIME TICKus)
 ******************************************************************************/
#define CLOCK_16MHz 1
#define CLOCK_20MHz  0.8
/*****************************************************************************/

/*******************************************************************************
 *                           GLOBAL INTERRUPTION
 ******************************************************************************/
#define GLOBAL_INTERRUPT INTCONbits.GIE
#define PERIPHERALS_INTERRUPT  INTCONbits.PEIE
/*****************************************************************************/

/*******************************************************************************
 *                               EXCEPTIONS
 ******************************************************************************/
#define EXCEPTION_CLOCK_NOT_SUPPORTED "This clock can not be used. Please use the definitions of CLOCK_16MHz or CLOCK_20MHZ";
#define EXCEPTION_PWM_WRONG "Wrong values to define pulse width modulation"
#define EXCEPTION_SERVOS_EXCEDED "Number of servos out of bound"
#define EXCEPTION_DEGREE_EXCEDED "Positions out of bound"
#define EXCEPTION_OUTPUT_NOT_SUPPORTED "This output can not be used. Please use: PORTB or PORTD"
/******************************************************************************/

/*******************************************************************************
 *                                 TIMER 1
 ******************************************************************************/
#define TIMER_1 T1CON
#define TIMER_1_INTERRUPT PIE1bits.TMR1IE
#define TIMER_1_OVERFLOW PIR1bits.TMR1IF
/******************************************************************************/

/*******************************************************************************
 *                                 SERVOS
 ******************************************************************************/
#define SERVO1_B PORTBbits.RB0
#define SERVO2_B PORTBbits.RB1
#define SERVO3_B PORTBbits.RB2
#define SERVO4_B PORTBbits.RB3
#define SERVO5_B PORTBbits.RB4
#define SERVO6_B PORTBbits.RB5
#define SERVO7_B PORTBbits.RB6
#define SERVO8_B PORTBbits.RB7

#define SERVO1_D PORTDbits.RD0
#define SERVO2_D PORTDbits.RD1
#define SERVO3_D PORTDbits.RD2
#define SERVO4_D PORTDbits.RD3
#define SERVO5_D PORTDbits.RD4
#define SERVO6_D PORTDbits.RD5
#define SERVO7_D PORTDbits.RD6
#define SERVO8_D PORTDbits.RD7
/******************************************************************************/
void SetClockBase(double CLOCK_BASE);
void SetPulseWidth(int minPulse_us, int maxPulse_us);
void SetServoScale(int min, int max);
void InitServos(unsigned short nServos, char port);
void MoveServoAsync(char servo_number, int degree);
void MoveAllServos(int degree);
int GetServoPosition(char servo_number);
int GetMinPosition();
int GetMaxPosition();

#endif	/* SERVOS_H */

