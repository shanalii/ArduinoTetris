/*
Tetris on the Arduino with Adafruit LED matrix
Adafruit RGB matrix panel libraries used
---
Shana Li, Lars Dreith
*/

#include <RGBmatrixPanel.h>

#define CLK  8 
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

// buttons
#define LEFT 13
#define RIGHT 11
#define ROTATE 12

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

// random seed
int seed = 0;

int blockType = -1; // current block type

unsigned long startTime = millis(); // current time to count when to lower the block
int increment = 200; // frequency of falling block, in ms

void setup() {

  // set up buttons
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(ROTATE, INPUT);

  Serial.begin(9600);
  // initialize board to be all 0
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 32; y++) {
      board[x][y] = 0;
    }
  }

  matrix.begin();

  randomSeed(seed);
  //newBlock(random(0, 6));
  newBlock(1);
  delay(500);
}

void loop() {

  seed++;

  // make the block fall every increment
  unsigned long curTime = millis();
  if (curTime > (startTime + increment)) {
    //Serial.println(curTime);
    startTime = curTime;
    fall();
  }

  if (digitalRead(LEFT) == HIGH) {
    left();
    delay(200);
  }

  else if (digitalRead(RIGHT) == HIGH) {
    right();
    delay(200);
  }

  else if (digitalRead(ROTATE) == HIGH) {
    rotate();
    delay(200);
  }
}

void newBlock(int r) {
  // set current block type
  blockType = r;
  //Serial.println(r);
  switch (r) {
    case 0:
      // the long boi
      x1 = 8;
      y1 = 0;
      x2 = 8;
      y2 = 1;
      x3 = 8;
      y3 = 2;
      x4 = 8;
      y4 = 3;
      break;
    case 1:
      // the square boi
      x1 = 8;
      y1 = 0;
      x2 = 9;
      y2 = 0;
      x3 = 8;
      y3 = 1;
      x4 = 9;
      y4 = 1;
      break;
    case 2:
      // flipped L
      x1 = 7;
      y1 = 0;
      x2 = 7;
      y2 = 1;
      x3 = 8;
      y3 = 1;
      x4 = 9;
      y4 = 1;
      break;
    case 3:
      // L
      x1 = 9;
      y1 = 0;
      x2 = 9;
      y2 = 1;
      x3 = 8;
      y3 = 1;
      x4 = 7;
      y4 = 1;
      break;
    case 4:
      // s
      x1 = 7;
      y1 = 1;
      x2 = 8;
      y2 = 1;
      x3 = 8;
      y3 = 0;
      x4 = 9;
      y4 = 0;
      break;
    case 5:
      // z
      x1 = 7;
      y1 = 0;
      x2 = 8;
      y2 = 0;
      x3 = 8;
      y3 = 1;
      x4 = 9;
      y4 = 1;
      break;
  }
  matrix.drawPixel(y1, x1, matrix.Color333(7, 0, 4));
  matrix.drawPixel(y2, x2, matrix.Color333(7, 0, 4));
  matrix.drawPixel(y3, x3, matrix.Color333(7, 0, 4));
  matrix.drawPixel(y4, x4, matrix.Color333(7, 0, 4));
}

