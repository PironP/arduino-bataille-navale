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

Find the 10 boats the computer hide on the game board

Move with the joystick and click on it to choose a case

Click on the joystick when the LED is Green to shoot

If you find shoot a boat the case become Red, otherwise it become yellow

You have a limited time so speed up :)
