/* Cool Controller
 * 2014 Andrew Raines
 */

// Include PID control library - https://github.com/br3ttb/Arduino-PID-Library/
#include <PID_v1.h>

// DEBUG DEFINITIONS
#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINT(x)   Serial.print(x)
#else
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
#endif

// ====================
// Pin defitions
// ====================
// Avoid the use of pins 0, 1 & 13 because they have LEDs which can interfere
// Pins 2 & 3 are the only interrupt pins available on the UNO
// Pins 5 & 6  - PWM timer 0 (messing with the prescaler on timer 0 will break timing calls)
// Pins 9 & 10 - PWM timer 1
// Pins 3 & 11 - PWM timer 2 (unavailable pins as we need them for other things!)
// Pins 11 & 13 are reserved for SPI data transfer
// Pins A0-A5 can be used as digital (14-19)

// Pins utilised for the encoder
#define ENCODER_A   14
#define ENCODER_B   15
#define ENCODER_BTN 16
#define ENCODER_DB  5    // encoder debounce period in ms

// Pins utilised for the fans
#define FAN_HOT_PWM  9   // Must be on 9 and 10 for Timer1
#define FAN_HOT_SIG  2
#define FAN_COLD_PWM 10  // Must be on 9 and 10 for Timer1
#define FAN_COLD_SIG 3

// Pins utilised for the TFT
//      TFT_SCLK 13
//      TFT_MOSI 11
#define TFT_CS   8
#define TFT_DC   7
#define TFT_RST  6

// Pins utilised for the TEC
#define TEC_PWM 5

// Pins utilised for temp sensors
#define TEMP_SIG 19



// ====================
// Program start
// ====================

// Set up PIDs
double cold_temp, tec_pwm, cold_set_temp, hot_temp, hotfan_pwm, hot_set_temp;
// Input (temp), Output (PWM), SetPoint (desired temp), Kp, Ki, Kd, direction
PID cold_pid(&cold_temp, &tec_pwm, &cold_set_temp,2,5,1, DIRECT);
PID hot_pid(&hot_temp, &hotfan_pwm, &hot_set_temp,2,5,1, DIRECT);

void setup() {
  String msg;
  
  // Set up serial channel for debug
  Serial.begin(9600);
  DEBUG_PRINTLN("========================");
  DEBUG_PRINTLN("SET UP PARAMETERS");
  DEBUG_PRINTLN("");
  
  setupEncoder();
  setupFans();
  setupTempSensors();
  
  DEBUG_PRINTLN("Setup complete.");
  DEBUG_PRINTLN("========================");
}


void loop() {

}

void doMenu() {
  if (encoderChanged()) {
    Serial.println(getEncoderPos());
    Serial.println(getEncoderBtn());
  }
}

/* Main Control Algorithm
 Basic Principle:
 The TEC is controlled such that the duty cycle is increased to decrease the cool side,
 or reduced if an alarm condition is met (MOSFET or hot side too hot).
 The hot fan is controlled such that the duty cycle is increased to decrease the hot side
 temperature to within operational temp, and reduced to reduce noise.
 The cold fan is controlled manually by the user.
*/
void setupControl() {
  // Initially set values to safe ones
  cold_temp     = 0;
  cold_set_temp = 999; // Switch off TEC
  hot_temp      = 999;
  hot_set_temp  = 0;   // Hot fan to 100%
  
  // Turn on PID controllers
  cold_pid.SetOutputLimits(0,100);
  hot_pid.SetOutputLimits(0,100);
  cold_pid.SetMode(AUTOMATIC);
  hot_pid.SetMode(AUTOMATIC);
}

#define HOT_ALARM_HI 80
#define FET_ALARM_HI 80
#define HOT_ALARM_LO 75
#define FET_ALARM_LO 75

void doControl() {
  int hot_temp, cold_temp, fet_temp;
  
  // TODO: Read temperatures
  
  if (hot_temp > HOT_ALARM_HI || fet_temp > FET_ALARM_HI) {
    // Alarm condition! Turn off the cold pid and set to safety
    cold_pid.SetMode(MANUAL);
    tec_pwm = 0;
  } else if (hot_temp < HOT_ALARM_LO && fet_temp < FET_ALARM_LO) {
    // Exit alarm condition.  Re-enable the cold pid
    cold_pid.SetMode(AUTOMATIC);
  }
  cold_pid.Compute();
  hot_pid.Compute();
  setHotFanSpeed(hotfan_pwm);
  setColdFanSpeed(tec_pwm);
}
