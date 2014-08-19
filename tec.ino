void setTecDutyCycle(int dutyCycle) {
  String msg = "TEC duty cycle: ";
  DEBUG_PRINTLN(msg + dutyCycle);
  analogWrite(TEC_PWM, (dutyCycle / 100) * 255);
}
