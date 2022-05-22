/*
Otto All moves Arduino test 
OttoDIY Arduino Project, 2020 | sfranzyshen
*/
/*
         --------------- 
        |     O   O     |
        |---------------|
YR 33==> |               | <== YL 19
         --------------- 
            ||     ||
RR 32==>   -----   ------  <== RL 21
         |-----   ------|
*/
#include <Otto9.h> //-- Otto Library version 9
#include "BluetoothSerial.h"
#include <lvgl.h>
#include <TFT_eSPI.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

static lv_disp_draw_buf_t draw_buf;    //定义显示器变量
static lv_color_t buf[TFT_WIDTH * 10]; //定义刷新缓存

TFT_eSPI tft = TFT_eSPI();
Otto9 Otto; //This is Otto!
BluetoothSerial SerialBT;

int bend1[] = {90, 90, 62, 35};
int bend2[] = {30, 10, 62, 105};
int ii;
char buf1[20], buf2[20];

void move(int moveId, int T);
void GesturePlay(int gesture);
void SingSong(int song);
/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();                                        //使能写功能
  tft.setAddrWindow(area->x1, area->y1, w, h);             //设置填充区域
  tft.pushColors((uint16_t *)&color_p->full, w * h, true); //写入颜色缓存和缓存大小
  tft.endWrite();                                          //关闭写功能

  lv_disp_flush_ready(disp); //调用区域填充颜色函数
}
LV_IMG_DECLARE(img_src_naughty)

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("OttoESP32"); //Bluetooth device name
  Otto.init(19, 33, 21, 32, false, 25, 25, 2, 3);
  Otto.home();
  Otto.sing(S_connection); //Otto wake up!
  pinMode(12, OUTPUT);
  digitalWrite(12, 1);
  tft.init(); //初始化
  tft.setRotation(0);
  lv_init();
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = TFT_WIDTH;
  disp_drv.ver_res = TFT_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // lv_obj_t *label = lv_label_create(lv_scr_act());
  // lv_label_set_text(label, "ESP32");
  // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_t *icon = lv_img_create(lv_scr_act());

  /*From variable*/
  lv_img_set_src(icon, &img_src_naughty);
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  char c;
  char *cmd;
  char *dat;
  int dd[2], i;
  if (SerialBT.available())
  {
    c = (char)SerialBT.read();
    //Serial.print(c);
    if (c == '\r')
    {
      buf1[ii] = '\0';
      //Serial.println(" ");
      Serial.println(buf1);
      cmd = strtok(buf1, " ");
      switch (cmd[1])
      {
      case 'M':
        for (i = 0; i < 2; i++)
        {
          dat = strtok(NULL, " ");
          if (dat != "" and dat != NULL)
          {
            dd[i] = atoi(dat);
          }
        }
        sprintf(buf2, "%s,%d,%d", cmd, dd[0], dd[1]);
        move(dd[0], dd[1]);
        Serial.println(buf2);
        break;
      case 'K':
        dat = strtok(NULL, " ");
        i = 0;
        if (dat != "" and dat != NULL)
        {
          dd[i] = atoi(dat);
        }
        sprintf(buf2, "%s,%d", cmd, dd[0]);
        Serial.println(buf2);
        // if (cmd[1] == 'H')
        //GesturePlay(dd[0]);
        // if (cmd[1] == 'K')
        SingSong(dd[0]);
        break;
      }
      ii = 0;
    }
    buf1[ii] = c;
    ii++;
    delay(20);
  }
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
void move(int moveId, int T)
{
  int moveSize = 30;

  switch (moveId)
  {
  case 0:
    Otto.home();
    break;
  case 1: //M 1 1000
    Otto.walk(3, T, 1);
    break;
  case 2: //M 2 1000
    Otto.walk(3, T, -1);
    break;
  case 3: //M 3 1000
    Otto.turn(3, T, 1);
    break;
  case 4: //M 4 1000
    Otto.turn(3, T, -1);
    break;
  case 5: //M 5 1000 30
          // Otto.updown(2, 1000, 60);
    Otto.tiptoeSwing(2, 1000, -30);
    break;
  case 6: //M 6 1000 30
    Otto.moonwalker(3, 1000, -30, 1);
    break;
  case 7: //M 7 1000 30
    Otto.moonwalker(3, 1000, -30, -1);
    break;
  case 8: //M 8 1000 30
          // Otto.swing(2, 1000, moveSize);
    Otto.updown(2, 1000, 30);
    break;
  case 9: //M 9 1000 30
    Otto.crusaito(3, 1000, moveSize, 1);
    break;
  case 10: //M 10 1000 30
    Otto.crusaito(3, 1000, moveSize, -1);
    break;
  case 11: //M 11 1000
    Otto.jump(2, 1000);
    break;
  case 12: //M 12 1000 30
    Otto.flapping(3, 1000, 30, 1);
    break;
  case 13: //M 13 1000 30
    Otto.flapping(3, 1000, 30, -1);
    break;
  case 14: //M 14 1000 20
    Otto.updown(2, 1000, -30);
    // Otto.tiptoeSwing(2, 1000, 30);
    break;
  case 15: //M 15 500
    Otto.bend(1, 500, 1);
    break;
  case 16: //M 16 500
    Otto.bend(1, 500, -1);
    break;
  case 17: //M 17 500
    Otto.shakeLeg(1, 500, 1);
    break;
  case 18: //M 18 500
    Otto.shakeLeg(1, 500, -1);
    break;
  case 19: //M 19 500 20
    Otto.updown(2, 1000, 60);
    //Otto.jitter(1, 500, 30);
    break;
  case 20: //M 20 500 15
    Otto.ascendingTurn(2, 500, -30);
    break;
  case 21: //M 20 500 15
    OttoDancing();
    break;
  }
  Otto.home();
}
//-- Function to receive gesture commands
// void GesturePlay(int gesture) {
//   switch (gesture) {
//     case 1: //H 1
//       Otto.playGesture(OttoHappy);
//       break;
//     case 2: //H 2
//       Otto.playGesture(OttoSuperHappy);
//       break;
//     case 3: //H 3
//       Otto.playGesture(OttoSad);
//       break;
//     case 4: //H 4
//       Otto.playGesture(OttoSleeping);
//       break;
//     case 5: //H 5
//       Otto.playGesture(OttoFart);
//       break;
//     case 6: //H 6
//       Otto.playGesture(OttoConfused);
//       break;
//     case 7: //H 7
//       Otto.playGesture(OttoLove);
//       break;
//     case 8: //H 8
//       Otto.playGesture(OttoAngry);
//       break;
//     case 9: //H 9
//       Otto.playGesture(OttoFretful);
//       break;
//     case 10: //H 10
//       Otto.playGesture(OttoMagic);
//       break;
//     case 11: //H 11
//       Otto.playGesture(OttoWave);
//       break;
//     case 12: //H 12
//       Otto.playGesture(OttoVictory);
//       break;
//     case 13: //H 13
//       Otto.playGesture(OttoFail);
//       break;
//     default:
//       break;
//   }
//   if (gesture != 13) {
//     Otto.home();
//   }
// }

