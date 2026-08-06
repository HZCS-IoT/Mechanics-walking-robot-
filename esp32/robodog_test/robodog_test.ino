/*
 * اختبار سيرfو — واحد واحد (LEDC)
 * FL=16  FR=26  RL=25  RR=22
 * Neutral: FL=0 FR=0 RL=110 RR=75
 *
 * Serial 115200:
 *   1=FL  2=FR(IO16)  3=RL  4=RR  ← اختيار رجل
 *   + أو w = زيادة 15°
 *   - أو s = نقص 15°
 *   n = neutral الكل
 *   p = طباعة الزوايا
 */

const int PIN_FL = 16;
const int PIN_FR = 26;
const int PIN_RL = 25;
const int PIN_RR = 22;

const int SERVO_HZ   = 50;
const int SERVO_BITS = 16;
const int STEP       = 8;
const int DELTA      = 15;

const int NEUTRAL_FL = 90;
const int NEUTRAL_FR = 90;
const int NEUTRAL_RL = 110;
const int NEUTRAL_RR = 75;

int posFL = NEUTRAL_FL;
int posFR = NEUTRAL_FR;
int posRL = NEUTRAL_RL;
int posRR = NEUTRAL_RR;

int selPin = PIN_FR;
int* selPos = &posFR;
const char* selName = "FR IO16";

uint32_t angleToDuty(int a) {
  a = constrain(a, 0, 180);
  return (map(a, 0, 180, 500, 2500) * 65535UL) / 20000;
}

void writeServo(int pin, int a) {
  ledcWrite(pin, angleToDuty(a));
}

bool attachLeg(int pin, const char* name) {
  bool ok = ledcAttach(pin, SERVO_HZ, SERVO_BITS);
  Serial.printf("%s GPIO%d -> %s\n", name, pin, ok ? "OK" : "FAIL");
  delay(50);
  return ok;
}

void moveOne(int pin, int& pos, int target) {
  target = constrain(target, 0, 180);
  while (pos != target) {
    if (pos < target) pos++; else pos--;
    writeServo(pin, pos);
    delay(STEP);
  }
}

void selectLeg(char c) {
  switch (c) {
    case '1': selPin = PIN_FL; selPos = &posFL; selName = "FL IO16"; break;
    case '2': selPin = PIN_FR; selPos = &posFR; selName = "FR IO26"; break;
    case '3': selPin = PIN_RL; selPos = &posRL; selName = "RL IO25"; break;
    case '4': selPin = PIN_RR; selPos = &posRR; selName = "RR IO22"; break;
    default: return;
  }
  Serial.printf("Selected: %s  angle=%d\n", selName, *selPos);
}

void goNeutralAll() {
  moveOne(PIN_FL, posFL, NEUTRAL_FL);
  moveOne(PIN_FR, posFR, NEUTRAL_FR);
  moveOne(PIN_RL, posRL, NEUTRAL_RL);
  moveOne(PIN_RR, posRR, NEUTRAL_RR);
  Serial.println("Neutral OK");
}

void printAll() {
  Serial.printf("FL=%d  FR=%d  RL=%d  RR=%d\n", posFL, posFR, posRL, posRR);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== Servo Test ===");
  Serial.println("1=FL 2=FR 3=RL 4=RR | +/- | n=neutral | p=print");

  attachLeg(PIN_FL, "FL");
  attachLeg(PIN_FR, "FR");
  attachLeg(PIN_RL, "RL");
  attachLeg(PIN_RR, "RR");

  goNeutralAll();
  selectLeg('1');
}

void loop() {
  if (!Serial.available()) return;
  char c = Serial.read();
  while (Serial.available()) Serial.read();

  if (c == '1' || c == '2' || c == '3' || c == '4') { selectLeg(c); return; }
  if (c == 'n' || c == 'N') { goNeutralAll(); return; }
  if (c == 'p' || c == 'P') { printAll(); return; }

  if (c == '+' || c == 'w' || c == 'W') {
    moveOne(selPin, *selPos, constrain(*selPos + DELTA, 0, 180));
    Serial.printf("%s = %d\n", selName, *selPos);
  } else if (c == '-' || c == 's' || c == 'S') {
    moveOne(selPin, *selPos, constrain(*selPos - DELTA, 0, 180));
    Serial.printf("%s = %d\n", selName, *selPos);
  } else {
    Serial.println("1/2/3/4 select | +/- move | n neutral | p print");
  }
}
