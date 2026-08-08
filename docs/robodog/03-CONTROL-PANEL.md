# Control Panel — لوحة التحكم (شرح كامل)

> **المجلد:** [`web/h/`](../../web/h/) — **6 ملفات** جاهزة للرفع.

---

## 0. ما هي لوحة التحكم؟

| | |
|--|--|
| **ما هي** | صفحات HTML تعمل في **المتصفح** — أزرار + صوت |
| **كيف تتواصل** | MQTT عبر الإنترنت → ESP32 |
| **أين تُhost** | **InfinityFree** (HTTPS مجاني) — **مو** على اللابتop محليًا |
| **من يستخدمها** | جوال (WiFi/4G) أو لابتوب — **نفس الرابط** |

> **لماذا لا `file://`؟** MQTT في المتصفح يحتاج **HTTPS + WSS** — الملف المحلي لا يوفّر ذلك.

---

## 1. الملفات في GitHub — ماذا يفعل كل واحد؟

```
Mechanics-walking-robot-/web/h/
├── index.html              ← صفحة البداية: اختيار يدوي أو صوت
├── manual.html             ← أزرار: ↑ ↓ 👋 ■
├── voice.html              ← ميكروفون: عربي / English
├── css/
│   └── style.css           ← الألوان، الأزرار، التخطيط
└── js/
    ├── api.js              ← MQTT + إرسال f/b/S/wL/wR
    └── voice-commands.js   ← تحويل الكلام → forward/wave_right/...
```

| # | الملف | بدونها ماذا يحدث؟ |
|---|-------|-------------------|
| 1 | `index.html` | ما عندك صفحة اختيار — تفتح manual مباشرة |
| 2 | `manual.html` | لا أزرار تحكم |
| 3 | `voice.html` | لا تحكم صوتي |
| 4 | `style.css` | صفحة بيضاء بدون تصميم |
| 5 | `api.js` | الأزرار ما ترسل MQTT |
| 6 | `voice-commands.js` | الصوت ما يُفهم |

**6 ملفات** — لا ينقصك شيء.

> **`mqtt.min.js`:** يُحمّل من CDN (`unpkg.com`) — **لا ترفعه** — يحتاج إنترنت عند فتح الصفحة.

> **شرح كل ملف:** [`web/h/README.md`](../../web/h/README.md)

---

## 2. إنشاء حساب InfinityFree (مرة واحدة)

