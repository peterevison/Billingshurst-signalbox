/*
  Billingshurst_AHB_test
  PE 08/09/2026

*/
//global variables
const int digitalPins[] = {A1, A2, A3}; 
const int pinCount = 3; // Total number of pins in the list
int j=0;

bool debug = HIGH;
void setup() {
  if (debug) {
    Serial.begin(9600);  //  setup Serial Monitor output for debugging statements
  }
   for (int i = 0; i < pinCount; i++) {
    pinMode(digitalPins[i], OUTPUT);
    digitalWrite(digitalPins[i], LOW);
  }
}

void loop() {
  Serial.print("start loop ");
  Serial.println(j);
  delay(2e3);
  digitalWrite(A1, HIGH);
  delay(20e3);
  digitalWrite(A1, LOW);
  delay(1e3);
  digitalWrite(A2, HIGH);
  delay(100);
  digitalWrite(A2, LOW);
  delay(1e3);
  digitalWrite(A3, HIGH);
  delay(100);
  digitalWrite(A3, LOW);
  delay(30e3);
  j++;
  //while (1);
}
