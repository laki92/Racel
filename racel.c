
#include <stdio.h>    // Used for printf() statements
#include <wiringPi.h> // Include WiringPi library!
#include <softPwm.h>
#include <wiringPiSPI.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int enc1Pin1 = 17;
int enc1Pin2 = 27;
int enc2Pin1 = 16;
int enc2Pin2 = 20;
int pwmPin = 18; // PWM LED - Broadcom pin 18, P1 pin 12
int m1pin = 23; // Regular LED - Broadcom pin 23, P1 pin 16
int m2pin = 21;

unsigned int stateEnc1=32767;
unsigned int stateEnc2=32767;

char Buff[100];

void sig_handler(int signo)
{
	if (signo == SIGINT)
	printf("received SIGINT\n");
	pwmWrite(pwmPin, 0);
}

void UpdateEncoder1(){

	printf("UpdateEncoder1\n");
	static char e1p1Old=0;
	char e1p1=0;
	char e1p2=0;
	char e2p1=0;
	char e2p2=0;

	e1p1=digitalRead(enc1Pin1);
	e1p2=digitalRead(enc1Pin2);

	if(!e1p1Old == e1p1){
		if(enc1Pin2)
			stateEnc1++;
		else
			stateEnc1--;
	}
	
}

void UpdateEncoder2(){
	printf("UpdateEncoder2\n");
	static char e2p1Old=0;
	char e2p1=0;
	char e2p2=0;

	e2p1=digitalRead(enc1Pin1);
	e2p2=digitalRead(enc1Pin2);

	if(!e2p1Old == e2p1){
		if(enc2Pin2)
			stateEnc2++;
		else
			stateEnc2--;
	}
	
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

int main(void)
{

	wiringPiSetupGpio(); // Initialize wiringPi -- using Broadcom pin numbers
	printf("wiring pi setup done\n");
	pinMode(pwmPin, PWM_OUTPUT); // Set PWMas PWM output
	pinMode(m1pin, OUTPUT);     // 
	pinMode(m2pin, OUTPUT);      // 
	pinMode(enc1Pin1, INPUT);
	pinMode(enc1Pin2, INPUT);
	pinMode(enc2Pin1, INPUT);
	pinMode(enc2Pin2, INPUT);


//	pullUpDnControl(butPin, PUD_UP); // Enable pull-up resistor on button

	char RetVal = -1;

	int pwmValue;
	char statusMove = 0;
	char statusStop = 0;

	char MotorDir1 = 0;
	char MotorDir2 = 1;

	char distance[4];
	char intesity[4];
	unsigned int iDistance1=0;
	unsigned int iDistance2=0;
	unsigned int stateDistance1=0;
	unsigned int stateDistance2=0;

	char BuffFlag = -1;

	wiringPiISR(enc1Pin1, INT_EDGE_BOTH, &enc1Pin1func);
	wiringPiISR(enc1Pin2, INT_EDGE_BOTH, &enc1Pin2func);
	wiringPiISR(enc2Pin1, INT_EDGE_BOTH, &enc2Pin1func);
	wiringPiISR(enc2Pin2, INT_EDGE_BOTH, &enc2Pin2func);

	RetVal=wiringPiSPISetup (0, 500000);

	while(1)
	{
		
		if(statusMove){
			statusMove=0;
			printf("statusMove\n");
			digitalWrite(m1pin, MotorDir1); 
			digitalWrite(m2pin, MotorDir2); 
			pwmWrite(pwmPin, pwmValue);
		}
		if(statusStop){
			printf("statusStop\n");
			statusStop=0;
			pwmWrite(pwmPin, 0);
			statusMove=1;
		}

		BuffFlag = wiringPiSPIDataRW (0, Buff, 100);
		if(!BuffFlag && !statusMove){
			printf("Command from SPI %s", Buff);
			switch(Buff[0]){
				case 'F':{
					printf("case F in\n");
					memcpy(&Buff[1], intesity, 4);
					memcpy(&Buff[1], distance, 4);
					pwmValue=atoi(intesity);
					iDistance1=atoi(distance);
					iDistance2=atoi(distance);
					stateDistance1+=iDistance1;
					stateDistance2+=iDistance2;
					MotorDir1=1;
					MotorDir2=0;
					
					
				}
				case 'B':{
					printf("case B in\n");
					memcpy(&Buff[1], intesity, 4);
					memcpy(&Buff[1], distance, 4);
					pwmValue=atoi(intesity);
					iDistance1=atoi(distance);
					iDistance2=atoi(distance);
					stateDistance1-=iDistance1;
					stateDistance2-=iDistance2;
					MotorDir1=1;
					MotorDir2=1;
				}
				case 'L':{
					printf("case L in\n");
					memcpy(&Buff[1], intesity, 4);
					memcpy(&Buff[1], distance, 4);
					pwmValue=atoi(intesity);
					iDistance1=atoi(distance);
					iDistance2=atoi(distance);
					stateDistance1+=iDistance1;
					stateDistance2-=iDistance2;
					MotorDir1=0;
					MotorDir2=0;
				}
				case 'R':{
					printf("case R in\n");
					memcpy(&Buff[1], intesity, 4);
					memcpy(&Buff[1], distance, 4);
					pwmValue=atoi(intesity);
					iDistance1=atoi(distance);
					iDistance2=atoi(distance);
					stateDistance1-=iDistance1;
					stateDistance2+=iDistance2;
					MotorDir1=0;
					MotorDir2=1;
				}
			}
		}
		if(stateEnc1 == stateDistance1)
			statusStop=0;

		if(stateEnc2 == stateDistance2)
			statusStop=0;
		
	}

	return 0;
}
