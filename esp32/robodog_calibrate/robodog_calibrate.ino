/*
 * Robot Dog — معايرة الزواia (LEDC — Core 3.x)
 * Smart Methods — BodyV2
 *
 * Pins: FL=27  FR=16  RL=22  RR=25
 * Serial Monitor 115200 — Newline = نعم
 *
 * ─── الأوامر ───
 *  1 2 3 4     اختيار رجل + تحريكها لوضعها المحفوظ
 *  + / -       زاوية ±1  (الرجل المختارة)
 *  > / <       زاوية ±5
 *  a           Homing — الكل لوضعها المحفوظ
 *  p           طباعة القيم — انسخها لـ robodog_mqtt
 *  j           تجربة وضع الجلوس
 *  s           وقوف من الجلوس
 *  h           مساعدة
 *
 *  ابدأ: اضغط 1 ثم + / - لين الوضع يصير م perfect → 2 → 3 → 4 → p
 */

const int PIN_FL = 27;
const int PIN_FR = 16;
const int PIN_RL = 22;
const int PIN_RR = 25;

const int SERVO_HZ   = 50;
const int SERVO_BITS = 16;
const int STEP_DELAY = 10;

// ===== قيم البداية — عدّلها بعد المعايرة =====
int neutralFL = 105;
int neutralFR = 105;
int neutralRL = 105;
int neutralRR = 105;

const int SIT_FRONT = 78;
const int SIT_REAR  = 148;
const int SIT_DELAY = 18;

int posFL = 105;
int posFR = 105;
int posRL = 105;
int posRR = 105;

int selected = 0;  // 1=FL 2=FR 3=RL 4=RR

bool okFL, okFR, okRL, okRR;

uint32_t angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  uint32_t pulseUs = map(angle, 0, 180, 500, 2500);
  return (pulseUs * ((1u << SERVO_BITS) - 1)) / 20000;
}

void writeServo(int pin, int angle) {
  ledcWrite(pin, angleToDuty(angle));
}

bool attachLeg(int pin, const char* name) {
  bool ok = ledcAttach(pin, SERVO_HZ, SERVO_BITS);
  Serial.printf("ledcAttach %s GPIO%d -> %s\n", name, pin, ok ? "OK" : "FAIL");
  delay(80);
  return ok;
}

void moveOne(int pin, int& pos, int target, int stepMs = STEP_DELAY) {
  target = constrain(target, 0, 180);
  if (pos < target) {
    for (; pos <= target; pos++) { writeServo(pin, pos); delay(stepMs); }
  } else if (pos > target) {
    for (; pos >= target; pos--) { writeServo(pin, pos); delay(stepMs); }
  }
}

void moveOneFast(int pin, int& pos, int target) {
  target = constrain(target, 0, 180);
  pos = target;
  writeServo(pin, pos);
}

int* neutralPtr() {
  switch (selected) {
    case 1: return &neutralFL;
    case 2: return &neutralFR;
    case 3: return &neutralRL;
    case 4: return &neutralRR;
    default: return nullptr;
  }
}

int getPin() {
  switch (selected) {
    case 1: return PIN_FL;
    case 2: return PIN_FR;
    case 3: return PIN_RL;
    case 4: return PIN_RR;
    default: return -1;
  }
}

int* getPosPtr() {
  switch (selected) {
    case 1: return &posFL;
    case 2: return &posFR;
    case 3: return &posRL;
    case 4: return &posRR;
    default: return nullptr;
  }
}

const char* legName() {
  switch (selected) {
    case 1: return "FL (IO27)";
    case 2: return "FR (IO16)";
    case 3: return "RL (IO22)";
    case 4: return "RR (IO25)";
    default: return "?";
  }
}

bool legOk() {
  switch (selected) {
    case 1: return okFL;
    case 2: return okFR;
    case 3: return okRL;
    case 4: return okRR;
    default: return false;
  }
}

void printHelp() {
  Serial.println("\n--- Commands ---");
  Serial.println("1 2 3 4  = select leg");
  Serial.println("+ -      = angle +/- 1");
  Serial.println("> <      = angle +/- 5");
  Serial.println("a        = all legs to saved neutral");
  Serial.println("p        = PRINT values (copy to mqtt code)");
  Serial.println("j        = test sit pose");
  Serial.println("s        = stand from sit");
  Serial.println("h        = this help");
}

