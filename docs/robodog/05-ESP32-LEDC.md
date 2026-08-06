# ESP32 + LEDC (بديل Servo.h)

## المشكلة

مكتبة `Servo.h` على **ESP32 Core 3.x** تعطي:

```
SOC_LEDC_TIMER_BIT_WIDE_NUM was not declared
```

## الحل

استخدم **LEDC** المدمج:

```cpp
ledcAttach(pin, 50, 16);
ledcWrite(pin, duty);
```

الزاوية 0–180° تُحوّل إلى PWM 500–2500 µs.

## اختبار سيرvo واحد

```cpp
const int PIN = 16;
void writeAngle(int a) {
  a = constrain(a, 0, 180);
  uint32_t d = (map(a, 0, 180, 500, 2500) * 65535UL) / 20000;
  ledcWrite(PIN, d);
}
void setup() {
  ledcAttach(PIN, 50, 16);
  writeAngle(90);
}
void loop() {}
```
