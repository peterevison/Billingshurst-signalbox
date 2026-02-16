/*
  Billingshurst signal box up
  PE 15/02/2026

  Billingshurst_up (hopefully same script as up-loaded to Nano 14/02/2026)
  what happened to my last mod?

*/

//global variables
const int signal_read_pin = 9;  // read down distant (signal A) input D9
const int lamp[8] = {0, 2, 3, 4, 5, 6, 7, 8}; // set up array to define digital o/p no. to each lamp e.g. lamp 1 is D2, lamp 2 is D3 etc, lamp[0] not used
bool pause; //status of pause switch (toggling on/off button or switch), pause when HIGH
bool debug = HIGH;
unsigned long interval = 0;  // in msec (use type unsigned long for timing - lasts ~50hrs, type int can go out of range)
int lamp_no;  // lamp (track section) number
int mini = 100;    // interval min time setting  100ms
int multi = 6;     // interval multiplier setting = 6x
// mini=100, multi=5 transit times: pot=min  5min; pot=mid 2min40sec; pot=max 40sec;
int nrings; // no. of bell rings
int analogPin = A0; // speed potentiometer wiper (middle terminal) connected to analog pin 0 (A0)

/*
  DIGITAL I/Os:
  D0-1   spare (reserved for debug comms)
  D2-8   o/ps  track diagram lamp outputs to relay module
  D9     o/p   BT bell output via relay module
  D10    i/p   signal 13 repeater 2.3V signal
  D11    i/p   BT bell push 10V signal
  D12    i/p   BT up block instrument  - 3V signal from BT UP commutator
*/
const int BT_BELL      = 9;
const int SIGNAL_13    = 10;
const int BT_BELL_PUSH = 11;
const int BLOCK_INSTR  = 12;
void setup() {
  if (debug) {
    Serial.begin(9600);  //  setup Serial Monitor output for speed debug
  }
  for (int thisPin = 2; thisPin <= 9; thisPin++) {  // set digital i/o pins 2 to 9 as outputs
    pinMode(thisPin, OUTPUT);
    digitalWrite(thisPin, HIGH); // set relays 2-9 initially de-energised (o/p = HIGH)
  }
  for (int thisPin = 10; thisPin <= 12; thisPin++) pinMode(thisPin, INPUT);  // set digital i/o pins 10 to 12 as inputs
}

void loop() {
  Serial.println("start loop");
  delay(2e3); // initial 2s delay
  ding(1);  // call to attention
  Serial.println("first bell sent, wait for reply");
  wait_for(BT_BELL_PUSH);
  delay(0.75e3);
  Serial.println("first bell read");
  ding(3);
  delay(0.3e3);
  ding(1);
  Serial.println("sent line clear request, wait for bell push");
  wait_for(BT_BELL_PUSH);
  Serial.println("BT bell push received, set block inst to LC");
  delay(2.0e3);
  wait_for(BLOCK_INSTR); // await input from BT up block instrument
  Serial.println("block instrument received");
  delay(5e3);
  ding(2);  // train entering section
  Serial.println("sent train entering section, so reply");
  wait_for(BT_BELL_PUSH);
  Serial.println("bell push received");
  delay(4e3);
  interval = multi * (analogRead(analogPin) + mini); // read speed input pin (from pot) to set interval
  digitalWrite(lamp[1], LOW);  // switch on lamp 1 (LOW gives lamp on)
  Serial.println("awaiting signal 13");
  wait_for(SIGNAL_13);    // pause at lamp 2 on until i/p goes low, ie signal 13 is reversed (all clear, on)
  Serial.println("signal 13 is off");
  delay(interval * 6);  // short section between 2 and 3
  digitalWrite(lamp[2], LOW);  // switch on lamp 2 (LOW gives lamp on)
  delay(interval);       // time taken for train to pass insulated section break
  digitalWrite(lamp[1], HIGH); // switch off lamp 1
  delay(interval * 10);  // time taken for train to reach next section break
  for (int lamp_no = 3; lamp_no <= 6; lamp_no++) {
    interval = multi * (analogRead(analogPin) + mini); // read speed input pin (from pot) to set interval
    if (debug) {
      Serial.print("lamp_no (ie section no) =");
      Serial.println(lamp_no);
    }
    digitalWrite(lamp[lamp_no], LOW); //switch on next lamp (unless final section has been reached)
    delay(interval); // time taken for train to pass insulated section break
    digitalWrite(lamp[lamp_no - 1], HIGH); // switch off previous lamp
    delay(interval * 10); // time taken for train to reach next section break
  }
  delay(interval * 25); // long section between 6 and 7
  digitalWrite(lamp[7], LOW);   // switch on final lamp
  delay(interval); // time taken for train to pass insulated section break
  digitalWrite(lamp[6], HIGH);   // switch off penultimate lamp
  delay(interval * 10);  // time taken for train to reach next section break
  digitalWrite(lamp[7], HIGH);   // switch off final lamp
  while (1); // stop further execution
}

void ding(int nrings) {
  for (int n = 1; n <= nrings; n++) {
    digitalWrite(BT_BELL, LOW);  // energise bell push relay briefly - change polarity if optocoupled relay is used
    delay(0.15e3);
    digitalWrite(BT_BELL, HIGH);
    delay(0.15e3);
  }
}

void wait_for(int input_signal)  {
  while (digitalRead(input_signal)) delay(100);
  return;
}
