/*
 * Robot Dog — اختبار القوة (Sweep Style)
 * Smart Methods — BodyV2
 *
 * نفس فكرة Sweep.ino — حركة تدريجية + مرآة (180-pos)
 * LEDC — ESP32 Core 3.x
 *
 * Pins: FL=27  FR=16  RL=22  RR=25
 * Serial 115200
 *
 * 1 = FL+FR (أمام — متعاكسين)
 * 2 = RL+RR (خلف — متعاكسين)
 * 3 = FL+RR (قطري)
 * 4 = FR+RL (قطري)
 * a = الكل sweep
 */

const int PIN_FL = 27;
const int PIN_FR = 16;
const int PIN_RL = 22;
const int PIN_RR = 25;

const int SERVO_HZ   = 50;
const int SERVO_BITS = 16;

// ═══ عدّل هنا — القوة والسرعة ═══
const int NEUTRAL     = 105;   // مركز الحركة
const int SWEEP_RANGE = 25;    // قوة/مدى — زِد = أقوى (15~35)
const int DELAY_FWD   = 25;    // سرعة للأمام — أقل = أسرع/أقوى
const int DELAY_BACK  = 15;    // سرعة للرجوع
// ═══════════════════════════════

int posA = NEUTRAL;
int posB = NEUTRAL;

uint32_t angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  uint32_t pulseUs = map(angle, 0, 180, 500, 2500);
  return (pulseUs * ((1u << SERVO_BITS) - 1)) / 20000;
}

void writeServo(int pin, int angle) {
  ledcWrite(pin, angleToDuty(angle));
}

bool attachLeg(int pin) {
  return ledcAttach(pin, SERVO_HZ, SERVO_BITS);
}

// مرآة حول NEUTRAL — مثل pos2 = 180 - pos لكن حول وضعك
int mirrorPos(int pos) {
  return constrain(2 * NEUTRAL - pos, 0, 180);
}

// Sweep بين رجلين — A يزيد، B ينقص (متعاكسين)
void sweepPair(int pinA, int& posA, int pinB, int& posB) {
  Serial.printf("Sweep GPIO%d + GPIO%d  range=%d\n", pinA, pinB, SWEEP_RANGE);

  posA = NEUTRAL;
  posB = NEUTRAL;
  writeServo(pinA, posA);
  writeServo(pinB, posB);
  delay(300);

  // للأمام — مثل Sweep: pos 90→180
  for (int p = NEUTRAL; p <= NEUTRAL + SWEEP_RANGE; p++) {
    posA = p;
    posB = mirrorPos(p);
    writeServo(pinA, posA);
    writeServo(pinB, posB);
    delay(DELAY_FWD);
  }
  delay(400);

  // للرجوع — pos 180→90
  for (int p = NEUTRAL + SWEEP_RANGE; p >= NEUTRAL; p--) {
    posA = p;
    posB = mirrorPos(p);
    writeServo(pinA, posA);
    writeServo(pinB, posB);
    delay(DELAY_BACK);
  }
  delay(300);
  Serial.println("Sweep done.");
}

// قطري — نفس الاتجاه (مشي)
void sweepPairSame(int pinA, int& posA, int pinB, int& posB) {
  Serial.printf("Diagonal sweep GPIO%d + GPIO%d\n", pinA, pinB);
  posA = NEUTRAL;
  posB = NEUTRAL;

  for (int p = NEUTRAL; p <= NEUTRAL + SWEEP_RANGE; p++) {
    posA = p;
    posB = p;
    writeServo(pinA, posA);
    writeServo(pinB, posB);
    delay(DELAY_FWD);
  }
  delay(400);
  for (int p = NEUTRAL + SWEEP_RANGE; p >= NEUTRAL; p--) {
    posA = p;
    posB = p;
    writeServo(pinA, posA);
    writeServo(pinB, posB);
    delay(DELAY_BACK);
  }
  Serial.println("Diagonal done.");
}

void handleCmd(char c) {
  switch (c) {
    case '1': sweepPair(PIN_FL, posA, PIN_FR, posB); break;
    case '2': sweepPair(PIN_RL, posA, PIN_RR, posB); break;
    case '3': sweepPairSame(PIN_FL, posA, PIN_RR, posB); break;
    case '4': sweepPairSame(PIN_FR, posA, PIN_RL, posB); break;
    case 'a':
    case 'A':
      sweepPair(PIN_FL, posA, PIN_FR, posB);
      delay(500);
      sweepPair(PIN_RL, posA, PIN_RR, posB);
      delay(500);
      sweepPairSame(PIN_FL, posA, PIN_RR, posB);
      delay(500);
      sweepPairSame(PIN_FR, posA, PIN_RL, posB);
      break;
    default:
      Serial.println("1=front pair 2=rear pair 3=FL+RR 4=FR+RL | a=all");
      Serial.printf("NEUTRAL=%d RANGE=%d DELAY=%d/%d\n",
                    NEUTRAL, SWEEP_RANGE, DELAY_FWD, DELAY_BACK);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== Sweep Strength Test ===");
  Serial.println("FL=27 FR=16 RL=22 RR=25");

  attachLeg(PIN_FL);
  attachLeg(PIN_FR);
  attachLeg(PIN_RL);
  attachLeg(PIN_RR);

  writeServo(PIN_FL, NEUTRAL);
  writeServo(PIN_FR, NEUTRAL);
  writeServo(PIN_RL, NEUTRAL);
  writeServo(PIN_RR, NEUTRAL);

  Serial.printf("NEUTRAL=%d  SWEEP_RANGE=%d (power)\n", NEUTRAL, SWEEP_RANGE);
  Serial.printf("DELAY_FWD=%d  DELAY_BACK=%d (speed)\n", DELAY_FWD, DELAY_BACK);
  Serial.println("Press 1/2/3/4 or a");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    while (Serial.available()) Serial.read();
    handleCmd(c);
  }
}
