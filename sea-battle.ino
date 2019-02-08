/***
 * 
 * Sea Battle
 * Made By
 *  - Prescilla Lecurieux
 *  - Pierre Piron
 *  
 *  2019
 */

#include "binary.h"
#include <Adafruit_NeoPixel.h>

// Matrix RGB
#define PIN 6

// Joystick
const int VRx = 0; // Connect to Analog Pin 0
const int VRy = 1; // Connect to Analog Pin 1
int SW = 4; // Connect to Digital Pin 4
int X = 0;
int Y = 0;
int pressed = 1;

// RGB LED
int redPin = 11;
int greenPin = 10;
int bluePin = 9;

// delay time between faces
unsigned long delaytime=500;
// Use to animate the board
int turn = 0;
// Player location
int playerXLocation = 0;
int playerYLocation = 0;

// Variables to stop the game
int gameEnded = 0;
int gameTime = 120;

// Define the RGB Matrix
Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800);

/* 
 *  Game Board
 *  0 = Blue / 1 = Red / 2 = Yellow  / 3 = Green
 */
byte board[8][8] = {{3,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};

/* 
 *  Location of boats and shoot
 *  0 no boat / 1 boat destroyed / 2 = place shoot with no boat / 3 = boat
 */
byte boats[8][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};

// Matrix Model for the end of the game
// happy face
byte hf[8][8] = {{0,0,1,1,1,1,0,0},{0,1,0,0,0,0,1,0},{1,0,1,0,0,1,0,1},{1,0,0,0,0,0,0,1},{1,0,1,0,0,1,0,1},{1,0,0,1,1,0,0,1},{0,1,0,0,0,0,1,0},{0,0,1,1,1,1,0,0}};
// sad face
byte sf[8][8] = {{0,0,1,1,1,1,0,0},{0,1,0,0,0,0,1,0},{1,0,1,0,0,1,0,1},{1,0,0,0,0,0,0,1},{1,0,0,1,1,0,0,1},{1,0,1,0,0,1,0,1},{0,1,0,0,0,0,1,0},{0,0,1,1,1,1,0,0}};


/* 
 * Setup the game 
 */
void setup() {
  strip.begin();
  strip.setBrightness(2);
  strip.show(); // Initialize all pixels to 'off'
  pinMode(SW, INPUT_PULLUP);
  digitalWrite(SW, HIGH);
  setupBoats();
}

/* 
 * game loop 
 */
void loop(){
  if (gameEnded == 0) {
    setGameBoard();
    drawBoard();
    listenJoystick();
    listenSelector();
    changeTurn();
    if (verifyVictory() == 1) {
      gameEnded = 1;
      showVictory();
    } else if (gameTime == 0) {
      gameEnded = 1;
      showDefeat();
    }
    gameTime--;
  }
}

/*
 * Set on the game board array the player and the shoots 
 */
void setGameBoard() {
 for (int i = 0; i < 8; i++) {
  for (int j = 0; j < 8; j++) {
    int TEMP = 0;
    if (boats[i][j] == 1) {
      TEMP = 1;
    } else if (boats[i][j] == 2) {
      TEMP = 2;
    }
    board[i][j] = TEMP;
  }
 }
 board[playerXLocation][playerYLocation] = 3;
}

/*
 * Light the led on the matrix
 * depending on the array result form the func above
 */
void drawBoard(){
  int ledNumber = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      uint32_t color = getStatus(i, j);
      strip.setPixelColor(ledNumber, color);
      ledNumber++;
    } 
  }
  strip.show();
  delay(delaytime);  
}


/* 
 *  Return a color depending on the value on the board and the turn
 *  0 = Blue: Water / 1 = Red: Destroyed Ship / 2 = Yellow: Shoot with no boat  / 3 = Green: Player
 *  
 *  return the status of a led (status and turn)
 */
uint32_t getStatus(int row, int column) {
  int square = board[row][column];
  if (square == 1) {
    return strip.Color(255, 0, 0);
  } else if (square == 2) {
    return strip.Color(247, 221, 0);
  } else if (square == 3) {
    return strip.Color(58, 221, 0);
  }
  if (row == turn) {
    return strip.Color(170, 208, 255);
  }
  if (row + 1 == turn) {
    return strip.Color(62, 196, 252);
  }
  return strip.Color(62, 119, 255);
  
}


/* 
 * used to make leds blink 
 * For the water animation
 */
void changeTurn() {
  if (turn == 7) {
    turn = 0;
  } else {
    turn += 1;
  }
}


/*
 * Listen for joystick movment
 * And moved the plyer
 */
