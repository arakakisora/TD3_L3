#include "Easing.h"

float Easing::Linear(float t) {
    return t;
}

float Easing::EaseInQuad(float t) {
    return t * t;
}

float Easing::EaseOutQuad(float t) {
    return t * (2 - t);
}

float Easing::EaseInOutQuad(float t) {
    return t < 0.5f ? 2 * t * t : -1 + (4 - 2 * t) * t;
}

float Easing::EaseInCubic(float t) {
    return t * t * t;
}

float Easing::EaseOutCubic(float t) {
    t -= 1.0f;
    return t * t * t + 1.0f;
}

float Easing::EaseInOutCubic(float t) {
    return t < 0.5f
        ? 4 * t * t * t
        : (t - 1.0f) * (2 * t - 2) * (2 * t - 2) + 1.0f;
}

float Easing::Lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float Easing::EaseLerp(float a, float b, float t, EasingFunction easingFunc) {
    return Lerp(a, b, easingFunc(t));
}

Vector2 Easing::EaseLerp(const Vector2& a, const Vector2& b, float t, Easing::EasingFunction easingFunc)
{
    float e = easingFunc(t);
    return {
        Easing::Lerp(a.x, b.x, e),
        Easing::Lerp(a.y, b.y, e)
    };
}

Vector3 Easing::EaseLerp(const Vector3& a, const Vector3& b, float t, Easing::EasingFunction easingFunc)
{
    float e = easingFunc(t);
    return {
        Easing::Lerp(a.x, b.x, e),
        Easing::Lerp(a.y, b.y, e),
        Easing::Lerp(a.z, b.z, e)
    };
}