//-- Function to receive sing commands
void SingSong(int song)
{
  switch (song)
  {
  case 1: //K 1
          //  Otto.putMouth(happyOpen);
    Otto.sing(S_connection);

    break;
  case 2: //K 2
    //Otto.putMouth(angry);
    //  Otto.putAnimationMouth(adivinawi,1);
    Otto.sing(S_disconnection);
    break;
  case 3: //K 3
          //  Otto.putMouth(bigSurprise);
    Otto.sing(S_surprise);
    break;
  case 4: //K 4
          //  Otto.putMouth(bigSurprise);
    Otto.sing(S_OhOoh);
    break;
  case 5: //K 5
    Otto.sing(S_OhOoh2);
    break;
  case 6: //K 6
          // Otto.putMouth(heart);
    Otto.sing(S_cuddly);
    break;
  case 7: //K 7
          // Otto.putMouth(smallSurprise);
    Otto.sing(S_sleeping);
    break;
  case 8: //K 8
          // Otto.putMouth(smile);
    Otto.sing(S_happy);
    break;
  case 9: //K 9
          // Otto.putMouth(happyOpen);
    Otto.sing(S_superHappy);
    break;
  case 10: //K 10
           // Otto.putMouth(smile);
    Otto.sing(S_happy_short);
    break;
  case 11: //K 11
           // Otto.putMouth(sad);
    Otto.sing(S_sad);
    break;
  case 12: //K 12
           // Otto.putMouth(confused);
    Otto.sing(S_confused);
    break;
  case 13: //K 13
           // Otto.putMouth(tongueOut);
    Otto.sing(S_fart1);
    break;
  case 14: //K 14
           // Otto.putMouth(tongueOut);
    Otto.sing(S_fart2);
    break;
  case 15: //K 15
           //  Otto.putMouth(tongueOut);
    Otto.sing(S_fart3);
    break;
  case 16: //K 16
           // Otto.putMouth(one);
    Otto.sing(S_mode1);
    break;
  case 17: //K 17
           // Otto.putMouth(two);
    Otto.sing(S_mode2);
    break;
  case 18: //K 18
           // Otto.putMouth(three);
    Otto.sing(S_mode3);
    break;
  case 19: //K 19
           // Otto.putMouth(okMouth);
    Otto.sing(S_buttonPushed);
    break;
  default:
    break;
  }
}
