# صور مرجعية | Reference Images

> صور روبوتات كلب مرجعية — للاستخدام في التوثيق والعرض

## مشروعنا

| | |
|---|---|
| **Exploded View** | [YouTube Video](https://youtu.be/dGe0Nn-FiPE) |
| **Onshape Assembly** | [View CAD](https://cad.onshape.com/documents/b26f18ef0000550a5887d678/w/48a6352dd9256db373579cc0/e/41e000305039f0b434641ddc) |

## روبوتات مرجعية

### Boston Dynamics Spot / SpotMini
- https://bostondynamics.com/products/spot/
- الشكل المرجعي الأصلي لمعظم مشاريع SpotMicro

### Stanford Doggo
- https://github.com/Nate711/StanfordDoggoProject
- Open-source QDD quadruped — 8 DOF, 4.8 kg

### MIT Mini Cheetah
- https://robotsguide.com/robots/minicheetah
- 12 DOF, 48×30 cm, 9 kg — أقرب spec لمشروعنا

### SpotMicro (Deok-yeon Kim)
- https://www.thingiverse.com/thing:3445283
- 12 servo, open-source 3D printable

### Unitree Go1
- https://www.unitree.com/products/go1
- 58 cm length, 12 kg — commercial large quadruped

## Leg Kinematics Diagram

```
        Body
         │
    [HAA] ← Hip Abduction (فتح/إغلاق)
         │
    Hip Link
         │
    [HFE] ← Hip Flexion (فخذ)
         │
   Upper Leg (Femur)
         │
    [KFE] ← Knee Flexion (ركبة)
         │
   Lower Leg (Tibia)
         │
       Foot
```

## Gait Pattern — Trot

```
Phase 1:  FL↑ RR↑  FR● RL●     (●=stance, ↑=swing)
Phase 2:  FL● RR●  FR↑ RL↑
```
