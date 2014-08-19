// Utilise the OneWire Library ( http://www.pjrc.com/teensy/td_libs_OneWire.html )
// Read from DS18B20 (and similar) digital temp sensors
#include <OneWire.h>

// Temperature sensor resolution (accuracy vs speed)
// 0x00 9-bit resolution  (93.75ms delay)
// 0x20 10-bit resolution (187.5ms delay)
// 0x40 11-bit resolution (375ms delay)
// 0x60 12-bit resolution (750ms delay)
const byte TEMP_RES = 0x00;

// Sensor configuration
const int NUM_TEMP_SENSORS = 6;
const byte TEMP_ADDRS[NUM_TEMP_SENSORS][8] = {
  {0x11, 0xF2, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
const String TEMP_NAMES[NUM_TEMP_SENSORS] = {
  "EXH HOT",
  "EXH COLD",
  "TEC HOT",
  "TEC COLD",
  "AMBIENT",
  "MOSFET"
};

// Set up temp sensor pin
OneWire temp(TEMP_SIG);  // (a 4.7K pull-up resistor is necessary)


// Temp sensor procedures
void setupTempSensors() {
  int i;
  for (i=0; i<NUM_TEMP_SENSORS; i++) {
    setupTempSensor((byte*)TEMP_ADDRS[i]);
  } 
}

void setupTempSensor(byte addr[8]) {
  #ifdef DEBUG
    int i;
    DEBUG_PRINT("Temp sensor "); DEBUG_PRINT(i); DEBUG_PRINT(" at addr: 0x");
    for (i=0; i<8; i++) {
      char hex[2];
      sprintf(hex, "%02X", (int)addr[i]);
      DEBUG_PRINT(hex);
    }
    DEBUG_PRINTLN("");
  #endif
  
  temp.reset();
  temp.select(addr);
  temp.write(0x4E);  // Write scratchpad
  temp.write(0x00);  // Th register
  temp.write(0x00);  // Tl register
  temp.write(TEMP_RES | 0x1F); // Config register
}

boolean temp_reading = false;

boolean getTemperatures() {
  int i;
  float celsius;
  
  if (!temp_reading) {
    temp.reset();
    temp.skip();          // Don't select, just broadcast
    temp.write(0x44, 0);  // Ask all sensors to read temps
    temp_reading = true;  // Mark as currently being read
  }
  if (temp.read() == 0) {
    // Temperature conversion not yet complete
    return false; // not yet ready
  }
  temp_reading = false; // Finished reading
  // Read each sensor
  for (i=0; i<NUM_TEMP_SENSORS; i++) {
    celsius = readTemp((byte*)TEMP_ADDRS[i]);
  }
  return true;
}
    
float readTemp(byte addr[8]) {
  int i;
  byte data[9];

  temp.reset();
  temp.select(addr);
  temp.write(0xBE);

  for (i=0; i<9; i++) { // we need 9 bytes
    data[i] = temp.read();
  }
  
  // Convert bytes to actual temperature (16 bit signed int)
  int16_t raw = (data[1] << 8) | data[0];
  byte cfg = (data[4] & 0x60);
  
  // Zero out any low bits which are undefined depending on resolution
  switch (cfg) {
    case 0x00: raw = raw & ~7; break;
    case 0x20: raw = raw & ~3; break;
    case 0x40: raw = raw & ~1; break;
  }
  
  // Return value is in Celsius
  return (float)raw / 16.0;
}
