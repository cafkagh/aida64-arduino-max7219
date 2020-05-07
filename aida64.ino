#include <Arduino.h>
#include "DigitLedDisplay.h"

DigitLedDisplay ld = DigitLedDisplay(2, 3, 4);//DIN CS CLK 

char inByte;

int lednum = 8; //LED位数
int reversed = 1; // 0不反转 1 翻转

long long int last_millis = 0;
int i = 0;
int connect = 0;

const char dig_led[28] = {
	B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,B01111111,B01111011,B01110111,B01111111,B01001110,B01111110,B01001111,B01000111,B01011110,B00110111,B00110000,B00111100,B00001110,B01111110,B01100111,B01011011,B11111000,B00111110,B10000000,B00000000
};
const char r_dig_led[28] = {
	B01111110,B00000110,B01101101,B01001111,B00010111,B01011011,B01111011,B00001110,B01111111,B01011111,B00111111,B01111111,B01111000,B01111110,B01111001,B00111001,B01111010,B00110111,B00000110,B01100110,B01110000,B01111110,B00111101,B01101101,B11111000,B01110110,B10000000,B00000000
};
const char sort[28] = {
	'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','L','O','P','S','T','U','.',' '
};

int find(char ar[], int n, char element)//查找元素并返回位置下标,find(数组，长度，元素)
{
	int i = 0;
	int index=-1;//原始下标，没找到元素返回-1
	for (i = 0; i <n; i++)
	{
		if (element ==ar[i])
		{
			index=i;//记录元素下标
		}
	}
	return index;//返回下标
}

void wait()
{
	if(millis() - last_millis > 5000 or connect == 0){
		for(int j = 1; j <= lednum; j++){
			ld.write(j, 0x01);
			delay(50);
			ld.write(j, 0x00);
		}  
		for(int j = lednum; j >=1; j--){
			ld.write(j, 0x01);
			delay(50);
			ld.write(j, 0x00);
		}  
	}
}

void setup()
{
	Serial.begin(9600);

	ld.setBright(1);
	ld.setDigitLimit(lednum);
}

void loop()
{

	if(Serial.available()>0)
	{

		inByte = Serial.read();

		if(millis() - last_millis > 500){
			ld.clear();
			i = 0;
			connect = 1;
		}

		if(i<=lednum){
			if(reversed == 0){
				ld.write(lednum-i, dig_led[find(sort,28,inByte)]);
			}else{
				ld.write(i+1, r_dig_led[find(sort,28,inByte)]);
			}
		}
		
		last_millis = millis();
		i++;
	}else{
		wait();
	}
}