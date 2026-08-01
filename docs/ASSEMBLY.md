# دليل التجميع — Assembly Guide

> Part 2 · Exploded View Documentation · Onshape + YouTube

---

## روابط Deliverables

| Deliverable | Link |
|-------------|------|
| **Onshape Assembly** | [Open in Onshape ↗](https://cad.onshape.com/documents/b26f18ef0000550a5887d678/w/48a6352dd9256db373579cc0/e/41e000305039f0b434641ddc) |
| **Exploded View Video** | [Watch on YouTube ↗](https://youtu.be/dGe0Nn-FiPE) |

---

## Exploded View — 13 Steps

الترتيب في **Exploded View** (من الخارج للداخل — تفكيك):

| Step | Component | Description |
|------|-----------|-------------|
| 1 | Top Cover + Final Fasteners | آخر قطعة تُركّب — الغطاء العلوي والمسامير النهائية |
| 2 | Foot Pads ×4 | أقدام/وسادات التلامس الأرضي |
| 3 | Lower Leg Links ×4 | عظام الساق السفلى (Tibia) |
| 4 | Knee Motors ×4 | محركات الركبة (KFE joint) |
| 5 | Upper Leg Links ×4 | عظام الفخذ (Femur) |
| 6 | Hip Pitch Motors ×4 | محركات الفخذ (HFE joint) |
| 7 | Hip Links ×4 | وصلات الورك |
| 8 | Hip Abduction Motors ×4 | محركات الورك (HAA joint) |
| 9 | Bottom Plate | القاعدة السفلية للجسم |
| 10 | Main Controller + IMU | لوحة التحكم وحساس التوازن |
| 11 | Battery Mount | حامل البطارية |
| 12 | Power Distribution Board | لوحة توزيع الطاقة |
| 13 | Internal Mounting Plates | الهيكل الداخلي — أول قطعة تُركّب |

---

## Assembly Order (Reverse — للتركيب الفعلي)

```
Step 13 → Internal structure
Step 12 → Power board
Step 11 → Battery mount
Step 10 → Controller + IMU
Step  9 → Bottom plate
Step  8 → Hip motors (4×)
Step  7 → Hip links (4×)
Step  6 → Upper leg motors (4×)
Step  5 → Upper leg links (4×)
Step  4 → Knee motors (4×)
Step  3 → Lower leg links (4×)
Step  2 → Foot pads (4×)
Step  1 → Top cover + final screws
```

---

## How the Video Was Made

```
Tool:     Onshape Exploded Views + Rollback Bar
Record:   Xbox Game Bar (Win + G)
Method:   Roll to end → Press ↑ step-by-step (13 steps)
Result:   Assembly animation (exploded → assembled)
Upload:   YouTube — https://youtu.be/dGe0Nn-FiPE
```

---

## Design Notes

### Leg Orientation Issue

الأرجل في التجميعة الحالية تظهر خارجة من **الجهة المقابلة** للوضع الم ideal.

- **السبب:** مسامير بمقاسات غير صحيحة من المورد
- **الحل:** استبدال المسامير بالمقاس الصحيح عند البناء الفعلي
- **CAD limitation:** ملف `.SLDPRT` imported كـ dumb solid — لا يمكن تعديل parametric

### Onshape Access

- الرابط **View Only** — للعرض فقط
- التعديل يتطلب إضافة email من مالك المستند
- مطابق لمتطلبات المهندس (لم يُطلب granting edit access)

---

## Bill of Materials (BOM) — Estimated

| # | Part | Qty | Notes |
|---|------|-----|-------|
| 1 | Body Top Cover | 1 | Main chassis |
| 2 | Body Bottom Plate | 1 | Base |
| 3 | Side Panels | 4 | Left/Right × Front/Rear |
| 4 | Internal Mounting Plates | 2-4 | Electronics mount |
| 5 | Hip Joint Assembly | 4 | One per leg |
| 6 | Upper Leg Link | 4 | Femur |
| 7 | Lower Leg Link | 4 | Tibia |
| 8 | Foot Pad | 4 | Ground contact |
| 9 | Fasteners (M3/M4) | ~40 | Screws, nuts, standoffs |
| 10 | Servo/BLDC Motors | 12 | 3 per leg |

---

[← Back to README](../README.md)
