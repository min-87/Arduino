#include <RoboCodeFingerprint.h>
RoboCodeFingerprint finger = RoboCodeFingerprint(2, 3);
void setup() {
 Serial.begin(9600);
 finger.begin(57600);
 Serial.println("Ready to write a new fingerprint");
 Serial.println("Please, type in ID # (from 0 to 127)");
}

void loop() {
if (Serial.available() > 0) {
  int id = Serial.parseInt();
  Serial.print("Writing ID #");Serial.println(id);
  finger.writeNewFinger(id);
  Serial.println("Ready tp write a new fingerprint");
  Serial.println("Please, type in the ID # (from 0 to 127)") ; 
}

}
