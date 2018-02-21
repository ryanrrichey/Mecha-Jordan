
/* 
 * File:   main.c
 * Author: ryanrichey
 *
 * Created on October 6, 2015, 3:28 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p24F16KA301.h>
#include "PIN_DEF.h"
#include "Functions.h"

//typedef int bool;

//LED gets 1.3 V and .05A... DO NOT FRY EMITTER



//_FICD(ICS_PGx2)
_FOSCSEL(FNOSC_FRC)
_FOSC(OSCIOFNC_OFF)// enables pin 8 to be used as IO instead of clock
        
double arenaMiddle = 23.5;
double Threshold = 1; // Play with the threshold to tune centering....
int turnClicks = 240; //~109 ticks on full step ~90 degree turn with 3" radius of wheel base, and 2.75" radius of wheel. tune this! 
double inchClicks = 49; // ~23  ticks on full step  gets one inch travel, based on 2.75" wheel diam...
int previousTurnDuration;
long lastMaximum = 0;  
int servoLR = 0;


double hunHztimer=0;
int arenaCounter = 0;
int turnCount = 0;
int moveCount = 0;
int backupCount = 0;
double loadingCount = 0;
int ballsCollected = 0;
unsigned char Start = 0;
int shotsTaken = 0;
double lightFrequency = 0;
double frequencyCounter = 0;
int lightState = 0;

//state definitions
enum State
{
    //main states...
    navigating,
    loading,
    scoring,
    testing,
    null,// (end)
    
    // navigation substates...
    null,
    starting,
    searchingForHopper,
    movingTowardHopper,
    
    //loading substates...
    inCorner,
    collecting,
    
    // scoring substates...
    searching,
    aiming,
    loadingBall,  
    shooting,
    end
    
};

enum State mainState = navigating;
enum State subState = starting;
//enum Direction
//{
//    left,
//    right,
//    center
//};



void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    _T1IF = 0;
    turnCount++;
    moveCount++;
    turnCount = turnCount%(4*turnClicks);
    //frequencyCounter++;
    
}
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
//void _ISR _T1Interrupt
{
    
    _T2IF = 0; // Timer interrupt flag clear
    
    backupCount++;
    //count++; 
    /*if (count > 2 && count <= 4) //turn right!
    {
        RMotorDirection = 1;
        LMotorDirection = 0;
        PR1 = 90; 
    }
    else if (count >4  && count <=6)// go straight again
    {
        PR1 = 60;
        RMotorDirection = 1;
        LMotorDirection = 1;
        //count = 0; repeat?
        //PR1=30;
    }
    else if(count >6)
    {
        PR1 = 0;
    }
     * */ //This is the code for FORWARD, left turn
    
    
           
}
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void)
//void _ISR _T1Interrupt
{
    
    _T3IF = 0; // Timer interrupt flag clear
    arenaCounter++; 
//    if(arenaCounter>95)// within 10 seconds
//    {
//        if(subState == collecting)// go out and shoot any balls you might have.
//        {
//            LMotorDirection = BACK;
//            RMotorDirection = BACK;
//            wheelSpeed = TURN;
//            motors(ON);
//            subState = movingTowardHopper;
//        }
//        
//        
//               
//    }
//    if(arenaCounter>100)
//    {
//        if(mainState != scoring)
//        {
//            LMotorDirection = BACK;
//            RMotorDirection = BACK;
//            wheelSpeed = TURN;
//            motors(ON);
//            subState = movingTowardHopper;
//        }
//    }
    if(arenaCounter>99)// game over
    {
       
        if(subState != collecting &&  subState != movingTowardHopper) //
        {
            motors(OFF);// game over
            Shooters = OFF;
            mainState = null;
            subState = null; 
            Drivers = 1; 
        }
        else
        {
            arenaCounter = 0;
            forwardToCenter();
            delay(TENTH_SEC);
            delay(TENTH_SEC);
            Drivers = 1;
//            mainState = endGame;
            subState = null;
            mainState = null;
//            dispenserServoPWM = armRetracted;
//            RMotorDirection = BACK;
//            LMotorDirection = BACK;
//            wheelSpeed = FAST;
//            motors(ON); 
//            //mainState = loading;
//            subState = movingTowardHopper;
            //Drivers = 1;
            
        }
        
        
               
    }
//    if(arenaCounter>104)
//    {
//        motors(OFF);
//        Shooters = OFF;
//        
//    }
          
}
void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void)
{
    _T4IF = 0;
    hunHztimer += .01;
    //turnCount++;
//    frequencyCounter++;
//    if(ADC1BUF11 > IRPWMThreshold && lightState == OFF)//some threshold)
//    {
//        
//        lightState = ON;
//        lightFrequency = 400.0/frequencyCounter;
//        
//        frequencyCounter = 0; //do something
//    }
//    else if(ADC1BUF11 < IRPWMThreshold && lightState == ON)
//    {
//        lightState = OFF;
//        
//    }
//    
    
}


