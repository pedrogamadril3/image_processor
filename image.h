#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "color.h"

class Image {
public:
    Image() = default;
    Image(size_t width, size_t height);

    size_t Width() const;
    size_t Height() const;
    bool IsEmpty() const;

    Color& At(size_t x, size_t y);
    const Color& At(size_t x, size_t y) const;

    const Color& NearestAt(int64_t x, int64_t y) const;

private:
    size_t width_ = 0;
    size_t height_ = 0;
    std::vector<Color> pixels_;
};
