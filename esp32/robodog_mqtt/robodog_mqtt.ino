/*
 * Robot Dog — ESP32 + MQTT + 4 Servos
 * Smart Methods — BodyV2 (Diagonal Gait)
 *
 * ─── كيف يشتغل المشروع ───
 * 1) الجوال/اللابتوب يرسل أمر (f, b, S, wL, wR...) عبر MQTT
 * 2) ESP32 يستقبل الأمر ويحرّك السيرvoات عبر LEDC (PWM)
 *
 * ─── أوامر MQTT (Topic: smartmethods/robodog/command) ───
 *   f   = خطوة للأمام
 *   b   = خطوة للخلف
 *   S   = وقوف (neutral)
 *   wL  = مصافحة يسار (FL)
 *   wR  = مصافحة يمين (FR)
 *   j   = جلس
 *
 * ─── المشي (Diagonal Gait) ───
 *   أمام: 1) FL+RR → neutral  2) FR+RL → neutral
 *   خلف:  نفس الأزواج — زوايا معكوسة
 *
 * ─── Pins (لا تستخدم IO27 — معطّل) ───
 *   FL=16  FR=4  RL=25  RR=22
 *
 * ─── قبل الرفع ───
 *   • عدّل WIFI_SSID و WIFI_PASS أدناه
 *   • Board: ESP32 Dev Module | Monitor: 115200
 *   • لا تستخدم Servo.h — LEDC فقط (Core 3.x)
 */

#include <WiFi.h>
#include <PubSubClient.h>

// ═══════════════════════════════════════════════════════════
// WiFi — ضع هنا بيانات شبكتك (2.4 GHz فقط — ESP32 لا يدعم 5 GHz)
// ═══════════════════════════════════════════════════════════
const char* WIFI_SSID = "YOUR_WIFI_NAME";      // اسم شبكة WiFi (مثال: MyHomeWiFi)
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";  // كلمة سر WiFi

// ═══════════════════════════════════════════════════════════
// MQTT — Broker عام مجاني (نفسه في لوحة التحكم web/h/)
// ═══════════════════════════════════════════════════════════
const char* MQTT_SERVER = "broker.hivemq.com";           // عنوان الـ Broker
const int   MQTT_PORT   = 1883;                           // ESP32 يستخدم 1883 (TCP)
const char* MQTT_TOPIC  = "smartmethods/robodog/command"; // Topic — لا تغيّره إلا لو غيّرت اللوحة أيضاً

// ═══════════════════════════════════════════════════════════
// GPIO — توصيل أسلاك السيرvo (Signal → GPIO)
// ═══════════════════════════════════════════════════════════
const int PIN_FL = 16;  // Front Left  — أمام يسار
const int PIN_FR = 4;   // Front Right — أمام يمين (زوايا معكوسة عن FL)
const int PIN_RL = 25;  // Rear Left   — خلف يسار
const int PIN_RR = 22;  // Rear Right  — خلف يمين

// ═══════════════════════════════════════════════════════════
// LEDC — إعداد PWM للسيرvo (50 Hz = معيار SG90)
// ═══════════════════════════════════════════════════════════
const int SERVO_HZ   = 50;   // تردد PWM بالهرتز
const int SERVO_BITS = 16;   // دقة PWM (16-bit)

// ═══════════════════════════════════════════════════════════
// Neutral — زوايا الوقوف بعد المعايرة (عدّلها حسب روبوتك)
// FL: 0=أمام  90=وقوف  180=خلف | FR معكوس في المشي
// ═══════════════════════════════════════════════════════════
const int NEUTRAL_FL = 95;
const int NEUTRAL_FR = 85;
const int NEUTRAL_RL = 110;
const int NEUTRAL_RR = 75;

// ═══════════════════════════════════════════════════════════
// المشي — حجم الخطوة والتوقيت
// ═══════════════════════════════════════════════════════════
const int FWD_DELTA  = 20;  // كم درجة تتحرك كل رجل في خطوة واحدة
const int MOVE_MS    = 0;   // تأخير بين كل درجة أثناء الحركة (0=أسرع)
const int PAUSE_MS   = 50;  // توقف قصير بين مرحلتي المشي

// ═══════════════════════════════════════════════════════════
// المصافحة — زوايا التلويح (3 دورات ثم رجوع neutral)
// FL: 90→0→(50↔0)×3 | FR: 90→180→(130↔180)×3
// ═══════════════════════════════════════════════════════════
const int WAVE_HOME    = 90;
const int WAVE_FL_LOW  = 0;
const int WAVE_FL_MID  = 50;
const int WAVE_FR_HIGH = 180;
const int WAVE_FR_MID  = 130;
const int WAVE_COUNT   = 3;    // عدد دورات التلويح
const int WAVE_PAUSE   = 100;  // ms بين حركات التلويح

