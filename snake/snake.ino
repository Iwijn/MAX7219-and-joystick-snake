#include "LedControl.h" // LedControl by Eberhard Fahle <e.fahle@wayoda.org> v1.0.6

LedControl lc = LedControl(2, 4, 3);

int joyEpsilon;
int snakeX[64];
int snakeY[64];
int direction;
int moveDelay;
int length;
int appleX;
int appleY;
void setStartVariables() {
  joyEpsilon = 100;
  snakeX[64] = {5};
  snakeY[64] = {5};
  direction = -1;
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

void lightScreen(bool state) {
  for (int row = 0; row < 8; row++) {
    if (state) {
      lc.setRow(0, row, 0xFF);
    } else {
      lc.setRow(0, row, 0x00);
    }
  }
}

void loop() {
  // save latest direction
  int joyX = analogRead(A0);
  int joyY = analogRead(A1);
  // move up
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
  
  // step
  if (millis() - prevMoveTime > moveDelay) {
    // check if apple eaten
    if (appleX == snakeX[0] && appleY == snakeY[0]) {
      length += 1;
      // todo: make apple not spawn in snake
      appleX = random(0,7);
      appleY = random(0,7);
    }

    // move entire snake
    Serial.println(length-2);
    for (int i = length - 2; i >= 0; i--) {
      Serial.println("test");
      //Serial.println(snakeX[i]);
      snakeX[i+1] = snakeX[i];
      snakeY[i+1] = snakeY[i];
    }

    // calculate new head of snake based on direction 
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

    // check if head touching snake
    for (int i = 1; i < length; i++) {
      if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
        for(int j = 0; j < 3; j++) {
          lightScreen(true);
          delay(100);
          lightScreen(false);
          delay(100);          
        }
        setStartVariables();
      }
    }

    // draw
    lc.clearDisplay(0);
    for (int i = 0; i < length; i++) {
      //Serial.println(snakeX[i]);
      lc.setLed(0, snakeX[i], snakeY[i], true);
    }
    lc.setLed(0, appleX, appleY, true);

    prevMoveTime = millis();
  }
}