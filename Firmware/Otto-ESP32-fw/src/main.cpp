//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-- Otto DIY App Firmware Version 13 with standard baudrate of 9600 for Bluetooth BLE modules.
//-- Otto DIY invests time and resources providing open source code and hardware,
//-- please support by purchasing kits from (https://www.ottodiy.com)
//-- Make sure to have installed all libraries: https://github.com/OttoDIY/OttoDIYLib
//-----------------------------------------------------------------
//-- If you wish to use this software under Open Source Licensing, you must contribute all your source code to the community and all text above must be included in any redistribution
//-- in accordance with the GPL when your application is distributed. See http://www.gnu.org/copyleft/gpl.html
//---------------------
/*
         ---------------
        |     O   O     |
        |---------------|
S3 YR 6==> |               | <== YL 4  S1
         ---------------
            ||     ||
S4 RR 7==>   -----   ------  <== RL 5  S2
         |-----   ------|
*/
//#include <SerialCommand.h>
#include <lvgl.h>
#include "HAL/HAL.h"
#include "Port/Display.h"
#include "App/App.h"
#include "App/image.h"
#include <Otto.h>

Otto Otto;

#define LeftLeg 4
#define RightLeg 6
#define LeftFoot 5
#define RightFoot 7
#define Buzzer 15

void setup()
{

    Otto.init(LeftLeg, RightLeg, LeftFoot, RightFoot, true, Buzzer); // Set the servo pins and Buzzer pin
    Otto.home();
    HAL::Init();
    Port_Init();
    App_Init();

    // anger1_animing();
    excited1_animing();
    // anger2_animing();
    Otto.sing(S_happy);
    delay(200);
}
void OttoDancing();

void loop()
{
    HAL::Update();
    delay(20);
    OttoDancing();
}

void OttoDancing()
{
    Otto.walk(1, 3000, 1);  // 2 steps, "TIME". IF HIGHER THE VALUE THEN SLOWER (from 600 to 1400), 1 FORWARD
    Otto.walk(2, 1000, -1); // 2 steps, T, -1 BACKWARD
    Otto.turn(2, 1000, 1);  // 3 steps turning LEFT
    Otto.home();
    delay(100);
    Otto.turn(2, 1000, -1); // 3 steps turning RIGHT
    Otto.bend(1, 500, 1);   // usually steps =1, T=2000
    Otto.bend(1, 2000, -1);
    Otto.shakeLeg(1, 1500, 1);
    Otto.home();
    delay(100);
    Otto.shakeLeg(1, 2000, -1);
    Otto.moonwalker(3, 1000, 25, 1);  // LEFT
    Otto.moonwalker(3, 1000, 25, -1); // RIGHT
    Otto.crusaito(2, 1000, 20, 1);
    Otto.crusaito(2, 1000, 20, -1);
    delay(100);
    Otto.flapping(2, 1000, 20, 1);
    Otto.flapping(2, 1000, 20, -1);
    delay(100);
    Otto.swing(2, 1000, 20);
    Otto.tiptoeSwing(2, 1000, 20);
    Otto.jitter(2, 1000, 20); // (small T)
    Otto.updown(2, 1500, 20); // 20 = H "HEIGHT of movement"T
    Otto.ascendingTurn(2, 1000, 50);
    Otto.jump(1, 2000);
    Otto.home();
}