void fall() {

  // check if it touches the bottom of the board, or is on top of already fallen blocks
  if (y1 == 31 || y2 == 31 || y3 == 31 || y4 == 31 || board[x1][y1 + 1] == 1 || board[x2][y2 + 1] == 1 || board[x3][y3 + 1] == 1 || board[x4][y4 + 1] == 1) {
    //Serial.println("stop");
    board[x1][y1] = 1;
    board[x2][y2] = 1;
    board[x3][y3] = 1;
    board[x4][y4] = 1;
    matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 7));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 0, 7));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 7));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 7));


    // check for full rows

    // array of full row numbers
    short fullRows[4];
    for (short i = 0; i < 4; i++) {
      fullRows[i] = -1;
    }

    // y1
    boolean full = 1;
    for (short x = 0; x < 16; x++) {
      if (board[x][y1] == 0) {
        full = 0;
        break;
      }
    }
    if (full) {
      insert(fullRows, y1);
      Serial.println("y1");
    }

    // y2
    full = 1;
    for (short x = 0; x < 16; x++) {
      if (board[x][y2] == 0) {
        full = 0;
        break;
      }
    }
    if (full) {
      insert(fullRows, y2);
      Serial.println("y2");
    }

    // y3
    full = 1;
    for (short x = 0; x < 16; x++) {
      if (board[x][y3] == 0) {
        full = 0;
        break;
      }
    }
    if (full) {
      insert(fullRows, y3);
      Serial.println("y3");
    }

    // y4
    full = 1;
    for (short x = 0; x < 16; x++) {
      if (board[x][y4] == 0) {
        full = 0;
        break;
      }
    }
    if (full) {
      insert(fullRows, y4);
      Serial.println("y4");
    }

    if (fullRows[0] != -1) {
      // number of rows to shift the row down
      short shift = 1;
      
      // index of fullRows
      short ind = 0;
      
      // shift rows from bottom to top
      for (short r = arr[0]; r < 0; r--) {
        
        // check if row r is empty
        boolean empty = 1;
        for (short x = 0; x < 16; x++) {
          if (board[x][r] == 1) {
            empty = 0;
            break;
          }
        }
        if (empty) {
          break;
        }
        
        if (ind <= 2) {
          short curind = ind; // current index of fullRows
          for (short i = curind; i <= 2; i++) {
            if (fullRows[i+1] != -1 && r == fullRows[i+1] + shift) {
              shift++;
              ind++;
            }
          }
        }
        
        // shift rows
        board[x][r] = board[x][r - shift];
        matrix.drawPixel(r, x, matrix.Color333(0, 0, 7));
      }
    }
    
    newBlock(1);

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

    matrix.drawPixel(y1, x1, matrix.Color333(7, 0, 4));
    matrix.drawPixel(y2, x2, matrix.Color333(7, 0, 4));
    matrix.drawPixel(y3, x3, matrix.Color333(7, 0, 4));
    matrix.drawPixel(y4, x4, matrix.Color333(7, 0, 4));
  }
}


// insertion sort
void insert(short arr[], short n) {
  short insert = 0;
  for (short i = 0; i < 4; i++) {

    if (arr[i] == -1 || arr[i] == n) {
      insert = i;
      break;
    }

    if (arr[i] < n) {
      for (short j = 3; j > i; j--) {
        arr[j] = arr[j - 1];
      }
      insert = i;
      break;
    }
  }

  arr[insert] = n;

  for (short k = 0; k < 4; k++) {
    Serial.println(arr[k]);
  }
}

void left() {

  boolean left = 1; // to move left or not

  // now check for boundaries
  switch (blockType) {
    case 0:
      if ((x1 < x4 && (x1 == 0 || board[x1 - 1][y1] == 1)) || (x4 == x1 && (x4 == 0 || board[x1 - 1][y1] == 1 || board[x1 - 1][y2] == 1 || board[x1 - 1][y3] == 1 || board[x1 - 1][y4] == 1))) {
        left = 0;
      }
      break;
    case 1: // square
      if (x1 == 0 || board[x1 - 1][y1] == 1 || board[x3 - 1][y3] == 1) {
        left = 0;
      }
      break;
    case 2: // flipped L
      if (y1 < y2 ) {
        if (x1 == 0 || board[x1 - 1][y1] == 1 || board[x2 - 1][y2] == 1) {
          left = 0;
        }
      } else if (y1 > y2) {
        if (x4 == 0 || board[x1 - 1][y1] == 1 || board[x4 - 1][y4] == 1) {
          left = 0;
        }
      } else {
        if (x2 == 0 || board[x2 - 1][y2] == 1 || board[x3 - 1][y3] == 1 || board[x4 - 1][y4] == 1) {
          left = 0;
        } else if (x1 == 0 || board[x1 - 1][y1] == 1 || board[x3 - 1][y3] == 1 || board[x4 - 1][y4] == 1) {
          left = 0;
        }
      }
      break;
    case 3: // L
      if (y1 < y2 ) {
        if (x4 == 0 || board[x4 - 1][y4] == 1 || board[x1 - 1][y1] == 1) {
          left = 0;
        }
      } else if (y1 > y2) {
        if (x1 == 0 || board[x1 - 1][y1] == 1 || board[x2 - 1][y2] == 1) {
          left = 0;
        }
      } else {
        if (x2 == 0 || board[x2 - 1][y2] == 1 || board[x3 - 1][y3] == 1 || board[x4 - 1][y4] == 1) {
          left = 0;
        } else if (x1 == 0 || board[x1 - 1][y1] == 1 || board[x3 - 1][y3] == 1 || board[x4 - 1][y4] == 1) {
          left = 0;
        }
      }
      break;
    case 4: // s
      if ((x1 < x2 && (x1 == 0 || board[x1 - 1][y1] == 1 || board[x3 - 1][y3] == 1)) || (x2 == x1 && (x2 == 0 || board[x1 - 1][y1] == 1 || board[x2 - 1][y2] == 1 || board[x4 - 1][y4] == 1))) {
        left = 0;
      }
      break;
    case 5: // z
      if ((x1 < x2 && (x1 == 0 || board[x1 - 1][y1] == 1 || board[x3 - 1][y3] == 1)) || (x2 == x1 && (x3 == 0 || board[x3 - 1][y3] == 1 || board[x4 - 1][y4] == 1 || board[x1 - 1][y1] == 1))) {
        left = 0;
      }
      break;
  }

  if (left) {
    matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));

    x1--;
    x2--;
    x3--;
    x4--;

    matrix.drawPixel(y1, x1, matrix.Color333(7, 0, 4));
    matrix.drawPixel(y2, x2, matrix.Color333(7, 0, 4));
    matrix.drawPixel(y3, x3, matrix.Color333(7, 0, 4));
    matrix.drawPixel(y4, x4, matrix.Color333(7, 0, 4));
  }
}

