/* 
 * File:   Functions.h
 * Author: RyanRichey
 *
 * Created on November 23, 2015, 4:34 PM
 */

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H


#include <stdio.h>
#include <stdlib.h>
#include <p24F16KA301.h>
#include "PIN_DEF.h"




extern int backupCount;
//extern int delayCount;
extern int turnCount;
extern int turnClicks;
extern int inchClicks;
extern int ballsCollected;
extern int previousTurnDuration;


void config_ad(void);
void config_pins(void);
void forwardInches(int inches);
void backupInches(int inches);
void delay(unsigned long delay);

//char getIRState(Direction facing);
//char checkSeeFlashingIR();



void config_ad(void)
{
// Configure Timer1
    T1CONbits.TON = 1; // Turns timer 1 on
    T1CONbits.TCS = 0; // Select internal Clock
    T1CONbits.TCKPS = 0b10; // Set prescale Value to 1/64th
    PR1 = FAST; // Set timer 1 period to 125 instruction cycles which should cause 500hz signal
    TMR1 = 0; // Set timer 1 to zero
    
    T2CONbits.TON = 1; // Turns timer 1 on
    T2CONbits.TCS = 0; // Select internal Clock
    T2CONbits.T32 = 0; // act as 16 bit, not 32 bit.
    T2CONbits.TCKPS = 0b11; // Set prescale Value to 1/256th
    PR2 = 312;//7812; // Set timer 1 period to 312 instruction cycles for 50hz wave
    TMR2 = 0; // Set timer 1 to zero
    
    T3CONbits.TON = 1; // Turns timer 3 on// use timer three for arena time.
    T3CONbits.TCS = 0; // Select internal Clock
    //T3CONbits.T32 = 0; // act as 16 bit, not 32 bit.
    T3CONbits.TCKPS = 0b11; // Set prescale Value to 1/256th
    PR3 = 15625;//7812; // Set timer 3 period to  instruction cycles for 1hz  timer
    TMR3 = 0; // Set timer 1 to zero
    
//Clear both OC1CON1 & OC1CON2 registers for both PWM outputs
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC2CON1 = 0;
    OC2CON2 = 0;
    
// Now Fill the registers
    OC1CON2bits.SYNCSEL = 0b01011;  //Sync edges with Timer 1
    OC1CON1bits.OCTSEL = 0b100;    //Choose Timer 1
    OC1CON1bits.OCM = 0b110;    // Edge aligned
    OC1R = .25*PR1;     // Timer counts that it is ON out of 1 Timer period
    
    OC2CON2bits.SYNCSEL = 0b01100;  //Sync edges with Timer 2
    OC2CON1bits.OCTSEL = 0b000;    //Choose Timer 2
    OC2CON1bits.OCM = 0b110;    // Edge aligned
    OC2R = .05*PR2;     // Timer counts that it is ON out of 1 Timer period
    
    OC3CON2bits.SYNCSEL = 0b01011;  //Sync edges with Timer 1
    OC3CON1bits.OCTSEL = 0b100;    //Choose Timer 1
    OC3CON1bits.OCM = 0b110;    // Edge aligned
    OC3R = .25*PR1;     // Timer counts that it is ON out of 1 Timer period

// AD1CHS registers (SAMPLE SELECT REGISTER)
    _ADON = 0;  // Turn A/D off until the end
    //_CH0NA = 0b0;   // Use Vss as a negative input
    //_CH0SA = 0b1001; // Select Multiplexer A and Analog an 9 which is pin  maybe comment this out!

// AD1CON1 register ()
    _ADSIDL = 0; // Continue AD while idle (NO)
    _MODE12 = 1; // Select a 12 bit converter rather than 10
    _FORM = 0b00; // Output Format (decimal)
    _SSRC = 7; // Auto sampling conversion
    _ASAM = 1; // Auto Sampling enabled
// AD1CSSL registers
    _CSS9 = 1;//right ir
    _CSS10 = 1;// center ir //AD1CSSL<15:0> -- Select lower channels to scan
    _CSS11 = 1;//left ir
    AD1CSSH = 0b0;

// AD1CON2 Register ()
    _PVCFG = 0;  // Use VDD as positive reference Voltage 
    _NVCFG = 0;  // Use VSS as negative reference Voltage
    _BUFREGEN = 1; // Results stored using channel indexed mode (now you can find result in ADC1BUF9)
    _CSCNA = 1;  // How do you want it to scan the MUX
    //_ALTS = 0;  // Sample MUX A only
    _SMPI = 0b10;  // Sample rate interrupt select (needs to reflect # of channels)

// AD1CON3 Register ()
    _ADRC = 0;  // Use system clock
    _SAMC = 1;//0;  // Auto sample time select bits
    _ADCS = 0x02;  // Select Conversion clock speed
    _ADON = 1;  // Turn A/D ON
}
void config_pins(void)
{
    _TRISA6 = 0;  // Set RA6 (pin 14) as an output This will control PWM of MOTOR L
    _TRISB0 = 0;  // Set RB0 (pin 4) as an output. This will control PWM of MOTOR R 
    _TRISA1 = 0;  // pin 3 Right Ultrasonic
    _TRISA0 = 0;  // pin 2 Left Ultrasonic
    _TRISB1 = 0;  // Servo PWM on pin 5! will control all servos on board!!!
   
    _ANSB15 = 1;// This allows the right IR sensor to be analog. (find maximum)
    _TRISB15 = 1;// digital input of right  IR sensor.
    _ANSB14 = 1; 
    _TRISB14 = 1;// see above center
    _ANSB13 = 1; 
    _TRISB13 = 1;// see above left
    
    
    //_TRISB14 = 1; // aiming IR s 
    
    //pins 13 and 14 are Digital, PWM for motor L
    //pins 4 and 12 are PWM for motor R, Digital
    _TRISB7 = 0; // this will control left motor direction on pin 11 
    _TRISB8 = 0;// pin 12 driver
    _TRISB9 = 0;// pin 13 driver
    
   
    _TRISB2 = 1; //set up the bumpers as input....
    _TRISA2 = 1; //set up the bumpers as input....
    _ANSB2 = 0 ;  // Set B2 as Analog
    _ANSA2 = 0 ;
   
    _T1IP = 4;
    _T1IE = 1;
    _T1IF = 0;
    
    _T2IP = 4;
    _T2IE = 1;
    _T2IF = 0;

    _T3IP = 4;
    _T3IE = 1;
    _T3IF = 0;
}


