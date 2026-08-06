/*
 * زواia كل قدم — معايرة المستخدم
 */

const int PIN_FL = 27, PIN_FR = 16, PIN_RL = 22, PIN_RR = 25;
const int SERVO_HZ = 50, SERVO_BITS = 16;
const int MOVE_DELAY = 12;

const int ANGLE_FL = 120;
const int ANGLE_FR = 0;
const int ANGLE_RL = 40;
const int ANGLE_RR = 180;   // SG90 max 180 (كان 360)

int posFL = 90, posFR = 90, posRL = 90, posRR = 90;
bool okFL, okFR, okRL, okRR;

uint32_t angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  return (map(angle, 0, 180, 500, 2500) * ((1u << SERVO_BITS) - 1)) / 20000;
}

void writeServo(int pin, int angle) { ledcWrite(pin, angleToDuty(angle)); }

void moveOne(int pin, int& pos, int target) {
  target = constrain(target, 0, 180);
  if (pos < target) for (; pos <= target; pos++) { writeServo(pin, pos); delay(MOVE_DELAY); }
  else if (pos > target) for (; pos >= target; pos--) { writeServo(pin, pos); delay(MOVE_DELAY); }
}

void printAngles() {
  Serial.printf("FL=%d FR=%d RL=%d RR=%d\n", ANGLE_FL, ANGLE_FR, ANGLE_RL, ANGLE_RR);
}

void applyAll() {
  if (okFL) moveOne(PIN_FL, posFL, ANGLE_FL);
  if (okFR) moveOne(PIN_FR, posFR, ANGLE_FR);
  if (okRL) moveOne(PIN_RL, posRL, ANGLE_RL);
  if (okRR) moveOne(PIN_RR, posRR, ANGLE_RR);
  printAngles();
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  okFL = ledcAttach(PIN_FL, SERVO_HZ, SERVO_BITS);
  okFR = ledcAttach(PIN_FR, SERVO_HZ, SERVO_BITS);
  okRL = ledcAttach(PIN_RL, SERVO_HZ, SERVO_BITS);
  okRR = ledcAttach(PIN_RR, SERVO_HZ, SERVO_BITS);
  applyAll();
}

void loop() {
  if (!Serial.available()) return;
  char c = Serial.read();
  while (Serial.available()) Serial.read();
  if (c == 'r' || c == 'R') applyAll();
  if (c == 'p' || c == 'P') printAngles();
}
