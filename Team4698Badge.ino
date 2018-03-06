#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <SPI.h>
#include "LedMatrix.h"

#define NUMBER_OF_DEVICES 2
#define CS_PIN D8

MDNSResponder mdns;
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

bool setupBool = false;
static const int EEPROMSize = 256;
int absDirection;

// Alright, let's get commenty


// This struct is universal in all instances of EEPROM reading in writing in the examples and main sketch
struct Config_t {
  // Max 40 chars of (hopefully) unicode(?) text to be displayed on the badge
  // In all honesty, it could be more, but 40 is a nice number that can hold a lot of info but not TOO much on such a small screen
  char badgeText[41];
  // "Intensity," because who calls brightness "brightness" anymore? Not the dudebros behind the MAX7219, that's for sure
  int intensity;
  // The delay between each frame of text.
  int scrollSpeed;
  // Direction and orientation of the text
  // Modes:
  // 0 & 1: left to right
  // 2: right to left
  // -1 & -2: same as their above counterparts, but upside down
  int scrollDirection;
} settings;

// ^ and nicely initialized for use globally.

// Getting the bytesize of the badgeText to base other crap off of (like the char array in saveSettings())
int badgeTextSize = sizeof Config_t().badgeText;

// ew
const char INDEX_HTML[] =
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>LED Badge Configuration</title>"
  "<style>"
  "\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
  "</style>"
  "</head>"
  "<body>"
  "<h1>LED Badge Configuration</h1>"
  "<FORM action=\"/\" method=\"post\">"
  "<P>"
  "Badge Text<br>"
  "<INPUT type=\"text\" name=\"badgeText\" maxlength=\"40\" required=\"true\"/><br><br>"
  "Badge Brightness<br>"
  "0 (lowest) to 15 (brightest)<br>"
  "Lower = more battery life<br>"
  "<SELECT name=\"intensity\">"
  "<OPTION value=\"0\">0</OPTION>"
  "<OPTION value=\"1\">1</OPTION>"
  "<OPTION value=\"2\">2</OPTION>"
  "<OPTION value=\"3\">3</OPTION>"
  "<OPTION value=\"4\">4</OPTION>"
  "<OPTION value=\"5\">5</OPTION>"
  "<OPTION value=\"6\">6</OPTION>"
  "<OPTION value=\"7\">7</OPTION>"
  "<OPTION value=\"8\">8</OPTION>"
  "<OPTION value=\"9\">9</OPTION>"
  "<OPTION value=\"10\">10</OPTION>"
  "<OPTION value=\"11\">11</OPTION>"
  "<OPTION value=\"12\">12</OPTION>"
  "<OPTION value=\"13\">13</OPTION>"
  "<OPTION value=\"14\">14</OPTION>"
  "<OPTION value=\"15\">15</OPTION>"
  "</SELECT><br><br>"
  "Text Speed<br>"
  "<SELECT name=\"speed\">"
  "<OPTION value=\"300\">0%</OPTION>"
  "<OPTION value=\"200\">40%</OPTION>"
  "<OPTION value=\"150\">55%</OPTION>"
  "<OPTION value=\"125\">65%</OPTION>"
  "<OPTION value=\"115\">70%</OPTION>"
  "<OPTION value=\"100\">75%</OPTION>"
  "<OPTION value=\"90\">80%</OPTION>"
  "<OPTION value=\"75\">85%</OPTION>"
  "<OPTION value=\"50\">90%</OPTION>"
  "<OPTION value=\"30\">100%</OPTION>"
  "</SELECT><br><br>"
  "Text Direction<br>"
  "<SELECT name=\"direction\">"
  "<OPTION value=\"1\">Left to Right</OPTION>"
  "<OPTION value=\"2\">Right to Left</OPTION>"
  "</SELECT><br><br>"
  "Upside Down<br>"
  "<INPUT type=\"checkbox\" name=\"upsideDown\" value=\"yes\"/><br><br><br>"
  "<INPUT type=\"submit\" value=\"Save!\">"
  "</P>"
  "</FORM>"
  "</body>"
  "</html>";

