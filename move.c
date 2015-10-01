#include "move.h"
#define START_VAL	32767

int pwmPin = 18; 
int m1pwm = 15; 
int m2pwm = 12;
int m1pin = 23; 
int m2pin = 21;

int endDistance1 = START_VAL;
int endDistance2 = START_VAL;

char Mot1Dir;
char Mot2Dir;


int moveForward(int speed, int distance){
	//Protection for H bridge
	softPwmWrite(m1pwm,0);
	softPwmWrite(m2pwm,0);
	printf("Status of sistem is move move forward speed=%d distance=%d\n", speed, distance);
	//setup direction of motors
	digitalWrite(m1pin, 1); 
	digitalWrite(m2pin, 0);
	//setup values for end of distance of encoders
	endDistance1+=distance;
	endDistance2-=distance;
	//direction of motors
	Mot1Dir=1;
	Mot2Dir=0;
	//setup speed
	softPwmWrite(m1pwm,speed);
	softPwmWrite(m2pwm,speed);
}

int moveBack(int speed, int distance){
	//Protection for H bridge
	softPwmWrite(m1pwm,0);
	softPwmWrite(m2pwm,0);
	printf("Status of sistem is move move back\n");
	//setup direction of motors
	digitalWrite(m1pin, 0); 
	digitalWrite(m2pin, 1); 
	//setup values for end of distance of encoders
	endDistance1-=distance;
	endDistance2+=distance;
	//direction of motors
	Mot1Dir=0;
	Mot2Dir=1;
	//setup speed
	softPwmWrite(m1pwm,speed);
	softPwmWrite(m2pwm,speed);
}

int moveLeft(int speed, int distance){	
//Protection for H bridge
	softPwmWrite(m1pwm,0);
	softPwmWrite(m2pwm,0);
	printf("Status of sistem is move move left\n");
	//setup direction of motors
	digitalWrite(m1pin, 0); 
	digitalWrite(m2pin, 0); 
	//setup values for end of distance of encoders
	endDistance1-=distance;
	endDistance2-=distance;
	//direction of motors
	Mot1Dir=0;
	Mot2Dir=0;
	//setup speed
	softPwmWrite(m1pwm,speed);
	softPwmWrite(m2pwm,speed);
}

int moveRight(int speed, int distance){
	//Protection for H bridge
	softPwmWrite(m1pwm,0);
	softPwmWrite(m2pwm,0);
	printf("Status of sistem is move move rght\n");
	//setup direction of motors
	digitalWrite(m1pin, 1); 
	digitalWrite(m2pin, 1); 
	//setup values for end of distance of encoders
	endDistance1+=distance;
	endDistance2+=distance;
	//direction of motors
	Mot1Dir=1;
	Mot2Dir=1;
	//setup speed
	softPwmWrite(m1pwm,speed);
	softPwmWrite(m2pwm,speed);
}