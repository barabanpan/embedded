int buzzerPin = 8;
int buttonsPin = A0;

int notes[] = {262, 294, 330, 349}; // C4, D4, E4, F4 
char notesNames[] = "CDEF";
int buttonsValues[] = {1023, 1002, 932, 510};  // +-2

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonsPin, INPUT);
}

void loop() {
  int v = analogRead(buttonsPin);
  int iN = -1;
  
  for (int i=0; i<4; i++) {
    if (buttonsValues[i]-2 <= v && v <= buttonsValues[i]+2) {
      iN = i;
      break;
    }
  }
  if (iN != -1) {
    Serial.print(v);
    Serial.print(" -> ");
    Serial.print(iN);
    Serial.print(" -> ");
    Serial.print(notes[iN]);
    Serial.print(" -> ");
    Serial.println(notesNames[iN]);

    tone(buzzerPin, notes[iN]);
  } else if (v != 0) {
    Serial.println(v);
  }
}