// more ew
const char SUCCESS_HTML[] =
  "<!DOCTYPE HTML>"
  "<html>"
  "<head>"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
  "<title>LED Badge Configuration</title>"
  "<style>"
  "\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
  "</style>"
  "</head>"
  "<body>"
  "<h1>Success. Reset without holding B to see the new settings.</h1>"
  "</body>"
  "</html>";

// Handling HTTP Requests.
void handleRoot()
{
  // Tiny sanity check #1 (seeing if there was text submitted, and if there was, send it over to the submit function to be processed)
  if (server.hasArg("badgeText")) {
    handleSubmit();
  }
  // but if no next was submitted, give them the form
  else {
    server.send(200, "text/html", INDEX_HTML);
  }
}

// if someone got to this, godspeed

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}


// OK, someone sent in some text, let's look at it and get ready to save it
void handleSubmit()
{
  // Tiny sanity check #2, if it somehow got lost... be amazed
  if (!server.hasArg("badgeText")) return returnFail("BAD ARGS");
  Serial.println(server.arg("badgeText"));
  // #3. If it's not empty, go ahead and send it over to the save function
  if (server.arg("badgeText") != "") {
    saveSettings(server.arg("badgeText"), server.arg("intensity"), server.arg("speed"), server.arg("direction"), server.arg("upsideDown"));
    // After saving, let the user know all is good.
    server.send(200, "text/html", SUCCESS_HTML);
  }
  else {
    // you did bad, son
    returnFail("Bad values");
  }
}


// Yay, time to save stuff to the ESP's EEPROM
void saveSettings(String badgeText, String intensity, String speed, String direction, String upsideDown) {
  // Turn our badgeText string into a char array
  char badgeTextChar[badgeTextSize];
  badgeText.toCharArray(badgeTextChar, badgeTextSize);

  // Turn our strings into ints.
  int intensityInt, speedInt, directionInt;
  intensityInt = intensity.toInt();
  speedInt = speed.toInt();
  directionInt = direction.toInt();

  // If upside down, turn it negative to notify the sketch later
  if (upsideDown == "yes") {
    directionInt = directionInt * -1;
  }

  // And character by character, put every char from our local to our global char Array.
  for (int i = 0; i < badgeTextSize; i++) {
    settings.badgeText[i] = badgeTextChar[i];
  }
  // And same goes to the ints.
  settings.intensity = intensityInt;
  settings.scrollSpeed = speedInt;
  settings.scrollDirection = directionInt;

  // Open up EEPROM.
  EEPROM.begin(EEPROMSize);
  // plop in our settings
  EEPROM.put(1, settings);
  // Set the "OK" flag to true
  EEPROM.write(0, 1);
  // Commit and end the session
  EEPROM.commit();
  EEPROM.end();

}

bool loadSettings() {
  // Open up EEPROM
  EEPROM.begin(EEPROMSize);
  // Check our "OK" flag
  if (EEPROM.read(0) == 1) {
    // If we're good, grab the settings and put them in our global struct
    EEPROM.get(1, settings);
    return true;
  } else {
    // Otherwise, tell papa loop() that we're not so happy
    return false;
  }
}

void returnOK()
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

// Necessary for the captive portal and in case some idjit goes to the wrong page.
void handleNotFound() {
  // So the below commented code works just fine, I just do this to make sure the user gets forced by the captive portal to the main page, or is at least asked to "sign in" by going to that page.

  server.send(200, "text/html", INDEX_HTML);


  /*
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
  */
}

