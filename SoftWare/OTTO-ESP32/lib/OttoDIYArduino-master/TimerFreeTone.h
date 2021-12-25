// "TimerFreeTone.h" by Tim Eckel - teckel@leethost.com Copyright 2016 License: GNU GPLv3 
// OttDIY Arduino Project, 2020

// SYNTAX:
//   TimerFreeTone( pin, frequency, duration [, volume ] ) - Play a note on pin at frequency in Hz for duration in milliseconds.
//     Parameters:
//       * pin        - Pin speaker is wired to (other wire to ground, be sure to add an inline 100 ohm resistor).
//       * frequency  - Play the specified frequency (should work fairly well in the 100 to 15000 Hz range).
//       * duration   - Set the duration to play in milliseconds. Range: 0 to 65535 (65.5 seconds).
//       * volume     - Optionally set the tone volume level (from 1 to 10), defaults to full volume (10).

#ifndef TimerFreeTone_h
	#define TimerFreeTone_h
 	#include <Arduino.h>

 	void TimerFreeTone(uint8_t pin, unsigned long frequency, unsigned int duration, uint8_t volume = 10);
#endif

//end
