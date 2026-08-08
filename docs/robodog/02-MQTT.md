# MQTT — RoboDog (شرح كامل)

> **MQTT** = بروتوكول رسائل خفيف — يربط **اللوحة** (جوال/لابتوب) بـ **ESP32** عبر **إنternet** — **بدون سيرفر خاص**.

---

## 0. ما هو MQTT؟ (بلغة بسيطة)

| المصطلح | الشرح |
|---------|-------|
| **Broker** | «صندوق بريد» على الإنترنت — يستقبل رسائل ويوصلها |
| **Topic** | «عنوان» الرسالة — مثل `smartmethods/robodog/command` |
| **Publish** | إرسال رسالة (اللوحة ترسل `f`) |
| **Subscribe** | الاشتراك لاستقبال رسائل (ESP32 يسمع على نفس Topic) |

**لماذا MQTT؟**
- اللوحة على **InfinityFree** والESP32 على **WiFi البيت** — ما يحتاجون نفس الشبكة
- Broker **مجاني** (`broker.hivemq.com`) — ما تحتاج AWS أو سيرفر

---

## 1. المخطط

```
[جوال / لابتوب]  ──publish──►  broker.hivemq.com  ──subscribe──►  ESP32  ──►  Servos
     web/h/*.html                  :8884/wss (لوحة)
                                   :1883     (ESP32)
```

| الجهاز | الدور | نوع الاتصال | لماذا منفذ مختلف؟ |
|--------|-------|-------------|-------------------|
| **اللوحة** | ترسل أمر (publish) | WSS — port **8884** | المتصفح يحتاج **SSL** (HTTPS) |
| **ESP32** | يستقبل (subscribe) | TCP — port **1883** | Arduino يستخدم TCP عادي |
| **Broker** | وسيط عام | `broker.hivemq.com` | مجاني — للتعلم والاختبار |

---

## 2. الإعدادات (لازم تطابق في الكود واللوحة)

| | القيمة | ماذا يعني |
|--|--------|-----------|
| Broker | `broker.hivemq.com` | عنوان السيرفر |
| Port ESP32 | `1883` | TCP بدون SSL |
| Port Web (WSS) | `8884` | WebSocket + SSL |
| Topic | `smartmethods/robodog/command` | «قناة» الأوامر — **لا تغيّرها** إلا في الاثنين |

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

> **تحذير:** لا تغيّر Topic في `.ino` فقط — اللوحة لن تصل. **وحّد القيم في الاثنين.**

---

## 3. خطوات ربط MQTT (بالترتيب)

### الخطوة 1 — ESP32 (مرة واحدة)

1. عدّل **WiFi** في `robodog_mqtt.ino`:
   ```cpp
   const char* WIFI_SSID = "YOUR_WIFI_NAME";      // اسم شبكة 2.4 GHz
   const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";  // كلمة السر
   ```
   > **لماذا 2.4 GHz؟** ESP32 **لا يدعم** WiFi 5 GHz — فقط 2.4.

2. **Upload** الكود على ESP32 (Board: ESP32 Dev Module)

3. افتح **Serial Monitor** → **115200 baud**

4. انتظر:
   ```
   === RoboDog Diagonal ===
   GPIO16 FL -> OK
   GPIO4 FR -> OK
   GPIO25 RL -> OK
   GPIO22 RR -> OK
   WiFi connected
   MQTT connected
   Ready
   ```

5. ESP32 **يتصل تلقائيًا** — ما تحتاج تضغط شي في Broker

---

### الخطوة 2 — لوحة التحكم (جوال أو لابتوب)

1. ارفع `web/h/` على **InfinityFree** — راجع [`03-CONTROL-PANEL.md`](03-CONTROL-PANEL.md)

2. افتح في **المتصفح** (Chrome / Safari / Edge):
   ```
   https://webtask1.free.je/h/index.html?v=6
   ```
   > **لماذا HTTPS؟** WSS (MQTT للمتصفh) **لا يعمل** على `file://` أو HTTP عادي.

3. انتظر شارة:
   ```
   MQTT: متصل ✓
   ```
   (تظهر في `manual.html` و `voice.html`)

4. اضغط **↑ أمام** → رسالة: `MQTT وصل للESP32: f (forward)`

| | جوال | لابتوب |
|--|------|--------|
| المتصفح | Chrome / Safari | Chrome / Edge |
| شبكة | WiFi **أو** 4G | إنترنت |
| HTTPS | ✅ مطلوب | ✅ مطلوب |
| `file://` محلي | ❌ MQTT لا يشتغل | ❌ |

