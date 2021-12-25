// OttoDIY version 0.3.9
// OttDIY Arduino Project, 2020

#include "Arduino.h"
#include "Otto9.h"
#define M_PI		3.14159265358979323846
// Otto9 or Humanoid initialization
void Otto9::init(int YL, int YR, int RL, int RR, bool load_calibration, int NoiseSensor, int Buzzer, int USTrigger, int USEcho, int LA, int RA) {
  _servo_pins[0] = YL;
  _servo_pins[1] = YR;
  _servo_pins[2] = RL;
  _servo_pins[3] = RR;
  _servo_totals = 4; // Otto9
  if(LA != -1) {
	_servo_pins[4] = LA;
	_servo_pins[5] = RA;
	_servo_totals = 6; // Humanoid
  }
  attachServos();
  setRestState(false);
  if(load_calibration) {
	  loadCalibration();
  }
  for(int i = 0; i < _servo_totals; i++) {
    _servo_positions[i] = 90;
  }
  //Buzzer & noise sensor pins: 
  pinBuzzer = Buzzer;
  pinNoiseSensor = NoiseSensor;

  pinMode(Buzzer,OUTPUT);
  pinMode(NoiseSensor,INPUT);
}

// Otto9Humanoid initialization (depreciated)
void Otto9::initHUMANOID(int YL, int YR, int RL, int RR,int LA, int RA, bool load_calibration, int NoiseSensor, int Buzzer, int USTrigger, int USEcho) {
	init(YL, YR, RL, RR, load_calibration, NoiseSensor, Buzzer, USTrigger, USEcho, LA, RA);
}

// Load Calibration Trims
void Otto9::loadCalibration() {
	for(int i = 0; i < _servo_totals; i++) {
		int servo_trim = EEPROM.read(i);
		if(servo_trim > 128)
			servo_trim -= 256;
		_servos[i].setTrim(servo_trim);
	}
}

// ATTACH & DETACH FUNCTIONS
void Otto9::attachServos() {
  for(int i = 0; i < _servo_totals; i++) {
    _servos[i].attach(_servo_pins[i]);
  }
}

void Otto9::detachServos() {
  for(int i = 0; i < _servo_totals; i++) {
    _servos[i].detach();
  }
}

// OSCILLATORS TRIMS
void Otto9::setTrims(int YL, int YR, int RL, int RR, int LA, int RA) {
  _servos[0].setTrim(YL);
  _servos[1].setTrim(YR);
  _servos[2].setTrim(RL);
  _servos[3].setTrim(RR);
  if(_servo_totals > 4) {
	_servos[4].setTrim(LA);
	_servos[5].setTrim(RA);
  }
}

void Otto9::saveTrimsOnEEPROM() {
  for(int i = 0; i < _servo_totals; i++) { 
    EEPROM.write(i, _servos[i].getTrim());
  } 
}

// BASIC MOTION FUNCTIONS
void Otto9::moveServos(int time, int  servo_target[]) {
  attachServos();
  if(getRestState()==true){
    setRestState(false);
  }
  if(time > 10) {
    for(int i = 0; i < _servo_totals; i++) 
      _increment[i] = ((servo_target[i]) - _servo_positions[i]) / (time / 10.0);
    _final_time =  millis() + time;

    for(int iteration = 1; millis() < _final_time; iteration++) {
      yield();
      _partial_time = millis() + 10;
      for(int i = 0; i < _servo_totals; i++) {
        _servos[i].setPosition(_servo_positions[i] + (iteration * _increment[i]));
      }
      while(millis() < _partial_time) { // pause
        yield();
      }
    }
  } else {
    for(int i = 0; i < _servo_totals; i++) {
      _servos[i].setPosition(servo_target[i]);
    }
  }
  for(int i = 0; i < _servo_totals; i++) { 
    _servo_positions[i] = servo_target[i];
  }
}

void Otto9::moveSingle(int position, int servo_number) {
  if(position > 180) 
    position = 180;
  if(position < 0) 
    position = 0;
  attachServos();
  if(getRestState() == true) {
    setRestState(false);
  }
  _servos[servo_number].setPosition(position);
}

