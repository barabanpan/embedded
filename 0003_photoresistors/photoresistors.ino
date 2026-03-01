#define redLEDpin 11
#define blueLEDpin 10
#define greenLEDpin 9

#define photoresistorPinForRed A0
#define photoresistorPinForGreen A1
#define photoresistorPinForBlue A2

const int photoresistors[3] = {photoresistorPinForRed, photoresistorPinForGreen, photoresistorPinForBlue};
const int LEDs[3] = {redLEDpin, greenLEDpin, blueLEDpin};
const String colors[3] = {"RED", "GREEN", "BLUE"};

void setup() {
  for (int i=0; i<3; i++) {
    pinMode(photoresistors[i], INPUT);
    pinMode(LEDs[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  for (int i=0; i<3; i++) {
    int inVal = analogRead(photoresistors[i]);
    int outVal = map(inVal, 0, 1023, 0, 255);
    analogWrite(LEDs[i], outVal);  // or 255-outVal
    log(i, inVal, outVal);
    delay(10);
  }

  delay(1000);  // to see change
}

void log(int i, int inV, int outV) {
  Serial.print(colors[i]);
  Serial.print("  in=");
  Serial.print(inV);
  Serial.print(" out=");
  Serial.println(outV); 
}

void calibrate() {
  int minVal = 1023;
  int maxVal = 0;

  while (millis() < 5000) {
    for (int i=0; i<3; i++) {
      int val = analogRead(photoresistors[i]);
      if (minVal > val)
        minVal = val;
      if (maxVal < val)
        maxVal = val;
      delay(10);
    }
  }
  Serial.println(minVal);
  Serial.println(maxVal);
}
