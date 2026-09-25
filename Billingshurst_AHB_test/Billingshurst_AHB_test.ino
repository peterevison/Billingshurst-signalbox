/*
  Billingshurst signal box AHB test (supersedes earlier simpler file of that name)
  edited from version of Billingshurst_down
  PE 125/9/26

  

*/

//global variables
const int AHB[3] = { A1, A2, A3 };  // array of Analogue i/o nos used for AHBs
const int AHBCount = 3;             // total number of AHBs
const int CRAY_LANE = 0;
const int ADVERSANE = 1;
const int BARNS_GREEN = 2;
/*
  DIGITAL I/Os: (includes analogue i/os used as digital o/ps A1, A2 and A3)
  D0-1   spare (reserved for debug comms)
  D2-8  o/ps   outputs to relay module
  D9     i/p   signal A input
  A1     o/p   Cray Lane AHB
  A2     o/p   Adversane AHB
  A3     o/p   Barns Green AHB
*/
const int signal_read_pin = 9;             // read down distant (signal A) input D9
int lamp[8] = { 0, 2, 3, 4, 5, 6, 7, 8 };  // set up array to define digital o/p no. to each lamp e.g. lamp 1 is D2, lamp 2 is D3 etc, lamp[0] not used
bool debug = HIGH;

void setup() {
  if (debug) {
    Serial.begin(9600);  //  setup Serial Monitor output for speed debug (note: don't use D0 or D1 for other purposes, reserve for monitor)
  }
   for (int i = 0; i < AHBCount; i++) {  // set analogue i/o pins for AHB o/ps
    pinMode(AHB[i], OUTPUT);
    digitalWrite(AHB[i], LOW);  // AHBs set to barriers up initially
  }
}

void loop() {
  if (debug) Serial.println("\n start loop");
  delay(3e3);
  activate_AHB(BARNS_GREEN);  // train reaches Barns Green AHB
  activate_AHB(ADVERSANE);
  activate_AHB(CRAY_LANE);
  delay(30e3);
}

void activate_AHB(int AHB_no) {  // provide a 100ms +ve going pulse to appropriate monostable i/p
  digitalWrite(AHB[AHB_no], HIGH);
  delay(100);
  digitalWrite(AHB[AHB_no], LOW);
  return;
}