void Otto9::oscillateServos(int A[], int O[], int T, double phase_diff[], float cycle) {
  for(int i = 0; i < _servo_totals; i++) {
    _servos[i].setO(O[i]);
    _servos[i].setA(A[i]);
    _servos[i].setT(T);
    _servos[i].setPh(phase_diff[i]);
  }
  double ref = millis();
  for(double x = ref; x <= T * cycle + ref; x = millis()) {
    for(int i = 0; i < _servo_totals; i++) {
      yield();
      _servos[i].refresh();
    }
  }
}

void Otto9::execute(int A[], int O[], int T, double phase_diff[], float steps) {
  attachServos();
  if(getRestState() == true) {
    setRestState(false);
  }
  int cycles = (int)steps;
  if(cycles >= 1) {   // Execute complete cycles
    for(int i = 0; i < cycles; i++) {
      oscillateServos(A, O, T, phase_diff);
    }
  }     
  oscillateServos(A, O, T, phase_diff, (float)steps - cycles); // Execute the final not complete cycle    
}

bool Otto9::getRestState() {
  return _isOttoResting;
}

void Otto9::setRestState(bool state) {
    _isOttoResting = state;
}

// PREDETERMINED MOTION SEQUENCES

// Otto movement: Home (Otto at rest position)
void Otto9::home() {
  if(getRestState() == false) { // Go to rest position only if necessary
	int homes[6] = {90, 90, 90, 90, 90, 90}; //All the servos at rest position
    moveServos(500, homes); //Move the servos in half a second
    detachServos();
    setRestState(true);
  }
}

// Otto movement: Jump
//  Parameters:
//    steps: Number of steps
//    T: Period
void Otto9::jump(float steps, int T) {
  int up[6] = {90, 90, 150, 30, 110, 70};
  int down[6] = {90, 90, 90, 90, 90, 90};
  moveServos(T, up);
  moveServos(T, down);
}

// Otto gait: Walking  (forward or backward)    
//  Parameters:
//    * steps:  Number of steps
//    * T : Period
//    * Dir: Direction: FORWARD / BACKWARD
void Otto9::walk(float steps, int T, int dir) {
  // Oscillator parameters for walking
  // Hip sevos are in phase
  // Feet servos are in phase
  // Hip and feet are 90 degrees out of phase
  //      -90 : Walk forward
  //       90 : Walk backward
  // Feet servos also have the same offset (for tiptoe a little bit)
  int A[6] = {30, 30, 20, 20, 90, 90};
  int O[6] = {0, 0, 4, -4, 0 , 0};
  double phase_diff[6] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90), 0, 0};
  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);  
}

// Otto gait: Turning (left or right)
//  Parameters:
//   * Steps: Number of steps
//   * T: Period
//   * Dir: Direction: LEFT / RIGHT
void Otto9::turn(float steps, int T, int dir) {
  // Same coordination than for walking (see Otto::walk)
  // The Amplitudes of the hip's oscillators are not igual
  // When the right hip servo amplitude is higher, the steps taken by
  //   the right leg are bigger than the left. So, the robot describes an left arc
  int A[6] = {30, 30, 20, 20, 90, 90};
  int O[6] = {0, 0, 4, -4, 0, 0};
  double phase_diff[6] = {0, 0, DEG2RAD(-90), DEG2RAD(-90), 0, 0}; 
  if(dir == LEFT) {  
    A[0] = 30; // Left hip servo
    A[1] = 10; // Right hip servo
  } else {
    A[0] = 10;
    A[1] = 30;
  }
  // Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps); 
}

// Otto gait: Lateral bend
//  Parameters:
//    steps: Number of bends
//    T: Period of one bend
//    dir: RIGHT=Right bend LEFT=Left bend
void Otto9::bend(int steps, int T, int dir) {
  // Parameters of all the movements. Default: Left bend
  int bend1[6] = {90, 90, 62, 35, 120, 60}; 
  int bend2[6] = {90, 90, 62, 105, 60, 120};
  int homes[6] = {90, 90, 90, 90, 90, 90};
  //T=max(T, 600); // Time of one bend, constrained in order to avoid movements too fast.
  if(dir == RIGHT) { // Changes in the parameters if right direction is chosen 
    bend1[2] = 145;
    bend1[3] = 120;
    bend2[2] = 75;
    bend2[3] = 120;
  }
  // Time of the bend movement. Fixed parameter to avoid falls
  int T2 = 800; 
  //Bend movement
  for(int i=0; i < steps; i++) {
    moveServos(T2 / 2, bend1);
    moveServos(T2 / 2, bend2);
    delay(T * 0.8);
    moveServos(500, homes);
  }
}

