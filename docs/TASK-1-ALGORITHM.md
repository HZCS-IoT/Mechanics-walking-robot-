# التاسك 1 — خوارزمية بناء الكلب الروبوتي (مشروع هذا العام)

> **Smart Methods — Mechanics · Task 5 · Part 1**  
> بحث وتوثيق لبناء كلب روبوتي **كبير** — **مستقل تماماً** عن تجميعة Onshape في الTask 2

---

## ⚠️ هذا الملف ≠ تجميعة Onshape

| | الTask 1 (هذا الملف) | الTask 2 |
|---|----------------------|----------|
| **المشروع** | **مشروع هذا العام** — كلب كبير | تجميعة BodyV2 على Onshape |
| **الغرض** | بحث + خوارزمية + تصميم نظري | Exploded View + فيديو |
| **الحجم** | 50–70 cm · 30–40 cm · <14 kg | تجميعة مهمة/تدريبية |

---

## 📋 جدول المحتويات

1. [نظرة عامة](#-نظرة-عامة)
2. [مواصفات مشروع هذا العام](#-مواصفات-مشروع-هذا-العام)
3. [خوارزمية البناء الكاملة](#-خوارزمية-البناء-الكاملة)
4. [التصميم الميكانيكي](#-التصميم-الميكانيكي)
5. [اختيار المحركات](#-اختيار-المحركات)
6. [نظام التوازن](#-نظام-التوازن)
7. [الإلكترونيات والتحكم](#-الالكترونيات-والتحكم)
8. [مقارنة مع مشاريع عالمية](#-مقارنة-مع-مشاريع-عالمية)
9. [المصادر](#-المصادر)

---

## 🎯 نظرة عامة

يهدف **مشروع هذا العام** إلى تصميم وبناء **كلب روبوتي (Quadruped Robot)** بحجم **كبير** مقارنة بالنماذج التعليمية الصغيرة، وفق المواصفات:

- **جسم مركزي (Chassis)** — إلكترونيات + بطارية
- **4 أرجل × 3 DOF** = **12 DOF** (حتى 14 مع رأس/ذيل)
- **نظام توازن** — IMU + PID + Inverse Kinematics

```
                    ┌─────────────────┐
                    │     HEAD        │  (اختياري — DOF 13-14)
                    ├─────────────────┤
         FL ────────│     BODY        │──────── FR
                    │  [Electronics]  │
         RL ────────│   [Battery/IMU] │──────── RR

         كل رجل = Hip + Upper Leg + Lower Leg + Foot
                   (3 DOF: Abduction, Hip Pitch, Knee Pitch)
```

---

## 📐 مواصفات مشروع هذا العام

| المواصfة | القيمة | ملاحظات |
|----------|--------|---------|
| **الطول** | 50 – 70 cm | من مقدمة الجسم إلى نهايته |
| **الارتفاع** | 30 – 40 cm | وضع الوقوف |
| **DOF** | 12 – 14 | 3 × 4 أرجل |
| **الوزن** | < 14 kg | شامل كل شيء |
| **الحجم** | كبير | أكبر من الكلب التعليمي الحالي |

| المفصل | الاسم | المحور | الوظيفة |
|--------|-------|--------|---------|
| 1 | Hip Abduction (HAA) | Roll | فتح/إغلاق الرجل |
| 2 | Hip Flexion (HFE) | Pitch | الفخذ أمام/خلف |
| 3 | Knee Flexion (KFE) | Pitch | ثني الركبة |

---

## 🔧 خوارزمية البناء الكاملة

```
┌─────────────────────────────────────────────────────────────┐
│              ROBOTIC DOG BUILD ALGORITHM                     │
│              (مشروع هذا العام — Large Quadruped)             │
├─────────────────────────────────────────────────────────────┤
│  PHASE 1: Requirements & Research                           │
│     ├── Specs: 50-70cm, 30-40cm, 12-14 DOF, <14kg          │
│     ├── Study: SpotMicro, Stanford Doggo, Mini Cheetah      │
│     └── Select: Servo vs BLDC QDD actuation                 │
│                          ↓                                   │
│  PHASE 2: Mechanical Design (CAD)                           │
│     ├── Body/Chassis · Leg linkage · Motor mounts           │
│                          ↓                                   │
│  PHASE 3: Actuator Selection & Torque Sizing                │
│                          ↓                                   │
│  PHASE 4: Electronics (MCU, IMU, Drivers, Battery)        │
│                          ↓                                   │
│  PHASE 5: Balance & Control (IK, Gait, PID)                 │
│                          ↓                                   │
│  PHASE 6: Fabrication & Physical Assembly                   │
│                          ↓                                   │
│  PHASE 7: Calibration & Testing                             │
└─────────────────────────────────────────────────────────────┘
```

> **Deep dive:** [`ALGORITHM.md`](ALGORITHM.md) — Mermaid flowchart + pseudocode

### PHASE 1 — المتطلبات

```
INPUT:  length=50..70cm, height=30..40cm, DOF=12..14, mass<14kg
OUTPUT: leg_length≈25..35cm, body≈40..55cm, motors=12
```

| روبوت مرجعي | DOF | الطول | الوزن |
|-------------|-----|-------|-------|
| **مشروع العام (Target)** | 12–14 | 50–70 cm | <14 kg |
| Unitree Go1 | 12 | 58 cm | 12 kg |
| MIT Mini Cheetah | 12 | 48 cm | 9 kg |
| Stanford Doggo | 8 | ~40 cm | 4.8 kg |
| SpotMicro | 12 | ~30 cm | ~2 kg |

---

## 🏗️ التصميم الميكانيكي

### هيكل الجسم

- Material: Aluminum 6061 / Carbon Fiber / PETG
- Body: ~45×25 cm base, 8–12 cm height
- 4 hip mounts at corners
- Weight target: < 3 kg (body only)

### الرجل (3 DOF)

```
Body → [HAA] → Hip → [HFE] → Femur → [KFE] → Tibia → Foot
```

| Segment | الطول (لجسم 60 cm) |
|---------|-------------------|
| Hip Link | 5–8 cm |
| Femur | 15–20 cm |
| Tibia | 15–20 cm |
| Foot | 3×5 cm |

---

## ⚙️ اختيار المحركات

### حساب العزم

```
τ = (m × g × L) / n_legs

مثال (14 kg, L=25 cm, 2 legs in trot):
  τ = (14 × 9.81 × 0.25) / 2 = 17.2 N·m ≈ 175 kg·cm
  τ_dynamic (×2 SF) = 350 kg·cm (knee)
```

| الخيار | المحرك | العزم | ملاحظات |
|--------|--------|-------|---------|
| ⭐ Recommended | Dynamixel XM430 | 42 kg·cm | Closed-loop |
| Budget | DS3225 | 25 kg·cm | Open-loop |
| Performance | TMotor + ODrive | High | Stanford Doggo style |

**Power:** 6S LiPo 5000mAh · 8–15A peak · 30–60 min runtime

---

## ⚖️ نظام التوازن

```
IMU (BNO055) → Sensor Fusion → PID (Roll/Pitch)
    → Foot position correction → IK → 12 Motor commands
```

### Static Balance
- قراءة IMU @ 100 Hz
- تصحيح Roll/Pitch → تعديل مواقع الأقدام
- CoG داخل Support Polygon (≥3 أرجل)

### Dynamic Balance (Trot)
```
Phase 1: FL+RR swing | FR+RL stance
Phase 2: Body shift
Phase 3: FR+RL swing | FL+RR stance
Phase 4: Body shift
+ PID correction during each phase
```

### PID (بداية tuning)
```
Kp_roll=0.5  Ki_roll=0.01  Kd_roll=0.05
Kp_pitch=0.5 Ki_pitch=0.01 Kd_pitch=0.05
```

### Inverse Kinematics
```
θ_HAA = atan2(y, sqrt(x² + z²))
θ_KFE = acos((L2²+L3²-d²)/(2·L2·L3)) - π
θ_HFE = acos((L2²+d²-L3²)/(2·L2·d)) + atan2(z,x)
```

---

## 💻 الإلكترونيات والتحكم

| المكون | النموذج | Interface |
|--------|---------|-----------|
| MCU | STM32F407 / RPi 4 | — |
| IMU | BNO055 / BNO085 | I2C |
| Servo Driver | PCA9685 | I2C |
| BLDC Driver | ODrive v3.6 | CAN/UART |
| Battery | 6S LiPo | — |

### Control Loops

| Loop | Hz | Task |
|------|-----|------|
| Motor command | 500 | PWM/CAN |
| Balance PID | 100 | IMU |
| Gait planner | 50 | Trajectories |

---

## 📊 مقارنة مع مشاريع عالمية

| Feature | SpotMicro | Doggo | Mini Cheetah | **مشروع العام** |
|---------|-----------|-------|--------------|-----------------|
| DOF | 12 | 8 | 12 | **12–14** |
| Mass | 2 kg | 4.8 kg | 9 kg | **<14 kg** |
| Size | Mini | Small | Medium | **Large** |
| Balance | Basic | IMU | Full dynamic | **IMU+PID** |

---

## 📚 المصادر

1. [Stanford Doggo — GitHub](https://github.com/Nate711/StanfordDoggoProject) · [Paper](https://arxiv.org/pdf/1905.04254)
2. [MIT Mini Cheetah](https://robotsguide.com/robots/minicheetah)
3. [SpotMicro — Thingiverse](https://www.thingiverse.com/thing:3445283)
4. [Quadruped Controller](https://github.com/Cyclemnt/quadruped-controller)
5. [Quadruped Stabilization](https://github.com/smnizza/quadruped-robot-stabilization)
6. [Unitree Go1](https://www.unitree.com/products/go1)
7. [Servo Torque Calculator](https://usecalcpro.com/tools/servo-motor-calculator)

> قائمة كاملة: [`SOURCES.md`](SOURCES.md) — قسم الTask 1

---

[← الفهرس الرئيسي](../README.md) · [الTask 2 — Exploded View →](TASK-2-EXPLODED-VIEW.md)