// So when our ESP boots
void setup(void)
{

  // Since on our PCB, our buttons short pins D1-4 to ground, we use the built-in Pullups on each pin to make it easy on us.
  // True = not pushed
  // False = pushed
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);

  // If button B is being held while booting, set the setup flag to true
  if (!digitalRead(D2)) {
    setupBool = true;
  }

  // We open up serial
  Serial.begin(115200);
  Serial.end();
  // and kill it since we dont need it. But we open it up anyway so the Serial.prints strewn around don't yell about Serial not being in the scope.

  // Tell our matrix to wake up
  ledMatrix.init();

  // Now, if we're setting stuff up...
  if (setupBool) {
    // Set up our captive portal
    dnsServer.start(DNS_PORT, "*", apIP);
    // The rest of the WiFi booting
    setupWiFi();

    // Basic Web server handling
    server.on("/", handleRoot);
    server.onNotFound(handleNotFound);
    server.begin();

  } else {
    // But if we're just being a badge, we turn off WiFi entirely (saves battery and bandwidth around the world.) <3
    WiFi.mode(WIFI_OFF);
    // Load settings and check if settings were able to be at the same time
    if (loadSettings()) {
      // If they were, set the brightness and text to be shown
      ledMatrix.setText(settings.badgeText);
      ledMatrix.setIntensity(settings.intensity);

      // If the scroll direction is negative, we know it's meant to be shown upside down.
      if (settings.scrollDirection < 0) {
        ledMatrix.setUpsideDown(true);
      }

      // Get an absolute so we can check just one variable here and in loop()
      absDirection = abs(settings.scrollDirection);

      if (absDirection == 2) {
        // If it's meant to be right to left, put the text in a suitable spot to be scrolled
        ledMatrix.setTextAlignment(3);
      }


      // Delay is done in the loop()
    } else {
      // But if our OK flag was false, then have our display say that it needs to be set up.
      ledMatrix.setText("No settings! Hold B and reset!");
      while (true) {
        ledMatrix.clear();
        ledMatrix.scrollTextLeft();
        ledMatrix.drawText();
        ledMatrix.commit();
        delay(50);
      }
    }
  }
}

// Ran during setup()
void setupWiFi() {
  // boot up our AP
  WiFi.mode(WIFI_AP);

  // Do a very smol amount of work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  const int len = 19;
  String macID = WiFi.macAddress().substring(len - 8, len);
  macID.replace(":", "");

  // Get the name/pass ready to be turned into a char Array
  String AP_NameString = "badge" + macID;

  // kinda ew but it works i guess
  // not my idea
  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  // Tell our user where to go and what the credentials are
  ledMatrix.setText("WiFi Name and Pass: " + AP_NameString + "      URL: 192.168.4.1");

  // And our AP should be ready to go.
  WiFi.softAP(AP_NameChar, AP_NameChar);
}


void loop(void)
{
  if (setupBool) {
    // Letting our DNS and web server work
    dnsServer.processNextRequest();
    server.handleClient();
    /*
       This was going to be used to change the text from "Wifi name and pass.... waiting..." to showing the IP, but I decided to just combine it with a bunch of spaces for simplicity's sake
      if(WiFi.softAPgetStationNum() > 0){
      ledMatrix.setNextText("URL: 192.168.4.1");
      }*/
    // And every once in a while, scroll our display a little bit
    ledMatrix.clear();
    ledMatrix.scrollTextLeft();
    ledMatrix.drawText();
    ledMatrix.commit();
    // This may eventually be changed to a millis() comparison, but in my testing, it works fine.
    delay(50);
  } else {
    // The while is just so our chip doesn't check a bool that isn't going to change.
    Serial.println(absDirection);
    while (true) {
      ledMatrix.clear();

      if (absDirection == 0 || absDirection == 1) {
        ledMatrix.scrollTextLeft();
      } else {
        ledMatrix.scrollTextRight();
      }
      ledMatrix.drawText();
      ledMatrix.commit();
      // The user-chosen delay.
      delay(settings.scrollSpeed);
    }
  }
}
