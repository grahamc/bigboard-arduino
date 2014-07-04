#include <string.h>
#include <stdio.h>

int clockPin = 12;
int dataPin = 11;
int oePin = 9;
int latchPin = 8;

//holders for infromation you're going to pass to shifting function
byte dataArrayRED[13];

const int TOP_TOP       (1 << 5);
const int TOP_LEFT      (1 << 6);
const int TOP_RIGHT     (1 << 2);
const int MIDDLE        (1 << 4);
const int BOTTOM_LEFT   (1 << 7);
const int BOTTOM_RIGHT  (1 << 0);
const int BOTTOM_BOTTOM (1 << 3);
const int DECIMAL       (1 << 1);

void setup() {
  Serial.begin(9600);

  pinMode(oePin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  digitalWrite(oePin, LOW); // Enable output

  dataArrayRED[0] =  (TOP_TOP | TOP_LEFT | TOP_RIGHT | BOTTOM_LEFT | BOTTOM_RIGHT | BOTTOM_BOTTOM);
  dataArrayRED[1]  = (TOP_RIGHT | BOTTOM_RIGHT);
  dataArrayRED[2]  = (TOP_TOP | TOP_RIGHT | MIDDLE | BOTTOM_LEFT | BOTTOM_BOTTOM);
  dataArrayRED[3]  = (TOP_TOP | TOP_RIGHT | MIDDLE | BOTTOM_RIGHT | BOTTOM_BOTTOM);
  dataArrayRED[4]  = (TOP_LEFT | MIDDLE | TOP_RIGHT | BOTTOM_RIGHT);
  dataArrayRED[5]  = (TOP_TOP | TOP_LEFT | MIDDLE | BOTTOM_RIGHT | BOTTOM_BOTTOM);
  dataArrayRED[6]  = (TOP_TOP | TOP_LEFT | MIDDLE | BOTTOM_LEFT | BOTTOM_RIGHT | BOTTOM_BOTTOM);
  dataArrayRED[7]  = (TOP_TOP | TOP_RIGHT | BOTTOM_RIGHT);
  dataArrayRED[8]  = (TOP_TOP | TOP_RIGHT | TOP_LEFT | MIDDLE | BOTTOM_RIGHT | BOTTOM_LEFT | BOTTOM_BOTTOM);
  dataArrayRED[9]  = (TOP_TOP | TOP_RIGHT | TOP_LEFT | MIDDLE | BOTTOM_RIGHT);
  dataArrayRED[10] = DECIMAL;
  dataArrayRED[11] = 0; // Off!
  dataArrayRED[12] = (TOP_TOP | TOP_RIGHT | TOP_LEFT | MIDDLE | BOTTOM_RIGHT | BOTTOM_LEFT | BOTTOM_BOTTOM | DECIMAL); // On!
}







void loop() {
  char input[11];
  input[0] = '\0';

  int location = 0;
  while (Serial.available () > 0) {
    delay(3);
    char c = Serial.read();

    if (location < 11) {
      input[location++] = c;
      input[location] = '\0';
    }
  }

  if (input[0] != '\0') {
    writeString(input);
  }
}

void writeString(char *input)
{
  digitalWrite(latchPin, LOW);
  int i;
  for (i = 0; i < 10; i++) {
    shiftInt(11);
  }
  for (i = 0; i < strlen(input); i++) {
    int digit = input[i] - '0';
    int next = input[i + 1];

    if (digit >= 0 && digit <= 9) {
      shiftInt(digit, next == '.');
    }
  }

  digitalWrite(latchPin, HIGH);
}

void shiftInt(int symbol, boolean decimalPoint = false)
{
  if (decimalPoint) {
    shiftOut(dataPin, clockPin, dataArrayRED[symbol] | dataArrayRED[10]);
  } else {
    shiftOut(dataPin, clockPin, dataArrayRED[symbol]);
  }
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i=0;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  for (i=7; i>=0; i--)
  {
    digitalWrite(myClockPin, 0);
    digitalWrite(myDataPin, myDataOut & (1 << i));
    digitalWrite(myClockPin, 1); // controller reads data pin on clock pin rise
    digitalWrite(myDataPin, 0); // Zero data pin to ensure consistent writes
  }

  digitalWrite(myClockPin, 0);
}

