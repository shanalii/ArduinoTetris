// testcolors demo for Adafruit RGBmatrixPanel library.
// Renders 512 colors on our 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420
// Library supports 4096 colors, but there aren't that many pixels!  :)

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// global variables
boolean board[16][32];

// coordinates of current shape
int x1 = 0;
int y1 = 0;
int x2 = 0;
int y2 = 0;
int x3 = 0;
int y3 = 0;
int x4 = 0;
int y4 = 0;

int blockType = -1; // current block type

unsigned long startTime = millis(); // current time to count when to lower the block
int increment = 500; // frequency of falling block, in ms

void setup() {
  Serial.begin(9600);
  // initialize board to be all 0
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 32; y++) {
      board[x][y] = 0;

    }
  }

  matrix.begin();

  //randomSeed(analogRead(0));
  newBlock();
  //newBlock(random(0, 6));
  //fall();
  delay(500);
}

void loop() {

  // make the block fall every increment
  unsigned long curTime = millis();
  if (curTime > (startTime + increment)) {
    //Serial.println(curTime);
    startTime = curTime;
    fall();
  }

  delay(800);
  rotate();
}

void newBlock() {
  Serial.println("newblock");

  // set current block type
  //blockType = r;
  //Serial.println(r);
  x1 = 8;
  y1 = 0;
  x2 = 8;
  y2 = 1;
  x3 = 8;
  y3 = 2;
  x4 = 8;
  y4 = 3;
  matrix.drawPixel(0, 8, matrix.Color333(0, 7, 7));
  matrix.drawPixel(1, 8, matrix.Color333(0, 7, 7));
  matrix.drawPixel(2, 8, matrix.Color333(0, 7, 7));
  matrix.drawPixel(3, 8, matrix.Color333(0, 7, 7));

  /*switch (r) {
    case 0:
      // the long boi
      board[8][0] = 1;
      matrix.drawPixel(0, 8, matrix.Color333(0, 7, 7));
      board[8][1] = 1;
      matrix.drawPixel(1, 8, matrix.Color333(0, 7, 7));
      board[8][2] = 1;
      matrix.drawPixel(2, 8, matrix.Color333(0, 7, 7));
      board[8][3] = 1;
      matrix.drawPixel(3, 8, matrix.Color333(0, 7, 7));
      break;
    case 1:
      // the square boi
      board[8][0] = 1;
      matrix.drawPixel(0, 8, matrix.Color333(7, 7, 0));
      board[8][1] = 1;
      matrix.drawPixel(1, 8, matrix.Color333(7, 7, 0));
      board[7][0] = 1;
      matrix.drawPixel(0, 7, matrix.Color333(7, 7, 0));
      board[7][1] = 1;
      matrix.drawPixel(1, 7, matrix.Color333(7, 7, 0));
      break;
    case 2:
      // flipped L
      board[8][1] = 1;
      matrix.drawPixel(1, 8, matrix.Color333(0, 1, 7));
      board[7][0] = 1;
      matrix.drawPixel(0, 7, matrix.Color333(0, 1, 7));
      board[7][1] = 1;
      matrix.drawPixel(1, 7, matrix.Color333(0, 1, 7));
      board[9][1] = 1;
      matrix.drawPixel(1, 9, matrix.Color333(0, 1, 7));
      break;
    case 3:
      // L
      board[8][1] = 1;
      matrix.drawPixel(1, 8, matrix.Color333(7, 4, 0));
      board[7][1] = 1;
      matrix.drawPixel(1, 7, matrix.Color333(7, 4, 0));
      board[9][1] = 1;
      matrix.drawPixel(1, 9, matrix.Color333(7, 4, 0));
      board[9][0] = 1;
      matrix.drawPixel(0, 9, matrix.Color333(7, 4, 0));
      break;
    case 4:
      // s
      board[8][1] = 1;
      matrix.drawPixel(1, 8, matrix.Color333(0, 7, 0));
      board[7][1] = 1;
      matrix.drawPixel(1, 7, matrix.Color333(0, 7, 0));
      board[8][0] = 1;
      matrix.drawPixel(0, 8, matrix.Color333(0, 7, 0));
      board[9][0] = 1;
      matrix.drawPixel(0, 9, matrix.Color333(0, 7, 0));
      break;
    case 5:
      // z
      board[8][1] = 1;
      matrix.drawPixel(1, 8, matrix.Color333(7, 0, 0));
      board[9][1] = 1;
      matrix.drawPixel(1, 9, matrix.Color333(7, 0, 0));
      board[8][0] = 1;
      matrix.drawPixel(0, 8, matrix.Color333(7, 0, 0));
      board[7][0] = 1;
      matrix.drawPixel(0, 7, matrix.Color333(7, 0, 0));
      break;
    case 6:
      // T but upside down
      board[8][1] = 1;
      matrix.drawPixel(1, 8, matrix.Color333(4, 0, 7));
      board[8][0] = 1;
      matrix.drawPixel(0, 8, matrix.Color333(4, 0, 7));
      board[7][1] = 1;
      matrix.drawPixel(1, 7, matrix.Color333(4, 0, 7));
      board[9][1] = 1;
      matrix.drawPixel(1, 9, matrix.Color333(4, 0, 7));
      break;
    default:
      // statements
      break;*/
  //}
}

void fall() {

  // check if it touches the bottom of the board, or is on top of already fallen blocks
  if (y1 == 31 || y4 == 31 || board[x1][y1 + 1] == 1 || board[x4][y4 + 1] == 1) {
    board[x1][y1] = 1;
    board[x2][y2] = 1;
    board[x3][y3] = 1;
    board[x4][y4] = 1;
    Serial.println(board[x1][y1]);
    matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 7));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 0, 7));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 7));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 7));
    newBlock();
  } else {
    // fall normally
    matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));

    y1++;
    y2++;
    y3++;
    y4++;

    matrix.drawPixel(y1, x1, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 7, 7));
  }
}

void left() {
  if (!((x1 <= x4 && (x1 == 0 || board[x1 - 1][y1] == 1)) || (x4 < x1 && (x4 == 0 || board[x4 - 1][y1] == 1)))) {
    matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
    
    x1--;
    x2--;
    x3--;
    x4--;

    matrix.drawPixel(y1, x1, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 7, 7));
  }
}

void right() {
  if (!((x1 >= x4 && (x1 == 15 || board[x1 + 1][y1] == 1)) || (x4 > x1 && (x4 == 15 || board[x4 + 1][y1] == 1)))) {
    matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
    
    x1++;
    x2++;
    x3++;
    x4++;

    matrix.drawPixel(y1, x1, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 7, 7));
  }
}


void rotate() {

  matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
  matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
  matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));

  if (x1 > x2 && y2 != 31 && board[x2][y2 + 1] != 1) {
    x1 = x2;
    y1 = y2 - 1;
    x3 = x2;
    y3 = y2 + 1;
    x4 = x2; 
    y4 = y2 + 2;
    matrix.drawPixel(y1, x1, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 7, 7));
  } else if (y2 != 15 && y2 > 1) {
    x1 = x2 + 1;
    y1 = y2;
    x3 = x2 - 1;
    y3 = y2;
    x4 = x2 - 2;
    y4 = y2;
    matrix.drawPixel(y1, x1, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 7, 7));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 7, 7));
  }

}