// Otto gait: Shake a leg
//  Parameters:
//    steps: Number of shakes
//    T: Period of one shake
//    dir: RIGHT=Right leg LEFT=Left leg
void Otto9::shakeLeg(int steps, int T,int dir) {
  int numberLegMoves = 2; // This variable change the amount of shakes
  // Parameters of all the movements. Default: Left leg
  int shake_leg1[6] = {90, 90, 58, 35, 90, 90};   
  int shake_leg2[6] = {90, 90, 58, 120, 100, 80};
  int shake_leg3[6] = {90, 90, 58, 60, 80, 100};
  int homes[6] = {90, 90, 90, 90, 90, 90};
  if(dir == RIGHT) { // Changes in the parameters if right leg is chosen
    shake_leg1[2] = 145;
    shake_leg1[3] = 122;
    shake_leg2[2] = 60;
    shake_leg2[3] = 122;
    shake_leg3[2] = 120;
    shake_leg3[3] = 122;
  }
  int T2 = 1000; // Time of the bend movement. Fixed parameter to avoid falls
  T = T - T2; // Time of one shake, constrained in order to avoid movements too fast.            
  T = max(T, 200 * numberLegMoves);  
  for(int j = 0; j < steps; j++) {
    // Bend movement
    moveServos(T2 / 2, shake_leg1);
    moveServos(T2 / 2, shake_leg2);
    // Shake movement
    for(int i = 0; i < numberLegMoves; i++) {
      moveServos(T / (2 * numberLegMoves), shake_leg3);
      moveServos(T / (2 * numberLegMoves), shake_leg2);
    }
    moveServos(500, homes); //Return to home position
  }
  delay(T);
}

// Otto movement: up & down
//  Parameters:
//    * steps: Number of jumps
//    * T: Period
//    * h: Jump height: SMALL / MEDIUM / BIG 
//              (or a number in degrees 0 - 90)
void Otto9::updown(float steps, int T, int h) {
  // Both feet are 180 degrees out of phase
  // Feet amplitude and offset are the same
  // Initial phase for the right foot is -90, so that it starts
  //   in one extreme position (not in the middle)
  int A[6]= {0, 0, h, h, h, h};
  int O[6] = {0, 0, h, -h, h, -h};
  double phase_diff[6] = {0, 0, DEG2RAD(-90), DEG2RAD(90),DEG2RAD(-90), DEG2RAD(90) };
  //-- Let's oscillate the servos!
  //_execute(A, O, T, phase_diff, steps); 
  execute(A, O, T, phase_diff, steps); 
}

// Otto movement: swinging side to side
//  Parameters:
//     steps: Number of steps
//     T : Period
//     h : Amount of swing (from 0 to 50 aprox)
void Otto9::swing(float steps, int T, int h) {
  // Both feet are in phase. The offset is half the amplitude
  // It causes the robot to swing from side to side
  int A[6]= {0, 0, h, h, h, h};
  int O[6] = {0, 0, h/2, -h/2, h, -h};
  double phase_diff[6] = {0, 0, DEG2RAD(0), DEG2RAD(0), DEG2RAD(0), DEG2RAD(0)};
  // Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps); 
}

// Otto movement: swinging side to side without touching the floor with the heel
//  Parameters:
//     steps: Number of steps
//     T : Period
//     h : Amount of swing (from 0 to 50 aprox)
void Otto9::tiptoeSwing(float steps, int T, int h) {
  // Both feets are in phase. The offset is not half the amplitude in order to tiptoe
  // It causes the robot to swing from side to side
  int A[6]= {0, 0, h, h, h, h};
  int O[6] = {0, 0, h, -h, h, -h};
  double phase_diff[6] = {0, 0, 0, 0, 0, 0};
  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps); 
}

