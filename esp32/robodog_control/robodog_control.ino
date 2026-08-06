/*
 * Robot Dog — ESP32 HTTP Client (XAMPP LOCAL) — نسخة قديمة
 * Smart Methods — BodyV2
 *
 * ⚠️ هذا Sketch للاختبار المحلي مع XAMPP — المشروع النهائي يستخدم MQTT:
 *    esp32/robodog_mqtt/robodog_mqtt.ino
 *
 * Board: ESP32 Dev Module | Monitor: 115200
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>

// ═══ WiFi — ضع بيانات شبكتك ═══
const char* WIFI_SSID = "YOUR_WIFI_NAME";      // اسم شبكة WiFi 2.4GHz
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";  // كلمة سر WiFi

// ═══ XAMPP على اللابتوب (نفس شبكة ESP32 والجوال) ═══
// غيّر IP إلى IP اللابتوب من cmd: ipconfig
const char* SERVER_URL = "http://192.168.x.x/h/get_state.php";

const unsigned long POLL_MS = 2000;       // كل كم ms يسأل السيرفر
const unsigned long ERROR_LOG_MS = 10000; // تقليل تكرار رسائل الخطأ

WiFiClient client;
String lastCommand = "";
unsigned long lastErrorLog = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("=== Robot Dog ESP32 — XAMPP LOCAL ===");
  connectWiFi();
  Serial.println("Server: " + String(SERVER_URL));
  Serial.println("Waiting for commands...");
  Serial.println();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  static unsigned long lastPoll = 0;
  if (millis() - lastPoll >= POLL_MS) {
    lastPoll = millis();
    fetchCommand();
  }
}

// ─── الاتصال بـ WiFi ───
void connectWiFi() {
  Serial.print("WiFi ");
  Serial.print(WIFI_SSID);
  Serial.print(" ... ");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 40) {
    delay(500);
    Serial.print(".");
    tries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("OK — IP: " + WiFi.localIP().toString());
  } else {
    Serial.println();
    Serial.println("FAILED — check SSID / password / signal");
  }
}

// ─── طباعة خطأ كل 10 ثواني فقط (بدون spam) ───
void logErrorThrottled(const String& msg) {
  if (millis() - lastErrorLog >= ERROR_LOG_MS) {
    lastErrorLog = millis();
    Serial.println("[!] " + msg);
  }
}

// ─── جلب آخر أمر من get_state.php ───
void fetchCommand() {
  HTTPClient http;

  if (!http.begin(client, SERVER_URL)) {
    logErrorThrottled("HTTP begin failed");
    return;
  }

  http.setTimeout(8000);
  http.addHeader("Accept", "application/json");

  int code = http.GET();

  if (code <= 0) {
    logErrorThrottled("HTTP error: " + http.errorToString(code));
    http.end();
    return;
  }

  if (code != 200) {
    logErrorThrottled("HTTP code: " + String(code));
    http.end();
    return;
  }

  String body = http.getString();
  http.end();

  if (body.indexOf("\"command\"") < 0) {
    logErrorThrottled("Not JSON — XAMPP running? Apache+MySQL green?");
    return;
  }

  String cmd = parseCommand(body);
  if (cmd.length() == 0) {
    logErrorThrottled("Empty command in JSON");
    return;
  }

  if (cmd != lastCommand) {
    lastCommand = cmd;
    Serial.println(">>> NEW COMMAND: " + cmd + " (" + commandLabel(cmd) + ")");
  }
}

// ─── استخراج "command" من JSON ───
String parseCommand(const String& json) {
  int key = json.indexOf("\"command\"");
  if (key < 0) return "";

  int colon = json.indexOf(':', key);
  int q1 = json.indexOf('"', colon + 1);
  int q2 = json.indexOf('"', q1 + 1);

  if (q1 < 0 || q2 < 0) return "";
  return json.substring(q1 + 1, q2);
}

// ─── ترجمة الحرف لاسم مقروء ───
String commandLabel(const String& c) {
  if (c == "f") return "forward";
  if (c == "b") return "backward";
  if (c == "l") return "left";
  if (c == "r") return "right";
  if (c == "S") return "stop";
  if (c == "j") return "sit";
  return "unknown";
}