/*
 * 
 */
int main(int argc, char** argv) {
    
    //config_ad();  // Call function
    //ANSB = 0x0000;
    //TRISBbits.TRISB2 = 1;  // Set B2 (pin 6) as an input
    
    config_ad();  // Call function
    config_pins();
    config_ic();
    PR1 = FAST;    
    Shooters = OFF;
    shooterServoPWM = .05*(PR2);
    dispenserServoPWM = .05*(PR2);
    Drivers = 0;
    
    enum Direction botIsFacing = center;
    //enum State mainState = navigating;
    mainState = navigating;
    //enum State subState = testing;
    //enum State subState = starting;
    subState = starting;
    //subState = testing;
    //_RB0 = 1;// turn the dang motors on....
    while(1)
    {   
        while(mainState == navigating)
        {
            switch(subState)
            {
                case testing:
                    //Shooters = ON;
                    //start whatever tests I want here
                    //LMotorDirection = BACK; // When you first start, be turning, looking for IR
                    //RMotorDirection = FWD;
                    //MotorPWM = STOP;
                    //Drivers= 1;
                    LMotorDirection = BACK; // When you first start, be turning, looking for IR
                    RMotorDirection = BACK;
                    wheelSpeed = TURN;
                    motors(ON);
                    
                    delay(TENTH_SEC);
                    //Drivers = 1;
                    
                    subState = movingTowardHopper;
                   
                    
                    
                    //if(getIRState(center) && checkMaximumDetected())
                    //{
                    //    shoot();
                    //}
                    //shoot();
                    //shooterServoPWM = .075*(PR2);
                    //forwardInches(10);
                    //rightTurn(90);
                    //delay(TENTH_SEC);
                    //subState = movingTowardHopper;
//                    if (arenaCounter == 105)
//                    {
//                        motors(OFF);
//                        subState = null;
//                    }
                    
                    //toggleDispenserServo();
                    
//                    if (arenaCounter>3)
//                    {
//                        toggleDispenserServo();
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        toggleDispenserServo();
//                        
//                        
//                        
//                        
//                        
//                        subState = null;
//                        break;
//                    }
//                    {
//                        motors(OFF);
//                        subState = null;
//                    }
////                    delay(TENTH_SEC);
//                    delay(TENTH_SEC);
//                    delay(TENTH_SEC);
//                    delay(TENTH_SEC);
//                    delay(TENTH_SEC);
//                    delay(TENTH_SEC);
//                    delay(TENTH_SEC);
                    //toggleDispenserServo();
                    
                    //subState = aiming;
                    //mainState = scoring;
                    
                    //subState = null;
                    break;
                    
                case starting:
                    Drivers = 0;
                    dispenserServoPWM = armRetracted;
                    arenaCounter = 0;
                    Shooters = OFF;
                    LMotorDirection = BACK; // When you first start, be turning, looking for IR
                    RMotorDirection = FWD;
                    wheelSpeed = TURN;
                    motors(ON);
                    subState = searchingForHopper;
                    break;
                    
                case searchingForHopper:
//                    
                    if (checkSeeFlashingIR())
                    {
                        if(arenaCounter >= 5)// you missed the first light, try the second light
                        {
                            //look now for the hopper IR
                            //right turn 90
                            //back up...
                            rightTurn(90.0);
                        }
                        RMotorDirection = BACK;
                        LMotorDirection = BACK;
                        wheelSpeed = FAST;
                        motors(ON); 
                        subState = movingTowardHopper;
                    }
                    
                    break;
                    
                case movingTowardHopper:
                    dispenserServoPWM = armRetracted;
                    if(checkWallsFound())//if bumpers hit
                    {
                        
                        delay(TENTH_SEC);
                        //delay(TENTH_SEC);
                        //Drivers = ON;
                        motors(OFF);
                        mainState = loading;
                        
                        subState = inCorner;
                        //subState = null; //for testing
                        // start backing up  until load distance.
                        // start loading (exit navigation, enter loading?)
                    }
                    break;
                default: 
                    break;
                    
                    
//                case movingTowardCenter:
//                    if(checkWallsFound())
//                    {
//                        PR1 = STOP;
//                        //forwardToCenter();
//                        subState = null;
//                        mainState = navigating;
//                        subState = inCorner;
//                        // start backing up  until load distance.
//                        // start loading (exit navigation, enter loading?)
//                    } 
                    //perform ultrasonic range check here?
                    
            }
        }
        while(mainState == loading)
        {
            switch(subState)
            {
                case inCorner:
                    ballsCollected = 1;
                    delay(HUN_SEC);
                    forwardInches(3.5);
                    delay(4*HUN_SEC);
                    Drivers = 1;
                    while(arenaCounter<10)
                    {
                        ballsCollected = 0;
                    }
                    
                    subState = collecting;
                    break;
                    
                case collecting:
                    triggerDispenser();
                    
                    if (ballsCollected >4)
                    {
//                        retractDispenserServo();
//                        delay((TENTH_SEC));
//                        delay((TENTH_SEC));
//                        delay((5*HUN_SEC));
                        
                        LMotorDirection = BACK;
                        RMotorDirection = BACK;
                        wheelSpeed = TURN;
                        retractDispenserServo();
                        motors(ON);
                        Drivers = 0;
                        subState = movingTowardHopper;
                    }
                    break;
                case movingTowardHopper:
                    if(checkWallsFound())
                    {
                        delay(TENTH_SEC);// keep smashing into the corner
                        //delay(TENTH_SEC);
                        //delay(TENTH_SEC);
                        //delay(TENTH_SEC);
                        //MotorPWM = STOP;
                        motors(OFF);
                        forwardToCenter();
                        botIsFacing = center;   
                        //subState = null;
                        //mainState = navigating;
                        
                        mainState = scoring;
                        subState = searching;
                        shotsTaken = 0;
                        
                        //subState = null; //for testing
                    }  
                    break;
                default:
                    break;
            }
        } 
       while(mainState == scoring)
       {
           
           switch(subState)
            {
                case searching:
                    //Shooters = ON; 
                    delay(MIL_SEC);
                    //delay(5*HUN_SEC);
                    //if(checkTargetFound(left))// ad right and center
                    //{
                    IRAimingTurn(left);
                    botIsFacing = left;
                    subState = aiming;
                    //break;
                        //subState = null; //for testing
                    //}
                    
//                    else if(checkTargetFound(right))// ad right and center
//                    {
//                        IRAimingTurn(left);
//                        botIsFacing = left;
//                        subState = aiming;
//                        break;
//                        //subState = null; //for testing
//                    }
//                    else //if(!checkTargetFound(left)&&!checkTargetFound(right))//(checkTargetFound(center))// ad right and center
//                    {
//                        //leftTurn(18.0);
//                        //delay(TENTH_SEC);
//                        IRAimingTurn(left);
//                        botIsFacing = center;
//                        subState = aiming;
//                        break;
//                        //subState = null; //for testing
//                    }
                    break;
                    
                case aiming:
                    //if(checkMaximumDetected())
                    
                    //
                    if(getIRState(center)&&checkMaximumDetected())
                    {
                        
                        
                        //stop turning.....
                        // record how much it turned
                        // change substate.....to shooting....
                        motors(OFF);
                        //_RA3 = 1;
                        previousTurnDuration = turnCount;
                        //subState = loadingBall;
                        subState = loadingBall;
                        loadingCount = hunHztimer;
                        //spool up shooters....
                        Shooters = ON; 
                        delay(TENTH_SEC);
                        delay(TENTH_SEC);
                        //delay(TENTH_SEC);
                        //Shooters = ON;
                        //subState = null;//for test
                    }
                    
                    break;
                case loadingBall:
                    
                    // "I haven't shot six yet, but maybe I didn't load enough.
                    if((hunHztimer - loadingCount)>3.0)
                    {
                        //try to load some more....
                       delay(TENTH_SEC);
                       returnFromTurn(botIsFacing);
                       botIsFacing = center;
                       delay(TENTH_SEC);
                       RMotorDirection = BACK;
                       LMotorDirection = BACK;
                       
                       wheelSpeed = FAST;
                       motors(ON);
                       retractDispenserServo();
                       subState = movingTowardHopper;
                       
                       mainState = navigating;
                       //shotsTaken = 0;
                       Shooters = OFF;
                       break;
                    } 
                    else if((hunHztimer - loadingCount)>1.8)
                    {
                        subState = shooting;
                        Drivers = ON;
                    }
                    
                    
                    // "I haven't shot six yet, but I don't have one loaded" 
//                    else if((arenaCounter - loadingCount)>2.5)
//                    {
//                        wiggle();// so, try to knock one loose.
//                        
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        if(getPhotoResistorState())
//                        {
//                            //delay(TENTH_SEC);
//                            returnFromTurn(botIsFacing);
//                            delay(TENTH_SEC);
//                            botIsFacing = center;
//                            subState = searching;
//                            break;
//                        }
//                    }
                    
                   
                    //the LEDs switched! re aim...
                    if((arenaCounter - 10)%30  == 30)//-29) <.5)//||(arenaCounter - 10)%30 == 0)// change goals when the lights do
                    {
                        delay(TENTH_SEC);
                        returnFromTurn(botIsFacing);
                        delay(TENTH_SEC);
                        botIsFacing = center;
                        subState = searching;
                        break;
                    }
                    
//                    if(!getIRState(center))// good stuff, but never saw the forward facing light....
//                    {
//                        delay(TENTH_SEC);
//                        delay(TENTH_SEC);
//                        if(!getIRState(center))
//                        {
//                            delay(TENTH_SEC);
//                            returnFromTurn(botIsFacing);
//                            delay(TENTH_SEC);
//                            botIsFacing = center;
//                            subState = searching;
//                            break;
//                        }    
//                    }
                    else if(getPhotoResistorState())
                    {
                        //change state to shooting
                        subState = shooting;
                        Drivers = ON;//This pin is inverted, so off is really on...
                        
                    }
                    
                    //timer saying no balls found....
                    
                    //solenoid here...
                    
                    break;
               case shooting:
                   
                   //the ball is loaded, shoot.
                   shoot();
                   
                   //small delay for reloading....
                   int i;
                   for (i= 0;i < 7; i++)
                   {
                       delay(HUN_SEC);
                   }
                   delay(TENTH_SEC); 
                   Drivers = OFF;//This pin is inverted, so off is really on...
                   
                  
                   //servo moves... pushes ball.. comes back
                   //shoot();
                   
                   //if(IR is off!)
                   //if(photo resistor still blocked, meaning ball didnt shoot.)
                   //if not  done shooting   <= six shots go to  loading ball.
                   //if successful six shots => go home...
                   //delay(TENTH_SEC);
                   //delay(TENTH_SEC);
//                   if(getPhotoResistorState())// maybe unused.....
//                   {
//                       shotsTaken--;
//                   }
//                   else if(!getPhotoResistorState() && (shotsTaken < 6))
//                   {
//                       subState = loadingBall;
//                   }
//                   else
//                   {
//                        returnFromTurn(botIsFacing);
//                        RMotorDirection = BACK;
//                        LMotorDirection = BACK;
//                        PR1 = FAST; 
//                        subState = movingTowardHopper;
//                        mainState = navigating;
//                   }
                   //delay(TENTH_SEC);
                   
                   if(shotsTaken>=6)
                   {
                       
                       //delay(TENTH_SEC);
                       //delay(TENTH_SEC);
                       delay(TENTH_SEC);
                       returnFromTurn(botIsFacing);
                       botIsFacing = center;
                       delay(TENTH_SEC);
                       RMotorDirection = BACK;
                       LMotorDirection = BACK;
                       
                       wheelSpeed = FAST;
                       motors(ON);
                       retractDispenserServo();
                       subState = movingTowardHopper;
                       mainState = navigating;
                       shotsTaken = 0;
                       Shooters = OFF;
                       break;
                   }
                   else
                   {
                        loadingCount = arenaCounter;
                        subState = loadingBall;
                   }
                  
                   break;
                default:
                   break;
                   
            }
        }
        while(mainState == endGame)
        {
            if(subState == movingTowardHopper)
            {
                if(checkWallsFound())
                {
                    //delay(TENTH_SEC);// keep smashing into the corner
                    //delay(TENTH_SEC);
                    //delay(TENTH_SEC);
                    //delay(TENTH_SEC);
                    //MotorPWM = STOP;
                    dispenserServoPWM = armRetracted;
                    motors(OFF);
                    forwardToCenter();
                    botIsFacing = center;   
                    //subState = null;
                    //mainState = navigating;

                    mainState = null;
                    subState = null;
                    shotsTaken = 0;
                    Drivers = 1;
                    dispenserServoPWM = armRetracted;

                    //subState = null; //for testing
                }  
            }
            
        }
    }
    return (EXIT_SUCCESS);
}



