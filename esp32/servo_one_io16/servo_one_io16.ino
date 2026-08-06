/*
 * اختبار سيرfو — IO27 (بدون ESP32Servo)
 * Smart Methods — BodyV2
 *
 * يشتغل مع ESP32 Core 3.x
 * أصفر → IO27 | أحمر → 5V | بني → GND
 *
 * Serial 115200 — لازم يطلع: *** LEDC v27 ***
 */

const int SERVO_PIN = 27;
const int SERVO_HZ  = 50;
const int SERVO_BITS = 16;

const int NEUTRAL    = 90;
const int ANGLE_HIGH = 130;
const int ANGLE_LOW  = 50;
const int STEP       = 15;

int pos = NEUTRAL;

uint32_t angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  uint32_t pulseUs = map(angle, 0, 180, 500, 2500);
  uint32_t maxDuty = (1u << SERVO_BITS) - 1;
  return (pulseUs * maxDuty) / 20000;
}

void writeAngle(int angle) {
  ledcWrite(SERVO_PIN, angleToDuty(angle));
}

void moveTo(int target) {
  target = constrain(target, 0, 180);
  Serial.printf("Move %d -> %d\n", pos, target);
  if (pos < target) {
    for (; pos <= target; pos++) { writeAngle(pos); delay(STEP); }
  } else {
    for (; pos >= target; pos--) { writeAngle(pos); delay(STEP); }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("\n*** LEDC v27 — no ESP32Servo ***");

  bool ok = ledcAttach(SERVO_PIN, SERVO_HZ, SERVO_BITS);
  Serial.printf("ledcAttach IO27 -> %s\n", ok ? "OK" : "FAIL");

  if (!ok) {
    Serial.println("FAIL — جرب IO22 أو IO25");
    return;
  }

  delay(300);
  writeAngle(NEUTRAL);
  delay(500);

  Serial.println("Moving 90 -> 130 -> 50 -> 90");
  moveTo(ANGLE_HIGH);
  delay(800);
  moveTo(ANGLE_LOW);
  delay(800);
  moveTo(NEUTRAL);
  Serial.println("Done");
}

void loop() {}
