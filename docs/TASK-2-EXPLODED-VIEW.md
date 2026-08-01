# الTask 2 — Exploded View (Onshape + YouTube)

> **Smart Methods — Mechanics · Task 5 · Part 2**  
> تجميعة **BodyV2** على Onshape — **مستقلة** عن بحث مشروع العام في الTask 1

---

## ⚠️ هذا الملف ≠ مشروع العام الكبير

| | الTask 2 (هذا الملف) | الTask 1 |
|---|----------------------|----------|
| **المشروع** | تجميعة Onshape (BodyV2) | مشروع هذا العام — كلب كبير |
| **الغرض** | Exploded View + فيديو | بحث + خوارزمية |
| **المواصفات** | تجميعة المهمة/التدريب | 50–70 cm · <14 kg · 12–14 DOF |

**لا يُفترض أن تطابق تجميعة Onshape مواصفات مشروع العام.**

---

## 🔗 Deliverables

| المورد | الرابط | الوصف |
|--------|--------|-------|
| **Onshape Assembly** | [فتح التجميعة ↗](https://cad.onshape.com/documents/b26f18ef0000550a5887d678/w/48a6352dd9256db373579cc0/e/41e000305039f0b434641ddc) | BodyV2 — **View Only** |
| **YouTube Video** | [Exploded View ↗](https://youtu.be/dGe0Nn-FiPE) | فيديو تجميع/تفكيك |

> **الوصول:** الرابط View Only — التعديل يحتاج إضافة email من المالك (لم يُطلب من المهندس).

---

## 📦 Exploded View — 13 Steps

**Exploded View واحد** — ترتيب من **الخارج للداخل** (تفكيك):

| Step | المكون |
|------|--------|
| 1 | الغطاء العلوي + المسامير النهائية |
| 2 | Foot Pads ×4 |
| 3 | Lower Leg Links ×4 |
| 4 | Knee joints ×4 |
| 5 | Upper Leg Links ×4 |
| 6 | Hip pitch joints ×4 |
| 7 | Hip Links ×4 |
| 8 | Hip abduction joints ×4 |
| 9 | Bottom Plate |
| 10 | Main Controller + IMU mount |
| 11 | Battery Mount |
| 12 | Power Distribution Board |
| 13 | Internal Mounting Plates |

### ترتيب التركيب (عكس Explode)

```
Step 13 → 12 → 11 → ... → 2 → 1
(من الداخل للخارج)
```

---

## 🎬 طريقة تصوير الفيديو

```
1. Onshape → Exploded Views → Activate view
2. Rollback bar → Roll to end (fully exploded)
3. Win + G → Record
4. Click rollback bar → ↑ step-by-step (13×)
5. Stop → Upload YouTube
```

**الفيديو:** [youtu.be/dGe0Nn-FiPE](https://youtu.be/dGe0Nn-FiPE)

---

## 🧩 مكونات التجميعة (BodyV2)

| # | المكون |
|---|--------|
| 1 | Body Top Cover |
| 2 | Body Bottom Plate |
| 3 | Side Panels ×4 |
| 4 | Internal Mounting Plates |
| 5 | Leg Assemblies ×4 |
| 6 | Hip Joints ×4 |
| 7 | Upper Leg Links ×4 |
| 8 | Lower Leg Links ×4 |
| 9 | Foot Pads ×4 |
| 10 | Fasteners (Screws/Bolts) |

---

## ⚠️ ملاحظات التصميم

### 1. الأرجل — خروج من الجهة المقابلة

الأرجل تظهر خارجة من **الجهة المقابلة** للوضع الم ideal.

- **السبب:** المسامير المُوردة **بمقاسات غير صحيحة** — من جهة التوريد
- **الحل:** استبدال المسامير بالمقاس الصحيح عند التجميع الفعلي

### 2. لماذا لم يُسمح بتعديل الملف؟

ملفات **`.SLDPRT`** (SolidWorks) تُ import كـ **Dumb Solid**:

| ✅ يُ import | ❌ لا يُ import |
|-------------|----------------|
| الهندسة 3D | Feature Tree |
| Direct Edit محدود | Sketches الأصلية |
| Features جديدة فوقها | Parametric History |

**+** الرابط **View Only** — لا تعديل أصلاً.

> [Onshape — Working with Imported CAD](https://cad.onshape.com/help/Content/Document/working_with_imported_cad.htm)

---

## 📚 مصادر (CAD & Exploded View)

1. [Onshape Exploded Views Help](https://cad.onshape.com/help/Content/Assembly/exploded_views.htm)
2. [10 Tips for Exploded Views](https://www.onshape.com/en/resource-center/tech-tips/10-advanced-tips-for-creating-exploded-views)
3. [Imported CAD Limitations](https://cad.onshape.com/help/Content/Document/working_with_imported_cad.htm)

> قائمة كاملة: [`SOURCES.md`](SOURCES.md) — قسم الTask 2

---

[← الفهرس الرئيسي](../README.md) · [الTask 1 — Algorithm →](TASK-1-ALGORITHM.md)
