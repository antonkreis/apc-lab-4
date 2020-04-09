#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

int melody[] = {										// Jingle Bells
	330, 330, 330, 330, 330, 330, 330, 392, 262,
	294, 330, 349, 349, 349, 349, 349, 330, 330,
	330, 330, 330, 294, 294, 330, 294, 392, 330,
	330, 330, 330, 330, 330, 330, 392, 262, 294, 
	330, 349, 349, 349, 349, 349, 330, 330, 330, 
	330, 392, 392, 349, 294, 262, 0
};


int clock[] = {
	250, 250, 500, 250, 250, 500, 250, 250, 375,
	125, 1000, 250, 250, 375, 125, 250, 250, 250,
	125, 125, 250, 250, 250, 250, 500, 500, 250,
	250, 500, 250, 250, 500, 250, 250, 375, 125,
	1000, 250, 250, 250, 250, 250, 250, 250, 125,
	125, 250, 250, 250, 250, 1000, 0
};

void getRandom(long max) {
	printf("\n===================================Random generator===");
	max--;
	outp(0x43, 0xB6);
	outp(0x61, inp(0x61) & 0xfc);
	long base = 1193180, kd, kdLow, kdHigh, result, kdMax;
	float procent;
	kd = base / max;
	kdMax = kd;
	outp(0x42, kd % 256);
	kd /= 256;
	outp(0x42, kd);
	outp(0x61, inp(0x61) | 1);
	printf("\nPress any key");
	getch();
	outp(0x43, 0x80);
	kdLow = inp(0x42);
	kdHigh = inp(0x42);
	outp(0x61, inp(0x61) & 0xfc);
	kd = kdHigh * 256 + kdLow;
	procent = 1 - (float)kd / kdMax;
	result = max * procent;
	printf("\nThe result %ld", result);
}


void printKd() {
	printf("===============================Division coefficient===");
	int kd[] = {0,0,0};
	int channelNumber = 0x00;
	int channelPort = 0x40;
	int kdLow, kdHigh, kdTemp, j;
	for (int iChan = 0; iChan < 3; iChan++) {
		for (j = 0; j < 10000; j++) {
			if (j == 2) {
				outp(0x61, inp(0x61) & 0xfc);
				outp(0x61, inp(0x61) | 1); // only 0 bit (turn on channel 2 but without speaker)
			}
			outp(0x43, channelNumber);

			kdLow  = inp(channelPort);
			kdHigh = inp(channelPort);
			kdTemp = kdHigh*256 + kdLow;

			if (kdTemp > kd[iChan]) {
				kd[iChan] = kdTemp;
			}
			if (j == 2) {
				outp(0x61, inp(0x61) & 0xfc); 
			}
		}
		printf("\nkd %d = %x", iChan, kd[iChan]);
		channelNumber += 0x40;
		channelPort++;
	}
}  

void printBinaryFormat(int word) {
	int binary[8], i;
	for (i = 0; i < 8; i++){
		binary[i] = word % 2;
		word /= 2;
	}
	for (i = 7; i >= 0; i--) {
		printf("%d", binary[i]);
	}
}

void printWords(){	
	printf("===============================State words===");
	outp(0x43, 0xE2); //11100010
	int stateChannel0 = inp(0x40);
	printf("\nThe state word of channel 0 (40h): ");
	printBinaryFormat(stateChannel0);
	outp(0x43, 0xE4); //11100100
	int stateChannel1 = inp(0x41);
	printf("\nThe state word of channel 1 (41h): ");
	printBinaryFormat(stateChannel1);
	outp(0x43, 0xE8); //11101000
	int stateChannel2 = inp(0x42);
	printf("\nThe state word of channel 2 (42h): ");
	printBinaryFormat(stateChannel2);
}

void setFrequency(unsigned int freq){
	long base = 1193180, kd;
	outp(0x43, 0xB6);
	kd = base / freq;
	outp(0x42, kd % 256);
	kd /= 256;
	outp(0x42, kd); 
}

void play() {
	int i;
    for(i = 0; melody[i] != 0 || clock[i] != 0; i++) {
		setFrequency(melody[i]);
		outp(0x61, inp(0x61) | 3);
		delay(clock[i]);
		outp(0x61, inp(0x61) & 0xfc);
		delay(50);
    }
} 

int main() {

	int i;
	long range;
	char selection;

    while (1) {
		system("cls");
		printf("==================LAB4=== \n");
		printf("1: Play sound \n");
		printf("2: Print KD\n");
		printf("3: Print condition words\n");
		printf("4: Generate random numbers\n");
		printf("5: Exit\n\n");
		printf("Choose the option: \n");
		selection = getch();
		switch(selection){
				case '1':
				system("cls");
				play();
				break;
		  case '2':
				system("cls");
				printKd();
				getch();
				break;
		  case '3':
				system("cls");
				printWords();
				getch();
				break;
		  case '4':
				system("cls");
				printf("Enter the above range of random: ");
				while (!scanf("%ld", &range) || range < 0 || range > 65535) {
					fflush(stdin);
					printf("Wrong input. Try again: ");
				}
				getRandom(range);
				getch();
			    break;
		  case '5':
			    printf("Exiting...");
				return 0;
		  default:
				printf("Error key! Please try again\n");
				break;
		}
  }
}

