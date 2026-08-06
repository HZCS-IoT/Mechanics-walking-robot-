<div align="center">

# Mechanics — Walking Robot
## RoboDog BodyV2 · ESP32 + MQTT

[![GitHub](https://img.shields.io/badge/GitHub-Mechanics--walking--robot--181717?style=for-the-badge&logo=github)](https://github.com/HZCS-IoT/Mechanics-walking-robot-)
[![Smart Methods](https://img.shields.io/badge/Smart%20Methods-Mechanics-00B4D8?style=for-the-badge)](https://github.com/HZCS-IoT)

**Smart Methods — الأساليب الذكية · 2026**

كلب رباعي الأرجل (4× SG90) — مشي · مصافحة · تحكم جوال/صوت

</div>

---

## 📄 الدليل الكامل

**[docs/robodog/README.md](docs/robodog/README.md)** — شرح من الألف للياء + **13 خطوة للتقليد**

| | |
|--|--|
| **تبي تسوي زيي؟** | [خطوات التقليد](docs/robodog/README.md#2-️-تبي-تسوي-زيي--خطوات-التقليد) |
| Firmware | [`esp32/robodog_mqtt/`](esp32/robodog_mqtt/) |
| Control Panel | [`web/h/`](web/h/) |
| فيديوهات تعليمية | [سيرفو · تشبيك](docs/robodog/README.md#12-فيديوهات-تعليمية-للمتابعين) |
| **فيديو تسليم Task** | [3 حركات](docs/robodog/README.md#13-فيديو-تسليم-task-mechanics-منفصل) |

> **ملاحظة:** المشي **بطء** — **لا grip** في الأقدام → اضغط «أمام» **عدة مرات**.

---

## 📁 هيكل المستودع

```
Mechanics-walking-robot-/
├── esp32/
│   ├── robodog_mqtt/       ← Firmware النهائي ★
│   ├── robodog_calibrate/  ← معايرة neutral
│   ├── servo_one_io16/     ← اختبار سيرفو واحد
│   └── ...
├── web/h/                  ← لوحة التحكم (InfinityFree) ★
│   ├── index.html
│   ├── manual.html
│   ├── voice.html
│   ├── css/
│   └── js/
└── docs/robodog/           ← التوثيق ★
    ├── README.md
    ├── 01-CODE.md
    ├── 02-MQTT.md
    ├── 03-CONTROL-PANEL.md
    ├── 04-TROUBLESHOOTING.md
    └── 05-ESP32-LEDC.md
```

---

## 🚀 البدء السريع

```bash
git clone https://github.com/HZCS-IoT/Mechanics-walking-robot-.git
```

1. اقرأ [`docs/robodog/README.md`](docs/robodog/README.md)
2. عدّل WiFi في `esp32/robodog_mqtt/robodog_mqtt.ino`
3. ارفع Firmware على ESP32
4. ارفع `web/h/` على InfinityFree
5. جرّب `manual.html` و `voice.html`

---

<p align="center">
  <b>Smart Methods — RoboDog BodyV2</b><br>
  <i>ESP32 · MQTT · LEDC · Control Panel · Voice</i>
</p>
