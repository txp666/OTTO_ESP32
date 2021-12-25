// MaxMatrix Version 1.0 Feb 2013 Copyright 2013 Oscar Kin-Chung Au
// OttDIY Arduino Project, 2020

#include "Arduino.h"
#include "Matrix9.h"

Otto_Matrix::Otto_Matrix() {}

void Otto_Matrix::init(byte _data, byte _load, byte _clock, byte _num, int _rotation) {
	data = _data;
	load = _load;
	clock = _clock;
	num = _num;
	rotation = _rotation;

	if((rotation > 4) || (rotation == 0))
		rotation = 1; // we have to have number between 1 and 4

	pinMode(data,  OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(load,  OUTPUT);
	digitalWrite(clock, HIGH); 

	setCommand(max7219_reg_scanLimit, 0x07);      
	setCommand(max7219_reg_decodeMode, 0x00);   // using an led matrix (not digits)
	setCommand(max7219_reg_shutdown, 0x01);     // not in shutdown mode
	setCommand(max7219_reg_displayTest, 0x00);  // no display test
	clearMatrix();                              // empty registers, turn all LEDs off
	setIntensity(0x0f);                         // the first 0x0f is the value you can set
}

void Otto_Matrix::setIntensity(byte intensity) {
	setCommand(max7219_reg_intensity, intensity);
}

void Otto_Matrix::clearMatrix() {
	for(int i = 0; i < 8; i++) 
		setColumnAll(i, 0);
	for(int i = 0; i < 8; i++)
		buffer[i] = 0;
	for(int i = 0; i < 80; i++)
		CHARbuffer[i] = 0;
}

void Otto_Matrix::setCommand(byte command, byte value) {
	digitalWrite(load, LOW);    
	for(int i = 0; i < num; i++) {
		shiftOut(data, clock, MSBFIRST, command);
		shiftOut(data, clock, MSBFIRST, value);
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
}

void Otto_Matrix::setColumn(byte col, byte value) {
	int n = col / 8;
	int c = col % 8;
	digitalWrite(load, LOW);    
	for(int i = 0; i < num; i++) {
		if(i == n) {
			shiftOut(data, clock, MSBFIRST, c + 1);
			shiftOut(data, clock, MSBFIRST, value);
		}
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
	buffer[col] = value;
}

void Otto_Matrix::setColumnAll(byte col, byte value) {
	digitalWrite(load, LOW);    
	for(int i = 0; i < num; i++) {
		shiftOut(data, clock, MSBFIRST, col + 1);
		shiftOut(data, clock, MSBFIRST, value);
		buffer[col * i] = value;
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
}

void Otto_Matrix::setDot(byte col, byte row, byte value) {
	bitWrite(buffer[col], row, value);
	int n = col / 8;
	int c = col % 8;
	digitalWrite(load, LOW);    
	for(int i = 0; i < num; i++) {
		if(i == n) {
			shiftOut(data, clock, MSBFIRST, c + 1);
			shiftOut(data, clock, MSBFIRST, buffer[col]);
		} else {
			shiftOut(data, clock, MSBFIRST, 0);
			shiftOut(data, clock, MSBFIRST, 0);
		}
  	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
}

// routine for the 6 x 5 matrix
void Otto_Matrix::writeFull(unsigned long value) {
	switch(rotation) {
		case 1:
			for(int r = 0; r < 5; r++) {
				for(int c = 0; c < 6; c++) {
					setDot(6 - c, 7 - r, (1L & (value >> r * 6 + c)));
				}
			}
			break;
		case 2:
			for(int r = 0; r < 5; r++) {
				for(int c = 0; c < 6; c++) {
					setDot(1 + c, r, (1L & (value >> r * 6 + c)));
				}
			}
			break;
		case 3:
			for(int r = 0; r < 5; r++) {
				for(int c = 0; c < 6; c++) {
					setDot(r, 6 - c, (1L & (value >> r * 6 + c)));
				}
			}
			break;
		case 4:
			for(int r = 0; r < 5; r++) {
				for(int c = 0; c < 6; c++) {
					setDot(7 - r, 1 + c, (1L & (value >> r * 6 + c)));
				}
			}
			break;
	}
}

void Otto_Matrix::sendChar(const byte data, byte pos, byte number, byte scrollspeed) {
	int charPos;
	if(scrollspeed < 50)
		scrollspeed = 50;
	if(scrollspeed > 150)
		scrollspeed = 150;
	charPos = pos * 8;
	CHARbuffer[0 + charPos] = 0;
	CHARbuffer[1 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[0]);
	CHARbuffer[2 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[1]);
	CHARbuffer[3 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[2]);
	CHARbuffer[4 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[3]);
	CHARbuffer[5 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[4]);
	CHARbuffer[6 + charPos] = pgm_read_byte(&Character_font_6x8[data].data[5]);
	CHARbuffer[7 + charPos] = 0;

	if(number == (pos + 1)) { // last character so display the total text
		for(int c = 0; c < 8; c++) { // show first character
			byte value = CHARbuffer[c];
			for(int r = 0; r < 8; r++) {
				switch(rotation) {
					case 1:
						setDot(c, 7 - r, (0b00000001 & (value >> r)));
						break;
					case 2:
						setDot(7 - c, r, (0b00000001 & (value >> r)));
						break;
					case 3:
						setDot(r, c, (0b00000001 & (value >> r)));
						break;
					case 4:
						setDot(7 - r, 7 - c, (0b00000001 & (value >> r)));
						break;
				}
			}
		}
		delay(500); // show first digit for longer
		for(int i = 0; i < ((number * 8) - 1); i++) { // shift buffer the correct number of characters (8 lines per character)
			CHARbuffer[i] = CHARbuffer[i + 1];
			for(int c = 0; c < 8; c++) {
				byte value = CHARbuffer[(1 + c) + i];
				for(int r = 0; r < 8; r++) {
					switch(rotation) {
						case 1:
							setDot(c, 7 - r, (0b00000001 & (value >> r)));
							break;
						case 2:
							setDot(7 - c, r, (0b00000001 & (value >> r)));
							break;
						case 3:
							setDot(r, c, (0b00000001 & (value >> r)));
							break;
						case 4:
							setDot(7 - r, 7 - c, (0b00000001 & (value >> r)));
							break;
					}
				}
			}
			delay(scrollspeed); // this sets the scroll speed
		}
		clearMatrix();
	}
}

//end

