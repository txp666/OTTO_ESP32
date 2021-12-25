// Oscillator.pde Generate sinusoidal oscillations in the servos
// GPL license (c) Juan Gonzalez-Gomez (Obijuan), Dec 2011
// OttDIY Arduino Project, 2020 | sfranzyshen

#include "Arduino.h"
#include "Oscillator.h"

Oscillator::Oscillator(int trim) {
  _trim = trim;
}

void Oscillator::setA(int A) {
  _A = A;
}

void Oscillator::setO(int O) {
  _O = O;
}

void Oscillator::setPh(double Ph) {
  _phase0 = Ph;
}

void Oscillator::setTrim(int trim) {
  _trim = trim;
}

int Oscillator::getTrim() {
  return _trim;
}

void Oscillator::stop() {
  _stop = true;
}

void Oscillator::play() {
  _stop = false;
}

void Oscillator::reset() {
  _phase = 0;
}

// Attach an oscillator to a servo
// Input: pin is the arduino pin were the servo
// is connected
void Oscillator::attach(int pin, bool rev) {
  if(!_servo.attached()) { // If the oscillator is detached, attach it.
    _servo.attach(pin); // Attach the servo and move it to the home position
    _servo.write(90);
    _TS = 30;
    _T = 2000;
    _NS = _T / _TS;
    _inc = 2 * M_PI / _NS;
    _previousMillis = 0;
    _A = 45; // Default parameters
    _phase = 0;
    _phase0 = 0;
    _O = 0;
    _stop = false;
    _rev = rev; // Reverse mode
  }
}

// Detach an oscillator from his servo
void Oscillator::detach() {
  if(_servo.attached())
    _servo.detach();
}

// Set the oscillator period, in ms
void Oscillator::setT(unsigned int T) {
  _T = T; // Assign the new period
  _NS = _T / _TS; // Recalculate the parameters
  _inc = 2 * M_PI / _NS;
}

// Manual set of the position
void Oscillator::setPosition(int position) {
  _servo.write(position + _trim);
}

// This function should be periodically called
// in order to maintain the oscillations. It calculates
// if another sample should be taken and position the servo if so
void Oscillator::refresh() {
  if(_next_sample()) { // Only When TS milliseconds have passed, the new sample is obtained
    if(!_stop) { // If the oscillator is not stopped, calculate the servo position
      _pos = round(_A * sin(_phase + _phase0) + _O); // Sample the sine function and set the servo pos
      if(_rev) 
        _pos = -_pos;
      _servo.write(_pos + 90 +_trim);
    }
    // Increment the phase
    // It is always increased, even when the oscillator is stop
    // so that the coordination is always kept
    _phase = _phase + _inc;
  }
}

// This function returns true if another sample
// should be taken (i.e. the TS time has passed since
// the last sample was taken
bool Oscillator::_next_sample() {
  _currentMillis = millis(); // Read current time
  if(_currentMillis - _previousMillis > _TS) { // Check if the timeout has passed
    _previousMillis = _currentMillis;   
    return true;
  }
  return false;
}

