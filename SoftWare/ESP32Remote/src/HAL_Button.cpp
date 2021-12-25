#include"HAL.h"
#include "OneButton.h"

OneButton button1(KEY1, true);
OneButton button2(KEY2, true);
OneButton button3(KEY3, true);
OneButton button4(KEY4, true);
OneButton button5(KEY5, true);

void KEY_Init()
{
  button1.attachClick(button1Click);
  button2.attachClick(button2Click);
  button3.attachClick(button3Click);
  button4.attachClick(button4Click);
  button5.attachClick(button5Click);
  button2.attachDoubleClick(button2DClick);
  button3.attachDoubleClick(button3DClick);
  button4.attachDoubleClick(button4DClick);
  button5.attachDoubleClick(button5DClick);
  button2.attachLongPressStop(button2LClick);
  button3.attachLongPressStop(button3LClick);
  button4.attachLongPressStop(button4LClick);
  button5.attachLongPressStop(button5LClick);
}
void KEY_Update()
{
  button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();
  button5.tick();
}