//IR SENSING
char getIRState(enum Direction facing)
{
    delay(50);
    switch (facing)
    {
        
        case right:
            if(ADC1BUF9 > 1500)//1700)//2000 
            {
                return CORNERFOUND;
            }
            break;
        case center:
            if(ADC1BUF10 > 1500)//2000 change this to the other buffer for the right ir
            {
                return CORNERFOUND;
            }
            break;
        case left:
            if(ADC1BUF11 > 1500)//2000
            {
                
                return CORNERFOUND;
                
            }
            break;
            
            
    }
    return false;
}
char checkSeeFlashingIR()// reference goal found event checking 
{
    static unsigned char lastState = NOTFOUND;
    unsigned char currentState;
    unsigned char eventStatus;
    currentState = getIRState(left); // change the parameter to check other corners if necessary.
    if ((currentState != lastState) && currentState==CORNERFOUND)
    {
        eventStatus = true; // this tells us the corner was found!
    }
    else
    {
        eventStatus = false;// the corner wasn't found. keep up the search navigation mode....
    }
    lastState = currentState;
    return(eventStatus);
    
}
char getPhotoResistorState()
{       
    if(ADC1BUF12 > 1700)// TWEAK THIS....
    {
        return true;
    }
    return false;
}
           
   //if(the IR voltage is higher than a certain threshold) 
   // { you are pointed enough in the right direction, 
    //  set state as good
    //  return CORNERFOUND;
    //}
    //else return NOTFOUND;
     //return false;

