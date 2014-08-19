// Utilise the TimerOne Library ( https://github.com/PaulStoffregen/TimerOne )
// For simple set up of PWM for fan control
#include <TimerOne.h>

void setupFans() {
  // Fan PWMs must be on timer1.  Fans require ~25kHz signal
  Timer1.initialize(40);  // 40 us = 25 kHz
  DEBUG_PRINTLN("PWM initialised on Timer 1 at 25kHz");
  DEBUG_PRINTLN("");
}

// PWM Procedures
void setHotFanSpeed(int dutyCycle) {
  String msg = "Hot fan duty cycle: ";
  DEBUG_PRINTLN(msg + dutyCycle);
  Timer1.pwm(FAN_HOT_PWM, (dutyCycle / 100) * 1023);
}
void setColdFanSpeed(int dutyCycle) {
  String msg = "Cold fan duty cycle: ";
  DEBUG_PRINTLN(msg + dutyCycle);
  Timer1.pwm(FAN_COLD_PWM, (dutyCycle / 100) * 1023);
}
