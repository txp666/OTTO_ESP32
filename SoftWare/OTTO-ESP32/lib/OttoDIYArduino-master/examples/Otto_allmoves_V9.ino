/*
Otto All moves Arduino test 
OttoDIY Arduino Project, 2020 | sfranzyshen
*/

#include <Otto9.h> //-- Otto Library version 9
Otto9 Otto;  //This is Otto!

int bend1[] = {90, 90, 62, 35}; 
int bend2[] = {30, 10, 62, 105};
  
void setup() {
  Serial.begin(115200);
  //Otto.init(D1, D6, D7, D5, false, 0, 1, 2, 3);
  Otto.init(2, 3, 4, 5, false, 0, 1, 2, 3);
  Otto.home();
}

void loop() {
  Otto.moveServos(1000, bend1);
  Otto.moveServos(3000, bend2);
  /*
  Otto.walk(1, 3000, 1);           // 2 steps, "TIME". IF HIGHER THE VALUE THEN SLOWER (from 600 to 1400), 1 FORWARD
  Otto.walk(2, 1000, -1);          // 2 steps, T, -1 BACKWARD 
  Otto.turn(2, 1000, 1);            // 3 steps turning LEFT
  Otto.home();
  delay(100);  
  Otto.turn(2, 1000, -1);         // 3 steps turning RIGHT 
  Otto.bend(1, 500, 1);           // usually steps =1, T=2000
  Otto.bend(1, 2000, -1);     
  Otto.shakeLeg(1, 1500, 1);
  Otto.home();
  delay(100);
  Otto.shakeLeg(1, 2000, -1);
  Otto.moonwalker(3, 1000, 25, 1); // LEFT
  Otto.moonwalker(3, 1000, 25,-1); // RIGHT  
  Otto.crusaito(2, 1000, 20, 1);
  Otto.crusaito(2, 1000, 20, -1);
  delay(100);
  Otto.flapping(2, 1000, 20, 1);
  Otto.flapping(2, 1000, 20, -1);
  delay(100);
  Otto.swing(2, 1000, 20);
  Otto.tiptoeSwing(2, 1000, 20);
  Otto.jitter(2, 1000, 20);       // (small T)
  Otto.updown(2, 1500, 20);       // 20 = H "HEIGHT of movement"T 
  Otto.ascendingTurn(2, 1000, 50);
  Otto.jump(1, 2000);
  Otto.home();
  */
  Serial.println("Finished");
}
