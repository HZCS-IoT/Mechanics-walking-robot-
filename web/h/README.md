# لوحة التحكم — `web/h/`

> **Smart Methods · RoboDog BodyV2**  
> هذه المجلد يحتوي **كل** ملفات لوحة التحكم — ارفعها كما هي على الاستضافة.

---

## ماذا يفعل كل ملف؟

| الملف | الوظيفة | متى يُستخدم |
|-------|---------|-------------|
| **`index.html`** | صفحة البداية — تختار **يدوي** أو **صوت** | أول رابط تفتحه |
| **`manual.html`** | أزرار: أمام · خلف · وقوف · تلويح | تحكم باللمس/الماوس |
| **`voice.html`** | ميكروفون — عربي أو English | تحكم بالصوت |
| **`css/style.css`** | الألوان · الأزرار · التصميم | يُحمّل تلقائي مع HTML |
| **`js/api.js`** | اتصال MQTT + إرسال الأوامر للـ ESP32 | يُحمّل في manual و voice |
| **`js/voice-commands.js`** | يحوّل الكلام → أمر (forward, wave_right...) | يُحمّل في voice فقط |

**6 ملفات** — لا ينقصك شيء.

---

## ما الذي **لا** يوجد محليًا؟

| المكتبة | من أين تُحمّل | لماذا |
|---------|---------------|-------|
| **`mqtt.min.js`** | CDN: `unpkg.com/mqtt` | المتصفح يحتاج HTTPS + WSS — المكتبة تُجلب من الإنترنت عند فتح الصفحة |

> **لا تحتاج** تنزّل `mqtt.min.js` يدويًا — الكود في `manual.html` و `voice.html` يضيف `<script>` للـ CDN.

---

## وين ترفعهم؟

```
GitHub:  Mechanics/web/h/*  ──►  InfinityFree:  public_html/h/*
```

**شرح مفصل:** [`docs/robodog/03-CONTROL-PANEL.md`](../../docs/robodog/03-CONTROL-PANEL.md)

---

## كيف تشتغل اللوحة (باختصار)

```
1. تفتح index.html على HTTPS (InfinityFree)
2. api.js يتصل بـ broker.hivemq.com:8884 (WSS)
3. تضغط زر أو تتكلم → api.js يرسل حرف (f, b, S, wL, wR)
4. ESP32 (مشترك على نفس Topic) يستقبل ويحرّك السيرفوات
```

> **اللوحة والESP32 ما يحتاجون نفس WiFi** — الربط عبر Broker على الإنترنت.

---

## إعدادات لازم تطابق ESP32

في [`js/api.js`](js/api.js):

```javascript
const MQTT_BROKER = 'wss://broker.hivemq.com:8884/mqtt';
const MQTT_TOPIC  = 'smartmethods/robodog/command';
```

**نفس القيم** في [`esp32/robodog_mqtt/robodog_mqtt.ino`](../../esp32/robodog_mqtt/robodog_mqtt.ino) — لا تغيّر Topic في جهاز واحد فقط.

---

## روابط بعد الرفع

```
https://YOUR-SITE.free.je/h/index.html?v=6
https://YOUR-SITE.free.je/h/manual.html?v=6
https://YOUR-SITE.free.je/h/voice.html?v=6
```

> `?v=6` يمنع المتصفح من عرض نسخة قديمة مخزّنة (cache).
