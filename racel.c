
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <softPwm.h>

#include "move.h"

#define BUFF_SIZE	20
#define FORWARD		'F'
#define BACK		'B'	
#define LEFT		'L'
#define RIGHT		'R'
#define START_VAL	32767

//Input pins for encoders
int enc1Pin1 = 17;
int enc1Pin2 = 27;
int enc2Pin1 = 20;
int enc2Pin2 = 16;

//initial value for encoders
unsigned int stateEnc1 = START_VAL;
unsigned int stateEnc2 = START_VAL;

void sig_handler(int signo)
{
	if (signo == SIGINT){
		printf("received SIGINT\n");
		softPwmWrite(m1pwm,0);
		softPwmWrite(m2pwm,0);
		exit(0);
	}
}

void timer_handler(int signo){
	if (signo >= SIGALRM){
		//printf("received SIGALRM\n");
		if(Mot1Dir){
			if(stateEnc1 > endDistance1)
				softPwmWrite(m1pwm,0);
				//pwmWrite(pwmPin, 0);
		}
		else {
			if(stateEnc1 < endDistance1)
				softPwmWrite(m1pwm,0);
				//pwmWrite(pwmPin, 0);	
		}
		if(Mot2Dir){
			if(stateEnc2 > endDistance2)
				softPwmWrite(m2pwm,0);
				//pwmWrite(pwmPin, 0);
		}
		else {
			if(stateEnc2 < endDistance2)
				softPwmWrite(m2pwm,0);
				//pwmWrite(pwmPin, 0);
		}
	}
}

void UpdateEncoder1(){

	printf("UpdateEncoder1\n");
	static char e1p1Old=0;
	unsigned char e1p1=0;
	unsigned char e1p2=0;

	e1p1=digitalRead(enc1Pin1);
	printf("e1p1 %d\n",e1p1);
	e1p2=digitalRead(enc1Pin2);
	printf("e1p2 %d\n",e1p2);

	if(e1p1Old && !e1p1){
		if(e1p2)
			stateEnc1++;
		else
			stateEnc1--;
	}
	e1p1Old=e1p1;
	printf("State stateEnc1 %d\n", stateEnc1);
}

void UpdateEncoder2(){
	printf("UpdateEncoder2\n");
	static char e2p1Old=0;
	char e2p1=0;
	char e2p2=0;

	e2p1=digitalRead(enc2Pin1);
	printf("e2p1 %d\n",e2p1);
	e2p2=digitalRead(enc2Pin2);
	printf("e2p2 %d", e2p2);

	if(e2p1Old && !e2p1){
		if(e2p2)
			stateEnc2++;
		else
			stateEnc2--;
	}
	e2p1Old=e2p1;
	printf("State stateEnc2 %d\n", stateEnc2);
}

void enc1Pin1func(){
	UpdateEncoder1();
}

void enc1Pin2func(){
	UpdateEncoder1();
}

void enc2Pin1func(){
	UpdateEncoder2();	
}

void enc2Pin2func(){
	UpdateEncoder2();	
}

int main(){

	struct sigaction act;

	wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers
	printf("wiring pi setup done\n");
	//pinMode(pwmPin, PWM_OUTPUT); // Set PWMas PWM output
	pinMode(m1pin, OUTPUT);      
	pinMode(m2pin, OUTPUT);
	pinMode(m1pwm, OUTPUT);
	pinMode(m2pwm, OUTPUT);      
	pinMode(enc1Pin1, INPUT);
	pinMode(enc1Pin2, INPUT);
	pinMode(enc2Pin1, INPUT);
	pinMode(enc2Pin2, INPUT);

	softPwmCreate(m1pwm, 0, 100);
	softPwmCreate(m2pwm, 0, 100);

	signal(SIGINT, sig_handler);

	act.sa_handler = timer_handler;

	sigaction(SIGALRM, &act, 0);
	ualarm(10000, 10000);

	char pom;
	int speed;
	int distance;

	char speedStr[4];
	char distanceStr[5];
	char Buff[BUFF_SIZE];

	printf("MAU\n");

	wiringPiISR(enc1Pin1, INT_EDGE_BOTH, &enc1Pin1func);
	wiringPiISR(enc1Pin2, INT_EDGE_BOTH, &enc1Pin2func);
	wiringPiISR(enc2Pin1, INT_EDGE_BOTH, &enc2Pin1func);
	wiringPiISR(enc2Pin2, INT_EDGE_BOTH, &enc2Pin2func);

	printf("Enter command\n");
	while(1){

			

		if(fgets(Buff, BUFF_SIZE, stdin) != NULL){
			
			printf("Buff\t%s\n", Buff);

			strncpy(speedStr, &Buff[1], 4);
			speed=atoi(speedStr);
			strncpy(distanceStr, &Buff[5], 4);
			distance=atoi(distanceStr);
			printf("Move with speed %d to distance %d\n", speed, distance);
			switch(Buff[0]){

				case FORWARD:{
					printf("Move forward\n");

					moveForward(speed, distance);
					break;
				}
				case BACK:{
					printf("Move back\n");
					moveBack(speed, distance);
					break;
				}
				case LEFT:{
					printf("Move left\n");
					moveLeft(speed, distance);
					break;
				}
				case RIGHT:{
					printf("Move right\n");
					moveRight(speed, distance);
					break;
				}
			}
		}
	}
	return 0;
}
