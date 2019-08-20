/*
 * BinClock.c
 * Jarrod Olivier
 * Modified for EEE3095S/3096S by Keegan Crankshaw
 * August 2019
 * 
 * <NGRBLE001> <KTRKUD001>
 * Date
*/

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h> //For printf functions
#include <stdlib.h> // For system functions
#include <math.h>
#include <string.h>
#include  <signal.h>

#include <softPwm.h> // Foe pulse width  modulation
#include "BinClock.h"
#include "CurrentTime.h"



int hours, mins, secs;
long lastInterruptTime = 0; //Used for button debounce
int RTC; //Holds the RTC instance

int HH,MM,SS;


//===============================
// FUNCTION DECLARATIONS		+
//===============================

void INThandler(int);
void PiCleanup(void);
void hourInc(void);


void  INThandler(int sig) // ref: http://en.cppreference.com/w/c/program/signal
{
     char  c;

     signal(sig, SIG_IGN);
     printf("\n Hectic!, Did you hit Ctrl-C?\n"
            "Do you really want to quit? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y'){
		PiCleanup();
		printf("Program gracefully exited");
        exit(0);
	 }
     else
          signal(SIGINT, INThandler);
     getchar(); // Get new line character
}


char binary[4];
char* Dec2Bin(int decValue){

            int nValue = (int)floor((log(decValue)/log(2))); //to get largest power radix will be raised to for initial division
            char symbols[2]= {'0','1'};
            int quotient,remainder;

            int count=0;
            for(int i = nValue ; i>=0 ;i--){
                quotient= (int)(decValue/pow(2,i)); //exact integer quotient
                remainder= decValue% (int)pow(2,i); //remainder
                decValue=remainder;
                binary[count]=symbols[quotient]; count++;//=symbols[quotient];
                }
                binary[count]='\0'; //null terminator for printing out
            return binary;
}



void initGPIO(void){
	/* 
	 * Sets GPIO using wiringPi pins. see pinout.xyz for specific wiringPi pins
	 * You can also use "gpio readall" in the command line to get the pins
	 * Note: wiringPi does not use GPIO or board pin numbers (unless specifically set to that mode)
	 */
	printf("Setting up\n");
	wiringPiSetup(); //This is the default mode. If you want to change pinouts, be aware
	
	RTC = wiringPiI2CSetup(RTCAddr); //Set up the RTC
	
	//Set up the LEDS
	for(int i; i < sizeof(LEDS)/sizeof(LEDS[0]); i++){
	    pinMode(LEDS[i], OUTPUT);
	}
	
	//Set Up the Seconds LED for PWM
	//Write your logic here
    softPwmCreate (LEDS[10], 0, 100); //PWM only possible for pin 0.
	
	// void softPwmWrite (int pin, int value) ; // To write to pin
	
	printf("LEDS done\n");
	
	//Set up the Buttons
	for(int j; j < sizeof(BTNS)/sizeof(BTNS[0]); j++){
		pinMode(BTNS[j], INPUT);
		pullUpDnControl(BTNS[j], PUD_UP);
	}
	
	//Attach interrupts to Buttons
	//Write your logic here
	
	printf("BTNS done\n");
	printf("Setup done\n");
}


/*
 * The main function
 * This function is called, and calls all relevant functions we've written
 */
int main(void){
	initGPIO();

	//Set random time (3:04PM)
	
	wiringPiI2CWriteReg8(RTC, 0x13+TIMEZONE, 0x3);//HRS
	wiringPiI2CWriteReg8(RTC, 0x4, 0x5); //MINS
	wiringPiI2CWriteReg8(RTC, 0x00, SEC); //SEC
	signal(SIGINT, INThandler);	 
	wiringPiISR (BTNS[1], INT_EDGE_FALLING,  &hourInc); //no buttons detected so far
	wiringPiISR (BTNS[1], INT_EDGE_FALLING,  &minInc);
	//wiringPiISR (BTNS[0], INT_EDGE_FALLING,  void (*function)(void));
	
	// Repeat this until we shut down
	for (;;){
		//Fetch the time from the RTC
		//Write your logic here
		
		//Function calls to toggle LEDs
		//Write your logic here
		hours = wiringPiI2CReadReg8 (RTC, 0x13+TIMEZONE);
		mins = wiringPiI2CReadReg8 (RTC, 0x4);
		secs = wiringPiI2CReadReg8 (RTC, 0x00);
		// Print out the time we have stored on our RTC
		lightHours(hFormat(hours));
		lightMins(mins);
		printf("The current time is: %x:%x:%x\n", hours, mins, secs);		

		//using a delay to make our program "less CPU hungry"
		delay(1000); //milliseconds
	}
	return 0;
}

/*
 * Change the hour format to 12 hours
 */
int hFormat(int hours){
	/*formats to 12h*/
	if (hours >= 24){
		hours = 0;
	}
	else if (hours > 12){
		hours -= 12;
	}
	return (int)hours;
}

/*
 * Turns on corresponding LED's for hours
 */
