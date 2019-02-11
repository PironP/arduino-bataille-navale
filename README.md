# Sea Battle

Arduino Project

## Componants

- Arduino Uno
- Joystick
- 8X8 RGB LED Matrix
- RGB LED
- 8 Digit with MAX7219 controller

## Connection

### RGB LED Matrix

DIN : PIN 6

### Joystick

VRX : PIN A0

VRY : PIN A1

SW : PIN 4

### RGB LED

BLUE : PIN 9

GREEN : PIN 10

RED : PIN 11

### 8 Digit Display

DIN : PIN 7

CS : PIN 6

CLOCK : PIN 5

## Library

DigitDisplay.h : https://github.com/ozhantr/DigitLedDisplay

Adafruit_NeoPixel.h : https://github.com/adafruit/Adafruit_NeoPixel

## The Game

Find the 10 boats on the game board that the computer had hidden.

Move and click the joystick to aim a case.

Click on the joystick at the right time (LED is Green), to power up your shoot.

If you hit a boat the case turns Red, otherwise it turns Yellow.

Time is limited, so hurry up ! :)
