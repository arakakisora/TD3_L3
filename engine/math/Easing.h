#pragma once
#include <cmath>
#include <functional>
#include "Vector3.h"
#include "Vector2.h"

class Easing {
public:
    // イージング関数の型（float t[0〜1] を受け取って float を返す）
    using EasingFunction = std::function<float(float)>;

    // 各種イージング関数
    static float Linear(float t);
    static float EaseInQuad(float t);
    static float EaseOutQuad(float t);
    static float EaseInOutQuad(float t);
    static float EaseInCubic(float t);
    static float EaseOutCubic(float t);
    static float EaseInOutCubic(float t);

    // 線形補間（Lerp）
    static float Lerp(float a, float b, float t);

    // イージング付き補間：EasingLerp
    static float EaseLerp(float a, float b, float t, EasingFunction easingFunc);

    // Vector2のイージング付き補間
    static Vector2 EaseLerp(const Vector2& a, const Vector2& b, float t, Easing::EasingFunction easingFunc);

    // Vector3のイージング付き補間
    static Vector3 EaseLerp(const Vector3& a, const Vector3& b, float t, Easing::EasingFunction easingFunc);
};


