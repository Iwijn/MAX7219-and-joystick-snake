#include "LedControl.h" // LedControl by Eberhard Fahle <e.fahle@wayoda.org> v1.0.6

LedControl lc = LedControl(2, 4, 3);

int joyEpsilon = 100; // how sensitive the joystick is
int snakeX[64]; // The x coordinates of the snake. The first coordinate in the list is the coordinate of the snake head.
int snakeY[64]; // The y coordinates of the snake. The first coordinate in the list is the coordinate of the snake head.
int direction;
int moveDelay;
int length;
int appleX;
int appleY;
void setStartVariables() {
  snakeX[64] = {5};
  snakeY[64] = {5};
  direction = -1; // snake does not move at the beginning.
  moveDelay = 200;
  length = 1;
  appleX = random(0,7);
  appleY = random(0,7);  
}

long int prevMoveTime = 0;
void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);
  randomSeed(millis());
  prevMoveTime = millis();
  setStartVariables();
}

void saveLatestDirection() {
  int joyX = analogRead(A0);
  int joyY = analogRead(A1);
  // direction is up
  if (joyX > 512 + joyEpsilon) {
    direction = 0;
  }
  // down
  if (joyX < 512 - joyEpsilon) {
    direction = 1;
  }
  // left
  if (joyY > 512 + joyEpsilon) {
    direction = 2;
  }
  // right
  if (joyY < 512 - joyEpsilon) {
    direction = 3;
  }
}

void checkAppleEaten() {
  if (appleX == snakeX[0] && appleY == snakeY[0]) {
    length += 1;
    // todo: make apple not spawn in snake
    appleX = random(0,7);
    appleY = random(0,7);
  }  
}

void moveSnakeBody() {
  for (int i = length - 2; i >= 0; i--) {
    snakeX[i+1] = snakeX[i];
    snakeY[i+1] = snakeY[i];
  }
}


void lightScreen(bool state) {
  for (int row = 0; row < 8; row++) {
    if (state) {
      lc.setRow(0, row, 0xFF);
    } else {
      lc.setRow(0, row, 0x00);
    }
  }
}
void deadAnimation() {
  for(int j = 0; j < 3; j++) {
    lightScreen(true);
    delay(100);
    lightScreen(false);
    delay(100);          
  }
}
void checkHeadTouchingBody() {
  for (int i = 1; i < length; i++) {
    if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
      deadAnimation();
      setStartVariables();
    }
  }
}

void calculateNewHeadPosition() {
  // move up
  if (direction == 0) {
    snakeY[0] = (snakeY[0] + 1) % 8;
  }
  // down
  if (direction == 1) {
    snakeY[0] = (snakeY[0] - 1 + 8) % 8;
  }
  // left
  if (direction == 2) {
    snakeX[0] = (snakeX[0] + 1) % 8;
  }
  // right
  if (direction == 3) {
    snakeX[0] = (snakeX[0] - 1 + 8) % 8;
  }
}

void drawScreen() {
  lc.clearDisplay(0);
  for (int i = 0; i < length; i++) {
    lc.setLed(0, snakeX[i], snakeY[i], true);
  }
  lc.setLed(0, appleX, appleY, true);
}

void loop() {
  saveLatestDirection();
  
  // step the snake forward
  if (millis() - prevMoveTime > moveDelay) {
    checkAppleEaten();    
    moveSnakeBody();
    calculateNewHeadPosition();    
    checkHeadTouchingBody();
    drawScreen();

    prevMoveTime = millis();
  }
}