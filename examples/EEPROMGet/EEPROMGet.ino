#include <EEPROM.h>

struct Config_t
{
  char badgeText[41];
  int intensity;
  int scrollSpeed;
};

void setup() {
  EEPROM.begin(256);
  Serial.begin(115200);
  Serial.end();
  Config_t settings;
  EEPROM.get(1, settings);
  Serial.println(EEPROM.read(0));
  Serial.println(settings.badgeText);
  Serial.println(settings.intensity);
  Serial.println(settings.scrollSpeed);
  EEPROM.end();
}

void loop() {
  delay(5000);
}