void listenJoystick() {
  X = -(analogRead(VRx)-508);
  Y = (analogRead(VRy)-508);

  if(X < -500) {
    // Gauche
    playerMove('l');
  }
  if(X > 400) {
    // Droite
    playerMove('r');
  }
  if(Y > 500) {
    // Haut
    playerMove('u');
  }
  if(Y < -400) {
    // Bas
    playerMove('d');
  }
  delay(200);
}

/*
 * Move the player depending on the direction received
 * Check if he is not at the edge of the board
 */
void playerMove(char direction) {
  if (direction == 'l' && playerXLocation > 0) {
    playerXLocation--;
  } else if (direction == 'r' && playerXLocation < 7) {
    playerXLocation++;
  } else if (direction == 'd' && playerYLocation > 0) {
    playerYLocation--;
  } else if (direction == 'u' && playerYLocation < 7) {
    playerYLocation++;
  }
}

/*
 * Listen for joystick click
 * And start the shooting phase
 */
void listenSelector() {
  pressed = digitalRead(SW);
  if(pressed == 0) {
    buttonPressed();
  }
}

/* 
 *  Shooting Phase
 * Light RGB LED (Red -> Blue -> Green)
 * The player must click when the LED is Green
 * If he succeed show him what he hit
 */
void buttonPressed() {
  int buttonPressed = 1;
  int loopTurn = 0;
  setColor(1);
  delay(1000);
  Serial.println(buttonPressed);
  while(buttonPressed == 1) {  
    turn ++;
    if (turn > 3) {
        turn = 1;
    }
    setColor(turn);
    buttonPressed = digitalRead(SW);
    delay(400);
  }
  // If turn 3 = hit green
  if (turn == 3) {
    handleHit();
  }
  delay(1000);
  turnOffLed();

}

/*
 * If the player succeed to shoot
 * Check if it is a boat or water
 * And change the boat array
 */
void handleHit() {
  int result = 0;
  if (boats[playerXLocation][playerYLocation] == 3) {
    boats[playerXLocation][playerYLocation] = 1;
    showHitResult(3);
  } else {
    boats[playerXLocation][playerYLocation] = 2;
    showHitResult(4);
  }
}

/*Show if the player hit a boat or juste water
* 3 = boat / 4 = water
 */
void showHitResult(int result) {
  for (int i = 0; i < 8; i++) {
    setColor(result);
    delay(100);
    turnOffLed();
    delay(100);
  }
}

/* 
 *  Place boats 10 randomly on the board
 */
void setupBoats() {
  randomSeed(analogRead(0));
  int xCoord = 0;
  int yCoord = 0;
    
  for (int i = 0; i < 10; i++) {
    xCoord = random(0, 8);
    yCoord = random(0, 8);
    if (boats[xCoord][yCoord] == 0) {
      boats[xCoord][yCoord] = 3;
    } else {
      i--;
    }
  }
}

/*
 * Check if there is at least one boat not destroyed
 * If there is return 0, the game is not over
 * Otherwise return 1, the player won
 */
int verifyVictory() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (boats[i][j] == 3) {
                return 0;
            }
        }
    }
    return 1;
}

/*
 * Display a Happy face to show the player he won
 */
void showVictory() {
   int ledNumber = 0;
   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        uint32_t color;
        if (hf[i][j] == 1) {
          color = strip.Color(255, 0, 0);
        } else {
          color = strip.Color(0, 0, 0);
        }
        strip.setPixelColor(ledNumber, color);
        ledNumber++;
      } 
   }
}

/*
 * Display a Sad face to show the player he lost
 */
void showDefeat() {
   int ledNumber = 0;
   for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        uint32_t color;
        if (sf[i][j] == 1) {
          color = strip.Color(255, 0, 0);
        } else {
          color = strip.Color(0, 0, 0);
        }
        strip.setPixelColor(ledNumber, color);
        ledNumber++;
      } 
   }
}

/*
*  Cette fonction prend trois arguments, l'un pour la luminosité des diodes rouges, vertes et bleues. 
*  Dans chaque cas, le nombre sera compris entre 0 et 255, ou 0 signifit off et 255 signifie une luminosité max.
*  La fonction appelle ensuite analogWrite pour régler la luminosité de chaque led.
*  1 affiche du rouge; 2 du bleue; 3 du vert
 */
void setColor(int turn) {
  int red = 0;
  int blue = 0;
  int green = 0;
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  if (turn == 1) {
    red = 255;
  } else if (turn == 2) {
    blue = 255;
  } else if (turn == 3) {
    green = 255;
  } else {
    green = 255;
    red = 255;
  }
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

/*
 * Turn the RGB Led off
 */
void turnOffLed() {
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0); 
}

