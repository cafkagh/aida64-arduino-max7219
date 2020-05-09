#include <Arduino.h>    
#include "LedControl.h"    
// 配置项    
int modules = 2; //模块数    
int fi_ms = 500; //帧间隔 毫秒    
int reversed = 1; // 0不反转 1 翻转    
int wait_ms = 5000; //数据超时提示 毫秒    
int brightness = 1; //亮度   

int DIN = 2;
int CS = 4;
int CLK = 3;
LedControl lc = LedControl(DIN, CS, CLK, modules);

int i = 0;
char inByte;
int connect = 0;
int lednum = modules * 8;
long long int last_millis = 0;
 
const char dig_led[28] = {    
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,B01111111,B01111011,B01110111,B01111111,B01001110,B01111110,B01001111,B01000111,B01011110,B00110111,B00110000,B00111100,B00001110,B01111110,B01100111,B01011011,B11111000,B00111110,B10000000,B00000000
};    
const char r_dig_led[28] = {    
    B01111110,B00000110,B01101101,B01001111,B00010111,B01011011,B01111011,B00001110,B01111111,B01011111,B00111111,B01111111,B01111000,B01111110,B01111001,B00111001,B01111010,B00110111,B00000110,B01100110,B01110000,B01111110,B00111101,B01101101,B11111000,B01110110,B10000000,B00000000
};    
const char sort[28] = {    
    '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','L','O','P','S','T','U','.',' '
};
//查找元素并返回位置下标,find(数组，长度，元素)
int find(char ar[], int n, char element){
    int i = 0;
    int index=-1;
    for (i = 0; i <n; i++){
        if (element ==ar[i]){
            index=i;
			break;
        }
    }
    return index;
}
void write(int pos, char byte){
	int row = (int)(pos/8);
	int col = 8-(pos%8)-1;
	lc.setRow(row, col, byte);
}
void wait(){
    if(millis() - last_millis > wait_ms or connect == 0){
        for(int j = 0; j < lednum-1; j++){
            write(j, 0x01);
            delay(50);
            write(j, 0x00);
        }
        for(int j = lednum-1; j >0; j--){
            write(j, 0x01);
            delay(50);
            write(j, 0x00);
        }
    }
}
void setup(){    
    Serial.begin(9600);    
	for (size_t row = 0; row < modules; row++){
		lc.shutdown(row, false);
		lc.setIntensity(row, brightness);
		lc.clearDisplay(row);
	}
}
void loop(){
    if(Serial.available()>0){
        inByte = Serial.read();
        if(millis() - last_millis > fi_ms){
            i = 0;
            connect = 1;
        }
        if(i<lednum){
            if(reversed == 0){
                write(i, dig_led[find(sort,28,inByte)]);
            }else{
                write(lednum-i-1, r_dig_led[find(sort,28,inByte)]);    
            }
        }
        last_millis = millis();
        i++;
    }else{
        wait();
    }
}