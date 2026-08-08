# MQTT — RoboDog

> **MQTT** = قناة رسائل بين اللوحة (جوال/لابتوب) والـ ESP32 عبر الإنترنت — **بدون سيرفر خاص**.

---

## المخطط

```
[جوال / لابتوب]  ──publish──►  broker.hivemq.com  ──subscribe──►  ESP32  ──►  Servos
     web/h/*.html                  :8884/wss (لوحة)
                                   :1883     (ESP32)
```

| الجهاز | الدور | نوع الاتصال |
|--------|-------|-----------|
| **اللوحة** | ترسل أمر (publish) | WSS — port **8884** |
| **ESP32** | يستقبل (subscribe) | TCP — port **1883** |
| **Broker** | وسيط عام | `broker.hivemq.com` |

---

## الإعدادات (لازم تطابق في الكود واللوحة)

| | القيمة |
|--|--------|
| Broker | `broker.hivemq.com` |
| Port ESP32 | `1883` |
| Port Web (WSS) | `8884` |
| Topic | `smartmethods/robodog/command` |

**ESP32** — في [`robodog_mqtt.ino`](../../esp32/robodog_mqtt/robodog_mqtt.ino):
```cpp
const char* MQTT_SERVER = "broker.hivemq.com";
const int   MQTT_PORT   = 1883;
const char* MQTT_TOPIC  = "smartmethods/robodog/command";
```

**اللوحة** — في [`web/h/js/api.js`](../../web/h/js/api.js):
```javascript
const MQTT_BROKER = 'wss://broker.hivemq.com:8884/mqtt';
const MQTT_TOPIC  = 'smartmethods/robodog/command';
```

> **لا تغيّر Topic** في جهاز واحد فقط — لازم **الاثنين** نفس القيمة.

---

## 🔌 خطوات ربط MQTT (بالترتيب)

### الخطوة 1 — ESP32 (مرة واحدة)

1. عدّل **WiFi** في `robodog_mqtt.ino`:
   ```cpp
   const char* WIFI_SSID = "YOUR_WIFI_NAME";
   const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
   ```
2. Upload الكود على ESP32
3. افتح **Serial Monitor** → **115200**
4. انتظر:
   ```
   === RoboDog Diagonal ===
   GPIO16 FL -> OK
   ...
   Ready
   ```
5. ESP32 **يتصل تلقائيًا** بـ WiFi ثم MQTT — **ما تحتاج تسوي شيء يدوي**

> ESP32 لازم على **WiFi 2.4GHz** — مو 5GHz.

---

### الخطوة 2 — لوحة التحكم (جوال أو لابتوب)

1. ارفع `web/h/` على **InfinityFree** (HTTPS)
2. افتح في الالمتصفح:
   ```
   https://webtask1.free.je/h/index.html?v=6
   ```
3. انتظر شارة:
   ```
   MQTT: متصل ✓
   ```
4. اضغط أي زر → يظهر `MQTT وصل للESP32: f`

| | جوال | لابتوب |
|--|------|--------|
| المتصفح | Chrome / Safari | Chrome / Edge |
| شبكة | WiFi **أو** 4G | إنترنت |
| HTTPS | ✅ مطلوب | ✅ مطلوب |
| `file://` | ❌ ما يشتغل | ❌ |

> **اللوحة والESP32 ما يحتاجون نفس WiFi** — الاتصال عبر Broker على الإنترنت.

---

### الخطوة 3 — تأكد الربط شغال

**من اللوحة:**
- اضغط **↑ أمام** → Serial Monitor يطبع: `>>> MQTT: f`

**من Serial Monitor:**
- إذا وصل الأمر → `FWD-1 FL+RR` وغيره

**إذا ما تحرك:**
1. تأكد ESP32 `Ready` + WiFi متصل
2. تأكد اللوحة `MQTT: متصل ✓`
3. تأكد **نفس Topic** في الكود واللوحة
4. راجع [04-TROUBLESHOOTING.md](04-TROUBLESHOOTING.md)

---

## كيف يشتغل (تدفق الرسالة)

```
1. تضغط زر «أمام» في manual.html
2. api.js يرسل publish → Topic → رسالة "f"
3. broker.hivemq.com يوصل الرسالة
4. ESP32 (مشترك على نفس Topic) يستقبل "f"
5. mqttCallback() → executeCommand("f") → stepForward()
6. السيرvoات تتحرك
```

**لا قاعدة بيانات** — رسالة فورية فقط.

---

## الأوامر

| رسالة | الحركة |
|-------|--------|
| `f` | خطوة أمام |
| `b` | خطوة خلف |
| `S` | وقوف |
| `wL` | مصافحة يسار |
| `wR` | مصافحة يمين |
| `j` | جلس |

---

## 🧪 اختبار بدون لوحة (MQTT Explorer — لابتوب)

مفيد للتأكد ESP32 يستقبل **قبل** ما تجرّب اللوحة.

1. حمّل [MQTT Explorer](http://mqtt-explorer.com/) (مجاني)
2. **Connect:**
   - Host: `broker.hivemq.com`
   - Port: `1883`
   - Protocol: MQTT
3. **Publish:**
   - Topic: `smartmethods/robodog/command`
   - Message: `f`
4. شوف Serial Monitor → `>>> MQTT: f`

> إذا Explorer يشتغل واللوحة لا → المشكلة في اللوحة/HTTPS.  
> إذا Explorer ما يشتغل → المشكلة ESP32/WiFi/Topic.

---

## ⚠️ مشاكل MQTT شائعة

| المشكلة | السبب | الحل |
|---------|-------|------|
| اللوحة «غير متصل» | `file://` أو HTTP | استخدم **InfinityFree** (HTTPS) |
| ESP32 ما يستقبل | WiFi غلط | راجع SSID/Password |
| الأمر يصل وما يتحرك | معايرة/أسلاك | [04-TROUBLESHOOTING.md](04-TROUBLESHOOTING.md) |
| Topic مختلف | تعديل في مكان واحد | وحّد Topic في `.ino` و `api.js` |

---

## ملخص سريع للمتابع

```
1. WiFi في robodog_mqtt.ino → Upload → Serial: Ready
2. ارفع web/h/ → InfinityFree
3. افتح index.html → MQTT: متصل ✓
4. اضغط أمام → الروبوت يتحرك ✅
```
