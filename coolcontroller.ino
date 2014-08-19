/* Cool Controller
 * 2014 Andrew Raines
 */

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
  if (encoderChanged()) {
    Serial.println(getEncoderPos());
    Serial.println(getEncoderBtn());
  }
  if (getTemperatures()) {
    
  }
}