void printStatus() {
  Serial.printf("\nSelected: %s\n", selected ? legName() : "none — press 1/2/3/4");
  Serial.printf("Saved: FL=%d FR=%d RL=%d RR=%d\n",
                neutralFL, neutralFR, neutralRL, neutralRR);
}

void printCopyPaste() {
  Serial.println("\n========== COPY TO robodog_mqtt.ino ==========");
  Serial.printf("const int NEUTRAL_FL = %d;\n", neutralFL);
  Serial.printf("const int NEUTRAL_FR = %d;\n", neutralFR);
  Serial.printf("const int NEUTRAL_RL = %d;\n", neutralRL);
  Serial.printf("const int NEUTRAL_RR = %d;\n", neutralRR);
  Serial.println("");
  Serial.println("// أو إذا الكل متقارب:");
  Serial.printf("const int NEUTRAL = %d;\n",
                (neutralFL + neutralFR + neutralRL + neutralRR) / 4);
  Serial.println("===============================================\n");
}

void selectLeg(int leg) {
  if (leg < 1 || leg > 4) return;
  selected = leg;
  if (!legOk()) {
    Serial.printf("WARN: %s attach failed\n", legName());
    return;
  }
  int pin = getPin();
  int* n = neutralPtr();
  int* p = getPosPtr();
  Serial.printf("\n>> Selected %s — neutral=%d\n", legName(), *n);
  moveOne(pin, *p, *n);
}

void adjustSelected(int delta) {
  if (!selected || !legOk()) {
    Serial.println("Select leg first: 1 2 3 4");
    return;
  }
  int* n = neutralPtr();
  int* p = getPosPtr();
  int pin = getPin();
  *n = constrain(*n + delta, 0, 180);
  moveOneFast(pin, *p, *n);
  Serial.printf("%s = %d\n", legName(), *n);
}

void homingAll() {
  Serial.println("\nHoming all...");
  if (okFL) moveOne(PIN_FL, posFL, neutralFL);
  if (okFR) moveOne(PIN_FR, posFR, neutralFR);
  if (okRL) moveOne(PIN_RL, posRL, neutralRL);
  if (okRR) moveOne(PIN_RR, posRR, neutralRR);
  Serial.println("Homing done.");
  printStatus();
}

void testSit() {
  Serial.println("\n>>> Sit test");
  if (okFL) moveOne(PIN_FL, posFL, SIT_FRONT, SIT_DELAY);
  if (okFR) moveOne(PIN_FR, posFR, SIT_FRONT, SIT_DELAY);
  delay(200);
  if (okRL) moveOne(PIN_RL, posRL, SIT_REAR, SIT_DELAY);
  if (okRR) moveOne(PIN_RR, posRR, SIT_REAR, SIT_DELAY);
  Serial.println("Sit done — press s to stand");
}

void standFromSit() {
  Serial.println("\n>>> Stand");
  if (okRL) moveOne(PIN_RL, posRL, neutralRL, SIT_DELAY);
  if (okRR) moveOne(PIN_RR, posRR, neutralRR, SIT_DELAY);
  delay(250);
  if (okFL) moveOne(PIN_FL, posFL, neutralFL, SIT_DELAY);
  if (okFR) moveOne(PIN_FR, posFR, neutralFR, SIT_DELAY);
  Serial.println("Stand done.");
}

void handleCmd(char c) {
  if (c >= '1' && c <= '4') { selectLeg(c - '0'); return; }

  switch (c) {
    case '+':
    case '=': adjustSelected(+1); break;
    case '-':
    case '_': adjustSelected(-1); break;
    case '>': adjustSelected(+5); break;
    case '<': adjustSelected(-5); break;
    case 'a':
    case 'A': homingAll(); break;
    case 'p':
    case 'P': printCopyPaste(); break;
    case 'j':
    case 'J': testSit(); break;
    case 's':
    case 'S': standFromSit(); break;
    case 'h':
    case 'H': printHelp(); break;
    default: break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println("\n=== Angle Calibration ===");
  Serial.println("FL=27 FR=16 RL=22 RR=25");

  okFL = attachLeg(PIN_FL, "FL");
  okFR = attachLeg(PIN_FR, "FR");
  okRL = attachLeg(PIN_RL, "RL");
  okRR = attachLeg(PIN_RR, "RR");

  homingAll();
  printHelp();
  printStatus();
  Serial.println("\nStart: press 1 then + / -");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    while (Serial.available()) Serial.read();
    handleCmd(c);
  }
}
