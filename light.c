#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#define SW 5
#define CDS 0
#define LED 1

int cdsControl(){
	int i;
	for (i=0;i <10000000;i++){
		fprintf(stderr, "%d", digitalRead(CDS));
		if(digitalRead(CDS) == HIGH){
			fprintf(stderr,"hiigh");
		  	delay(1000);
		}	
		delay(1000);
	}
}
int main(){
	if (wiringPiSetup() == -1){
		printf("error setup");
		exit(0);
	}
	fprintf(stderr, "start");
	pinMode(CDS, INPUT);
	cdsControl();
	return 0;
}
