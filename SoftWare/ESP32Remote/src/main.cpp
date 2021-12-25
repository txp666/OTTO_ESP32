#include "HAL.h"
#include "BluetoothSerial.h"

String name = "OttoESP32";
bool connected;
BluetoothSerial SerialBT;

void setup()
{
  pinMode(12, OUTPUT);
  digitalWrite(12, 1);
  Serial.begin(115200);
  SerialBT.begin("ESP32Remote", true);
  Serial.println("The device started in master mode, make sure remote BT device is on!");
  connected = SerialBT.connect(name);
  if (connected)
  {
    Serial.println("Connected Succesfully!");
    Audio_PlayMusic("Connect");
  }

  if (SerialBT.disconnect())
  {
    Serial.println("Disconnected Succesfully!");
  }
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();
  KEY_Init();
  Buzz_init();
  Audio_Init();
}

void loop()
{

  while (!SerialBT.connected())
  {
    Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
    SerialBT.connect(name);
  }
  int value_pinx = analogRead(pinX);
  int value_piny = analogRead(pinY);
  if (value_pinx > 3000)
  {
    Serial.println("左");
    SerialBT.print("M 3 500\r");
    delay(1000);
  }
  if (value_pinx < 700)
  {
    Serial.println("右");
    SerialBT.print("M 4 500\r");
    delay(1000);
  }
  if (value_piny > 3000)
  {
    Serial.println("前");
    SerialBT.print("M 1 500\r");
    delay(1000);
  }
  if (value_piny < 700)
  {
    Serial.println("后");
    SerialBT.print("M 2 500\r");
    delay(1000);
  }
  KEY_Update();
  Audio_Update();
}

void button1Click()
{
  SerialBT.print("M 21\r");
  Buzz_Tone(500, 20);
}
void button2Click()
{
  SerialBT.print("M 8\r");
  Buzz_Tone(500, 20);
}
void button3Click()
{
  SerialBT.print("M 14\r");
  Buzz_Tone(500, 20);
}
void button4Click()
{
  SerialBT.print("M 20\r");
  Buzz_Tone(500, 20);
}
void button5Click()
{
  SerialBT.print("M 5\r");
  Buzz_Tone(500, 20);
}
void button2DClick()
{
  SerialBT.print("M 19\r");
  Buzz_Tone(700, 20);
}
void button3DClick()
{
  SerialBT.print("M 13\r");
  Buzz_Tone(700, 20);
}
void button4DClick()
{
  SerialBT.print("M 7\r");
  Buzz_Tone(700, 20);
}
void button5DClick()
{
  SerialBT.print("M 17\r");
  Buzz_Tone(700, 20);
}
void button2LClick()
{
  SerialBT.print("M 15\r");
  Buzz_Tone(300, 5);
}
void button3LClick()
{
  SerialBT.print("M 10\r");
  Buzz_Tone(300, 5);
}
void button4LClick()
{
  SerialBT.print("M 12\r");
  Buzz_Tone(300, 5);
}
void button5LClick()
{
  SerialBT.print("M 6\r");
  Buzz_Tone(300, 5);
}