char checkTargetFound(enum Direction side)// reference goal found event checking
{
    static unsigned char lastState = NOTFOUND;
    unsigned char currentState;
    unsigned char eventStatus;
    //_ANSB15 = 0; //we want digital read of IR Sensor
    currentState = getIRState(side);
    if ((currentState != lastState) && currentState == CORNERFOUND)     
    {
        eventStatus = true; // this tells us the left corner was found to be active!
    }
    else
    {
        eventStatus = false;// the corner wasn't active.
    }
    lastState = currentState;
    return(eventStatus);
    
}
//char checkRightTargetFound()// reference goal found event checking
//{
//    static unsigned char lastState = NOTFOUND;
//    unsigned char currentState;
//    unsigned char eventStatus;
//    //_ANSB14 = 0; //we want digital read of IR Sensor
//    currentState = rightIR;
//    if ((currentState != lastState) && currentState == CORNERFOUND)     
//    {
//        eventStatus = true; // this tells us the right corner was found to be active!
//    }
//    else
//    {
//        eventStatus = false;// the corner wasn't active.
//    }
//    lastState = currentState;
//    return(eventStatus);
//    
//}
//char checkCenterTargetFound()// reference goal found event checking
//{
//    static unsigned char lastState = NOTFOUND;
//    unsigned char currentState;
//    unsigned char eventStatus;
//    //_ANSB14 = 0; //we want digital read of IR Sensor
//    currentState = rightIR;
//    //todo, IR for center
//    if ((currentState != lastState) && currentState == CORNERFOUND)     
//    {
//        eventStatus = true; // this tells us the right corner was found to be active!
//    }
//    else
//    {
//        eventStatus = false;// the corner wasn't active.
//    }
//    lastState = currentState;
//    return(eventStatus);
//    
//}

void triggerDispenser()
{
    backupInches(2);
    forwardInches(2);
    ballsCollected++;
}
//  Motion and Position Control
void forwardInches(int inches) //not an event checker.... // needs work
{
    
    backupCount = 0;// this motion should be controlled by ultrasonic....
    /*while((arenaMiddle-LeftUS) >= Threshold && (arenaMiddle - RightUS >= Theshold)
    {
        RMotorDirection = BACK;
        LMotorDirection = BACK;
        PR1 = SLOW;
    }
    //if count has gone too long, you may have missed the target..... play with threshold, or restart, heading back to the wall...
    */
    //if (backupCount > 4 ) // retry
    while (backupCount <= 4)// play with these values to tun time... also, I can increase the resolutuion by using a faster timer
        
    {
        RMotorDirection = FWD;
        LMotorDirection = FWD;
        PR1 = SLOW;
    }
    PR1=STOP;
}
void backupInches(int inches) //not an event checker....
{
    backupCount = 0;// this motion should be controlled by ultrasonic....
    /*while((arenaMiddle-LeftUS) >= Threshold && (arenaMiddle - RightUS >= Theshold)
    {
        RMotorDirection = BACK;
        LMotorDirection = BACK;
        PR1 = SLOW;
    }
    //if count has gone too long, you may have missed the target..... play with threshold, or restart, heading back to the wall...
    */
    //if (backupCount > 4 ) // retry
    while (backupCount <= 4)// play with these values to tun time... also, I can increase the resolutuion by using a faster timer
        
    {
        RMotorDirection = FWD;
        LMotorDirection = FWD;
        PR1 = SLOW;
    }
    PR1=STOP;
}
void forwardToCenter() //not an event checker.... // calls inch mover func
{
    backupCount = 0;// this motion should be controlled by ultrasonic....
    /*while((arenaMiddle-LeftUS) >= Threshold && (arenaMiddle - RightUS >= Theshold)
    {
        RMotorDirection = BACK;
        LMotorDirection = BACK;
        PR1 = SLOW;
    }
    //if count has gone too long, you may have missed the target..... play with threshold, or restart, heading back to the wall...
    */
    //if (backupCount > 4 ) // retry
    while (backupCount <= 4)// play with these values to tun time... also, I can increase the resolutuion by using a faster timer
        
    {
        RMotorDirection = FWD;
        LMotorDirection = FWD;
        PR1 = SLOW;
    }
    PR1=STOP;
}
void rightTurn(int degrees)
{
    turnCount = 0;
    LMotorDirection = FWD;
    RMotorDirection = BACK;
    PR1=FAST;
    while (turnCount < turnClicks*(degrees/90.0))// maybe add the Ultrasonic sensor checking.....
    {
        
        //bad code!!! blocking!? any way to fix this?
        
    }
    PR1 = STOP;
}
void leftTurn(int degrees)
{
    turnCount = 0;
    LMotorDirection = BACK;
    RMotorDirection = FWD;
    PR1=FAST;
    while (turnCount < turnClicks*(degrees/90.0))// maybe add the Ultrasonic sensor checking.....
    {
        
        //bad code!!! blocking!? any way to fix this?
        
    }
    PR1 = STOP;
   // LMotorDirection = BACK;
   //RMotorDirection = BACK;
}
void IRAimingTurn(enum Direction dir)
{
    turnCount = 0;
    PR1=TURN;
    switch(dir)
    {
        case left:
            LMotorDirection = BACK;
            RMotorDirection = FWD;
            break;
            
        case right:
            LMotorDirection = FWD;
            RMotorDirection = BACK;
            break;
        case center:
            break;/// change this to make center alignment
    }
}
void IRAimingLeftTurn()
{
    //turnCount = 0;
    LMotorDirection = FWD;
    RMotorDirection = BACK;
    
   
}