// ═══════════════════════════════════════════════════════════
// الجلس — زوايا اختيارية (أمر j)
// ═══════════════════════════════════════════════════════════
const int SIT_FRONT_FL = 90;
const int SIT_FRONT_FR = 90;
const int SIT_REAR_RL  = 90;
const int SIT_REAR_RR  = 90;

// ═══════════════════════════════════════════════════════════
// متغيرات — الزاوية الحالية لكل رجل (يتتبعها الكود)
// ═══════════════════════════════════════════════════════════
int posFL = NEUTRAL_FL;
int posFR = NEUTRAL_FR;
int posRL = NEUTRAL_RL;
int posRR = NEUTRAL_RR;

WiFiClient espClient;
PubSubClient mqtt(espClient);

// ─── تحويل زاوية 0–180° إلى قيمة duty لـ LEDC ───
// SG90 يتوقع نبضة 500µs (0°) إلى 2500µs (180°)
uint32_t angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  return (map(angle, 0, 180, 500, 2500) * 65535UL) / 20000;
}

// ─── كتابة زاوية على سيرvo واحد ───
void writeServo(int pin, int angle) {
  ledcWrite(pin, angleToDuty(angle));
}

// ─── تهيئة قناة LEDC لرجل واحدة ───
// مهم: Serial.printf("GPIO%d %s", pin, name) — ترتيب الوسائط صح!
bool attachLeg(int pin, const char* name) {
  bool ok = ledcAttach(pin, SERVO_HZ, SERVO_BITS);
  Serial.printf("GPIO%d %s -> %s\n", pin, name, ok ? "OK" : "FAIL");
  delay(100);
  return ok;
}

// ─── تحريك رجل واحدة درجة درجة (سلاسة) ───
void moveOneLeg(int pin, int& pos, int target) {
  target = constrain(target, 0, 180);
  Serial.printf("Leg GPIO%d  %d -> %d\n", pin, pos, target);
  while (pos != target) {
    if (pos < target) pos++; else pos--;
    writeServo(pin, pos);
    delay(25);
  }
}

// ─── اختبار FL فقط (أمر t) ───
void testFL() {
  Serial.println("=== FL TEST IO16 ===");
  moveOneLeg(PIN_FL, posFL, 65);
  delay(400);
  moveOneLeg(PIN_FL, posFL, 115);
  delay(400);
  moveOneLeg(PIN_FL, posFL, NEUTRAL_FL);
  Serial.println("=== FL TEST DONE ===");
}

// ─── تحريك الأرجل الأربع معاً (متزامن) ───
void moveCoordinated(int tgtFL, int tgtFR, int tgtRL, int tgtRR) {
  tgtFL = constrain(tgtFL, 0, 180);
  tgtFR = constrain(tgtFR, 0, 180);
  tgtRL = constrain(tgtRL, 0, 180);
  tgtRR = constrain(tgtRR, 0, 180);

  while (posFL != tgtFL || posFR != tgtFR || posRL != tgtRL || posRR != tgtRR) {
    if (posFL < tgtFL) posFL++; else if (posFL > tgtFL) posFL--;
    if (posFR < tgtFR) posFR++; else if (posFR > tgtFR) posFR--;
    if (posRL < tgtRL) posRL++; else if (posRL > tgtRL) posRL--;
    if (posRR < tgtRR) posRR++; else if (posRR > tgtRR) posRR--;

    writeServo(PIN_FL, posFL);
    writeServo(PIN_FR, posFR);
    writeServo(PIN_RL, posRL);
    writeServo(PIN_RR, posRR);
    delay(MOVE_MS);
  }
}

// ─── رجوع كل الأرجل لوضع الوقوف ───
void goNeutralAll() {
  moveCoordinated(NEUTRAL_FL, NEUTRAL_FR, NEUTRAL_RL, NEUTRAL_RR);
}

// ─── دوال مساعدة — حساب زاوية الهدف مع DELTA ───
int tgtFwdPlus(int pos)  { return constrain(pos + FWD_DELTA, 0, 180); }
int tgtBackPlus(int pos) { return constrain(pos - FWD_DELTA, 0, 180); }
int tgtFwdMinus(int pos) { return constrain(pos - FWD_DELTA, 0, 180); }
int tgtBackMinus(int pos){ return constrain(pos + FWD_DELTA, 0, 180); }

// ─── اتجاه كل رجل في المشي — FR معكوس عن FL ───
int fwdFL(int p) { return tgtFwdMinus(p); }
int fwdFR(int p) { return tgtFwdPlus(p); }
int fwdRL(int p) { return tgtFwdMinus(p); }
int fwdRR(int p) { return tgtFwdPlus(p); }

int backFL(int p) { return tgtBackMinus(p); }
int backFR(int p) { return tgtBackPlus(p); }
int backRL(int p) { return tgtBackMinus(p); }
int backRR(int p) { return tgtBackPlus(p); }

