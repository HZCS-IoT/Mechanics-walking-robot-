/*
 * Robot Dog — ESP32 + MQTT + 4 Servos
 * Smart Methods — BodyV2 (Diagonal Gait)
 *
 * Forward (f):  1) FL+RR → neutral  →  2) FR+RL → neutral  (عكس زوايا الخلف)
 * Backward (b/l): 1) FL+RR → neutral  →  2) FR+RL → neutral
 * Stand (S):      رجوع neutral
 * Wave: wL=مصافحة يسار (FL)  wR=مصافحة يمين (FR)
 *   FL: 90 → 0 → (50 ↔ 0) × 3 → neutral
 *   FR: 90 → 180 → (130 ↔ 180) × 3 → neutral
 *
 * FL: 0=أمام  90=وقوف  180=خلف
 * FR: معكوس (أمام=115  خلف=65) مع DELTA=25
 * Pins: FL=16  FR=4  RL=25  RR=22
 */

#include <WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "STC1";
const char* WIFI_PASS = "M05490707580";

const char* MQTT_SERVER = "broker.hivemq.com";
const int   MQTT_PORT   = 1883;
const char* MQTT_TOPIC  = "smartmethods/robodog/command";

const int PIN_FL = 16;
const int PIN_FR = 4;
const int PIN_RL = 25;
const int PIN_RR = 22;

const int SERVO_HZ   = 50;
const int SERVO_BITS = 16;

const int NEUTRAL_FL = 95;
const int NEUTRAL_FR = 85;
const int NEUTRAL_RL = 110;
const int NEUTRAL_RR = 75;

const int FWD_DELTA  = 20;
const int MOVE_MS    = 0;
const int PAUSE_MS   = 50;

const int WAVE_HOME    = 90;
const int WAVE_FL_LOW  = 0;
const int WAVE_FL_MID  = 50;
const int WAVE_FR_HIGH = 180;
const int WAVE_FR_MID  = 130;
const int WAVE_COUNT   = 3;
const int WAVE_PAUSE   = 100;

const int SIT_FRONT_FL = 90;
const int SIT_FRONT_FR = 90;
const int SIT_REAR_RL  = 90;
const int SIT_REAR_RR  = 90;

int posFL = NEUTRAL_FL;
int posFR = NEUTRAL_FR;
int posRL = NEUTRAL_RL;
int posRR = NEUTRAL_RR;

WiFiClient espClient;
PubSubClient mqtt(espClient);

uint32_t angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  return (map(angle, 0, 180, 500, 2500) * 65535UL) / 20000;
}

void writeServo(int pin, int angle) {
  ledcWrite(pin, angleToDuty(angle));
}

bool attachLeg(int pin, const char* name) {
  bool ok = ledcAttach(pin, SERVO_HZ, SERVO_BITS);
  Serial.printf("GPIO%d %s -> %s\n", pin, name, ok ? "OK" : "FAIL");
  delay(100);
  return ok;
}

void moveOneLeg(int pin, int& pos, int target) {
  target = constrain(target, 0, 180);
  Serial.printf("Leg GPIO%d  %d -> %d\n", pin, pos, target);
  while (pos != target) {
    if (pos < target) pos++; else pos--;
    writeServo(pin, pos);
    delay(25);
  }
}

void testFL() {
  Serial.println("=== FL TEST IO16 ===");
  moveOneLeg(PIN_FL, posFL, 65);
  delay(400);
  moveOneLeg(PIN_FL, posFL, 115);
  delay(400);
  moveOneLeg(PIN_FL, posFL, NEUTRAL_FL);
  Serial.println("=== FL TEST DONE ===");
}

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

void goNeutralAll() {
  moveCoordinated(NEUTRAL_FL, NEUTRAL_FR, NEUTRAL_RL, NEUTRAL_RR);
}

int tgtFwdPlus(int pos)  { return constrain(pos + FWD_DELTA, 0, 180); }
int tgtBackPlus(int pos) { return constrain(pos - FWD_DELTA, 0, 180); }
int tgtFwdMinus(int pos) { return constrain(pos - FWD_DELTA, 0, 180); }
int tgtBackMinus(int pos){ return constrain(pos + FWD_DELTA, 0, 180); }

int fwdFL(int p) { return tgtFwdMinus(p); }
int fwdFR(int p) { return tgtFwdPlus(p); }
int fwdRL(int p) { return tgtFwdMinus(p); }
int fwdRR(int p) { return tgtFwdPlus(p); }

int backFL(int p) { return tgtBackMinus(p); }
int backFR(int p) { return tgtBackPlus(p); }
int backRL(int p) { return tgtBackMinus(p); }
int backRR(int p) { return tgtBackPlus(p); }

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

void sitPose() {
  moveCoordinated(SIT_FRONT_FL, SIT_FRONT_FR, SIT_REAR_RL, SIT_REAR_RR);
}

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

void executeCommand(const char* cmd) {
  if (strcmp(cmd, "f") == 0) stepForward();
  else if (strcmp(cmd, "b") == 0 || strcmp(cmd, "l") == 0) stepBackward();
  else if (strcmp(cmd, "S") == 0) goNeutralAll();
  else if (strcmp(cmd, "j") == 0) sitPose();
  else if (strcmp(cmd, "wL") == 0) waveLeft();
  else if (strcmp(cmd, "wR") == 0) waveRight();
  else if (strcmp(cmd, "t") == 0) testFL();
}

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

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  for (int i = 0; i < 40 && WiFi.status() != WL_CONNECTED; i++) delay(500);
}

void connectMQTT() {
  if (!WiFi.isConnected() || mqtt.connected()) return;
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  mqtt.setBufferSize(256);
  char id[20];
  snprintf(id, sizeof(id), "ESP32Dog_%04X", random(0xffff));
  if (mqtt.connect(id)) mqtt.subscribe(MQTT_TOPIC);
}

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

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (WiFi.status() == WL_CONNECTED && !mqtt.connected()) connectMQTT();
  mqtt.loop();
  delay(10);
}