char checkThresholdDetected()// have you found the maximum yet?
{
    //int current value = ADC1BUFx
    //previous maximum variable
    //compare the current with the previous....
    // if the
    static long lastMaximum = 0;    
    long sum = 0;
    unsigned char eventStatus;
    int filterAmount = 3000;
    int i = 0;
    for(i = 0; i < filterAmount; i++)
    {
        sum += ADC1BUF10;
    }
    long average = sum/filterAmount;
    if(average >= lastMaximum)
    {
        lastMaximum = average;
        eventStatus = false;
    }
    else
    {
        eventStatus = true;
        lastMaximum = 0;// reset maximum....
        
    }
    //volatile int currentValue = ADC1BUF10;
    
    return eventStatus;
}

char checkBallLoaded()
{
    //static unsigned char lastState = false;
    unsigned char currentState;
    unsigned char eventStatus;
    currentState = getPhotoResistorState(); // change the parameter to check other corners if necessary.
    if ((currentState /*!= lastState) && currentState*/==true))// 
    {
        eventStatus = true; // this tells us the ball was found!
    }
    else
    {
        eventStatus = false;// the ball wasn't found. 
    }
    //lastState = currentState;
    return(eventStatus);
}
void returnFromTurn(enum Direction facing)
{
    turnCount = 0;
    if(facing == right)
    {
        LMotorDirection = BACK;
        RMotorDirection = FWD;
        while (turnCount < previousTurnDuration)// maybe add the Ultrasonic sensor checking.....
        {

            //bad code!!! blocking!? any way to fix this?

        }
        previousTurnDuration = 0;//?
        
    }
}
char ballDidntShoot()
{
    delay(TENTH_SEC);// make sure
//    unsigned char lastball = true;
//    unsigned char currentState;
//    unsigned char eventStatus;
//    currentState = getPhotoResistorState(); // change the parameter to check other corners if necessary.
//    if ((currentState == lastball))// 
//    {
//        eventStatus = true; // the ball did not fire.
//    }
//    else
//    {
//        eventStatus = false;// the ball wasn't found. 
//    }
//    //lastState = currentState;
//    return(eventStatus);
   
    return (getPhotoResistorState());
}
char checkWallsFound()
{
    unsigned char wallContact;
    if(LBumper == 1 && RBumper == 1)
    {
        wallContact = true;
    }
    else
    {
        wallContact = false;
    }
    return wallContact;
    
}
void delay(unsigned long delay)
{
    unsigned long delayCount = 0;
    // delay while loop
    while (delayCount < delay)/// delay....
    {
        delayCount++;
    }
}
void shoot()//push a ball forward
{
    // watch out for OC2R changing when not in the function
    
    OC2R = .05*(PR2);
    delay(TENTH_SEC);
    OC2R = .075*PR2;
    delay(TENTH_SEC);
    OC2R = .05*PR2;
    
    
}

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTIONS_H */