// ─── خطوة للأمام — زوجان قطريان ───
void stepForward() {
  Serial.printf("FWD-1 FL+RR  FL %d->%d\n", posFL, fwdFL(posFL));
  moveCoordinated(fwdFL(posFL), posFR, posRL, fwdRR(posRR));
  delay(PAUSE_MS);
  goNeutralAll();

  Serial.println("FWD-2 FR+RL");
  moveCoordinated(posFL, fwdFR(posFR), fwdRL(posRL), posRR);
  delay(PAUSE_MS);
  goNeutralAll();
}

// ─── خطوة للخلف — نفس الأزواج، زوايا معكوسة ───
void stepBackward() {
  Serial.printf("BACK-1 FL+RR  FL %d->%d\n", posFL, backFL(posFL));
  moveCoordinated(backFL(posFL), posFR, posRL, backRR(posRR));
  delay(PAUSE_MS);
  goNeutralAll();

  Serial.println("BACK-2 FR+RL");
  moveCoordinated(posFL, backFR(posFR), backRL(posRL), posRR);
  delay(PAUSE_MS);
  goNeutralAll();
}

// ─── وضع الجلوس (أمر j) ───
void sitPose() {
  moveCoordinated(SIT_FRONT_FL, SIT_FRONT_FR, SIT_REAR_RL, SIT_REAR_RR);
}

// ─── مصافحة يسار — رجل FL ───
void waveLeft() {
  Serial.println("WAVE FL — 90->0->50->0 x3");
  moveOneLeg(PIN_FL, posFL, WAVE_HOME);
  moveOneLeg(PIN_FL, posFL, WAVE_FL_LOW);
  for (int i = 0; i < WAVE_COUNT; i++) {
    moveOneLeg(PIN_FL, posFL, WAVE_FL_MID);
    delay(WAVE_PAUSE);
    moveOneLeg(PIN_FL, posFL, WAVE_FL_LOW);
    delay(WAVE_PAUSE);
  }
  moveOneLeg(PIN_FL, posFL, NEUTRAL_FL);
}

// ─── مصافحة يمين — رجل FR ───
void waveRight() {
  Serial.println("WAVE FR — 90->180->130 x3");
  moveOneLeg(PIN_FR, posFR, WAVE_HOME);
  moveOneLeg(PIN_FR, posFR, WAVE_FR_HIGH);
  for (int i = 0; i < WAVE_COUNT; i++) {
    moveOneLeg(PIN_FR, posFR, WAVE_FR_MID);
    delay(WAVE_PAUSE);
    moveOneLeg(PIN_FR, posFR, WAVE_FR_HIGH);
    delay(WAVE_PAUSE);
  }
  moveOneLeg(PIN_FR, posFR, NEUTRAL_FR);
}

// ─── تنفيذ الأمر الواصل من MQTT ───
void executeCommand(const char* cmd) {
  if (strcmp(cmd, "f") == 0) stepForward();
  else if (strcmp(cmd, "b") == 0 || strcmp(cmd, "l") == 0) stepBackward();
  else if (strcmp(cmd, "S") == 0) goNeutralAll();
  else if (strcmp(cmd, "j") == 0) sitPose();
  else if (strcmp(cmd, "wL") == 0) waveLeft();
  else if (strcmp(cmd, "wR") == 0) waveRight();
  else if (strcmp(cmd, "t") == 0) testFL();
}

// ─── Callback — يُستدعى عند وصول رسالة MQTT ───
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (length >= 16) length = 15;
  char cmd[16];
  memcpy(cmd, payload, length);
  cmd[length] = '\0';
  if (length == 0) return;
  Serial.print(">>> MQTT: ");
  Serial.println(cmd);
  executeCommand(cmd);
}

// ─── الاتصال بـ WiFi (يعيد المحاولة تلقائياً في loop) ───
void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  for (int i = 0; i < 40 && WiFi.status() != WL_CONNECTED; i++) delay(500);
}

// ─── الاتصال بـ MQTT والاشتراك في Topic ───
void connectMQTT() {
  if (!WiFi.isConnected() || mqtt.connected()) return;
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  mqtt.setBufferSize(256);
  char id[20];
  snprintf(id, sizeof(id), "ESP32Dog_%04X", random(0xffff));
  if (mqtt.connect(id)) mqtt.subscribe(MQTT_TOPIC);
}

// ─── setup — يُشغّل مرة واحدة عند التشغيل ───
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n=== RoboDog Diagonal ===");

  attachLeg(PIN_FL, "FL");
  attachLeg(PIN_FR, "FR");
  attachLeg(PIN_RL, "RL");
  attachLeg(PIN_RR, "RR");

  goNeutralAll();
  connectWiFi();
  connectMQTT();
  Serial.println("Ready");
}

// ─── loop — يبقى يفحص WiFi/MMQTT ويستقبل الأوامر ───
void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (WiFi.status() == WL_CONNECTED && !mqtt.connected()) connectMQTT();
  mqtt.loop();
  delay(10);
}