| خطوة | ماذا تسوي |
|------|-----------|
| 1 | ادخل [infinityfree.com](https://www.infinityfree.com/) |
| 2 | **Sign Up** — email + password |
| 3 | **Create Account** — اختر subdomain (مثلاً `webtask1.free.je`) |
| 4 | انتظر التفعيل (دقائق) |
| 5 | من Dashboard → **Control Panel** → **File Manager** |

**ماذا تحصل:**
- مجلد **`public_html`** = جذر موقعك
- رابط: `https://YOUR-NAME.free.je/`

---

## 3. وين تحط الملفات على الاستضافة؟

### الهيكل الصحيح

```
public_html/                    ← جذر الموقع (InfinityFree)
└── h/                          ← أنشئ هذا المجلد
    ├── index.html
    ├── manual.html
    ├── voice.html
    ├── css/
    │   └── style.css
    └── js/
        ├── api.js
        └── voice-commands.js
```

### الهيكل الغلط (تجنّبه)

```
❌ public_html/web/h/index.html     ← زيادة مجلد web
❌ public_html/index.html           ← خارج h/
❌ public_html/h/js/api.js فقط      ← نسيان HTML و css
```

> **القاعدة:** محتويات `web/h/` من GitHub → **مباشرة** داخل `public_html/h/`

---

## 4. خطوات الرفع (File Manager)

| # | الخطوة | التفصيل |
|---|--------|---------|
| 1 | File Manager | InfinityFree Dashboard |
| 2 | `public_html` | افتح الجذر |
| 3 | مجلد `h` | New Folder → اسم **`h`** |
| 4 | من جهازك | افتح `Mechanics/web/h/` بعد Clone |
| 5 | Upload | 3 ملفات HTML → داخل `h/` |
| 6 | Upload | مجلد **`css`** كامل (في داخله `style.css`) |
| 7 | Upload | مجلد **`js`** كامل (`api.js` + `voice-commands.js`) |
| 8 | تحقق | 6 ملفات + هيكل يطابق §3 |
| 9 | افتح الرابط | `https://YOUR-SITE.free.je/h/index.html?v=6` |

**طرق الرفع:**
- **File Manager** — سحب وإفلات (Drag & Drop)
- **FTP** — FileZilla (اختياري — نفس المسار `public_html/h/`)

---

## 5. بعد الرفع — اختبار

| # | ماذا تجرّب | النتيجة المتوقعة |
|---|-----------|------------------|
| 1 | `index.html?v=6` | صفحتان: يدوي · صوت |
| 2 | `manual.html?v=6` | أزرار + **MQTT: متصل ✓** (أخضر) |
| 3 | زر ↑ أمام | رسالة: `MQTT وصل للESP32: f` |
| 4 | ESP32 Serial | `>>> MQTT: f` |
| 5 | `voice.html?v=6` | اسمح للميكروفون → قل **forward** |

> **`?v=6`:** يمنع المتصفح من عرض نسخة قديمة (cache). غيّر الرقم بعد كل تحديث.

**مثال (موقعنا):**
```
https://webtask1.free.je/h/index.html?v=6
https://webtask1.free.je/h/manual.html?v=6
https://webtask1.free.je/h/voice.html?v=6
```

---

## 6. التحكم من الجوال

| # | الخطوة |
|---|--------|
| 1 | Chrome أو Safari |
| 2 | WiFi **أو** 4G (ما يحتاج نفس شبكة ESP32) |
| 3 | افتح `https://YOUR-SITE.free.je/h/index.html?v=6` |
| 4 | **يدوي:** `manual.html` — ↑ ↓ 👋 ■ |
| 5 | **صوت:** `voice.html` — اختر عربي → 🎤 → **سلm** |

**نصائح جوال:**
- أضف الصفحة للـ Home Screen (اختصار)
- إذا MQTT ما يتصل — جرّب Chrome بدل Safari

---

## 7. التحكم من اللابتوب

| # | الخطوة |
|---|--------|
| 1 | Chrome أو Edge |
| 2 | **نفس الرابط** — لا تفتح الملف من Explorer (`file://`) |
| 3 | `manual.html` — ماوس |
| 4 | `voice.html` — 🔒 اسمح للميكروفون |

| | جوال | لابتوب |
|--|------|--------|
| المتصفح | Chrome / Safari | Chrome / Edge |
| الشبكة | WiFi أو 4G | إنترنت |
| HTTPS | ✅ | ✅ |
| ESP32 WiFi | منفصل — OK | منفصل — OK |

---

## 8. الأزرار — manual.html

| زر في اللوحة | اسم داخلي (api.js) | MQTT | ماذا يفعل الروبوت |
|--------------|-------------------|------|-------------------|
| ↑ أمام | forward | `f` | خطوة أمام (gait) |
| ↓ خلف | backward | `b` | خطوة خلف |
| 👋 تلويح | wave_left / wave_right | `wL` / `wR` | مصافحة يسار/يمين |
| ■ وقوف | stop | `S` | neutral |

> **لا يمين/يسار مشي** — هذا الروبوت BodyV2 — أمام/خلف فقط.

---

## 9. أوامر الصوت — voice.html

| عربي | English | → api.js | MQTT |
|------|---------|----------|------|
| للأمام / قدام | forward | forward | `f` |
| للخلف / ورا | backward | backward | `b` |
| قف / توقف | stop | stop | `S` |
| **سلm / صافح / سلام** | shake hands / wave right | wave_right | `wR` |
| صافح يسار | wave left | wave_left | `wL` |
| اجلس | sit | sit | `j` |

**كيف يشتغل الصوت:**
1. المتصفح يسجل كلامك (Web Speech API)
2. `voice-commands.js` يطابق الكلمات مع `VOICE_RULES`
3. `api.js` يرسل الحرف عبر MQTT

> **قائمة كلمات كاملة:** [`web/h/js/voice-commands.js`](../../web/h/js/voice-commands.js)

---

## 10. لوحتان تحكم — أيهما هذا؟

| | **لوحة BodyV2 (هذا المشروع)** | لوحة Algorithm (مستقبلي) |
|--|-------------------------------|---------------------------|
| **المسار** | `web/h/` في هذا الريpo | XAMPP `htdocs/h/` — روبوت 12–14 DOF |
| **الحركات** | أمام · خلف · مصافحة | + يمين · يسار |
| **الروبوت** | 4× SG90 — BodyV2 | روبوت متقدم (Task 1) |
| **الاستخدام** | **الحالي** — InfinityFree | **لاحقًا** — نفس فكرة MQTT |

> **المتابع:** استخدم **`web/h/`** فقط لهذا الروبوت.

---

## 11. أخطاء شائعة في الرفع

| ❌ خطأ | ✅ الصح | لماذا |
|-------|--------|-------|
| رفع `web/` كامل | محتويات `web/h/` فقط | المسار يصير `/web/h/h/` |
| HTML في `public_html/` | داخل `h/` | الرابط `/h/index.html` |
| نسيان `css/` | ارفع المجلد كامل | بدون CSS = صفحة بيضاء |
| نسيان `js/` | ارفع api.js + voice-commands.js | الأزرار لا ترسل |
| `file://` محلي | HTTPS InfinityFree | WSS لا يعمل محليًا |
| بدون `?v=` | أضف `?v=6` | cache قديم |

---

## 12. MQTT — الخطوة التالية

بعد الرفع → [`02-MQTT.md`](02-MQTT.md) — ربط ESP32 + اللوحة.

🎬 **تشبيك:** [YouTube — Breadboard + ESP32](https://youtu.be/DsWuTCv1QBQ)

> **طاقة:** Gait يحرّك **2 سيرفو** في كل مرحلة — اختبار 2 **آمن**.
