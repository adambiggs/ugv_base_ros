// <<<<<<<<<<=== === ===SSD1306: 0x3C=== === ===>>>>>>>>>>
// 0.91inch OLED
bool screenDefaultMode = true;

unsigned long currentTimeMillis = millis();
unsigned long lastTimeMillis = millis();

// default
String screenLine_0;
String screenLine_1;
String screenLine_2;
String screenLine_3;

// custom
String customLine_0;
String customLine_1;
String customLine_2;
String customLine_3;

// #include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH   128 // OLED display width, in pixels
#define SCREEN_HEIGHT  32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// init oled ctrl functions.
void init_oled(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.display();
}


// Updata all data and flash the screen.
void oled_update() {
  display.clearDisplay();
  display.setCursor(0,0);

  display.println(screenLine_0);
  display.println(screenLine_1);
  display.println(screenLine_2);
  display.println(screenLine_3);

  display.display();
}

// Calculate battery percentage (3S Li-ion: 12.6V full, 9.6V empty)
float calculateBatteryPercentage(float voltage) {
  const float fullVoltage = 12.6;  // 4.2V per cell * 3
  const float emptyVoltage = 9.6;  // 3.2V per cell * 3
  
  if (voltage >= fullVoltage) return 100.0;
  if (voltage <= emptyVoltage) return 0.0;
  
  return ((voltage - emptyVoltage) / (fullVoltage - emptyVoltage)) * 100.0;
}

// dev info update on oled.
void oledInfoUpdate() {
  currentTimeMillis = millis();
  if (currentTimeMillis - lastTimeMillis > 10000) {
    inaDataUpdate();
    lastTimeMillis = currentTimeMillis;
  } else {
    return;
  }
  if (!screenDefaultMode) {
    return;
  }

  float batteryPct = calculateBatteryPercentage(loadVoltage_V);
  screenLine_2 = "Batt: " + String((int)batteryPct) + "%";
  screenLine_3 = "V:" + String(loadVoltage_V,1) + "V I:" + String(current_mA/1000.0,1) + "A";
  oled_update();
}

// oled ctrl.
void oledCtrl(byte inputLineNum, String inputMegs) {
  screenDefaultMode = false;
  switch (inputLineNum) {
  case 0: customLine_0 = inputMegs;break;
  case 1: customLine_1 = inputMegs;break;
  case 2: customLine_2 = inputMegs;break;
  case 3: customLine_3 = inputMegs;break;
  }
  display.clearDisplay();
  display.setCursor(0,0);

  display.println(customLine_0);
  display.println(customLine_1);
  display.println(customLine_2);
  display.println(customLine_3);

  display.display();
}

// set oled as default.
void setOledDefault(){
  screenDefaultMode = true;
  inaDataUpdate();
  float batteryPct = calculateBatteryPercentage(loadVoltage_V);
  screenLine_2 = "Batt: " + String((int)batteryPct) + "%";
  screenLine_3 = "V:" + String(loadVoltage_V,1) + "V I:" + String(current_mA/1000.0,1) + "A";
  oled_update();
  lastTimeMillis = currentTimeMillis;
}