> **اللوحة والESP32 ما يحتاجون نفس WiFi** — الاتصال عبر Broker على الإنترنت.

---

### الخطوة 3 — تأكد الربط شغال

**من اللوحة:**
- اضغط **↑ أمام** → Serial Monitor: `>>> MQTT: f`

**من Serial Monitor:**
- `FWD-1 FL+RR` ثم `FWD-2 FR+RL`

**إذا ما تحرك — راجع بالترتيب:**
1. ESP32 `Ready` + WiFi متصل؟
2. اللوحة `MQTT: متصل ✓`؟
3. **نفس Topic** في `.ino` و `api.js`؟
4. [`04-TROUBLESHOOTING.md`](04-TROUBLESHOOTING.md)

---

## 4. كيف تشتغل الرسالة (خطوة بخطوة)

```
1. تضغط زر «أمام» في manual.html
       ↓
2. api.js: sendRobotCommand('forward')
       ↓
3. CMD_LETTER['forward'] = 'f'
       ↓
4. mqttClient.publish('smartmethods/robodog/command', 'f')
       ↓
5. broker.hivemq.com يستقبل ويوزّع للمشتركين
       ↓
6. ESP32 (subscribe على نفس Topic) يستقبل 'f'
       ↓
7. mqttCallback() → executeCommand('f') → stepForward()
       ↓
8. السيرفوات تتحرك (FL+RR ثم FR+RL)
```

**لا قاعدة بيانات** — رسالة **فورية** (fire-and-forget).  
**QoS 0** — أسرع؛ إذا فاتت رسالة، اضغط الزر مرة ثانية.

---

## 5. الأوامر — جدول كامل

| رسالة MQTT | من اللوحة (api.js) | الحركة | ملاحظة |
|------------|-------------------|--------|--------|
| `f` | forward | خطوة أمام | اضغط عدة مرات |
| `b` | backward | خطوة خلف | |
| `S` | stop | وقوف neutral | حرف S كبير |
| `wL` | wave_left | مصافحة يسار (FL) | |
| `wR` | wave_right | مصافحة يمين (FR) | سلm / صافح |
| `j` | sit | جلس | اختياري |

---

## 6. اختبار بدون لوحة (MQTT Explorer)

**متى تستخدمه؟** قبل ما تلوم اللوحة — تأكد ESP32 يستقبل.

1. حمّل [MQTT Explorer](http://mqtt-explorer.com/) (مجاني — Windows/Mac/Linux)
2. **Connect:**
   - Host: `broker.hivemq.com`
   - Port: `1883`
   - Protocol: MQTT (TCP)
3. **Publish:**
   - Topic: `smartmethods/robodog/command`
   - Payload: `f` (حرف واحد)
4. Serial Monitor → `>>> MQTT: f`

| النتيجة | المعنى |
|---------|--------|
| Explorer ✅ + اللوحة ❌ | المشكلة في اللوحة / HTTPS / js |
| Explorer ❌ | المشكلة ESP32 / WiFi / Topic |
| Explorer ✅ + تحرك ✅ | كل شي تمام — جرّب اللوحة |

---

## 7. مشاكل MQTT شائعة

| المشكلة | السبب | الحل |
|---------|-------|------|
| «MQTT: غير متصل» | `file://` أو HTTP | InfinityFree **HTTPS** |
| «فشل الاتصال» | لا إنترنت / Firewall | جرّب 4G أو شبكة ثانية |
| ESP32 ما يستقبل | WiFi غلط | SSID/Pass · 2.4 GHz |
| أمر يصل وما يتحرك | أسلاك/معايرة | [`04-TROUBLESHOOTING.md`](04-TROUBLESHOOTING.md) |
| Topic مختلف | تعديل في مكان واحد | وحّد `.ino` + `api.js` |

---

## 8. أمان وخصوصية (مهم للمتابع)

| النقطة | الشرح |
|--------|-------|
| Broker **عام** | أي شخص يعرف Topic يقدر يرسل — **للتعلم فقط** |
| Topic مشترك | `smartmethods/robodog/command` — غيره لـ topic فريد إذا تبي خصوصية |
| WiFi في `.ino` | **لا ترفع** SSID/Password على GitHub |
| HiveMQ مجاني | قد يكون بطيء أحيانًا — طبيعي |

---

## 9. ملخص سريع

```
1. WiFi في robodog_mqtt.ino → Upload → Serial: Ready
2. ارفع web/h/ → InfinityFree → public_html/h/
3. افتح index.html?v=6 → manual → MQTT: متصل ✓
4. اضغط أمام → Serial: >>> MQTT: f → الروبوت يتحرك ✅
```