// Otto gait: Jitter 
//  Parameters:
//    steps: Number of jitters
//    T: Period of one jitter 
//    h: height (Values between 5 - 25)   
void Otto9::jitter(float steps, int T, int h) {
  // Both feet are 180 degrees out of phase
  // Feet amplitude and offset are the same
  // Initial phase for the right foot is -90, so that it starts
  //   in one extreme position (not in the middle)
  // h is constrained to avoid hit the feets
  //h = min(25, h);
  int A[6]= {h, h, 0, 0, 0, 0};
  int O[6] = {0, 0, 0, 0, 0, 0};
  double phase_diff[6] = {DEG2RAD(-90), DEG2RAD(90), 0, 0, 0, 0};
  // Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps); 
}

// Otto gait: Ascending & turn (Jitter while up&down)
//  Parameters:
//    steps: Number of bends
//    T: Period of one bend
//    h: height (Values between 5 - 15) 
void Otto9::ascendingTurn(float steps, int T, int h) {
  // Both feet and legs are 180 degrees out of phase
  // Initial phase for the right foot is -90, so that it starts
  //   in one extreme position (not in the middle)
  // h is constrained to avoid hit the feets
  // h = min(20, h);
  int A[6]= {h, h, h, h, 40, 40};
  int O[6] = {0, 0, h+4, -h+4, 0, 0};
  double phase_diff[6] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90), 0, 0};
  // Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps); 
}

// Otto gait: Moonwalker. Otto moves like Michael Jackson
//  Parameters:
//    Steps: Number of steps
//    T: Period
//    h: Height. Typical valures between 15 and 40
//    dir: Direction: LEFT / RIGHT
void Otto9::moonwalker(float steps, int T, int h, int dir) {
  // This motion is similar to that of the caterpillar robots: A travelling
  // wave moving from one side to another
  // The two Otto's feet are equivalent to a minimal configuration. It is known
  // that 2 servos can move like a worm if they are 120 degrees out of phase
  // In the example of Otto, the two feet are mirrored so that we have:
  //    180 - 120 = 60 degrees. The actual phase difference given to the oscillators
  //  is 60 degrees.
  //  Both amplitudes are equal. The offset is half the amplitud plus a little bit of
  //  offset so that the robot tiptoe lightly
  int phi = (-dir * 90);
  int A[6]= {0, 0, h, h, h, h};
  int O[6] = {0, 0, h/2+2, -h/2 -2, -h, h};
  double phase_diff[6] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi), DEG2RAD(phi), DEG2RAD(phi)};
  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps); 
}

// Otto gait: Crusaito. A mixture between moonwalker and walk
//   Parameters:
//     steps: Number of steps
//     T: Period
//     h: height (Values between 20 - 50)
//     dir:  Direction: LEFT / RIGHT
void Otto9::crusaito(float steps, int T, int h, int dir) {
  int A[6]= {25, 25, h, h, 0, 0};
  int O[6] = {0, 0, h/2+ 4, -h/2 - 4, 0, 0};
  double phase_diff[6] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir), 0, 0};
  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}

// Otto gait: Flapping
//  Parameters:
//    steps: Number of steps
//    T: Period
//    h: height (Values between 10 - 30)
//    dir: direction: FOREWARD, BACKWARD
void Otto9::flapping(float steps, int T, int h, int dir) {
  int A[6]= {12, 12, h, h, 0, 0};
  int O[6] = {0, 0, h - 10, -h + 10, 0, 0};
  double phase_diff[6] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir), 0, 0};
  // Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}

// Otto movement: Hands up
void Otto9::handsup() {
  if(_servo_totals > 4) {
	int homes[6] = {90, 90, 90, 90, 20, 160}; //
	moveServos(1000, homes);   //Move the servos in half a second
  }
}

// Otto movement: Wave , either left or right
void Otto9::handwave(int dir){
  if(_servo_totals > 4) {
	if(dir == RIGHT) {
		int A[6] = {0, 0, 0, 0, 30, 0}; // right hand wave
		int O[6] = {0, 0, 0, 0, -30, -40};
		double phase_diff[6] = {0, 0, 0, 0, DEG2RAD(0),0};
		// Let's oscillate the servos!
		execute(A, O, 1000, phase_diff, 5); 
	}
	if(dir == LEFT) {
		int A[6] = {0, 0, 0, 0, 0, 30}; // left hand wave
		int O[6] = {0, 0, 0, 0, 40, 60};
		double phase_diff[6] = {0, 0, 0, 0, 0, DEG2RAD(0)};
		// Let's oscillate the servos!
		execute(A, O, 1000, phase_diff, 1); 
	}
  }
}

