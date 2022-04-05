// Plotting and manipulating graphics on 160x128 screen
//define commands for matrixes and module + screen constants
#include <SPI.h>
#include <TFT.h>
#include <noDelay.h>
#define CS   10
#define DC   9
#define RESET  8

TFT myScreen = TFT(CS, DC, RESET);
noDelay resetTimer(3000);
	
const byte SCREENWIDTH = 160;
const byte SCREENHEIGHT = 128;
struct particleSingle {
	byte x;
	byte y;
	byte oldX;
	byte oldY;
	byte velocity;
	int angle;
	byte deathCounter;
	byte deathFlag;
};
particleSingle particleArray[50];
uint8_t amountOfParticles = sizeof(particleArray) / sizeof(particleArray[0]);
void particleSetup(particleSingle *array);
void showParticle(particleSingle *array);
void removeParticle(particleSingle *array);
void setup() {
	myScreen.begin();
	myScreen.background(0, 0, 0);
	myScreen.stroke(255, 255, 255);
	Serial.begin(9600);
	SPI.begin();
	particleSetup(particleArray);
	resetTimer.start();
}
void loop() {
	if (resetTimer.update()){
		particleSetup(particleArray);
		resetTimer.start();
	}
	else {
		showParticle(particleArray);
	}
//	delay(80);
	removeParticle(particleArray);
}
void showParticle(particleSingle *array){
	myScreen.stroke(255, 255, 255);
	for (byte i = 0; i < amountOfParticles; i++){
		if (array[i].deathFlag == 0){
			myScreen.rect(array[i].x, array[i].y, 2, 2);
			array[i].oldX = array[i].x;
			array[i].oldY = array[i].y; 
			array[i].y += array[i].velocity * sin(array[i].angle);
			array[i].x += array[i].velocity * cos(array[i].angle);
			array[i].deathCounter ++;
		}
		if (array[i].deathCounter >= 10 | array[i].y > SCREENHEIGHT){
			array[i].deathFlag = 1;
		}
//		if (array[i].deathFlag == 1){
//			particleReset(array[i].x, array[i].y, array[i].velocity, array[i].deathCounter, array[i].deathFlag, array[i].angle);
//		}
	}	
}
void removeParticle(particleSingle *array){
	myScreen.stroke(0, 0, 0);
	for (byte i = 0; i < amountOfParticles; i++){
		myScreen.rect(array[i].oldX, array[i].oldY, 2, 2);
	}	
}
void particleReset(byte &x, byte &y, byte &velocity, byte &deathCounter, byte &deathFlag, int &angle){
	y = SCREENHEIGHT / 2;
	x =  SCREENWIDTH / 2;//random(0, SCREENWIDTH - 1);
	velocity = random(5, 20);
	angle = random(1, 359);
	deathCounter = random(1, 3);
	deathFlag = 0;
}
void particleSetup(particleSingle *array){
	for (byte i = 0; i < amountOfParticles; i++){
		particleReset(array[i].x, array[i].y, array[i].velocity, array[i].deathCounter, array[i].deathFlag, array[i].angle);

	}
}
