# شرح الكود — Firmware و Gait

> **الملف الرئيسي:** [`esp32/robodog_mqtt/robodog_mqtt.ino`](../../esp32/robodog_mqtt/robodog_mqtt.ino)

---

## 1. Pins — توصيل GPIO

| رجل | GPIO | الاختصار | ماذا يعني |
|-----|------|----------|-----------|
| FL | **16** | Front Left | أمام يسار |
| FR | **4** | Front Right | أمام يمين — زوايا **معكوسة** عن FL |
| RL | **25** | Rear Left | خلف يسار |
| RR | **22** | Rear Right | خلف يمين |

> **IO27 معطّل** — لا تستخدمه (مشكلة hardware سابقة).

**سلك السيرفو (3 أسلاك):**

| لون شائع | الوظيفة | يوصل على |
|----------|---------|----------|
| برتقالي/أصفر | Signal (PWM) | GPIO المناسب |
| أحمر | VCC (+5V) | 5V (USB أو مصدر خارجي) |
| بني/أسود | GND | GND مشترك مع ESP32 |

---

## 2. Neutral — وضع الوقوف

| رجل | زاوية | لماذا مختلفة؟ |
|-----|-------|---------------|
| FL | 95 | كل رجل تركيبها مختلف — المعايرة تحدد القيمة |
| FR | 85 | FR **معكوس** — نفس «وقوف» فيزيائي ≠ نفس رقم |
| RL | 110 | |
| RR | 75 | |

**Neutral** = الزاوية اللي الروبوت **مستقر** — لا يسقط ولا يرفع جسمه.

**كيف تعدّلها:**
1. ارفع [`robodog_calibrate`](../../esp32/robodog_calibrate/)
2. اضبط من Serial Monitor
3. انسخ القيم إلى `NEUTRAL_*` في `robodog_mqtt.ino`

---

## 3. المشي — Diagonal Gait

### لماذا diagonal؟

| | |
|--|--|
| **الفكرة** | تحريك **قطرين** مت opposite — FL+RR ثم FR+RL |
| **الفائدة** | ثبات أفضل من تحريك 4 أرجل مع بعض |
| **الطاقة** | **2 سيرفو** فقط في كل مرحلة — USB يكفي غالبًا للاختبار |

### خطوة للأمام (`f`)

```
مرحلة 1:  FL −20°  +  RR −20°  →  رجوع neutral
مرحلة 2:  FR +20°  +  RL +20°  →  رجوع neutral
```

> **FR معكوس:** أمام = **+20** لـ FR بينما FL = **−20** — لأن السيرفو مركّب باتجاه معاكس.

### خطوة للخلف (`b`)

نفس الأزواج — الزوايا **معكوسة** (FL +20 · FR −20).

### ثوابت التعديل

```cpp
const int FWD_DELTA  = 20;   // حجم الخطوة بالدرجات — زِدها = خطوة أكبر
const int MOVE_MS    = 0;    // تأخير بين كل درجة (0 = أسرع)
const int PAUSE_MS   = 50;   // توقف بين المرحلتين
```

| الثابت | ماذا يغيّر |
|--------|-----------|
| `FWD_DELTA` | مسافة الخطوة — جرّب 15–30 |
| `MOVE_MS` | سلاسة الحركة — أعلى = أبطأ |
| `PAUSE_MS` | استقرار بين المرحلتين |

---

## 4. بطء المشي وعدم الـ grip

| النقطة | الشرح |
|--------|-------|
| كل `f` | **خطوة واحدة** — مو مشي مستمر |
| لا grip | أطراف الأقدام **بدون rubber/claws** — تنزلق على الأرض |
| من اللوحة | اضغط **↑ أمام عدة مرات** للمسافة |
| **مو عطل** | سلوك متوقع — راجع [README §11](README.md#11-️-ملاحظة-مهمة--بطء-المشي-وعدم-ال-grip) |

---

## 5. المصافحة / التلويح

| أمر | الرجل | المسار | الدورات |
|-----|-------|--------|---------|
| `wL` | FL | 90° → 0° → (50° ↔ 0°) × 3 → neutral | 3 |
| `wR` | FR | 90° → 180° → (130° ↔ 180°) × 3 → neutral | 3 |

**لماذا FR مختلف؟** السيرفو الأمامي الأيمن مركّب **معكوس** — المسار 90→180 بدل 90→0.

```cpp
const int WAVE_COUNT = 3;     // عدد دورات التلويح
const int WAVE_PAUSE = 100;   // ms بين الحركات
```

---

## 6. أوامر MQTT — جدول كامل

| حرف | الدالة | ماذا يحدث |
|-----|--------|-----------|
| `f` | `stepForward()` | خطوة أمام (gait) |
| `b` | `stepBackward()` | خطوة خلف |
| `S` | `standNeutral()` | رجوع كل الأرجل neutral |
| `wL` | `waveLeft()` | تلويح FL |
| `wR` | `waveRight()` | تلويح FR |
| `j` | `sitDown()` | جلس (اختياري) |

**Topic:** `smartmethods/robodog/command` — حرف **واحد** فقط في كل رسالة.

---

## 7. تدفق الكود (من MQTT إلى السيرفو)

```
mqttCallback(payload)
    ↓
executeCommand('f')
    ↓
stepForward()
    ↓
movePair(FL, RR, delta)  →  standNeutral()
    ↓
movePair(FR, RL, delta)  →  standNeutral()
    ↓
writeServo(pin, angle)  →  ledcWrite(pin, duty)
```

---

## 8. Sketches مساعدة

| المجلد | متى |
|--------|-----|
| `servo_one_io16` | أول اختبار — GPIO16 |
| `robodog_calibrate` | ضبط neutral |
| `robodog_test` | 4 أرجل من Serial |
| `robodog_mqtt` | **النهائي** |

> **شرح مفصل لكل sketch:** [`06-HELPER-SKETCHES.md`](06-HELPER-SKETCHES.md)

---

## 9. Serial Monitor — ماذا تتوقع

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

عند أمر من اللوحة:
```
>>> MQTT: f
FWD-1 FL+RR
FWD-2 FR+RL
```
