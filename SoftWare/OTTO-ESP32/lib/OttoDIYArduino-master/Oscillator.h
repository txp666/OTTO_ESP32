// Oscillator.pde Generate sinusoidal oscillations in the servos
// GPL license (c) Juan Gonzalez-Gomez (Obijuan), Dec 2011
// OttDIY Arduino Project, 2020 | sfranzyshen

#ifndef Oscillator_h
#define Oscillator_h

#if defined(ESP32)
# include <ESP32Servo.h>
#else
# include <Servo.h>
#endif

#ifndef DEG2RAD // Macro for converting from degrees to radians
  #define DEG2RAD(g) ((g) * M_PI) / 180
#endif

class Oscillator {
  public:
    Oscillator(int trim = 0);
    void attach(int pin, bool rev = false);
    void detach();
    void setA(int A);
    void setO(int O);
    void setPh(double Ph);
    void setT(unsigned int T);
    void setTrim(int trim);
    int getTrim();
    void setPosition(int position); 
    void stop();
    void play();
    void reset();
    void refresh();
  private:
    bool _next_sample();  
    Servo _servo; // Servo that is attached to the oscillator
    // Oscillators parameters
    int _A;  // Amplitude (degrees)
    int _O;  // Offset (degrees)
    unsigned int _T;  // Period (miliseconds)
    double _phase0;   // Phase (radians)
    // Internal variables
    int _pos;         // Current servo pos
    int _trim;        // Calibration offset
    double _phase;    // Current phase
    double _inc;      // Increment of phase
    double _NS;       // Number of samples
    unsigned int _TS; // sampling period (ms)
    long _previousMillis; 
    long _currentMillis;
    bool _stop; // Oscillation mode. If true, the servo is stopped
    bool _rev; // Reverse mode
};

#endif
