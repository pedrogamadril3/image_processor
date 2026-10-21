#include "color.h"

#include <algorithm>

namespace {
    float ClampColor(float value) {
        return std::min(std::max(value, 0.0f), 1.0f);
    }
} // namespace

Color &Color::operator+=(const Color &other) {
    b += other.b;
    g += other.g;
    r += other.r;
    return *this;
}

Color &Color::operator*=(float factor) {
    b *= factor;
    g *= factor;
    r *= factor;
    return *this;
}

Color Color::Clamped() const {
    return {ClampColor(b), ClampColor(g), ClampColor(r)};
}

Color operator+(Color fir, const Color &sec) {
    fir += sec;
    return fir;
}

Color operator*(Color color, float factor) {
    color *= factor;
    return color;
}

bool operator==(const Color &fir, const Color &sec) {
    return fir.b == sec.b && fir.g == sec.g && fir.r == sec.r;
}
