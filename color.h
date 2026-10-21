#pragma once

struct Color {
    float b = 0;
    float g = 0;
    float r = 0;

    Color& operator+=(const Color& other);
    Color& operator*=(float factor);

    Color Clamped() const;
};

Color operator+(Color fir, const Color& sec);
Color operator*(Color color, float factor);
bool operator==(const Color& fir, const Color& sec);
