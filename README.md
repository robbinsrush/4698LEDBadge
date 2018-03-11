# Team4698Badge
## Designed and programmed by Rush Robbins
![3D Model of Badge](https://i.imgur.com/V3JbVzn.png)

This repository is dedicated to the code/libraries, 3D models, schematics, and PCB gerbers behind FIRST Robotics team **4698**, Rio Robotics' LED badge. 

The MAX7219 library used is a fork of @squix78's ESP8266 library with a slight modification to the order in which the matrices are drawn onto, as well as utilizing Adafruit's GFX font instead of squix's "cp437" font.

The library used can do the following:

- set pixels
- write text with a simple font
- scroll text left or right 
- oscillate text between the two ends

## Video Demonstration

https://streamable.com/58ed7

## Usage

1. Assemble the PCB (tutorial coming later?)

2. Upload code to ESP8266

3. Hold B and power on/reset the device

4. Use wifi-enabled device to connect to the given access point (password is the same as the access point name)
  - If you can not do this step, you may also use the example EEPROMSet to set it there instead of using a Wifi device.

5. Reset the device after seeing the success page.

6. Wear your badge with pride!

## Notes about assembly

- When using my PCB, it is most likely the text on the side of the matrix has to be facing to the right ([example shown here](https://i.imgur.com/QNqjoC9.jpg))
- On v1 (current) of the PCB, R is on D4 (GPIO2), which is also the onboard_led pin. Pushing it lights the LED. No adverse effects have been found yet. 
- Several inputs for 5V and 3.3V are put around the board to allow for easier access for batteries and the like. ([example shown here](https://i.imgur.com/4QV5dsC.png))
- As of writing, only 1 pushbutton (B) is in use in my code, but if you write your own code you can use the rest just fine
- A ball-chain necklace or similar length of material can be threaded through the large holes at the very top of the PCB **and are connected to circuit ground.** [Example image](https://i.imgur.com/z4SjA52.jpg)
## Parts
- 1x Custom PCB (designed by your truly) (Gerbers in repo and [here at EasyEDA](https://easyeda.com/nullstalgia/Badge_THT_and_MAX-0fb6900ba40b4741a26781ce35c9b7fa))
- 1x Wemos D1 Mini
- 1x or 4x pushbuttons (6x6mm) (as of writing, only 1 (B) is in use in my code, but if you write your own code you can use the rest)
- 2x 32mmx32mm 8x8 LED Matrix units. (Model I used: 1088AS)
- 2x 220uF caps (5mmx7mm) [Link from LCSC](https://lcsc.com/product-detail/Aluminum-Electrolytic-Capacitors-Leaded_220uF-10V_C43320.html)
- 2x 0.1uF caps. (Those common ones that say 104)
- 2x 30kΩ resistor (for ISET for the MAX7219)
- 2x MAX7219 I(DIP-24 package)

For battery operation:

- For the batch I am making, I am using a single 18650 battery cell and a charge/protect/boost circuit. I am also using a 100k resistor instead of the regular 30k to save on current draw in exchange for brightness (not as important in indoor conditions).

(NOTE: it is not advised to cheap out on the battery and charging circuit. Components can be replaced, injuries can be avoided; human lives can't be replaced, and injuries can be life-affecting)

- My setup images: https://imgur.com/a/ubTjM

#### Applicable copyrights:

- Adafruit Industries (for GFX font)
- Espressif (for ESP8266) <3
- Sparkfun (for some WiFi code)
- Arduino (for the amazing open platform)
- Autodesk (for the software used for the 3D model)
- JLCPCB/EasyEDA (for the PCB and Schematic)
- Wemos (for the D1 mini)
- [Github User Squix78 for the original library used](https://github.com/squix78/MAX7219LedMatrix)
- Rio Americano (for being the school I go to and who's logo and team I use and attend)
- Me! 

