# المشاكل والحلول

| المشكلة | الحل |
|---------|------|
| `Servo.h` compile error | استخدم **LEDC** — راجع [05-ESP32-LEDC.md](05-ESP32-LEDC.md) |
| `Serial.printf(name, pin)` crash | الصح: `printf("GPIO%d %s", pin, name)` |
| Reboot loop | خطأ printf أو طاقة USB — افصل السيرvo عند الرفع |
| `undefined setup/loop` | لا تنسخ رسالة الخطأ داخل `.ino` |
| FL لا تتحرك | تبديل أسلاك أمامية — FL=**16** FR=**4** |
| IO27 لا يعمل | المنفذ معطّل — لا تستخدمه |
| COM busy | أغلق Serial Monitor قبل Upload |
| Panel غير متصل | تأكد HTTPS/WSS — استخدم InfinityFree |
| المشي بطي / ما يتحرك كثير | طبيعي — **لا grip** في الأقدام؛ اضغط `f` **عدة مرات** |
| خطوة واحدة فقط | كل MQTT `f` = gait واحد — ليس مشيًا مستمرًا |
