/* 
   An Arduino Library for OSBee Shield
   http://bee.opensprinkler.com
   
   License: Creative Commons CC-SA 3.0
   
   Written by : Ray Wang (Rayshobby LLC)
   info@rayshobby.net
   
*/

#ifndef _OSBEE_H
#define _OSBEE_H

// Default boost voltage, unit is Volt
#define DEFAULT_BOOST_VOLTAGE 16.0

// Default duty cycle (percentage)
#define DEFAULT_BOOST_DUTY    25

// Default pulse length, unit is millisecond
#define DEFAULT_PULSE_LENGTH  25

#define pinBOOST    3 // boost PWM pin
#define pinBOOST_FB 0 // boost feedback pin (analog pin)
#define pinBATT_FB  1 // battery voltage feedback pin (analog pin)
#define pinSET_A    6 // Set reset pins for the solenoid
#define pinRST_A    7

// Turn on serial debug messages 
#define SERIAL_DEBUG 

class OSBee {
private:
  int voltage_level;  // analog value for boost converter feedback
  int pulse_length;   // pulse length (in milliseconds)
  int duty_cycle;     // duty cycle for PWM
  bool initialized;   
  void boost();       // boost voltage
public:
  OSBee();
  void begin();
  // set boost voltage, generally between 9 and 22
  void setVoltage(float v);
  // set pulse length (generally between 20 to 200)
  void setPulseLength(int l);
  // set duty cycle, generally between 5 and 50
  // increasing duty cycle results in higher current draw and faster boost time
  // decreasing duty cycle results in lower current draw and slower boost time
  void setDutyCycle(int c);
  void closeA();
  void openA();
  float getBattVoltage(); // get battery voltage
};

#endif