void right() {

  boolean right = 1; // to move right or not

  // now check for boundaries
  switch (blockType) {
    case 0:
      if ((x1 > x4 && (x1 == 15 || board[x1 + 1][y1] == 1)) || (x4 == x1 && (x4 == 15 || board[x1 + 1][y1] == 1 || board[x1 + 1][y2] == 1 || board[x1 + 1][y3] == 1 || board[x1 + 1][y4] == 1))) {
        right = 0;
      }
      break;
    case 1: // square
      if (x2 == 15 || board[x2 + 1][y2] == 1 || board[x4 + 1][y4] == 1) {
        right = 0;
      }
      break;
    case 2: // flipped L
      if (y1 < y2 ) {
        if (x4 == 15 || board[x1 + 1][y1] == 1 || board[x4 + 1][y4] == 1) {
          right = 0;
        }
      } else if (y1 > y2) {
        if (x1 == 15 || board[x1 + 1][y1] == 1 || board[x2 + 1][y2] == 1) {
          right = 0;
        }
      } else {
        if (x2 == 15 || board[x2 + 1][y2] == 1 || board[x3 + 1][y3] == 1 || board[x4 + 1][y4] == 1) {
          right = 0;
        } else if (x1 == 15 || board[x1 + 1][y1] == 1 || board[x3 + 1][y3] == 1 || board[x4 + 1][y4] == 1) {
          right = 0;
        }
      }
      break;
    case 3: // L
      if (y1 < y2 ) {
        if (x1 == 15 || board[x1 + 1][y1] == 1 || board[x2 + 1][y2] == 1) {
          right = 0;
        }
      } else if (y1 > y2) {
        if (x4 == 15 || board[x1 + 1][y1] == 1 || board[x4 + 1][y4] == 1) {
          right = 0;
        }
      } else {
        if (x2 == 15 || board[x2 + 1][y2] == 1 || board[x3 + 1][y3] == 1 || board[x4 + 1][y4] == 1) {
          right = 0;
        } else if (x1 == 15 || board[x1 + 1][y1] == 1 || board[x3 + 1][y3] == 1 || board[x4 + 1][y4] == 1) {
          right = 0;
        }
      }
      break;
    case 4: // s
      if ((x1 < x2 && (x4 == 15 || board[x4 + 1][y4] == 1 || board[x2 + 1][y2] == 1)) || (x2 == x1 && (x3 == 15 || board[x3 + 1][y3] == 1 || board[x1 + 1][y1] == 1 || board[x4 + 1][y4] == 1))) {
        right = 0;
      }
      break;
    case 5: // z
      if ((x1 < x2 && (x4 == 15 || board[x4 + 1][y4] == 1 || board[x2 + 1][y2] == 1)) || (x2 == x1 && (x1 == 15 || board[x1 + 1][y1] == 1 || board[x4 + 1][y4] == 1 || board[x2 + 1][y2] == 1))) {
        right = 0;
      }
      break;
  }

  if (right) {
    matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y2, x2, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
    matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));

    x1++;
    x2++;
    x3++;
    x4++;

    matrix.drawPixel(y1, x1, matrix.Color333(7, 0, 4));
    matrix.drawPixel(y2, x2, matrix.Color333(7, 0, 4));
    matrix.drawPixel(y3, x3, matrix.Color333(7, 0, 4));
    matrix.drawPixel(y4, x4, matrix.Color333(7, 0, 4));
  }
}


