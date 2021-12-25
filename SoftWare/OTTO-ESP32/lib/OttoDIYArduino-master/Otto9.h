// OttoDIY version 0.3.9
// OttDIY Arduino Project, 2020

#ifndef OTTO9_h
#define OTTO9_h

#include <EEPROM.h>
#include "Oscillator.h"
#include "TimerFreeTone.h"
#include "US.h"
//#include "BatReader9.h"
#include "Matrix9.h"
#include "Mouth9.h"
#include "Sound9.h"
#include "Gestures9.h"

//-- Constants
#define FORWARD     1
#define BACKWARD    -1
#define LEFT        1
#define RIGHT       -1
#define SMALL       5
#define MEDIUM      15
#define BIG         30

class Otto9 {
  public:
    void init(int YL, int YR, int RL, int RR, bool load_calibration,
    	int NoiseSensor, int Buzzer, int USTrigger, int USEcho, int LA = -1, int RA = -1); // Otto9 or Humanoid initialization
    void initHUMANOID(int YL, int YR, int RL, int RR,int LA, int RA, bool load_calibration,
    	int NoiseSensor, int Buzzer, int USTrigger, int USEcho); // Otto9Humanoid initialization (depreciated)
    void attachServos(); // Attach & detach functions
    void detachServos();
    void setTrims(int YL, int YR, int RL, int RR, int LA = 0, int RA = 0); // Oscillator Trims
    void saveTrimsOnEEPROM();
    void moveServos(int time, int servo_target[]); // Predetermined Motion Functions
    void moveSingle(int position,int servo_number);
    void oscillateServos(int A[6], int O[6], int T, double phase_diff[6], float cycle = 1);
    void execute(int A[6], int O[6], int T, double phase_diff[6], float steps = 1.0);
    void home(); // HOME = Otto at rest position
    bool getRestState();
    void setRestState(bool state);
    void jump(float steps = 1, int T = 2000); // Predetermined Motion Functions
    void walk(float steps = 4, int T = 1000, int dir = FORWARD);
    void turn(float steps = 4, int T = 2000, int dir = LEFT);
    void bend(int steps = 1, int T = 1400, int dir = LEFT);
    void shakeLeg(int steps = 1, int T = 2000, int dir = RIGHT);
    void updown(float steps = 1, int T = 1000, int h = 20);
    void swing(float steps = 1, int T = 1000, int h = 20);
    void tiptoeSwing(float steps = 1, int T = 900, int h = 20);
    void jitter(float steps = 1, int T = 500, int h = 20);
    void ascendingTurn(float steps = 1, int T = 900, int h = 20);
    void moonwalker(float steps = 1, int T = 900, int h = 20, int dir = LEFT);
    void crusaito(float steps = 1, int T = 900, int h = 20, int dir = FORWARD);
    void flapping(float steps = 1, int T = 1000, int h = 20, int dir = FORWARD);
    void handsup();
    void handwave(int dir = RIGHT);
    void loadCalibration();
  
    // Sensors functions
    float getDistance(); // US sensor
    int getNoise();      // Noise Sensor
/*
    // Battery
    double getBatteryLevel();
    double getBatteryVoltage();
*/    
    // Mouth & Animations
    void putMouth(unsigned long int mouth, bool predefined = true);
    void putAnimationMouth(unsigned long int anim, int index);
    void clearMouth();

    // Sounds
    void _tone(float noteFrequency, long noteDuration, int silentDuration);
    void bendTones(float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration);
    void sing(int songName);

    // Gestures
    void playGesture(int gesture);
    void initMATRIX(int DIN, int CS, int CLK, int rotate);
    void matrixIntensity(int intensity);
    void initBatLevel(int batteryPIN);
    void setLed(byte X, byte Y, byte value);
    void writeText(const char * s, byte scrollspeed);
  
  private:
    Oscillator _servos[6];
    int _servo_pins[6];
    int _servo_trims[6];
    int _servo_positions[6];
    int _servo_totals;
    unsigned long _final_time;
    unsigned long _partial_time;
    float _increment[6];
    bool _isOttoResting;
    //BatReader9 battery;
    US us;
    Otto_Matrix ledmatrix;
    int pinBuzzer;
    int pinNoiseSensor;
    unsigned long int getMouthShape(int number);
    unsigned long int getAnimShape(int anim, int index);
};
#endif

//end
