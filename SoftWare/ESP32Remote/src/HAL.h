#ifndef __HAL_H
#define __HAL_H

#include <Arduino.h>
#include "OneButton.h"
#include "config.h"

void KEY_Init();
void KEY_Update();
void button1Click();
void button2Click();
void button3Click();
void button4Click();
void button5Click();
void button2DClick();
void button3DClick();
void button4DClick();
void button5DClick();
void button2LClick();
void button3LClick();
void button4LClick();
void button5LClick();

/* Buzzer */
void Buzz_init();
void Buzz_SetEnable(bool en);
void Buzz_Tone(uint32_t freq, int32_t duration = 0);

/* Audio */
void Audio_Init();
void Audio_Update();
bool Audio_PlayMusic(const char *name);

#endif