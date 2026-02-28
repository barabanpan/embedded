const int dataPin = 8;
const int latchPin = 7;
const int clockPin = 6;
uint16_t states; // 5 plus rows, 11 minus cols

const int picLength = 46;
const int pic[5][picLength] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("setup");
  // 5 + rows all LOW, 11 - cols all HIGH, to ensure LEDs are off
  states = 0b0000011111111111;
  rewriteShiftRegisters();
}

void rewriteShiftRegisters() {
  uint8_t firstSR = states >> 8;
  uint8_t secondSR  = states & 0xFF;
  shiftOut(dataPin, clockPin, LSBFIRST, secondSR);
  shiftOut(dataPin, clockPin, LSBFIRST, firstSR);
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}

void setRow(int ind, bool val) {
  if (val)
    states |= (1 << ((7-ind)+8));
  else
    states &= ~(1 << ((7-ind)+8));
}

void setCol(int ind, bool val) {
  if (val)
    states |= (1 << (10-ind));
  else
    states &= ~(1 << (10-ind));
}

void refreshScreen(int frame) {
  // iterate over rows (anodes, +)
  for (int i = 0; i < 5; i++) {
    setRow(i, HIGH);

    // iterate over cols (cathodes, -) that are HIGH
    for (int j = 0; j < 11; j++) {
      if (pic[i][frame + j] == 1) {
        // turn the LED on by writing cathode LOW
        setCol(j, LOW);
      }
    }
    rewriteShiftRegisters();

    for (int j = 0; j < 11; j++) {
      if (pic[i][frame + j] == 1)
        setCol(j, HIGH);
    }

    // turn off the row
    setRow(i, LOW);
    rewriteShiftRegisters();
  }
}

int frame = 0;
unsigned long prevFrameTime = millis();
const unsigned long timePerFrame = 400;

void loop() {
  if (millis() - prevFrameTime > timePerFrame) {
    // shift the pic by 1
    frame += 1;
    if (frame > picLength - 11)  // start again
      frame = 0;
    
    prevFrameTime = millis();
  }
  refreshScreen(frame); 
}