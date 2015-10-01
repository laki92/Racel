#include <stdio.h>
#include <wiringPi.h>

extern int pwmPin; 
extern int m1pwm; 
extern int m2pwm;
extern int m1pin; 
extern int m2pin;

extern int endDistance1;
extern int endDistance2;

extern char Mot1Dir;
extern char Mot2Dir;

int moveForward(int speed, int distance);
int moveBack(int speed, int distance);
int moveLeft(int speed, int distance);
int moveRight(int speed, int distance);