///////////////////////////////////////////////////////////////////
//-- SOUNDS -----------------------------------------------------//
///////////////////////////////////////////////////////////////////

void Otto9::_tone (float noteFrequency, long noteDuration, int silentDuration){

    // tone(10,261,500);
    // delay(500);

      if(silentDuration==0){silentDuration=1;}

      tone(Otto9::pinBuzzer, noteFrequency, noteDuration);
      delay(noteDuration);       //milliseconds to microseconds
      //noTone(PIN_Buzzer);
      delay(silentDuration);     
}


void Otto9::bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration){

  //Examples:
  //  bendTones (880, 2093, 1.02, 18, 1);
  //  bendTones (note_A5, note_C7, 1.02, 18, 0);

  if(silentDuration==0){silentDuration=1;}

  if(initFrequency < finalFrequency)
  {
      for (int i=initFrequency; i<finalFrequency; i=i*prop) {
          _tone(i, noteDuration, silentDuration);
      }

  } else{

      for (int i=initFrequency; i>finalFrequency; i=i/prop) {
          _tone(i, noteDuration, silentDuration);
      }
  }
}

void Otto9::sing(int songName){
  switch(songName){

    case S_connection:
      _tone(note_E5,50,30);
      _tone(note_E6,55,25);
      _tone(note_A6,60,10);
    break;

    case S_disconnection:
      _tone(note_E5,50,30);
      _tone(note_A6,55,25);
      _tone(note_E6,50,10);
    break;

    case S_buttonPushed:
      bendTones (note_E6, note_G6, 1.03, 20, 2);
      delay(30);
      bendTones (note_E6, note_D7, 1.04, 10, 2);
    break;

    case S_mode1:
      bendTones (note_E6, note_A6, 1.02, 30, 10);  //1318.51 to 1760
    break;

    case S_mode2:
      bendTones (note_G6, note_D7, 1.03, 30, 10);  //1567.98 to 2349.32
    break;

    case S_mode3:
      _tone(note_E6,50,100); //D6
      _tone(note_G6,50,80);  //E6
      _tone(note_D7,300,0);  //G6
    break;

    case S_surprise:
      bendTones(800, 2150, 1.02, 10, 1);
      bendTones(2149, 800, 1.03, 7, 1);
    break;

    case S_OhOoh:
      bendTones(880, 2000, 1.04, 8, 3); //A5 = 880
      delay(200);

      for (int i=880; i<2000; i=i*1.04) {
           _tone(note_B5,5,10);
      }
    break;

    case S_OhOoh2:
      bendTones(1880, 3000, 1.03, 8, 3);
      delay(200);

      for (int i=1880; i<3000; i=i*1.03) {
          _tone(note_C6,10,10);
      }
    break;

    case S_cuddly:
      bendTones(700, 900, 1.03, 16, 4);
      bendTones(899, 650, 1.01, 18, 7);
    break;

    case S_sleeping:
      bendTones(100, 500, 1.04, 10, 10);
      delay(500);
      bendTones(400, 100, 1.04, 10, 1);
    break;

    case S_happy:
      bendTones(1500, 2500, 1.05, 20, 8);
      bendTones(2499, 1500, 1.05, 25, 8);
    break;

    case S_superHappy:
      bendTones(2000, 6000, 1.05, 8, 3);
      delay(50);
      bendTones(5999, 2000, 1.05, 13, 2);
    break;

    case S_happy_short:
      bendTones(1500, 2000, 1.05, 15, 8);
      delay(100);
      bendTones(1900, 2500, 1.05, 10, 8);
    break;

    case S_sad:
      bendTones(880, 669, 1.02, 20, 200);
    break;

    case S_confused:
      bendTones(1000, 1700, 1.03, 8, 2); 
      bendTones(1699, 500, 1.04, 8, 3);
      bendTones(1000, 1700, 1.05, 9, 10);
    break;

    case S_fart1:
      bendTones(1600, 3000, 1.02, 2, 15);
    break;

    case S_fart2:
      bendTones(2000, 6000, 1.02, 2, 20);
    break;

    case S_fart3:
      bendTones(1600, 4000, 1.02, 2, 20);
      bendTones(4000, 3000, 1.02, 2, 20);
    break;

  }
}
//end

