#include<timers.h>
#include "servos.h"

typedef struct {
    unsigned Degree;
    char Enable : 1;
} Servo;

static double CLOCK_MIN_TICK = CLOCK_20MHz;
static short CORRECTION_TIME = 101;
static Servo servos[8];
static unsigned short N_SERVOS;
static int MIN_POS = 1000; //1ms
static int MAX_POS = 2000; //2ms
static unsigned int FrameServo = 2500; //20ms:8servos = 2.5ms
static unsigned int MinPulseServo = 1000; //1ms
static unsigned int MaxPulseServo = 2000; //2ms
static double ConversionFactor = 0;
static short Pulso = 0;
static char PORT;

/******************************************************************************/
/*                          INTERRUPTION DECLARATION
 ******************************************************************************/
void Interruption();
void ServoInterruption();
#pragma code high_vector=0x08

void interrupt_at_high_vector() {
    _asm GOTO Interruption _endasm
}
#pragma code
#pragma interrupt Interruption
/******************************************************************************/

/*******************************************************************************
 *                             Control Output
 ******************************************************************************/
void Output(char servo_number, unsigned short val) {
    if (PORT == (char) &PORTB) {
        switch (servo_number) {
            case 0:
                SERVO1_B = val;
                break;
            case 1:
                SERVO2_B = val;
                break;
            case 2:
                SERVO3_B = val;
                break;
            case 3:
                SERVO4_B = val;
                break;
            case 4:
                SERVO5_B = val;
                break;
            case 5:
                SERVO6_B = val;
                break;
            case 6:
                SERVO7_B = val;
                break;
            case 7:
                SERVO8_B = val;
                break;
        }
    } else if (PORT == (char) &PORTD) {
        switch (servo_number) {
            case 0:
                SERVO1_D = val;
                break;
            case 1:
                SERVO2_D = val;
                break;
            case 2:
                SERVO3_D = val;
                break;
            case 3:
                SERVO4_D = val;
                break;
            case 4:
                SERVO5_D = val;
                break;
            case 5:
                SERVO6_D = val;
                break;
            case 6:
                SERVO7_D = val;
                break;
            case 7:
                SERVO8_D = val;
                break;
        }
    } else {
        return EXCEPTION_OUTPUT_NOT_SUPPORTED;
    }
}
/******************************************************************************/

/*******************************************************************************
 *                                 Servos
 ******************************************************************************/
void SetClockBase(double CLOCK_BASE) {
    if (CLOCK_BASE == CLOCK_16MHz) {
        CORRECTION_TIME = 117; //0.101ms
    } else if (CLOCK_BASE == CLOCK_20MHz) {
        CORRECTION_TIME = 117; //0.101ms
    } else {
        return EXCEPTION_CLOCK_NOT_SUPPORTED;
    }

    CLOCK_MIN_TICK = CLOCK_BASE;
}

void SetPulseWidth(int minPulse_us, int maxPulse_us) {
    if (minPulse_us <= 0 || maxPulse_us <= minPulse_us)
        return EXCEPTION_PWM_WRONG;

    MinPulseServo = minPulse_us;
    MaxPulseServo = maxPulse_us;
    MIN_POS = minPulse_us;
    MAX_POS = maxPulse_us;
}

void SetServoScale(int min, int max) {
    int temp = 0;
    MIN_POS = min;
    MAX_POS = max;
    if (min >= 0 && max > min) {
        temp = max - min;
    } else {
        MIN_POS = MinPulseServo;
        MAX_POS = MaxPulseServo;
    }

    ConversionFactor = (double) temp / (MaxPulseServo - MinPulseServo);
}

int GetServoPosition(char servo_number) {
    Servo *s;
    s = &servos[servo_number - 1];
    return (int) ((ConversionFactor == 0) ? (s->Degree * CLOCK_MIN_TICK) : (ConversionFactor * ((s->Degree * CLOCK_MIN_TICK) - MinPulseServo)));
}

int GetMinPosition() {
    return MIN_POS;
}

int GetMaxPosition() {
    return MAX_POS;
}

void InitServos(unsigned short nServos, char port) {
    Servo *s;
    int i;

    PORT = port;
    N_SERVOS = nServos;
    for (i = 0; i < N_SERVOS; i++) {

        s = &servos[i];
        s->Degree = 0;
        s->Enable = 0;
    }

    //Interruption Enable
    GLOBAL_INTERRUPT = 1;
    PERIPHERALS_INTERRUPT = 1;
    TIMER_1_INTERRUPT = 1;

    //Configuration of Frame
    FrameServo = (int) FrameServo / CLOCK_MIN_TICK;

    //Configuration Timer1
    WriteTimer1(65535 - FrameServo);
    TIMER_1 = 0b00100001; //Prescaler 1:4
}

void MoveServoAsync(char servo_number, int degree) {
    Servo *s;

    if (servo_number > N_SERVOS || servo_number <= 0)
        return EXCEPTION_SERVOS_EXCEDED;

    if (degree < MIN_POS || degree > MAX_POS)
        return EXCEPTION_DEGREE_EXCEDED;

    s = &servos[servo_number - 1];
    s->Degree = (int) (((ConversionFactor == 0) ? degree : ((degree / ConversionFactor) + MinPulseServo)) / CLOCK_MIN_TICK);
    s->Enable = 1;
}

void MoveAllServos(int degree) {
    Servo *s;
    int i;

    if (degree < MIN_POS || degree > MAX_POS)
        return EXCEPTION_DEGREE_EXCEDED;

    for (i = 0; i < N_SERVOS; i++) {
        s = &servos[i];
        s->Degree = (int) (((ConversionFactor == 0) ? degree : ((degree / ConversionFactor) + MinPulseServo)) / CLOCK_MIN_TICK);
        s->Enable = 1;
    }
}
/******************************************************************************/

/*******************************************************************************
 *                               SERVO INTERRUPTION
 ******************************************************************************/
void ServoInterruption() {
    static char servo_number = 0;
    static Servo *s;

    s = &servos[servo_number];

    if (s->Enable) {
        if (!Pulso) {
            WriteTimer1(65535 - s->Degree + CORRECTION_TIME);
            Output(servo_number, 1);
        } else {
            WriteTimer1((65535 - FrameServo) + s->Degree + CORRECTION_TIME);
            Output(servo_number, 0);
            servo_number = (servo_number + 1)&7;
        }
        Pulso = ~Pulso;
    } else {

        WriteTimer1(65535 - FrameServo);
        servo_number = (servo_number + 1)&7;
    }
}

void Interruption() {
    if (TIMER_1_OVERFLOW) {
        ServoInterruption();
        TIMER_1_OVERFLOW = 0;
    }
}
/******************************************************************************/
