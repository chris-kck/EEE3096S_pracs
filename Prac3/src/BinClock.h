#ifndef BINCLOCK_H
#define BINCLOCK_H

//Some reading (if you want)
//https://stackoverflow.com/questions/1674032/static-const-vs-define-vs-enum

// Function definitions
int hFormat(int hours);
void lightHours(int units);
void lightMins(int units);
int hexCompensation(int units);
int decCompensation(int units);
void initGPIO(void);
void secPWM(int units);
void hourInc(void);
void minInc(void);
void toggleTime(void);

// define constants
const char RTCAddr = 0x6f;
const char SEC = 0x00; // see register table in datasheet
const char MIN = 0x01;
const char HOUR = 0x02;
const char TIMEZONE = 2; // +02H00 (RSA)

// define pins
const int LEDS[] = {12,3,2,0,24,23,22,21,14,13,25}; //H0-H3 0-12, M0-M5 13-24, S0 25
//const int LEDS[] = {0,2,3,12,13,14,21,22,23,24,25}; //H0-H3 0-12, M0-M5 13-24, S0 25
const int SECS = 1;
const int BTNS[] = {29,28}; // B0, B1


#endif
