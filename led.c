#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#define LED1 1
#define LED2 4
#define LED3 5
#define LED4 6
#define SW1 10
#define SW2 11

void candle(double on, double off){
	digitalWrite(LED1, 1);
	delay(on * 1000);
	digitalWrite(LED1, 0);
	delay(off * 1000);
}


int main(void){
	double sub = 0.0;
	if (wiringPiSetup() < 0){
		exit(0);
		printf("exit");
	}
	int rcv = 0, rcv2 = 0;
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);

	pinMode(SW1, INPUT);
	pinMode(SW2, INPUT);

	for (int i =0;i<1;i++){
		
		digitalWrite(LED1, 1);
		digitalWrite(LED2, 0);
		delay(1000);
		digitalWrite(LED1, 0);
		digitalWrite(LED2, 1);
		delay(1000);
		digitalWrite(LED3, 0);
		digitalWrite(LED4, 1);
		delay(1000);
		digitalWrite(LED3, 1);
		digitalWrite(LED4, 0);
		delay(1000);
		digitalWrite(LED2, 0);
	}
	printf("start");
	while(1){
//		delay(2000);
	//	printf("s\n");
		rcv = digitalRead(SW1);
//		printf("%d", rcv);
		//delay(2000);
		digitalWrite(LED1, rcv);
		digitalWrite(LED2, rcv);
		rcv2 = digitalRead(SW2);
		if(rcv2 == 1){
			digitalWrite(LED3, 0);
			digitalWrite(LED4, 0);
			delay(1000);
			digitalWrite(LED3, 1);
			digitalWrite(LED4, 1);
			delay(1000);
		}

	}
	//
	//rcv = digitalRead(SW1);
	printf("%d", rcv);
	//digitalWrite(LED1, 1);
	
	//printf("on");
	delay(2000);
	//digitalWrite(LED1, 0);
	//printf("ok");
	



	//for(int i =0;i <40;i++){
	//	sub = i * 0.0005;
	//	candle(0.02 - sub, 0.01);
	//}
	
	return 0;
}
