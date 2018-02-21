/* 
 * File:   PIN_DEF.h
 * Author: RyanRichey
 *
 * Created on November 15, 2015, 7:22 AM
 */

#ifndef PIN_DEF_H


#define CORNERFOUND 1
#define NOTFOUND 0
#define PRESSED 0 


//motor settings
#define STOP 0
#define FAST 100
#define SLOW 120
#define TURN 140
#define FWD 0
#define BACK 1
#define ON 1
#define true 1
#define false 0
#define TENTH_SEC 29450
#define HUN_SEC 2945
#define MIL_SEC 295
#define wheelSpeed PR1

//delete me?
 #define LBumper PORTBbits.RB2
    #define RBumper PORTAbits.RA2
    //#define hopperIR PORTBbits.RB13// this is for the HOPPER IR sensor
    #define rightIR PORTBbits.RB15 // right IR Sensor PIN 18 was RB15
    #define centerIR PORTBbits.RB14 // center IR Sensor PIN 17
    #define leftIR PORTBbits.RB13 // left IR Sensor PIN 16

    #define TurretPWM OC3R 
    #define MotorPWM OC1R // change this to be matching PWMs
   

#define TurretDirection PORTBbits.RB9 //driver
    #define RMotorDirection PORTBbits.RB8 //driver
    #define LMotorDirection PORTBbits.RB7 //driver


#define LeftUS PORTAbits.RA0  //TRISA0 = 0;  // pin 2 Left Ultrasonic
#define RightUS PORTAbits.RA1 //TRISA1 = 0;  // pin 3 Right Ultrasonic

enum Direction
{
    left,
    right,
    center
};



#define	PIN_DEF_H

#ifdef	__cplusplus
extern "C" {
#endif
    




#ifdef	__cplusplus
}
#endif

#endif	/* PIN_DEF_H */




