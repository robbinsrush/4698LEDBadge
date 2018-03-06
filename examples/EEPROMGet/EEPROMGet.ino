#include <EEPROM.h>

struct Config_t
{
  char badgeText[41];
  int intensity;
  int scrollSpeed;
  int scrollDirection;
};

void setup() {
  EEPROM.begin(256);
  Serial.begin(115200);
  Config_t settings;
  EEPROM.get(1, settings);
  Serial.println(EEPROM.read(0));
  Serial.println(settings.badgeText);
  Serial.println(settings.intensity);
  Serial.println(settings.scrollSpeed);
  Serial.println(settings.scrollDirection);
  EEPROM.end();
}

void loop() {
  delay(5000);
}
