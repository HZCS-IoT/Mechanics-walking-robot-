# MQTT — RoboDog

## المخطط

```
[جوال / لابتوب]  ──publish──►  broker.hivemq.com  ──►  ESP32  ──►  4 Servos
     panel.html                    :1883 (ESP32)
                                   :8884/wss (المتصفح)
```

## الإعدادات

| | القيمة |
|--|--------|
| Broker | `broker.hivemq.com` |
| Port ESP32 | `1883` |
| Port Web (WSS) | `8884` |
| Topic | `smartmethods/robodog/command` |

## الأوامر

| أمر | الوظيفة |
|-----|---------|
| `f` | خطوة أمام |
| `b` | خطوة خلف |
| `S` | وقوف (neutral) |
| `wL` | تلويح يسار (FL) |
| `wR` | تلويح يمين (FR) |

## كيف يشتغل

1. **Control Panel** ينشر (publish) نص قصير على الـ Topic.
2. **ESP32** مشترك (subscribe) على نفس الـ Topic.
3. عند وصول رسالة → `executeCommand()` ينفّذ الحركة.

**لا قاعدة بيانات** — رسالة فورية فقط.

## اختبار من MQTT Explorer (لابتوب)

1. اتصل بـ `broker.hivemq.com:1883`
2. Publish → Topic: `smartmethods/robodog/command` → Message: `f`
