/*
  Billingshurst_bell_test
  PE 07/02/2026

*/
//global variables
bool debug = HIGH;
float interval = 0;  // in msec (use type unsigned long for timing - lasts ~50hrs, type int can go out of range)
float mini = 10;    // interval min time setting  10ms
float multi = 2.5;     // interval multiplier setting = 2.5
int analogPin = A0; // speed potentiometer wiper (middle terminal) connected to analog pin 0 (A0)
/*
  DIGITAL I/Os:
  D9     o/p   BT bell output via relay module
  D11    i/p   BT bell push 10V signal
*/
const int BT_BELL      = 9;
const int BT_BELL_PUSH = 11;
void setup() {
  if (debug) {
    Serial.begin(9600);  //  setup Serial Monitor output for debugging statements
  }
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH); // set relay 9 initially de-energised (o/p = HIGH)
  pinMode(11, INPUT);  // set digital i/o pin 11 as input
}

void loop() {
  Serial.println("start loop");
  delay(2e3); // initial 2s delay
  interval = multi * (analogRead(analogPin) + mini); // read pot input pin 0-2V ie reads 0-409
  Serial.print("dwell time (ms) = ");
  Serial.println(interval);
  delay(2e3);
  ding(3);  // call to attention
  Serial.println("first bell sent, wait for reply");
  wait_for(BT_BELL_PUSH);
  delay(1e3);
  Serial.println("first bell read");
  delay(2e3);
  //while (1); // stop further execution
}

void ding(int nrings) {
  for (int n = 1; n <= nrings; n++) {
    digitalWrite(BT_BELL, LOW);  // energise bell push relay briefly - change polarity if optocoupled relay is used
    delay(interval);
    digitalWrite(BT_BELL, HIGH);
    delay(0.5e3);
  }
  return;
}

void wait_for(int input_signal)  {
  while (digitalRead(input_signal)) delay(100);
  return;
}
