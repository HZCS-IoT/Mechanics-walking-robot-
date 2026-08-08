# Control Panel

## الملفات

[`web/h/`](../../web/h/) — لوحة التحكم الكاملة (يدوي + صوت).

---

## رفع InfinityFree (مرة واحدة)

1. ادخل File Manager في InfinityFree
2. افتح مجلد **`h/`**
3. ارفع **كل** محتويات [`web/h/`](../../web/h/):
   - `index.html` · `manual.html` · `voice.html`
   - `css/style.css`
   - `js/api.js` · `js/voice-commands.js`
4. افتح (حدّث الكach):

```
https://webtask1.free.je/h/index.html?v=6
```

> `?v=6` يجبر المتصفh على تحميل النسخة الجديدة.

---

## 📱 التحكم من الجوال

1. افتح **Chrome** أو **Safari**
2. تأكد **WiFi أو 4G** شغال (ما يحتاج نفس شبكة ESP32)
3. ادخل:
   - **الرئيسية:** `https://webtask1.free.je/h/index.html?v=6`
   - **يدوي:** `https://webtask1.free.je/h/manual.html?v=6`
   - **صوت:** `https://webtask1.free.je/h/voice.html?v=6`
4. انتظر **MQTT: متصل ✓**
5. **يدوي:** اضغط ↑ أمام · ↓ خلف · 👋 تلوiح (يسار/يمين)
6. **صوت:** اختر 🇸🇦 عربي أو 🇬🇧 English → 🎤 → تكلم

> **المشي:** كل ضغطة «أمام» = خطoة واحدة — اضغط **عدة مرات** (لا grip في الأقدام).

---

## 💻 التحكم من اللابتوب

1. افتح **Chrome** أو **Edge**
2. **لا تفتح** الملف محليًا (`file://`) — MQTT ما يشتغl بدون HTTPS
3. استخدم **نفس روابط InfinityFree** أعلاه
4. **يدوي:** `manual.html` — أزرار التحكم
5. **صوت:** `voice.html` — اسمح للميكروفون عند طلب المتصفh
6. اللابتوب يحتاج **إنترنت** — ESP32 على WiFi منفصل

| | جوال | لابتop |
|--|------|--------|
| المتصفh | Chrome / Safari | Chrome / Edge |
| الشبكة | WiFi أو 4G | إنترنت |
| HTTPS | ✅ | ✅ |
| MQTT | عبر الإنترنت | عبر الإنترنت |

---

## الأزرار (manual.html)

| زر | MQTT |
|----|------|
| ↑ أمام | `f` |
| ↓ خلف | `b` |
| 👋 تلوiح | `wL` أو `wR` |
| ■ وقوف | `S` |

## أوامر الصوت (voice.html)

| عربي | English | MQTT |
|------|---------|------|
| للأمام / قدام | forward | `f` |
| للخلف / ورا | backward | `b` |
| قف | stop | `S` |
| **سلm / صافح** | shake hands / wave right | `wR` |
| صافح يسار | wave left | `wL` |

---

## 🔌 تشبيك الروبوت (ESP32)

🎬 **[YouTube — Breadboard + ESP32](https://youtu.be/DsWuTCv1QBQ)**

> **طاقة:** المشi يحرّك **2 سيرvo** في كل مرحلة — مو الأربعة مع بعض. تشغيل 2 فقط للاختبار **آمن** وما يضر.
