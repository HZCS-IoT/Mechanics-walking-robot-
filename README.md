<div align="center">

# 🤖 مشروع الكلب الروبوتي
### Robotic Dog — Algorithm & Exploded View

[![Smart Methods](https://img.shields.io/badge/Smart%20Methods-Mechanics-00B4D8?style=for-the-badge)](https://github.com/HZCS-IoT)
[![Task](https://img.shields.io/badge/Task-5-FF006E?style=for-the-badge)](https://github.com/HZCS-IoT/mechanics-Algorithm-exploded-view-)
[![DOF](https://img.shields.io/badge/DOF-12--14-7209B7?style=for-the-badge)]()
[![Mass](https://img.shields.io/badge/Mass-%3C14kg-3A0CA3?style=for-the-badge)]()
[![Onshape](https://img.shields.io/badge/CAD-Onshape-0066CC?style=for-the-badge&logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIxNiIgaGVpZ2h0PSIxNiIgZmlsbD0iI2ZmZiIgdmlld0JveD0iMCAwIDE2IDE2Ij48cGF0aCBkPSJNOCAwYTEgMSAwIDAgMSAxIDF2MTRhMSAxIDAgMCAxLTIgMFYxYTF1MSAwIDAgMSAxLTF6Ii8+PC9zdmc+)](https://cad.onshape.com/documents/b26f18ef0000550a5887d678/w/48a6352dd9256db373579cc0/e/41e000305039f0b434641ddc)
[![YouTube](https://img.shields.io/badge/Video-Exploded%20View-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://youtu.be/dGe0Nn-FiPE)

**Smart Methods — Mechanics · المهمة الخامسة (Task 5)**  
خوارزمية بناء كلب روبوتي رباعي الأرجل + Exploded View على Onshape  
*مشروع هذا العام — 2026*

---

### 🔗 Quick Links

| Onshape Assembly | YouTube Video | Algorithm Deep Dive | Assembly Guide |
|:----------------:|:-------------:|:-----------------:|:--------------:|
| [Open CAD ↗](https://cad.onshape.com/documents/b26f18ef0000550a5887d678/w/48a6352dd9256db373579cc0/e/41e000305039f0b434641ddc) | [Watch ↗](https://youtu.be/dGe0Nn-FiPE) | [ALGORITHM.md](docs/ALGORITHM.md) | [ASSEMBLY.md](docs/ASSEMBLY.md) |

</div>

---

## 📋 جدول المحتويات

1. [نظرة عامة](#-نظرة-عامة)
2. [مواصفات المشروع](#-مواصفات-مشروع-هذا-العام)
3. [الجزء الثاني — التجميع و Exploded View](#-الجزء-الثاني--التجميع-و-exploded-view)
4. [ملاحظات التصميم](#-ملاحظات-التصميم)
5. [الجزء الأول — خوارزمية البناء الكاملة](#-الجزء-الاول--خوارزمية-البناء-الكاملة)
6. [هيكل الميكانيكا](#-هيكل-الميكانيكا)
7. [اختيار المحركات](#-اختيار-المحركات)
8. [نظام التوازن](#-نظام-التوازن-balance-system)
9. [الإلكترونيات والتحكم](#-الالكترونيات-والتحكم)
10. [خطوات التجميع](#-خطوات-التجميع-العملية)
11. [مقارنة مع مشاريع عالمية](#-مقارنة-مع-مشاريع-عالمية)
12. [المصادر والمراجع](#-المصادر-والمراجع)

---

## 🎯 نظرة عامة

يهدف هذا المشروع إلى **تصميم وبناء كلب روبوتي (Quadruped Robot)** بحجم **أكبر** من النماذج التعليمية الصغيرة المعتادة، مع الالتزام بمواصفات **"مشروع هذا العام"** الصادرة من Smart Methods.

الكلب الروبوتي من نوع **Quadruped** — أي روبوت رباعي الأرجل يحاكي حركة الكلب الحقيقي. يتكون من:

- **جسم مركزي (Chassis/Body)** — يحمل الإلكترونيات والبطارية
- **4 أرجل** — كل رجل بـ **3 درجات حرية (3 DOF)**
- **12–14 DOF إجمالي** — حسب إضافة مفصل الرأس أو الذيل
- **نظام توازن** — IMU + PID + Inverse Kinematics

```
                    ┌─────────────────┐
                    │     HEAD        │  (اختياري — DOF 13-14)
                    ├─────────────────┤
         FL ────────│                 │──────── FR
         (Front     │     BODY        │      (Front
          Left)     │   (Chassis)     │       Right)
                    │  [Electronics]  │
         RL ────────│   [Battery]     │──────── RR
         (Rear      │   [IMU]         │      (Rear
          Left)     └─────────────────┘       Right)

         كل رجل = Hip + Upper Leg + Lower Leg + Foot
                   (3 DOF: Abduction, Hip Pitch, Knee Pitch)
```

---

## 📐 مواصفات مشروع هذا العام

| المواصفة | القيمة المطلوبة | ملاحظات |
|----------|-----------------|---------|
| **الطول (Length)** | 50 – 70 cm | من مقدمة الجسم إلى نهايته |
| **الارتفاع (Height)** | 30 – 40 cm | من الأرض إلى أعلى الجسم (وضع الوقوف) |
| **درجات الحرية (DOF)** | 12 – 14 | 3 DOF × 4 أرجل = 12 (أساسي) |
| **الوزن الأقصى (Mass)** | < 14 kg | شامل المحركات والبطارية والهيكل |
| **الحجم** | كبير | أكبر من الكلب الروبوتي التعليمي الحالي |

### توزيع DOF المقترح (12 DOF)

| المفصل | الاسم التقني | المحور | الوظيفة |
|--------|-------------|--------|---------|
| **1** | Hip Abduction (HAA) | Roll | فتح/إغلاق الرجل عن الجسم |
| **2** | Hip Flexion (HFE) | Pitch | حركة الفخذ للأمام/الخلف |
| **3** | Knee Flexion (KFE) | Pitch | ثني/مد الركبة |

> **DOF 13–14 (اختياري):** مفصل رأس (Pan/Tilt) أو ذيل للتوازن الديناميكي — كما في [MIT Mini Cheetah](https://robotsguide.com/robots/minicheetah).

---

## 🎬 الجزء الثاني — التجميع و Exploded View

### 🔗 روابط المشروع

| المورد | الرابط | الوصف |
|--------|--------|-------|
| **Onshape Assembly** | [فتح التجميعة](https://cad.onshape.com/documents/b26f18ef0000550a5887d678/w/48a6352dd9256db373579cc0/e/41e000305039f0b434641ddc) | التجميعة الكاملة — **عرض فقط** (View Only) |
| **YouTube Video** | [exploded view](https://youtu.be/dGe0Nn-FiPE) | فيديو Exploded View — خطوات التفكik والتجميع |

> **ملاحظة الوصول:** الرابط مشارك بوضع **View Only**. التعديل يتطلب إضافة البريد الإلكتروني يدوياً من مالك المستند — وهذا مطابق لمتطلبات المهندس (لم يُطلب منح صلاحيات تعديل).

### 📦 Exploded View — منهجية التنفيذ

تم إنشاء **Exploded View واحد** يحتوي على **13 خطوة (Steps)** بترتيب من **الخارج إلى الداخل**:

```
Step 1  → آخر مسمار/قطعة تُركّب (الخارج)
Step 2  → ...
...
Step 13 → القلب/الهيكل الداخلي (الداخل)
```

**أداة العرض:** Onshape Exploded Views Panel + Rollback Bar  
**تسجيل الفيديو:** Screen Recording (Xbox Game Bar) + Rollback Bar (↑/↓)  
**الترتيب في الفيديو:** من fully exploded → fully assembled (تركيب)

### 🧩 مكونات التجميعة (Assembly Breakdown)

| # | المكون | الوصف |
|---|--------|-------|
| 1 | **Body Top Cover** | الغطاء العلوي للجسم |
| 2 | **Body Bottom Plate** | القاعدة السفلية |
| 3 | **Side Panels (×4)** | الألواح الجانبية |
| 4 | **Internal Mounting Plates** | لوحات تثبيت داخلية (إلكترونيات) |
| 5 | **Leg Assemblies (×4)** | تجميعات الأرجل الكاملة |
| 6 | **Hip Joints (×4)** | مفاصل الورك |
| 7 | **Upper Leg Links (×4)** | عظام الفخذ |
| 8 | **Lower Leg Links (×4)** | عظام الساق السفلى |
| 9 | **Foot Pads (×4)** | أقدام/تلامس أرضي |
| 10 | **Fasteners (Screws/Bolts)** | مسامير وصواميل التثبيت |

---

## ⚠️ ملاحظات التصميم

### 1. الأرجل — خروج من الجهة المقابلة

> **الملاحظة:** الأرجل في التجميعة الحالية تظهر خارجة من **الجهة المقابلة** للوضع الم ideal.

**السبب:** المسامير والصواميل المُوردة **لم تكن بالمقاسات الصحيحة** — وهذا **خارج نطاق مسؤولية المصمم** ويُوثَّق هنا للشفافية.

**الحل الم planned:** استبدال المسامير بالمقاسات الصحيحة عند التجميع الفعلي (Physical Build) — يتطلب تعديل ثقوب التثبيت أو استخدام Standoffs بالطول المناسب.

### 2. لماذا لم يُسمح بتعديل الملف في Onshape؟

الملفات المُستوردة بصيغة **`.SLDPRT`** (SolidWorks Part) تُ import إلى Onshape كـ **"Dumb Solid"** — أي:

| ما يُ import | ما **لا** يُ import |
|-------------|---------------------|
| ✅ الهندسة 3D (الأوجه والحجم) | ❌ Feature Tree (شجرة العمليات) |
| ✅ يمكن Direct Edit محدود | ❌ Sketches الأصلية |
| ✅ يمكن إضافة Features جديدة فوقها | ❌ Parametric History |
| ✅ Assembly mates جديدة | ❌ تعديل البُعد parametrically |

**السبب التقني:** كل نظام CAD (SolidWorks, Onshape, Fusion 360) له **Feature Tree خاص** — لا يوجد ترجمة 1:1 بين الأنظمة. Onshape يستورد **النتيجة النهائية للهندسة فقط**.

**الحلول المتاحة:**
1. **Direct Editing** — Move Face, Delete Face, Replace Face
2. **إعادة بناء** القطعة natively في Onshape (للتعديلات الكبيرة)
3. **طلب الملف الأصلي** من SolidWorks للتعديل هناك
4. **View Only link** — لا يسمح بأي تعديل أصلاً (وضع المشاركة الحالي)

> مصدر: [Onshape Help — Working with Imported CAD](https://cad.onshape.com/help/Content/Document/working_with_imported_cad.htm)

---

## 🔧 الجزء الأول — خوارزمية البناء الكاملة

### الخوارزمية الرئيسية (Master Algorithm)

```
┌─────────────────────────────────────────────────────────────┐
│                    ROBOTIC DOG BUILD ALGORITHM               │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  PHASE 1: Requirements & Research                           │
│     ├── Define specs (50-70cm, 30-40cm, 12-14 DOF, <14kg)  │
│     ├── Study reference robots (SpotMicro, Doggo, Cheetah)   │
│     └── Select actuation strategy (Servo vs BLDC QDD)       │
│                          ↓                                   │
│  PHASE 2: Mechanical Design (CAD)                           │
│     ├── Body/Chassis design                                  │
│     ├── Leg linkage (3 DOF per leg)                         │
│     ├── Motor mounting & coaxial drives                     │
│     └── Exploded view for assembly documentation            │
│                          ↓                                   │
│  PHASE 3: Actuator Selection & Sizing                         │
│     ├── Torque calculation per joint                        │
│     ├── Speed requirements for gait                         │
│     └── Weight budget allocation                            │
│                          ↓                                   │
│  PHASE 4: Electronics Architecture                          │
│     ├── Main controller (STM32 / Raspberry Pi / Teensy)     │
│     ├── Motor drivers (PCA9685 / ODrive / Dynamixel bus)  │
│     ├── IMU (BNO055 / MPU6050)                              │
│     └── Power system (BMS, battery, distribution)           │
│                          ↓                                   │
│  PHASE 5: Balance & Control Software                        │
│     ├── Forward/Inverse Kinematics                          │
│     ├── Gait generation (Walk, Trot, Stand)                 │
│     ├── PID balance controller (Roll, Pitch)                  │
│     └── IMU feedback loop                                   │
│                          ↓                                   │
│  PHASE 6: Fabrication & Assembly                            │
│     ├── 3D print / CNC / Waterjet parts                    │
│     ├── Motor & electronics installation                    │
│     └── Mechanical assembly (per exploded view order)       │
│                          ↓                                   │
│  PHASE 7: Calibration & Testing                             │
│     ├── IMU calibration                                     │
│     ├── Servo zero-position calibration                     │
│     ├── Static balance test (4-leg stand)                   │
│     ├── Dynamic gait test (walk, trot)                      │
│     └── Load & endurance testing                            │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

### PHASE 1: المتطلبات والبحث

#### 1.1 تحليل المواصفات

```
INPUT:
  - length_target    = 50..70 cm
  - height_target    = 30..40 cm
  - dof_target       = 12..14
  - mass_limit       = 14 kg

OUTPUT:
  - leg_length       ≈ 25..35 cm (upper + lower)
  - body_length      ≈ 40..55 cm
  - body_width       ≈ 20..30 cm
  - motor_count      = 12 (minimum)
```

#### 1.2 دراسة المراجع

| روبوت | DOF | الطول | الارتفاع | الوزن | Actuation |
|-------|-----|-------|----------|-------|-----------|
| **مشروعنا (Target)** | 12–14 | 50–70 cm | 30–40 cm | <14 kg | Servo/BLDC |
| MIT Mini Cheetah | 12 | 48 cm | 30 cm | 9 kg | Custom BLDC |
| Stanford Doggo | 8 | ~40 cm | ~20 cm | 4.8 kg | TMotor + ODrive |
| SpotMicro | 12 | ~30 cm | ~15 cm | ~2 kg | SG90/MG996R |
| Unitree Go1 | 12 | 58 cm | 40 cm | 12 kg | Custom servo |

> **الاستنتاج:** مشروعنا أقرب لـ **Unitree Go1** و **MIT Mini Cheetah** من حيث الحجم — يتطلب **محركات عزم عالي** وليس servos صغيرة.

---

### PHASE 2: التصميم الميكانيكي

#### 2.1 هيكل الجسم (Body/Chassis)

```
Body Design Requirements:
├── Material: Aluminum 6061 / Carbon Fiber / PETG (3D print)
├── Shape: Rectangular box — easy electronics access
├── Mounting: 4 hip joints at corners
├── Internal: Battery bay, PCB mounts, cable routing
└── Weight target: < 3 kg (body only)
```

**خطوات CAD:**
1. رسم **Sketch** للقاعدة (Base Plate) — أبعاد 45×25 cm
2. **Extrude** الجدران الجانبية — ارتفاع 8–12 cm
3. إضافة **Hip Mounting Points** — 4 نقاط at corners
4. **Fillets** على الحواف — تقليل stress concentration
5. **Internal ribs** — تقوية بدون زيادة وزن كبيرة
6. **Top cover** — removable للصيانة
7. **Exploded View** — توثيق التجميع

#### 2.2 تصميم الرجل (Leg — 3 DOF)

كل رجل تتبع kinematic chain:

```
Body → [HAA Motor] → Hip Link → [HFE Motor] → Upper Leg → [KFE Motor] → Lower Leg → Foot

         Abduction          Hip Pitch              Knee Pitch
         (Y-axis)           (X-axis)               (X-axis)
```

**Reference leg dimensions (for 60cm body):**

| Segment | الطول | Material |
|---------|-------|----------|
| Hip Link | 5–8 cm | Aluminum / PETG |
| Upper Leg (Femur) | 15–20 cm | Carbon fiber tube / Aluminum |
| Lower Leg (Tibia) | 15–20 cm | Carbon fiber tube / Aluminum |
| Foot | 3×5 cm | Rubber pad / PETG |

---

### PHASE 3: اختيار المحركات

#### 3.1 حساب العزم المطلوب (Torque Calculation)

**المعادلة الأساسية:**

```
τ = (m × g × L) / n_legs

حيث:
  τ  = العزم المطلوب (N·m)
  m  = كتلة الروبوت (kg)
  g  = 9.81 m/s²
  L  = طول الذراع (m) — من المفصل إلى مركز ثقل
  n  = عدد الأرجل الحاملة (2 للـ trot, 3 للـ walk, 4 للـ stand)
```

**مثال — مشروعنا (14 kg, leg segment 25 cm, 2 legs supporting):**

```
Static torque (knee joint):
  τ = (14 × 9.81 × 0.25) / 2 = 17.2 N·m ≈ 175 kg·cm

With 2× safety factor (dynamic):
  τ_required = 350 kg·cm per knee joint
```

#### 3.2 جدول المحركات الم recommended

| المفصل | الحمل | Servo (Budget) | BLDC (Performance) |
|--------|-------|----------------|---------------------|
| **HAA (Hip Abduction)** | Medium | DS3225 (25 kg·cm) | TMotor MN5212 + ODrive |
| **HFE (Hip Pitch)** | High | DS3218 (20 kg·cm) | Custom QDD actuator |
| **KFE (Knee)** | **Highest** | DS3225 (25 kg·cm) × 2 | TMotor + 5:1 planetary |

**للمشروع بحجمنا (>10 kg) — التوصية:**

| الخيار | المحرك | العزم | الوزن/محرك | السعر | ملاحظات |
|--------|--------|-------|-----------|-------|---------|
| ⭐ **Recommended** | **Dynamixel XM430** | 4.1 N·m (42 kg·cm) | 82g | ~$280 | Closed-loop, daisy-chain |
| Budget | DS3225 Digital | 25 kg·cm | 60g | ~$15 | Open-loop, no feedback |
| Performance | TMotor MN5212 + ODrive | High | 150g | ~$200 | QDD — Stanford Doggo style |
| Industrial | Unitree A1 Actuator | 33.5 N·m | 920g | N/A | Same class as Go1 |

> **12 محرك × 82g (XM430) = 984g** — ~7% من budget الـ 14 kg ✅

#### 3.3 Power Budget

```
Power System Design:
├── Battery: 6S LiPo (22.2V) × 5000mAh
├── Expected current (12 servos active): 8–15A peak
├── BMS: 6S 30A with balance charging
├── Distribution: Power board + fuse per leg
└── Runtime target: 30–60 min (walking)
```

---

### PHASE 4: الإلكترونيات

```
┌──────────────────────────────────────────────┐
│                  BODY CHASSIS                 │
│  ┌────────────┐  ┌──────────┐  ┌──────────┐ │
│  │ Main MCU   │  │   IMU    │  │  Battery │ │
│  │ STM32 H7 / │  │ BNO055 / │  │ 6S LiPo  │ │
│  │ RPi 4      │  │ MPU6050  │  │ 5000mAh  │ │
│  └─────┬──────┘  └────┬─────┘  └──────────┘ │
│        │ I2C/SPI      │ I2C                  │
│  ┌─────▼──────────────▼─────────────────────┐ │
│  │         Motor Driver Board               │ │
│  │  PCA9685 (servo) / ODrive (BLDC)        │ │
│  └─────┬────────────────────────────────────┘ │
│        │ PWM / CAN                            │
│   ┌────┴────┬────────┬────────┐              │
│   FL      FR        RL       RR              │
│  (3 srv) (3 srv)  (3 srv)  (3 srv)          │
└──────────────────────────────────────────────┘
```

| المكون | النموذج الم suggested | الوظيفة | Interface |
|--------|----------------------|---------|-----------|
| **Main Controller** | STM32F407 / Raspberry Pi 4 | Gait + Balance + IK | — |
| **IMU** | BNO055 / BNO085 | Roll, Pitch, Yaw | I2C |
| **Servo Driver** | PCA9685 (16-ch PWM) | 12 servo control | I2C |
| **BLDC Driver** | ODrive v3.6 (×4) | 12 BLDC motors | CAN/UART |
| **Power Distribution** | Custom PDB | Battery → motors | — |
| **E-Stop** | Relay (Gigavac P105) | Emergency stop | GPIO |

---

## ⚖️ نظام التوازن (Balance System)

### لماذا التوازن ضروري؟

كلب رباعي الأرجل **inherently unstable** — مثل العمود على طرف واحد. بدون نظام توازن:
- ❌ يسقط عند أول خطوة
- ❌ لا يستطيع الوقوف على 3 أرجل أثناء المشي
- ❌ لا يتعامل مع uneven terrain

### معمارية نظام التوازن

```
┌─────────────┐     ┌──────────────┐     ┌───────────────┐
│    IMU      │────▶│  Sensor      │────▶│  Balance      │
│  (BNO055)   │     │  Fusion      │     │  Controller   │
│ Roll,Pitch  │     │  (Kalman)    │     │  (PID)        │
└─────────────┘     └──────────────┘     └───────┬───────┘
                                                  │
                    ┌─────────────────────────────▼───────┐
                    │         Body Pose Correction         │
                    │  ΔRoll → adjust left/right legs     │
                    │  ΔPitch → adjust front/rear legs    │
                    └─────────────────┬─────────────────────┘
                                      │
                    ┌─────────────────▼─────────────────────┐
                    │       Inverse Kinematics (IK)          │
                    │  Desired foot positions → Joint angles │
                    └─────────────────┬─────────────────────┘
                                      │
                    ┌─────────────────▼─────────────────────┐
                    │         12 Motor Commands              │
                    └───────────────────────────────────────┘
```

### 5.1 Static Balance (الوقوف الثابت)

```
Algorithm: STATIC_BALANCE
─────────────────────────
INPUT:  imu_roll, imu_pitch, current_joint_angles[12]
OUTPUT: corrected_joint_angles[12]

1. READ IMU → roll, pitch
2. IF |roll| > threshold OR |pitch| > threshold:
     a. Calculate CoG offset from center
     b. Adjust foot positions to bring CoG over support polygon
     c. Solve IK for new foot positions
     d. SEND corrected angles to motors
3. REPEAT at 100 Hz
```

**Support Polygon:** المنطقة التي يلامس فيها الأرض ≥3 أرجل — يجب أن يكون **مركز الثقل (CoG)** داخلها.

### 5.2 Dynamic Balance (التوازن أثناء المشي)

```
Algorithm: TROT_GAIT_WITH_BALANCE
──────────────────────────────────
Phase cycle (4 phases):

Phase 1: FL + RR swing, FR + RL stance
Phase 2: Transition — body shift over stance legs
Phase 3: FR + RL swing, FL + RR stance
Phase 4: Transition — body shift over stance legs

During each phase:
  1. Generate swing trajectory (parabolic arc)
  2. READ IMU → detect body tilt
  3. PID correct stance leg positions
  4. Solve IK → motor commands
  5. Execute at 50-100 Hz control loop
```

### 5.3 PID Controller للتوازن

```
// Balance PID — per axis
error_roll  = target_roll  - imu_roll    // target = 0
error_pitch = target_pitch - imu_pitch   // target = 0

correction_roll  = Kp_r × error_roll  + Ki_r × ∫error_roll  + Kd_r × d(error_roll)/dt
correction_pitch = Kp_p × error_pitch + Ki_p × ∫error_pitch + Kd_p × d(error_pitch)/dt

// Map to foot adjustments
for each leg:
  foot_x_offset = correction_pitch × leg_lever_arm
  foot_y_offset = correction_roll  × leg_lever_arm
  
// Starting PID gains (tune experimentally):
Kp_roll  = 0.5    Ki_roll  = 0.01   Kd_roll  = 0.05
Kp_pitch = 0.5    Ki_pitch = 0.01   Kd_pitch = 0.05
```

### 5.4 Inverse Kinematics (IK)

```
// 3-DOF Leg IK — geometric solution
INPUT:  foot_position(x, y, z) relative to hip
OUTPUT: θ_HAA, θ_HFE, θ_KFE

L1 = hip_link_length
L2 = upper_leg_length (femur)
L3 = lower_leg_length (tibia)

// Step 1: Hip Abduction
θ_HAA = atan2(y, sqrt(x² + z²))

// Step 2: Reach in sagittal plane
d = sqrt(x² + y² + z²)
θ_KFE = acos((L2² + L3² - d²) / (2 × L2 × L3)) - π

// Step 3: Hip Flexion
α = acos((L2² + d² - L3²) / (2 × L2 × d))
β = atan2(z, x)
θ_HFE = α + β

RETURN (θ_HAA, θ_HFE, θ_KFE)
```

---

## 💻 الإلكترونيات والتحكم

### Software Stack

```
Layer 4: [User Interface]     — Web UI / Bluetooth remote / ROS
Layer 3: [Gait Planner]       — Walk, Trot, Stand, Sit commands
Layer 2: [Balance Controller] — PID + IMU feedback
Layer 1: [Motor Interface]    — PWM / CAN commands to drivers
Layer 0: [Hardware]           — MCU, IMU, Motors, Battery
```

### Control Loop Timing

| Loop | Frequency | Task |
|------|-----------|------|
| **Motor command** | 500 Hz | Send PWM/CAN to motors |
| **Balance PID** | 100 Hz | IMU read + correction |
| **Gait planner** | 50 Hz | Foot trajectory update |
| **User input** | 20 Hz | Command processing |

---

## 🔩 خطوات التجميع العملية

> **الترتيب:** من الداخل إلى الخارج (عكس Exploded View) — كما في [فيديو YouTube](https://youtu.be/dGe0Nn-FiPE)

```
Step 13 → Step 1  (Assembly order)
─────────────────────────────────

 STEP 13 │ تركيب الهيكل الداخلي (Internal mounting plates)
 STEP 12 │ تثبيت لوحة توزيع الطاقة (Power distribution board)
 STEP 11 │ تركيب حامل البطارية (Battery mount)
 STEP 10 │ تثبيت Main Controller + IMU
 STEP  9 │ تركيب القاعدة السفلية (Bottom plate)
 STEP  8 │ تثبيت محركات الورك ×4 (Hip motors at body corners)
 STEP  7 │ تركيب Hip Links ×4
 STEP  6 │ تثبيت محركات الفخذ ×4 (Upper leg motors)
 STEP  5 │ تركيب Upper Leg links ×4
 STEP  4 │ تثبيت محركات الركبة ×4 (Knee motors)
 STEP  3 │ تركيب Lower Leg links ×4
 STEP  2 │ تركيب Foot pads ×4
 STEP  1 │ تثبيت الغطاء العلوي + المسامير النهائية (Top cover + final fasteners)
```

### قائمة فحص ما قبل التشغيل (Pre-Flight Checklist)

- [ ] جميع المسامير مُشَدّدة بالعزم الصحيح
- [ ] المحركات تتحرك في الاتجاه الصحيح (zero calibration)
- [ ] IMU مُثبّت على مركز الجسم ومعاير
- [ ] البطارية مشحونة + BMS يعمل
- [ ] E-Stop يعمل
- [ ] Static balance test — 4 legs on ground, body level
- [ ] Single leg lift test — 3 legs stand stable
- [ ] Walk test — slow trot on flat surface

---

## 📊 مقارنة مع مشاريع عالمية

```
                    DOF    Length   Height   Mass    Cost
                    ───    ──────   ──────   ────    ────
Our Project  ████  12-14   50-70cm  30-40cm  <14kg   $$
Unitree Go1  ████   12     58cm     40cm    12kg    $$$$
Mini Cheetah ████   12     48cm     30cm     9kg    Research
Stanford Doggo ███   8     40cm     20cm    4.8kg   $
SpotMicro    ████   12     30cm     15cm     2kg    $
Boston Spot  ████   12     84cm     61cm    32kg    $$$$$

                    ▲
                    └── Our project targets the "large hobby/research" segment
```

| Feature | SpotMicro | Stanford Doggo | MIT Mini Cheetah | **مشروعنا** |
|---------|-----------|----------------|------------------|-------------|
| DOF | 12 | 8 | 12 | **12–14** |
| Mass | 2 kg | 4.8 kg | 9 kg | **<14 kg** |
| Actuation | SG90 Servo | TMotor QDD | Custom BLDC | **Servo/BLDC** |
| Balance | Basic gyro | IMU + custom | Full dynamic | **IMU + PID** |
| Open Source | ✅ | ✅ | Partial | **✅** |
| Size class | Mini | Small | Medium | **Large** |

---

## 📚 المصادر والمراجع

### مشاريع مرجعية

1. **Stanford Doggo** — [GitHub](https://github.com/Nate711/StanfordDoggoProject) · [Paper (ICRA 2019)](https://arxiv.org/pdf/1905.04254)
2. **MIT Mini Cheetah** — [Biomimetic Robotics Lab](https://biomimetic.mit.edu/research)
3. **SpotMicro (Original)** — [Thingiverse #3445283](https://www.thingiverse.com/thing:3445283)
4. **SpotMicro ROS** — [mike4192/spotMicro](https://github.com/mike4192/spotMicro)
5. **Nova Spot Micro 3** — [Instructables Guide](https://www.instructables.com/Nova-Spot-Micro-a-Spot-Mini-Clone/)
6. **Quadruped Controller** — [Cyclemnt/quadruped-controller](https://github.com/Cyclemnt/quadruped-controller)
7. **Quadruped Stabilization** — [smnizza/quadruped-robot-stabilization](https://github.com/smnizza/quadruped-robot-stabilization)

### أوراق علمية

8. Bjelonic et al., *"Stanford Doggo: An Open-Source, Quasi-Direct-Drive Quadruped"*, ICRA 2019
9. Li et al., *"Balance Control of a Quadruped Robot Based on Foot Fall Adjustment"*, Applied Sciences 2022
10. *"Training and Transfer of a PID Balance Controller for Quadruped Robots"*, SciELO 2024

### CAD & Onshape

11. [Onshape Exploded Views Documentation](https://cad.onshape.com/help/Content/Assembly/exploded_views.htm)
12. [Working with Imported CAD (SLDPRT)](https://cad.onshape.com/help/Content/Document/working_with_imported_cad.htm)
13. [10 Advanced Tips for Exploded Views](https://www.onshape.com/en/resource-center/tech-tips/10-advanced-tips-for-creating-exploded-views)

### Actuators & Motors

14. [Servo Torque Calculator](https://usecalcpro.com/tools/servo-motor-calculator)
15. [ROBOTIS Dynamixel Torque Guide](https://www.robotis.us/robotis-blog/torque-ratings/)
16. [Society of Robots — Torque Calculation](https://www.societyofrobots.com/robotforum/index.php?topic=8005.0)

### مراجع بصرية

17. [Boston Dynamics Spot](https://bostondynamics.com/products/spot/)
18. [Unitree Go1](https://www.unitree.com/products/go1)
19. [SpotMicro Community Gallery](https://www.personalrobots.biz/spotmicro-is-an-open-source-robotic-dog/)

> 📄 **قائمة مصادر موسعة:** [`docs/SOURCES.md`](docs/SOURCES.md)

---

## 📁 هيكل المستودع

```
Mechanics/
├── README.md                 ← Main documentation (Part 1 + Part 2)
├── LICENSE                   ← MIT License
├── docs/
│   ├── ALGORITHM.md          ← Deep dive: flowcharts, pseudocode, PID
│   ├── ASSEMBLY.md           ← Exploded view steps + BOM
│   └── SOURCES.md            ← Full reference list (19+ sources)
├── assets/
│   └── reference-images/     ← Visual references + kinematics diagrams
└── .gitignore
```

---

## 👤 المشروع

| | |
|---|---|
| **البرنامج** | Smart Methods — Mechanics |
| **المهمة** | Task 5 — Robotic Dog |
| **Onshape** | [View Assembly](https://cad.onshape.com/documents/b26f18ef0000550a5887d678/w/48a6352dd9256db373579cc0/e/41e000305039f0b434641ddc) |
| **YouTube** | [Exploded View Video](https://youtu.be/dGe0Nn-FiPE) |

---

<p align="center">
  <b>Smart Methods — الأساليب الذكية</b><br>
  <i>Mechanics Program · 2026</i>
</p>
