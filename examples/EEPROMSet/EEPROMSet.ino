#include <EEPROM.h>

struct Config_t
{
  char badgeText[41];
  int intensity;
  int scrollSpeed;
};

void setup() {
  EEPROM.begin(256);
  Config_t settings = {
    "YOUR TEXT HERE",
    0,
    50
  };

  EEPROM.put(1, settings);
  EEPROM.write(0,1);
  EEPROM.commit();
  EEPROM.end();
}

void loop() {
  delay(5000);
}
