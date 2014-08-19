// Utilise the Encoder Library ( http://www.pjrc.com/teensy/td_libs_Encoder.html )
// We set the no interrupts to keep the library as small as possible as we don't
// intend on using interrupt functionality for the input controller.
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

// Utilise the Bounce2 Library ( https://github.com/thomasfredericks/Bounce-Arduino-Wiring )
// Allows debouncing of button inputs
#include <Bounce2.h>

// Set up the encoder
Encoder enc(ENCODER_A, ENCODER_B);
// Set up encoder button debouncer
Bounce enc_btn = Bounce(); 
// Encoder position and button status
long enc_pos_val = -999;
int  enc_btn_val = HIGH;

void setupEncoder() {
  // Set up encoder button
  pinMode(ENCODER_BTN, INPUT);
  digitalWrite(ENCODER_BTN, HIGH); // internal pull-up
  enc_btn.attach(ENCODER_BTN); // set up debouncer
  enc_btn.interval(ENCODER_DB); // debounce interval
  DEBUG_PRINT("Encoder: A: "); DEBUG_PRINT(ENCODER_A); 
  DEBUG_PRINT(" B: ");         DEBUG_PRINT(ENCODER_B);
  DEBUG_PRINT(" button: ");    DEBUG_PRINT(ENCODER_BTN);
  DEBUG_PRINT(" debounce: ");  DEBUG_PRINT(ENCODER_DB);
  DEBUG_PRINTLN("");
}

boolean encoderChanged() {
  boolean changed = false;
  // read the encoder position (it does 4 for each tick)
  long pos = enc.read();
  if (pos >= enc_pos_val + 4 || pos <= enc_pos_val - 4) {
    enc_pos_val = pos;
    changed = true;
  }
  
  enc_btn.update();
  int value = enc_btn.read();
  if (value != enc_btn_val) {
    enc_btn_val = value;
    changed = true;
  }
  
  return changed;
}

long getEncoderPos() {
  // We will return the current value then reset to 0
  long val = enc_pos_val;
  enc.write(0);
  enc_pos_val = 0;
  return enc_pos_val;
}

long getEncoderBtn() {
  return enc_btn_val;
}
