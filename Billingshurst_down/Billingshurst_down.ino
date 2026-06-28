/*
  Billingshurst signal box down line
  PE 17/09/25

  Billingshurst_down25-12A
  Modified 01/06/26 to move train to section 2 whilst held waiting for distant signal
  plus some preparations for inclusion of AHB operation

*/

//global variables
const int signal_read_pin = 9;             // read down distant (signal A) input D9
int lamp[8] = { 0, 2, 3, 4, 5, 6, 7, 8 };  // set up array to define digital o/p no. to each lamp e.g. lamp 1 is D2, lamp 2 is D3 etc, lamp[0] not used
bool debug = HIGH;
unsigned long interval = 0;  // in msec (use type unsigned long for timing - lasts ~50hrs, type int can go out of range)
int lamp_no;                 // lamp (track section) number
int mini = 100;              // interval min time setting
int multi = 6;               // interval multiplier setting  // mini=100 multi=6 transit times: pot=min 7min30sec; pot=mid 4min45sec; pot=max 50sec;
int analogPin = A0;          // speed potentiometer wiper (middle terminal) connected to analog pin 0 (A0)

void setup() {
  if (debug) {
    Serial.begin(9600);  //  setup Serial Monitor output for speed debug (note: don't use D0 or D1 for other purposes, reserve for monitor)
  }
  for (int thisPin = 2; thisPin <= 8; thisPin++) {  // set digital i/o pins 2 to 8 as outputs
    pinMode(thisPin, OUTPUT);
    digitalWrite(thisPin, HIGH);  // set relays initially de-energised (o/p = HIGH), this gives all lamps off initially
  }
  pinMode(9, INPUT_PULLUP);  // set D9 as digital input
}

void loop() {
  if (debug) Serial.println("\n start loop");
  interval = multi * (analogRead(analogPin) + mini);  // read speed input pin (from pot) to set interval
  // Serial.println(interval);          // speed pot debug value (read in Tools->Serial Monitor)
  delay(4e3);
  digitalWrite(lamp[1], LOW);                            // switch on lamp 1 (LOW gives lamp on)
  delay(interval * 5);                                   // time taken for train to reach next section break
  digitalWrite(lamp[2], LOW);                            // switch on lamp 2 (LOW gives lamp on)
  delay(interval);                                       // time taken for train to pass insulated section break = 1 interval
  digitalWrite(lamp[1], HIGH);                           // switch off lamp 1
  if (debug) Serial.println("train in section 2, signal A");
  while (digitalRead(signal_read_pin)) delay(interval);  // pause at lamp 2 until distant A lever is reversed (all clear, on)
  if (debug) Serial.println("signal A is off");
  delay(interval * 10);                                  //Lamp 2 to lamp 3 is a long section
// Barns Green AHB
  delay(interval * 10);
  for (int lamp_no = 3; lamp_no <= 6; lamp_no++) {      // continue to section 6 regardless of signals
    interval = multi * (analogRead(analogPin) + mini);  // read speed input pin (from pot) to set interval
    if (debug) {
      Serial.print("lamp_no (ie section no) =");
      Serial.println(lamp_no);
    }
    digitalWrite(lamp[lamp_no], LOW);       //switch on next lamp
    delay(interval);                        // time taken for train to pass insulated section break
    digitalWrite(lamp[lamp_no - 1], HIGH);  // switch off previous lamp
    delay(interval * 10);                   // time taken for train to reach next section break
  }
  digitalWrite(lamp[7], LOW);   // switch on final section lamp
  delay(interval);
  digitalWrite(lamp[6], HIGH);
  delay(interval * 10);         
  // Adversane AHB
  delay(interval * 10);
  // Cray Lane AHB
  delay(interval * 10);       
  digitalWrite(lamp[7], HIGH);  // switch off final lamp
  if (debug) Serial.println("idle until reset");
  while (1)
    ;  // stop further execution
}
