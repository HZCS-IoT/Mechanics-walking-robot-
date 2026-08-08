# ESP32 + LEDC — لماذا لا نستخدم Servo.h؟

> **LEDC** = طريقة ESP32 الرسمية لتوليد **PWM** — وهو ما يحتاجه سيرفو SG90 للتحرك.

---

## 1. ما المشكلة مع Servo.h؟

على **ESP32 Arduino Core 3.x**، مكتبة `Servo.h` تعطي خطأ compile:

```
SOC_LEDC_TIMER_BIT_WIDE_NUM was not declared
```

| | |
|--|--|
| **لماذا** | المكتبة القديمة تعتمد على API تغيّر في Core 3.x |
| **الحل** | استخدم **LEDC** المدمج في ESP32 — بدون مكتبة خارجية |
| **أين الكود** | [`esp32/robodog_mqtt/robodog_mqtt.ino`](../../esp32/robodog_mqtt/robodog_mqtt.ino) |

---

## 2. ما هو PWM وLEDC؟ (ببساطة)

### SG90 كيف يفهم الزاوية؟

السيرفو **ما يقرأ «90 درجة»** — يقرأ **عرض نبضة** (Pulse Width):

| الزاوية | عرض النبضة |
|---------|------------|
| 0° | ~500 µs |
| 90° | ~1500 µs |
| 180° | ~2500 µs |

**PWM** = إشارة تتكرر 50 مرة/ثانية (50 Hz) — كل «نبضة» فيها مدة HIGH تحدد الزاوية.

**LEDC** = وحدة في ESP32 تولّد هذه الإشارة على أي GPIO.

---

## 3. الكود الأساسي

### تهيئة (مرة واحدة في setup)

```cpp
ledcAttach(pin, 50, 16);  // pin, 50Hz, 16-bit resolution
```

| المعامل | المعنى |
|---------|--------|
| `pin` | GPIO (مثلاً 16 لـ FL) |
| `50` | تردد 50 Hz — **معيار SG90** |
| `16` | دقة 16-bit — دقة أعلى = حركة أنعم |

### كتابة زاوية

```cpp
ledcWrite(pin, duty);
```

`duty` = قيمة محسوبة من الزاوية — **مو** الزاوية مباشرة.

---

## 4. تحويل الزاوية → duty

```cpp
uint32_t angleToDuty(int angle) {
  angle = constrain(angle, 0, 180);
  return (map(angle, 0, 180, 500, 2500) * 65535UL) / 20000;
}
```

| خطوة | ماذا يحدث |
|------|-----------|
| `constrain(0, 180)` | يمنع زوايا خارج النطاق |
| `map(..., 500, 2500)` | 0°→500µs · 180°→2500µs |
| `* 65535 / 20000` | يحوّل لقيمة LEDC 16-bit |

---

## 5. sketch اختبار — سيرفو واحد

**المجلد:** [`esp32/servo_one_io16/`](../../esp32/servo_one_io16/)

```cpp
const int PIN = 16;

void writeAngle(int a) {
  a = constrain(a, 0, 180);
  uint32_t d = (map(a, 0, 180, 500, 2500) * 65535UL) / 20000;
  ledcWrite(PIN, d);
}

void setup() {
  ledcAttach(PIN, 50, 16);
  writeAngle(90);  // وقوف
}

void loop() {}
```

**ماذا تتوقع:** السيرفو يثبت على 90° — بدون اهتزاز.

---

## 6. 4 سيرفوات — 4 قنوات LEDC

في `robodog_mqtt.ino`:

```cpp
initLeg(PIN_FL, "FL");  // GPIO16
initLeg(PIN_FR, "FR");  // GPIO4
initLeg(PIN_RL, "RL");  // GPIO25
initLeg(PIN_RR, "RR");  // GPIO22
```

كل pin = **قناة LEDC مستقلة** — الأربعة يتحركون بدون تداخل.

---

## 7. أخطاء شائعة مع LEDC

| الخطأ | السبب | الحل |
|-------|-------|------|
| Compile `Servo.h` | استخدمت المكتبة القديمة | احذف `#include <Servo.h>` |
| السيرفو ما يتحرك | `ledcAttach` ما استُدعي | استدعِ في `setup` قبل `ledcWrite` |
| زاوية غلط | duty محسوبة غلط | استخدم `angleToDuty` |
| Reboot + printf | ترتيب `%d` `%s` | `printf("GPIO%d %s", pin, name)` |

---

## 8. لماذا 50 Hz؟

| | |
|--|--|
| **SG90** مصمم لـ **50 Hz** | تغيير التردد قد يخرب الحركة |
| **LEDC** يدعم ترددات أخرى | لا تغيّر إلا إذا تعرف specs السيرفو |

---

## 9. ملخص

```
Servo.h ❌  →  LEDC ✅
الزاوية 0–180°  →  map إلى 500–2500µs  →  ledcWrite
4 pins  →  4× ledcAttach في setup
```

> **التالي:** [`06-HELPER-SKETCHES.md`](06-HELPER-SKETCHES.md) — ترتيب الاختبار قبل MQTT.
