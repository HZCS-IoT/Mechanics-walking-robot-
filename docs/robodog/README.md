# RoboDog BodyV2 — دليل كامل (من الألف للياء)

> **Smart Methods · Mechanics · Task 5**  
> كلب رباعي الأرجل (4× SG90) يُتحكم به من الجوال أو اللابتوب عبر **MQTT** → **ESP32** → **Servos**

---

## فهرس المحتويات

1. [نظرة عامة](#1-نظرة-عامة)
2. [🛠️ تبي تسوي زيي؟ — خطوات التقليد](#2-️-تبي-تسوي-زيي--خطوات-التقليد)
3. [المكونات والتشبيك](#3-المكونات-والتشبيك)
4. [رفع كود ESP32](#4-رفع-كود-esp32)
5. [LEDC — لماذا لا Servo.h](#5-ledc--لماذا-لا-servoh)
6. [MQTT — كيف يصل الأمر للروبوت](#6-mqtt--كيف-يصل-الأمر-للروبوت)
7. [لوحة التحكم (Control Panel)](#7-لوحة-التحكم-control-panel)
8. [التحكم بالصوت (عربي + English)](#8-التحكم-بالصوت-عربي--english)
9. [حركة المشي](#9-حركة-المشي)
10. [المصافحة / التلويح](#10-المصافحة--التلويح)
11. [⚠️ ملاحظة مهمة — بطء المشي وعدم الـ grip](#11-️-ملاحظة-مهمة--بطء-المشي-وعدم-ال-grip)
12. [🎬 فيديوهات تعليمية (للمتابعين)](#12-فيديوهات-تعليمية-للمتابعين)
13. [📦 فيديو تسليم Task Mechanics (منفصل)](#13-فيديو-تسليم-task-mechanics-منفصل)
14. [المشاكل والحلول](#14-المشاكل-والحلول)
15. [هيكل الملفات في المستودع](#15-هيكل-الملفات-في-المستودع)
16. [ملفات تفصيلية إضافية](#16-ملفات-تفصيلية-إضافية)

---

## 1. نظرة عامة

### ماذا يفعل المشروع؟

```
[جوال / لابتوب]  →  لوحة تحكم (HTML)  →  MQTT Broker  →  ESP32  →  4 سيرفو
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

## 2. 🛠️ تبي تسوي زيي؟ — خطوات التقليد

> **اتبع الخطوات بالترتيب** — كل خطوة مربوطة بمجلد أو ملف في هذا المستودع.

### الخطوات السريعة

| # | ماذا تسوي | أين في المستودع | ملاحظة |
|---|-----------|-----------------|--------|
| **1** | Clone المستودع | `Mechanics-walking-robot-` | [الرابط](https://github.com/HZCS-IoT/Mechanics-walking-robot-) |
| **2** | اقرأ الدليل (هذا الملف) | `docs/robodog/README.md` | من الألف للياء |
| **3** | افهم LEDC (لا Servo.h) | [`docs/robodog/05-ESP32-LEDC.md`](05-ESP32-LEDC.md) | مهم قبل الرفع |
| **4** | شبّك الأسلاك | [فيديو YouTube ↗](https://youtu.be/DsWuTCv1QBQ) + [`esp32/robodog_mqtt/`](../../esp32/robodog_mqtt/) | FL=16 FR=4 RL=25 RR=22 |
| **5** | اختبر سيرفو واحد | [`esp32/servo_one_io16/`](../../esp32/servo_one_io16/) | GPIO16 |
| **6** | معايرة neutral | [`esp32/robodog_calibrate/`](../../esp32/robodog_calibrate/) | انسخ القيم لـ mqtt |
| **7** | عدّل WiFi وارفع Firmware | [`esp32/robodog_mqtt/robodog_mqtt.ino`](../../esp32/robodog_mqtt/robodog_mqtt.ino) | `YOUR_WIFI_NAME` |
| **8** | تأكد Serial Monitor | `Ready` على 115200 | MQTT متصل |
| **9** | افهم MQTT | [`docs/robodog/02-MQTT.md`](02-MQTT.md) | Topic + أوامر |
| **10** | ارفع لوحة التحكم | [`web/h/`](../../web/h/) → InfinityFree | [03-CONTROL-PANEL.md](03-CONTROL-PANEL.md) — **جوال أو لابتوب** |
| **11** | جرّب من **الجوال** | `index.html?v=6` → manual أو voice | Chrome/Safari + WiFi أو 4G |
| **12** | جرّب من **اللابتوب** | نفس الرابط في Chrome/Edge | HTTPS مطلوب (InfinityFree) |
| **13** | إذا صار خطأ | [`docs/robodog/04-TROUBLESHOOTING.md`](04-TROUBLESHOOTING.md) | حلول جاهزة |

### مسار المجلدات (بالترتيب)

```
Mechanics/
│
├── docs/robodog/          ← ابدأ هنا (1) اقرأ README + 05-LEDC + 02-MQTT
│
├── esp32/
│   ├── servo_one_io16/    ← (2) اختبار سيرفو واحد
│   ├── robodog_calibrate/ ← (3) معايرة الزوايا
│   └── robodog_mqtt/      ← (4) الكود النهائي — ارفعه على ESP32
│
└── web/h/                 ← (5) ارفعه على InfinityFree
    ├── index.html
    ├── manual.html
    ├── voice.html
    ├── css/
    └── js/
```

### Checklist — علّم ✅ وأنت تمشي

- [ ] **1.** Clone + فتح `docs/robodog/README.md`
- [ ] **2.** قراءة [`05-ESP32-LEDC.md`](05-ESP32-LEDC.md)
- [ ] **3.** تشبيك حسب [فيديو YouTube — Breadboard + ESP32](https://youtu.be/DsWuTCv1QBQ)
- [ ] **4.** رفع `servo_one_io16` → السيرفو يتحرك
- [ ] **5.** رفع `robodog_calibrate` → تعديل neutral → نسخ للـ mqtt
- [ ] **6.** تعديل WiFi في `robodog_mqtt.ino` → Upload
- [ ] **7.** Serial Monitor → `Ready`
- [ ] **8.** رفع `web/h/` كامل على InfinityFree
- [ ] **9.** **جوال:** افتح الرابط → `manual.html` → جرّب `f` · `wR`
- [ ] **10.** **لابتوب:** نفس الرابط في Chrome → `voice.html` → قل **سلم** أو **forward**
- [ ] **11.** إذا مشكلة → [`04-TROUBLESHOOTING.md`](04-TROUBLESHOOTING.md)

---

## 3. المكونات والتشبيك

### المكونات

| # | القطعة |
|---|--------|
| 1 | ESP32 |
| 2 | Breadboard + أسلاك |
| 3 | 4× Servo SG90 (FL, FR, RL, RR) |
| 4 | مصدر طاقة مناسب للسيرفو (USB قد لا يكفي أثناء الحركة) |
| 5 | WiFi 2.4GHz (ESP32 + نفس الشبكة أو إنترنت للوحة) |

### توصيل السيرفو على ESP32

| رجل | GPIO | ملاحظة |
|-----|------|--------|
| **FL** (Front Left) | **16** | أمام يسار |
| **FR** (Front Right) | **4** | أمام يمين — زوايا **معكوسة** عن FL |
| **RL** (Rear Left) | **25** | خلف يسار |
| **RR** (Rear Right) | **22** | خلف يمين |

> **IO27 معطّل** — لا تستخدمه.

### 🎬 فيديو التشبيك (Breadboard + ESP32)

**[YouTube ↗ — طريقة تشبيك الروبوت](https://youtu.be/DsWuTCv1QBQ)**

### ⚡ الطاقة — تشغيل 2 سيرفو فقط (لا يضر)

| نقطة | التفسير |
|------|---------|
| **Gait diagonal** | المشي يحرّك **زوجين فقط** في كل مرحلة: FL+RR ثم FR+RL — **مو الأربعة مع بعض** |
| **USB محدود** | USB اللابتوب قد **ما يكفي** لتشغيل 4 سيرفوات تحت حمل — طبيعي |
| **آمن للاختبار** | تقدر تبدأ بـ **2 سيرفو** موصولين وتختبر — **ما يضر** ESP32 |
| **نحن شغّالين** | المشروع يعمل بـ **2 في كل خطوة** — مو لازم الأربعة يتحركون بنفس اللحظة |
| **تحسين** | مصدر 5V خارجي (2A+) للسيرفوات إذا تبي الأربعة مع بعض لاحقًا |

---

## 4. رفع كود ESP32

### الملف الرئيسي

[`esp32/robodog_mqtt/robodog_mqtt.ino`](../../esp32/robodog_mqtt/robodog_mqtt.ino)

### خطوات Arduino IDE

1. ثبّت **ESP32 Board Package** (Core 3.x).
2. افتح `robodog_mqtt.ino`.
3. عدّل **WiFi** في أعلى الملف:
   ```cpp
   const char* WIFI_SSID = "YOUR_WIFI_NAME";      // اسم شبكة WiFi 2.4GHz
   const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";  // كلمة سر WiFi
   ```
4. Board: **ESP32 Dev Module** | Monitor: **115200**
5. **افصل أسلاك السيرفو** أثناء Upload إذا reboot loop.
6. Upload → انتظر `Ready` في Serial Monitor.

### زوايا Neutral (بعد المعايرة)

| رجل | زاوية |
|-----|-------|
| FL | 95 |
| FR | 85 |
| RL | 110 |
| RR | 75 |

### sketches مساعدة

| المجلد | متى تستخدمه |
|--------|-------------|
| `servo_one_io16` | أول اختبار — سيرفو واحد |
| `robodog_calibrate` | ضبط neutral قبل mqtt |
| `robodog_test` | اختبار أرجل من Serial |
| `robodog_sweep_test` | sweep زوايا |

> شرح الكود: [`01-CODE.md`](01-CODE.md)

---

## 5. LEDC — لماذا لا Servo.h

على **ESP32 Core 3.x** مكتبة `Servo.h` تعطي compile error — استخدم **LEDC**.

```cpp
ledcAttach(pin, 50, 16);
ledcWrite(pin, duty);
```

> تفاصيل: [`05-ESP32-LEDC.md`](05-ESP32-LEDC.md)

---

## 6. MQTT — كيف يصل الأمر للروبوت

```
[جوال] ──publish──► broker.hivemq.com ──► ESP32 ──► Servos
```

| | القيمة |
|--|--------|
| Broker | `broker.hivemq.com` |
| Port ESP32 | `1883` |
| Port Web | `8884` (WSS) |
| Topic | `smartmethods/robodog/command` |

| رسالة | الحركة |
|-------|--------|
| `f` | أمام |
| `b` | خلف |
| `S` | وقوف |
| `wL` / `wR` | مصافحة |
| `j` | جلس |

> تفاصيل: [`02-MQTT.md`](02-MQTT.md)

---

## 7. لوحة التحكم (Control Panel)

> **نفس اللوحة** تشتغل من **الجوال** أو **اللابتوب** — اختر الطريقة اللي تناسبك.

### 📱 التحكم من الجوال

1. ارفع `web/h/` على InfinityFree (مرة واحدة) — راجع [`03-CONTROL-PANEL.md`](03-CONTROL-PANEL.md)
2. افتح **Chrome** أو **Safari** على الجوال
3. ادخل الرابط (WiFi أو **4G** — ما يحتاج نفس شبكة ESP32):
   ```
   https://webtask1.free.je/h/index.html?v=6
   ```
4. اختر **تحكم يدوي** (`manual.html`) أو **تحكم بالصوت** (`voice.html`)
5. انتظر **MQTT: متصل ✓**
6. **يدوي:** اضغط ↑ أمام · ↓ خلف · 👋 تلويح
7. **صوت:** اختر عربي/English → 🎤 → قل **سلم** أو **forward**

### 💻 التحكم من اللابتوب

1. نفس خطوة الرفع على InfinityFree
2. افتح **Chrome** أو **Edge** (HTTPS مطلوب — **لا تفتح** `file://` محلي)
3. نفس الرابط:
   ```
   https://webtask1.free.je/h/index.html?v=6
   ```
4. **يدوي:** `manual.html` — أزرار أمام/خلف/مصافحة
5. **صوت:** `voice.html` — الميكروفون يحتاج إذن المتصفح
6. ESP32 لازم على **WiFi** — اللابتوب يحتاج **إنترنت** فقط (MQTT عبر الإنترنت)

| | جوال | لابتوب |
|--|------|--------|
| المتصفح | Chrome / Safari | Chrome / Edge |
| الشبكة | WiFi أو 4G | WiFi + إنترنت |
| HTTPS | ✅ InfinityFree | ✅ InfinityFree |
| `file://` محلي | ❌ | ❌ (MQTT ما يشتغل) |
| الصوت | 🎤 + إذن mic | 🎤 + إذن mic |

### الملفات — [`web/h/`](../../web/h/)

```
web/h/
├── index.html       ← اختيار: يدوي أو صوت
├── manual.html      ← أزرار
├── voice.html       ← ميكروفون
├── css/style.css
└── js/
    ├── api.js
    └── voice-commands.js
```

### رفع InfinityFree

1. File Manager → مجلد **`h/`**
2. ارفع **كل** محتويات `web/h/`
3. افتح: `https://YOUR-SITE.free.je/h/index.html?v=6`

> تفاصيل: [`03-CONTROL-PANEL.md`](03-CONTROL-PANEL.md)

---

## 8. التحكم بالصوت (عربي + English)

| عربي | MQTT |
|------|------|
| للأمام / قدام | `f` |
| للخلف / ورا | `b` |
| قف | `S` |
| **سلم / صافح** | `wR` |
| صافح يسار | `wL` |

| English | MQTT |
|---------|------|
| forward / backward | `f` / `b` |
| shake hands / wave right | `wR` |
| wave left | `wL` |

---

## 9. حركة المشي

```
خطوة 1:  FL + RR  →  neutral
خطوة 2:  FR + RL  →  neutral
```

| | FL | FR |
|--|-----|-----|
| أمام | −20 | +20 |
| خلف | +20 | −20 |

---

## 10. المصافحة / التلويح

| | المسار |
|--|--------|
| FL `wL` | 90 → 0 → (50↔0)×3 → neutral |
| FR `wR` | 90 → 180 → (130↔180)×3 → neutral |

---

## 11. ⚠️ ملاحظة مهمة — بطء المشي وعدم الـ grip

- المشي **خطوة بخطوة** — كل `f` = خطوة واحدة.
- **لا grip** في أطراف الأقدام → اضغط **أمام عدة مرات**.
- **سلوك متوقع** — ليس عطلًا.

---

## 12. 🎬 فيديوهات تعليمية (للمتابعين)

> هذه الفيديوهات **لشرح وتقليد المشروع** — أي شخص يبي يسوي زيي يتابعها.

| # | الوصف | الرابط |
|---|--------|--------|
| 1 | شرح مبسط لحركة السيرفو لأرجل الروبوت | [YouTube Shorts ↗](https://youtube.com/shorts/KG-kgXi6Zhg) |
| 2 | تشبيك الروبوت — Breadboard + ESP32 | [YouTube ↗](https://youtu.be/DsWuTCv1QBQ) |

---

## 13. 📦 فيديو تسليم Task Mechanics (منفصل)

> **هذا الفيديو خاص بتسليم مهمة Mechanics** — **ليس** جزءًا من خطوات التقليد العامة.  
> يُعرض فيه الروبوت ينفّذ **ثلاث حركات مطلوبة** للتسليم عبر لوحة التحكم.

### الفيديو

🎬 **[تجربة الروبوت — لوحة التحكم (3 حركات)](https://youtube.com/shorts/XNNBhHW6kjc)**

### الحركات الثلاث المطلوبة في التسليم

| # | الحركة | الأمر | من اللوحة |
|---|--------|-------|-----------|
| **1** | **المشي للأمام** | `f` | زر ↑ أمام (اضغط عدة مرات) |
| **2** | **المشي للخلف** | `b` | زر ↓ خلف |
| **3** | **المصافحة / التلويح** | `wR` أو `wL` | زر تلويح + اختيار يمين/يسار |

> الفيديو يوثّق تنفيذ هذه الثلاث حركات من `manual.html` أو `voice.html` — للتقييم والتسليم فقط.

---

## 14. المشاكل والحلول

| المشكلة | الحل |
|---------|------|
| `Servo.h` error | LEDC — [`05-ESP32-LEDC.md`](05-ESP32-LEDC.md) |
| FL لا تتحرك | FL=**16** FR=**4** |
| IO27 لا يعمل | لا تستخدمه |
| Panel غير متصل | InfinityFree + HTTPS — لا `file://` |
| المشي بطء | طبيعي — **لا grip**؛ اضغط `f` عدة مرات |
| USB ضعيف / reboot | طبيعي — Gait يحرّك **2 سيرفو** فقط؛ جرّب 2 للاختبار |

> كامل: [`04-TROUBLESHOOTING.md`](04-TROUBLESHOOTING.md)

---

## 15. هيكل الملفات في المستودع

```
Mechanics/
├── esp32/
│   ├── robodog_mqtt/       ← Firmware النهائي ★
│   ├── robodog_calibrate/  ← معايرة
│   ├── servo_one_io16/     ← اختبار واحد
│   └── ...
├── web/h/                  ← Control Panel ★
└── docs/robodog/           ← التوثيق ★
    ├── README.md           ← هذا الملف
    ├── 01-CODE.md
    ├── 02-MQTT.md
    ├── 03-CONTROL-PANEL.md
    ├── 04-TROUBLESHOOTING.md
    └── 05-ESP32-LEDC.md
```

---

## 16. ملفات تفصيلية إضافية

| ملف | المحتوى |
|-----|---------|
| [01-CODE.md](01-CODE.md) | Pins · neutral · gait |
| [02-MQTT.md](02-MQTT.md) | MQTT مفصل |
| [03-CONTROL-PANEL.md](03-CONTROL-PANEL.md) | InfinityFree |
| [04-TROUBLESHOOTING.md](04-TROUBLESHOOTING.md) | أخطاء وحلول |
| [05-ESP32-LEDC.md](05-ESP32-LEDC.md) | LEDC |

---

<p align="center">
  <b>Smart Methods — RoboDog BodyV2</b><br>
  <i>اتبع <a href="#2-️-تبي-تسوي-زيي--خطوات-التقليد">خطوات التقليد</a> · فيديو التسليم <a href="#13-فيديو-تسليم-task-mechanics-منفصل">منفصل</a></i>
</p>
