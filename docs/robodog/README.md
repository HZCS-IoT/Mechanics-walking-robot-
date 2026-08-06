# RoboDog BodyV2 — دليل كامل (من الألف للياء)

> **Smart Methods · Mechanics · Task 5**  
> كلب رباعي الأرجل (4× SG90) يُتحكم به من الجوال أو اللابتوب عبر **MQTT** → **ESP32** → **Servos**

---

## فهرس المحتويات

1. [نظرة عامة](#1-نظرة-عامة)
2. [المكونات والتشبيك](#2-المكونات-والتشبيك)
3. [رفع كود ESP32](#3-رفع-كود-esp32)
4. [LEDC — لماذا لا Servo.h](#4-ledc--لماذا-لا-servoh)
5. [MQTT — كيف يصل الأمر للروبوت](#5-mqtt--كيف-يصل-الأمر-للروبوت)
6. [لوحة التحكم (Control Panel)](#6-لوحة-التحكم-control-panel)
7. [التحكم بالصوت (عربي + English)](#7-التحكم-بالصوت-عربي--english)
8. [حركة المشي](#8-حركة-المشي)
9. [المصافحة / التلويح](#9-المصافحة--التلويح)
10. [⚠️ ملاحظة مهمة — بطء المشي وعدم الـ grip](#10-️-ملاحظة-مهمة--بطء-المشي-وعدم-ال-grip)
11. [🎬 فيديوهات YouTube](#11-فيديوهات-youtube)
12. [المشاكل والحلول](#12-المشاكل-والحلول)
13. [هيكل الملفات في المستودع](#13-هيكل-الملفات-في-المستودع)
14. [ملفات تفصيلية إضافية](#14-ملفات-تفصيلية-إضافية)

---

## 1. نظرة عامة

### ماذا يفعل المشروع؟

```
[جوال / لابتوب]  →  لوحة تحكم (HTML)  →  MQTT Broker  →  ESP32  →  4 سيرvo
```

| الطبقة | التقنية |
|--------|---------|
| التحكم | صفحة ويب (أزرار + ميكروفون) |
| الاتصال | MQTT عبر `broker.hivemq.com` |
| المتحكم | ESP32 |
| المحركات | 4× SG90 عبر LEDC (PWM) |

### ما الذي يستطيع الروبوت فعله؟

| الحركة | أمر MQTT | ملاحظة |
|--------|----------|--------|
| خطوة للأمام | `f` | اضغط **عدة مرات** للمسافة |
| خطوة للخلف | `b` | خطوة واحدة لكل أمر |
| وقوف | `S` | رجوع لوضع neutral |
| مصافحة يسار | `wL` | رجل FL |
| مصافحة يمين | `wR` | رجل FR |
| جلس | `j` | اختياري |

> **لا يوجد** يمين/يسار كحركة مشي — فقط أمام/خلف + مصافحة.

---

## 2. المكونات والتشبيك

### المكونات

| # | القطعة |
|---|--------|
| 1 | ESP32 |
| 2 | Breadboard + أسلاك |
| 3 | 4× Servo SG90 (FL, FR, RL, RR) |
| 4 | مصدر طاقة مناسب للسيرvo (USB قد لا يكفي أثناء الحركة) |
| 5 | WiFi (نفس شبكة ESP32 أو إنترنت للوحة) |

### توصيل السيرvo على ESP32

| رجل | GPIO | ملاحظة |
|-----|------|--------|
| **FL** (Front Left) | **16** | أمام يسار |
| **FR** (Front Right) | **4** | أمام يمين — زوايا **معكوسة** عن FL |
| **RL** (Rear Left) | **25** | خلف يسار |
| **RR** (Rear Right) | **22** | خلف يمين |

> **IO27 معطّل** على لوحتك — لا تستخدمه.

### فيديو التشبيك

🎬 [طريقة تشبيك الروبوت في البريد بورد والـ ESP32](https://youtu.be/DsWuTCv1QBQ)

---

## 3. رفع كود ESP32

### الملف الرئيسي

[`esp32/robodog_mqtt/robodog_mqtt.ino`](../../esp32/robodog_mqtt/robodog_mqtt.ino)

### خطوات Arduino IDE

1. ثبّت **ESP32 Board Package** (Core 3.x).
2. افتح `robodog_mqtt.ino`.
3. عدّل **WiFi** في أعلى `robodog_mqtt.ino`:
   ```cpp
   const char* WIFI_SSID = "YOUR_WIFI_NAME";      // اسم شبكة WiFi 2.4GHz
   const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";  // كلمة سر WiFi
   ```
4. اختر Board: **ESP32 Dev Module**.
5. **افصل أسلاك السيرvo** أثناء الرفع إذا حصل reboot loop.
6. Upload → افتح Serial Monitor (**115200**) → انتظر `Ready`.

### زوايا Neutral (وقوف) — بعد المعايرة

| رجل | زاوية |
|-----|-------|
| FL | 95 |
| FR | 85 |
| RL | 110 |
| RR | 75 |

### اتجاهات FL و FR

| | FL | FR |
|--|-----|-----|
| 0° | أمام | — |
| 90° | وقوف | وقوف |
| 180° | خلف | — |
| أمام | −DELTA | **+DELTA** (معكوس) |
| خلف | +DELTA | **−DELTA** |

```cpp
const int FWD_DELTA  = 20;   // حجم الخطوة
const int MOVE_MS    = 0;    // سرعة الحركة بين درجات الزاوية
const int PAUSE_MS   = 50;   // توقف بين مراحل المشي
```

### sketches مساعدة (اختياري)

| المجلد | الغرض |
|--------|--------|
| `robodog_calibrate` | معايرة neutral |
| `robodog_test` | اختبار أرجل |
| `servo_one_io16` | اختبار سيرvo واحد على GPIO16 |
| `robodog_sweep_test` | sweep زوايا |

---

## 4. LEDC — لماذا لا Servo.h

على **ESP32 Core 3.x** مكتبة `Servo.h` تعطي خطأ compile:

```
SOC_LEDC_TIMER_BIT_WIDE_NUM was not declared
```

**الحل:** LEDC المدمج:

```cpp
ledcAttach(pin, 50, 16);   // 50 Hz, 16-bit
ledcWrite(pin, duty);      // duty من زاوية 0–180°
```

تحويل الزاوية إلى PWM (500–2500 µs):

```cpp
uint32_t angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  return (map(angle, 0, 180, 500, 2500) * 65535UL) / 20000;
}
```

> تفاصيل: [05-ESP32-LEDC.md](05-ESP32-LEDC.md)

---

## 5. MQTT — كيف يصل الأمر للروبوت

### المخطط

```
[جوال / لابتوب]  ──publish──►  broker.hivemq.com  ──►  ESP32  ──►  4 Servos
     web/h/*.html                  :8884/wss (متصفح)
                                   :1883 (ESP32)
```

### الإعدادات

| | القيمة |
|--|--------|
| Broker | `broker.hivemq.com` |
| Port ESP32 | `1883` |
| Port Web (WSS) | `8884` |
| Topic | `smartmethods/robodog/command` |

### الأوامر

| رسالة | الحركة |
|-------|--------|
| `f` | خطوة أمام |
| `b` | خطوة خلف |
| `S` | وقوف |
| `wL` | مصافحة يسار |
| `wR` | مصافحة يمين |
| `j` | جلس |

### آلية العمل

1. اللوحة **تنشر (publish)** حرفًا واحدًا على الـ Topic.
2. ESP32 **مشترك (subscribe)** على نفس الـ Topic.
3. عند وصول رسالة → `executeCommand()` ينفّذ الحركة.

**لا قاعدة بيانات مطلوبة** — رسالة فورية فقط.

### اختبار بدون لوحة (MQTT Explorer)

1. اتصل بـ `broker.hivemq.com:1883`
2. Publish → Topic: `smartmethods/robodog/command` → Message: `f`

> تفاصيل: [02-MQTT.md](02-MQTT.md)

---

## 6. لوحة التحكم (Control Panel)

### الملفات (محليًا)

```
web/h/
├── index.html      ← اختيار: يدوي أو صوت
├── manual.html     ← أزرار: أمام، خلف، وقوف، تلويح
├── voice.html      ← ميكروفون عربي/إنجليزي
├── css/style.css
└── js/
    ├── api.js              ← MQTT + أوامر
    └── voice-commands.js   ← قواعد الصوت
```

### رفع على InfinityFree

1. ادخل **File Manager** في InfinityFree.
2. افتح مجلد **`h/`** على الاستضافة.
3. ارفع **كل** محتويات `web/h/` (HTML + css + js).
4. افتح في المتصفح (حدّث الكاش):

| الصفحة | الرابط |
|--------|--------|
| الرئيسية | `https://webtask1.free.je/h/index.html?v=6` |
| يدوي | `https://webtask1.free.je/h/manual.html?v=6` |
| صوت | `https://webtask1.free.je/h/voice.html?v=6` |

> `?v=6` يجبر المتصفح على تحميل النسخة الجديدة.

### الأزرار (manual.html)

| زر | MQTT |
|----|------|
| ↑ أمام | `f` |
| ↓ خلف | `b` |
| ■ وقوف | `S` |
| 👋 تلويح | `wL` أو `wR` حسب الاختيار |

### من الجوال

1. Chrome أو Safari.
2. WiFi أو 4G.
3. انتظر **MQTT: متصل ✓**.
4. اضغط الأزرار — للأمام **عدة مرات** للمسافة.

---

## 7. التحكم بالصوت (عربي + English)

### الاستخدام

1. افتح `voice.html`.
2. اختر **عربي** أو **English**.
3. اضغط 🎤 → انتظر **أحمر** → تكلم.
4. يتوقف تلقائيًا بعد السكوت.

### أوامر عربي 🇸🇦

| قل | النتيجة |
|----|---------|
| للأمام / قدام | `f` |
| للخلف / ورا | `b` |
| قف / توقف | `S` |
| **سلم / صافح / مرحبا** | `wR` (مصافحة يمين) |
| صافح يسار / تلويح يسار | `wL` |
| اجلس | `j` |

### English 🇬🇧

| Say | Result |
|-----|--------|
| forward / backward | `f` / `b` |
| stop / stand | `S` |
| **shake hands / wave right / hi** | `wR` |
| wave left | `wL` |
| sit | `j` |

> للإنجليزي اختر **English** — وإلا المتصفح قد يكتب الكلمات بحروف عربية غلط.

---

## 8. حركة المشي

### Gait (Diagonal)

**أمام وخلف** — نفس أزواج الأرجل، زوايا معكوسة:

```
خطوة 1:  FL + RR  →  neutral
خطوة 2:  FR + RL  →  neutral
```

| | FL | FR |
|--|-----|-----|
| أمام (`f`) | −20 | +20 |
| خلف (`b`) | +20 | −20 |

🎬 [شرح مبسط لحركة السيرvo لأرجل الروبوت](https://youtube.com/shorts/KG-kgXi6Zhg)

---

## 9. المصافحة / التلويح

### يسار — FL (`wL`)

```
90 → 0 → (50 ↔ 0) × 3 → neutral
```

### يمين — FR (`wR`)

```
90 → 180 → (130 ↔ 180) × 3 → neutral
```

🎬 [تجربة المشي والمصافحة مع لوحة التحكم](https://youtube.com/shorts/XNNBhHW6kjc)

---

## 10. ⚠️ ملاحظة مهمة — بطء المشي وعدم ال‑ grip

المشي في هذا النموذج **بطيء** ومقصود **خطوة بخطوة** — ليس مشيًا مستمرًا.

**السبب:** أطراف الأقدام **لا تحتوي على grip** (لا rubber، لا claws)، فالقدم **لا تمسك الأرض** وتنزلق.

| ما يحدث | التفسير |
|---------|---------|
| ضغطة واحدة = خطوة صغيرة | كل `f` = دورة gait واحدة |
| انزلاق | بدون grip لا دفع كافٍ للجسم |
| الحل المؤقت | اضغط **أمام** عدة مرات |
| تحسين مستقبلي | rubber feet / grip pads |

> **سلوك متوقع** — ليس عطلًا في الكود أو MQTT.

---

## 11. فيديوهات YouTube

| # | الوصف | الرابط |
|---|--------|--------|
| 1 | شرح مبسط لحركة السيرvo لأرجل الروبوت | [Shorts ↗](https://youtube.com/shorts/KG-kgXi6Zhg) |
| 2 | تجربة لوحة التحكم — المشي والمصافحة | [Shorts ↗](https://youtube.com/shorts/XNNBhHW6kjc) |
| 3 | تشبيك الروبوت — Breadboard + ESP32 | [YouTube ↗](https://youtu.be/DsWuTCv1QBQ) |

---

## 12. المشاكل والحلول

| المشكلة | الحل |
|---------|------|
| `Servo.h` compile error | استخدم **LEDC** — [05-ESP32-LEDC.md](05-ESP32-LEDC.md) |
| `Serial.printf(name, pin)` crash | الصح: `printf("GPIO%d %s", pin, name)` |
| Reboot loop | خطأ printf أو طاقة — افصل السيرvo عند Upload |
| `undefined setup/loop` | لا تنسخ رسالة الخطأ داخل `.ino` |
| FL لا تتحرك | FL=**16** FR=**4** (تبديل أسلاك أمامية) |
| IO27 لا يعمل | معطّل — لا تستخدمه |
| COM busy | أغلق Serial Monitor قبل Upload |
| Panel غير متصل | HTTPS/WSS — InfinityFree |
| المشي بطي | طبيعي — **لا grip**؛ اضغط `f` عدة مرات |
| الصوت لا يعمل | Chrome/Edge + HTTPS + إذن ميكروفون |

> تفاصيل: [04-TROUBLESHOOTING.md](04-TROUBLESHOOTING.md)

---

## 13. هيكل الملفات في المستودع

```
Mechanics/
├── esp32/
│   └── robodog_mqtt/
│       └── robodog_mqtt.ino    ← Firmware النهائي
├── web/
│   └── h/                      ← Control Panel (ارفعها InfinityFree)
│       ├── index.html
│       ├── manual.html
│       ├── voice.html
│       ├── css/style.css
│       └── js/
│           ├── api.js
│           └── voice-commands.js
└── docs/
    └── robodog/
        ├── README.md           ← هذا الملف (دليل A–Z)
        ├── 01-CODE.md
        ├── 02-MQTT.md
        ├── 03-CONTROL-PANEL.md
        ├── 04-TROUBLESHOOTING.md
        └── 05-ESP32-LEDC.md
```

---

## 14. ملفات تفصيلية إضافية

| ملف | المحتوى |
|-----|---------|
| [01-CODE.md](01-CODE.md) | Pins · neutral · gait · ثوابت |
| [02-MQTT.md](02-MQTT.md) | MQTT مفصل + اختبار |
| [03-CONTROL-PANEL.md](03-CONTROL-PANEL.md) | رفع InfinityFree |
| [04-TROUBLESHOOTING.md](04-TROUBLESHOOTING.md) | أخطاء وحلول |
| [05-ESP32-LEDC.md](05-ESP32-LEDC.md) | LEDC بدل Servo.h |

---

## البدء السريع (Checklist)

- [ ] 1. شبّك السيرvo حسب الجدول — [فيديو التشبيك](https://youtu.be/DsWuTCv1QBQ)
- [ ] 2. عدّل WiFi في `robodog_mqtt.ino` وارفع على ESP32
- [ ] 3. Serial Monitor → `Ready` + MQTT متصل
- [ ] 4. ارفع `web/h/` على InfinityFree
- [ ] 5. افتح `manual.html?v=6` → جرّب أمام / مصافحة
- [ ] 6. افتح `voice.html?v=6` → قل **سلم** أو **forward**

---

<p align="center">
  <b>Smart Methods — RoboDog BodyV2</b><br>
  <i>ESP32 · MQTT · LEDC · Control Panel · Voice</i>
</p>
