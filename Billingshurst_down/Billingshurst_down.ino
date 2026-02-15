/*
  Billingshurst signal box down line
  PE 17/09/25

  Billingshurst_down25-12A

*/

//global variables
const int signal_read_pin = 9;  // read down distant (signal A) input D9
int lamp[8] = {0, 2, 3, 4, 5, 6, 7, 8}; // set up array to define digital o/p no. to each lamp e.g. lamp 1 is D2, lamp 2 is D3 etc, lamp[0] not used
bool debug = 0;
unsigned long interval = 0;  // in msec (use type unsigned long for timing - lasts ~50hrs, type int can go out of range)
int lamp_no;  // lamp (track section) number
int mini = 100;    // interval min time setting
int multi = 6;     // interval multiplier setting  // mini=100 multi=6 transit times: pot=min 7min30sec; pot=mid 4min45sec; pot=max 50sec;
int analogPin = A0; // speed potentiometer wiper (middle terminal) connected to analog pin 0 (A0)

void setup() {
  if (debug) {
    Serial.begin(9600);  //  setup Serial Monitor output for speed debug (note: don't use D0 or D1 for other purposes, reserve for monitor)
  }
  for (int thisPin = 2; thisPin <= 8; thisPin++) {  // set digital i/o pins 2 to 8 as outputs
    pinMode(thisPin, OUTPUT);
    digitalWrite(thisPin, HIGH); // set relays initially de-energised (o/p = HIGH), this gives all lamps off initially
  }
  pinMode(9, INPUT);  // set D9 as digital input
}

void loop() {
  interval = multi * (analogRead(analogPin) + mini); // read speed input pin (from pot) to set interval
  // Serial.println(interval);          // speed pot debug value (read in Tools->Serial Monitor)
  digitalWrite(lamp[1], LOW);  // switch on lamp 1 (LOW gives lamp on)
  while  (digitalRead(signal_read_pin))  delay(interval);  // pause at lamp 1 until distant A lever is reversed (all clear, on)
  delay(interval * 5);  // time taken for train to reach next section break
  digitalWrite(lamp[2], LOW);  // switch on lamp 2 (LOW gives lamp on)
  delay(interval); // time taken for train to pass insulated section break = 1 interval
  digitalWrite(lamp[1], HIGH);     // switch off lamp 1
  delay(interval * 20);  //Lamp 2 to lamp 3 is a long section
  for (int lamp_no = 3; lamp_no < 8; lamp_no++) {  // complete remaining sections regardless of signals
    interval = multi * (analogRead(analogPin) + mini); // read speed input pin (from pot) to set interval
    if (debug) {
      Serial.print("lamp_no (ie section no) =");
      Serial.println(lamp_no);
    }
    if (lamp_no <= 7)  digitalWrite(lamp[lamp_no], LOW); //switch on next lamp (unless final section has been reached)
    delay(interval); // time taken for train to pass insulated section break
    digitalWrite(lamp[lamp_no - 1], HIGH); // switch off previous lamp
    delay(interval * 10); // time taken for train to reach next section break
  }
  digitalWrite(lamp[7], HIGH);   // switch off final lamp
  while (1); // stop further execution
}