void rotate() {

  // check for boundaries
  switch (blockType) {
    case 0:
      if (x1 == x4 && !(x2 == 15 || x2 <= 1 || board[x2 + 1][y2] == 1 || board[x2 - 1][y2] == 1 || board[x2 - 2][y2] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x1 = x2 + 1;
        y1 = y2;
        x3 = x2 - 1;
        y3 = y2;
        x4 = x2 - 2;
        y4 = y2;
      } else if (x4 < x1 && !(y2 >= 30 || board[x2 - 1][y2] == 1 || board[x2 + 1][y2] == 1 || board[x2 + 2][y2] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x1 = x2;
        y1 = y2 - 1;
        x3 = x2;
        y3 = y2 + 1;
        x4 = x2;
        y4 = y2 + 2;
      }
      break;
    case 1: // square
      break;
    case 2: // flipped L
      if (y1 < y2 && !(y2 >= 30 || board[x2][y2 + 1] == 1 || board[x2][y2 + 2] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x1 = x3;
        y1 = y3;
        x3 = x2;
        y3 = y2 + 1;
        x4 = x2;
        y4 = y2 + 2;
      } else if (y1 > y2 && !(y2 <= 1 || board[x2][y2 - 1] == 1 || board[x2][y2 - 2] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x1 = x3;
        y1 = y3;
        x3 = x2;
        y3 = y2 - 1;
        x4 = x2;
        y4 = y2 - 2;
      } else if (x1 > x2 && !(x2 <= 1 || board[x2 - 1][y2] == 1 || board[x2 - 2][y2] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x1 = x3;
        y1 = y3;
        x3 = x2 - 1;
        y3 = y2;
        x4 = x2 - 2;
        y4 = y2;
      } else if (x2 > x1 && !(x2 >= 14 || board[x2 + 1][y2] == 1 || board[x2 + 2][y2] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x1 = x3;
        y1 = y3;
        x3 = x2 + 1;
        y3 = y2;
        x4 = x2 + 2;
        y4 = y2;
      }
      break;
    case 3: // L
      if (y1 < y2 && !(x2 == 15 || y1 == 0 || board[x1][y1 - 1] == 1 || board[x2 + 1][y2] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x3 = x1;
        y3 = y1;
        x1 = x2 + 1;
        y1 = y2;
        x4 = x2;
        y4 = y2 - 2;
      } else if (y1 > y2 && !(x2 == 0 || y1 == 31 || board[x2 - 1][y2] == 1 || board[x1][y1 + 1] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x3 = x1;
        y3 = y1;
        x1 = x2 - 1;
        y1 = y2;
        x4 = x2;
        y4 = y2 + 2;
      } else if (x1 > x2 && !(y2 == 31 || x1 == 15 || board[x2][y2 + 1] == 1 || board[x1 + 1][y1] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x3 = x1;
        y3 = y1;
        x1 = x2;
        y1 = y2 + 1;
        x4 = x2 + 2;
        y4 = y2;
      } else if (x2 > x1 && !(x1 == 0 || y2 == 0 || board[x2][y2 - 1] == 1 || board[x1 - 1][y1] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x3 = x1;
        y3 = y1;
        x1 = x2;
        y1 = y2 - 1;
        x4 = x2 - 2;
        y4 = y2;
      }
      break;
    case 4: // s
      if (x1 < x2 && !(y2 == 15 || board[x2 + 1][y2] == 1 || board[x4][y4 + 2] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x1 = x3;
        y1 = y3;
        x3 = x2 + 1;
        y3 = y2;
        x4 = x3;
        y4 = y3 + 1;
      } else if (x2 == x1 && !(y2 == 0 || board[x2 - 1][y4] == 1 || board[x2][y2 + 1] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x1 = x2 - 1;
        y1 = y2;
        x3 = x2;
        y3 = y2 - 1;
        x4 = x3 + 1;
        y4 = y3;
      }
      break;
    case 5: // z
      if (x1 < x2 && !(y2 == 0 || board[x2][y2 - 1] == 1 || board[x1][y1 + 1] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x3 = x1;
        y3 = y1;
        x1 = x2;
        y1 = y2 - 1;
        x4 = x3;
        y4 = y3 + 1;
      } else if (x2 == x1 && !(x2 == 15 || board[x2 + 1][y2] == 1 || board[x1 - 1][y1] == 1)) {
        matrix.drawPixel(y1, x1, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y3, x3, matrix.Color333(0, 0, 0));
        matrix.drawPixel(y4, x4, matrix.Color333(0, 0, 0));
        x1 = x2 - 1;
        y1 = y2;
        x3 = x2;
        y3 = y2 + 1;
        x4 = x3 + 1;
        y4 = y3;
      }
      break;
  }

  matrix.drawPixel(y1, x1, matrix.Color333(7, 0, 4));
  matrix.drawPixel(y3, x3, matrix.Color333(7, 0, 4));
  matrix.drawPixel(y4, x4, matrix.Color333(7, 0, 4));

}
