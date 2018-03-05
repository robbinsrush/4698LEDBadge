#include <SPI.h>
#include "LedMatrix.h"
#define NUMBER_OF_DEVICES 2
#define CS_PIN D8

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

bool screenTest = true;
bool brightnessTest = true;
bool buttonTest = true;

int screenTestDelay = 100;

bool buttonGood = false;

void setup() {
  ledMatrix.setTextAlignment(0);
  ledMatrix.init();

  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
}

void loop() {
  //Screen testing
  if (screenTest) {
    //One pixel at a time, left to right
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < (NUMBER_OF_DEVICES * 8); x++) {
        ledMatrix.clear();
        ledMatrix.setPixel(x, y);
        ledMatrix.commit();
        delay(screenTestDelay);
      }
    }
    ledMatrix.clear();

    //Same as previous, but persists until the end
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < (NUMBER_OF_DEVICES * 8); x++) {
        ledMatrix.setPixel(x, y);
        ledMatrix.commit();
        delay(screenTestDelay);
      }
    }

    //Tests brightness from min to max to min.
    if (brightnessTest) {
      for (int intensity = 0; intensity < 16; intensity++) {
        ledMatrix.setIntensity(intensity);
        delay(screenTestDelay);
      }
      for (int intensity = 15; intensity >= 0; intensity--) {
        ledMatrix.setIntensity(intensity);
        delay(screenTestDelay);
      }
    }
  }

  //Button test
  if (buttonTest) {

    buttonGood = false;
    ledMatrix.clear();
    ledMatrix.setText("L");
    ledMatrix.drawText();
    ledMatrix.commit();
    while (!buttonGood) {
      if (!digitalRead(D3)) {
        buttonGood = true;
      }
      delay(15);
    }

    buttonGood = false;
    ledMatrix.clear();
    ledMatrix.setText("R");
    ledMatrix.drawText();
    ledMatrix.commit();
    while (!buttonGood) {
      if (!digitalRead(D4)) {
        buttonGood = true;
      }
      delay(15);
    }

    buttonGood = false;
    ledMatrix.clear();
    ledMatrix.setText("B");
    ledMatrix.drawText();
    ledMatrix.commit();
    while (!buttonGood) {
      if (!digitalRead(D2)) {
        buttonGood = true;
      }
      delay(15);
    }

    buttonGood = false;
    ledMatrix.clear();
    ledMatrix.setText("A");
    ledMatrix.drawText();
    ledMatrix.commit();
    while (!buttonGood) {
      if (!digitalRead(D1)) {
        buttonGood = true;
      }
      delay(15);
    }
  }
  // All tests done, restart to the beginning.
}
