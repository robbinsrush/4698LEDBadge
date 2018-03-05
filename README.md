# 4698LEDBadge
## Designed and programmed by Rush Robbins
![3D Model of Badge](https://i.imgur.com/V3JbVzn.png)

This repository is dedicated to the code/libraries, 3D models, schematics, and PCB gerbers behind FIRST Robotics team **4698**, Rio Robotics' LED badge. 

The MAX7219 library used is a fork of @squix78's ESP8266 library with a slight modification to the order in which the matrices are drawn onto, as well as utilizing Adafruit's GFX font instead of squix's "cp437" font.

The library used can do the following:

- set pixels
- write text with a simple font
- scroll text left or right 
- oscillate text between the two ends

## Usage

Todo

## Notes about assembly

Todo

## Parts
- 1x Custom PCB (designed by your truly)
- 1x Wemos D1 Mini
- 1x or 4x pushbuttons (6x6mm) (as of writing, only 1 is in use in my code, but if you write your own code you can use the rest)
- 2x 32mmx32mm 8x8 LED Matrix units. (Model I used: 1088AS)
- 2x 220uF caps (5mmx7mm) [Link from LCSC](https://lcsc.com/product-detail/Aluminum-Electrolytic-Capacitors-Leaded_220uF-10V_C43320.html)
- 2x 0.1uF caps. (Those common ones that say 104)
- 2x 30kΩ resistor (for ISET for the MAX2719)
- 2x MAX7219 I(DIP-24 package)

For battery operation:

- ***TODO HELP PLS***

#### Applicable copyrights:

- Adafruit Industries (for GFX font)
- Espressif (for ESP8266) <3
- Sparkfun (for some WiFi code)
- Arduino (for the amazing open platform)
- Autodesk (for the software used for the 3D model)
- JLCPCB/EasyEDA (for the PCB and Schematic)
- Wemos (for the D1 mini)
- Me! 