void lightHours(int units){
	// Write your logic to light up the hour LEDs here
	char* hours =Dec2Bin(2);
	printf("HRS is: %s , %d \n" , hours, strlen(hours));
	for (int i=0; i<strlen(hours); i++){
		if (hours[i]=='1') {
		digitalWrite (LEDS[i], HIGH);
		printf("printed 1 \n");
		}
		else {digitalWrite (LEDS[i], LOW);
		printf("printed 0 \n");}
		}
	
}

/*
 * Turn on the Minute LEDs
 */
void lightMins(int units){
	//Write your logic to light up the minute LEDs here
	char* minutes =Dec2Bin(units);
	printf("\n MINS is: %s , %d \n" , minutes, strlen(minutes));
	for (int i=0; i<strlen(minutes); i++){
		if (minutes[i]=='1') {
		digitalWrite (LEDS[4+i], HIGH);
		printf("printed 1 ");
		}
		else {digitalWrite (LEDS[4+i], LOW);
		printf("printed 0 ");}
		}
}

/*
 * PWM on the Seconds LED
 * The LED should have 60 brightness levels
 * The LED should be "off" at 0 seconds, and fully bright at 59 seconds
 */
void secPWM(int units){
	// Write your logic here
}

/*
 * hexCompensation
 * This function may not be necessary if you use bit-shifting rather than decimal checking for writing out time values
 */
int hexCompensation(int units){
	/*Convert HEX or BCD value to DEC where 0x45 == 0d45 
	  This was created as the lighXXX functions which determine what GPIO pin to set HIGH/LOW
	  perform operations which work in base10 and not base16 (incorrect logic) 
	*/
	int unitsU = units%0x10;

	if (units >= 0x50){
		units = 50 + unitsU;
	}
	else if (units >= 0x40){
		units = 40 + unitsU;
	}
	else if (units >= 0x30){
		units = 30 + unitsU;
	}
	else if (units >= 0x20){
		units = 20 + unitsU;
	}
	else if (units >= 0x10){
		units = 10 + unitsU;
	}
	return units;
}


/*
 * decCompensation
 * This function "undoes" hexCompensation in order to write the correct base 16 value through I2C
 */
int decCompensation(int units){
	int unitsU = units%10;

	if (units >= 50){
		units = 0x50 + unitsU;
	}
	else if (units >= 40){
		units = 0x40 + unitsU;
	}
	else if (units >= 30){
		units = 0x30 + unitsU;
	}
	else if (units >= 20){
		units = 0x20 + unitsU;
	}
	else if (units >= 10){
		units = 0x10 + unitsU;
	}
	return units;
}


/*
 * hourInc
 * Fetch the hour value off the RTC, increase it by 1, and write back
 * Be sure to cater for there only being 23 hours in a day
 * Software Debouncing should be used
 */
void hourInc(void){
	//Debounce
	long interruptTime = millis();
	printf("BUTTON PRESSED");
	if (interruptTime - lastInterruptTime>200){
		printf("Interrupt 1 triggered, %x\n", hours);
		//Fetch RTC Time
		hours = wiringPiI2CReadReg8 (RTC, 0x13+TIMEZONE);
		//Increase hours by 1, ensuring not to overflow
		hours++; // if else
		//Write hours back to the RTC
		wiringPiI2CWriteReg8(RTC, 0x13+TIMEZONE, HOUR);//HRS
	
	}
	lastInterruptTime = interruptTime;
}

/* 
 * minInc
 * Fetch the minute value off the RTC, increase it by 1, and write back
 * Be sure to cater for there only being 60 minutes in an hour
 * Software Debouncing should be used
 */
void minInc(void){
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		printf("Interrupt 2 triggered, %x\n", mins);
		//Fetch RTC Time
		mins = wiringPiI2CReadReg8 (RTC, 0x4);
		//Increase minutes by 1, ensuring not to overflow
		mins++;
		//Write minutes back to the RTC
		wiringPiI2CWriteReg8(RTC, 0x4, mins); //MINS
	}
	lastInterruptTime = interruptTime;
}

//This interrupt will fetch current time from another script and write it to the clock registers
//This functions will toggle a flag that is checked in main
void toggleTime(void){
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime>200){
		HH = getHours();
		MM = getMins();
		SS = getSecs();

		HH = hFormat(HH);
		HH = decCompensation(HH);
		wiringPiI2CWriteReg8(RTC, HOUR, HH);

		MM = decCompensation(MM);
		wiringPiI2CWriteReg8(RTC, MIN, MM);

		SS = decCompensation(SS);
		wiringPiI2CWriteReg8(RTC, SEC, 0b10000000+SS);

	}
	lastInterruptTime = interruptTime;
}


void PiCleanup(void){
	
	// we reset all registers and switch off LEDs
	for(int i; i < sizeof(LEDS)/sizeof(LEDS[0]); i++){
	    pinMode(LEDS[i], INPUT); //hacky method to switch off all LEDs
	}
}