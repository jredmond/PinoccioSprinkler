/* 
   An Arduino Library for OSBee Shield
   http://bee.opensprinkler.com
   
   License: Creative Commons CC-SA 3.0
   
   Written by : Ray Wang (Rayshobby LLC)
   info@rayshobby.net
   
*/
//#include <Serial.h>
#include "Arduino.h"
#include "OSBee.h"

// Constructor function
OSBee::OSBee() {
  voltage_level = 0;
  pulse_length = 0;
  duty_cycle = 0;
  initialized = false;
}

// Must call this function first
void OSBee::begin() {

  // !!!!!! IMPORTANT !!!!!!
  // Must set ADC to use internal reference
  analogReference(INTERNAL); 
    
  setVoltage(DEFAULT_BOOST_VOLTAGE);
  setPulseLength(DEFAULT_PULSE_LENGTH);
  setDutyCycle(DEFAULT_BOOST_DUTY);

  // !!!!!! IMPORTANT !!!!!!
  // Set pins in output mode, and clear all pins
  pinMode(pinSET_A, OUTPUT);
  digitalWrite(pinSET_A, LOW);
  
  pinMode(pinRST_A, OUTPUT);
  digitalWrite(pinRST_A, LOW);
      
  // Set timer 3 to fast pwm mode corresponding to pin D3 (pinBOOST)
  // The ideal frequency depends on the inductor, but it'll only be important if you need alot more power.
  // set prescaler to 1
  TCCR3B = _BV(CS00);
  TCCR3A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01) | _BV(WGM00);
        
  initialized = true;
  
  // close all valves
  closeA();
  
#ifdef SERIAL_DEBUG
  Serial.println("Initialized.");
#endif  
}

// Boost voltage
void OSBee::boost() {
  if (!initialized) return;
  
  // !!!!!! IMPORTANT !!!!!!
  // Must set ADC to use internal reference
  analogReference(INTERNAL);  

#ifdef SERIAL_DEBUG
  Serial.print("Boosting...");
#endif

  unsigned int measure;
 
  analogWrite(pinBOOST, 0);
  
  measure = analogRead(pinBOOST_FB);
  if (measure < voltage_level)
    analogWrite(pinBOOST, duty_cycle * 256 / 100);  // set duty cycle
    
  unsigned long t = millis();
  while (measure < voltage_level) {
    measure = analogRead(pinBOOST_FB);
    #ifdef SERIAL_DEBUG
      Serial.println(String(measure));
    #endif
    // set a time limit : 30 seconds
    if (millis() > t + 30000)  break;
  }
  analogWrite(pinBOOST, 0);


#ifdef SERIAL_DEBUG
  Serial.println("done.");
#endif
}

// Calculate the desired analog input reading, for a given voltage
void OSBee::setVoltage(float v) {

  // On OSBee, the boost feedback divider is 100K top, 4.7K bottom.
  // So the analog reading should be:
  // voltage * 47 / (1000 + 47) / 1.1 * 1024
  // where 1.1 is ATmega328's internal voltage reference
  voltage_level = (int)(v*41.8);

#ifdef SERIAL_DEBUG
  Serial.print("Voltage level: ");
  Serial.println(voltage_level);
#endif
}

// Set pulse length (usually between 50 to 200)
void OSBee::setPulseLength(int l) {
  pulse_length = l;
}

void OSBee::setDutyCycle(int c) {
  if (c>0 && c<=50)
    duty_cycle = c;
}

// Close valve A
void OSBee::closeA() {
  int set_pin = pinSET_A;
  int rst_pin = pinRST_A;
  
  digitalWrite(set_pin, LOW);
  digitalWrite(rst_pin, LOW);
  
  boost();
  
  digitalWrite(rst_pin, HIGH);
  delay(pulse_length);
  digitalWrite(rst_pin, LOW);
  
#ifdef SERIAL_DEBUG
  Serial.println("Valve A closed");
#endif  
}

// Open valve A
void OSBee::openA() {
  int set_pin = pinSET_A;
  int rst_pin = pinRST_A;
  
  digitalWrite(set_pin, LOW);
  digitalWrite(rst_pin, LOW);
  
  boost();
  
  digitalWrite(set_pin, HIGH);
  delay(pulse_length);
  digitalWrite(set_pin, LOW);
  
#ifdef SERIAL_DEBUG
  Serial.println("Valve A opened.");
#endif  
}

// get battery voltage
float OSBee::getBattVoltage() {
  // !!!!!! IMPORTANT !!!!!!
  // Must set ADC to use internal reference
  analogReference(INTERNAL); 
  
  // On OSBee, the battery feedback divider is 470K top, 100K bottom.
  // analog reading = voltage * 100 / (100 + 470) / 1.1 * 1024
  // where 1.1 is ATmega328's internal voltage reference
  return analogRead(pinBATT_FB) * 0.00612;
}

