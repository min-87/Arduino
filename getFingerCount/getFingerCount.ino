#include <RoboCodeFingerprint.h>
RoboCodeFingerprint finger = RoboCodeFingerprint(2,3);
void setup() {
  Serial.begin(9600);
  finger.begin(57600);
  finger.getTemplateCount();
  Serial.print("Sensor contains");
  Serial.print(finger.templateCount);
  Serial.println("templates");
  Serial.println("Waiting for valid finger...");

}

void loop() {
 
}
