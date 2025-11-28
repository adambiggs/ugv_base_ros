// LED LEDC channels for old API (ESP32 Arduino v2.x)
#define LED_CH_IO4 0
#define LED_CH_IO5 1

void led_pin_init(){
  // Old API: setup channel then attach pin
  ledcSetup(LED_CH_IO4, FREQ, ANALOG_WRITE_BITS);
  ledcSetup(LED_CH_IO5, FREQ, ANALOG_WRITE_BITS);
  ledcAttachPin(IO4_PIN, LED_CH_IO4);
  ledcAttachPin(IO5_PIN, LED_CH_IO5);
}

void led_pwm_ctrl(int io4Input, int io5Input) {
  ledcWrite(LED_CH_IO4, constrain(io4Input, 0, 255));
  ledcWrite(LED_CH_IO5, constrain(io5Input, 0, 255));
}
