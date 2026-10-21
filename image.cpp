#include "image.h"

#include <algorithm>

Image::Image(size_t width, size_t height) : width_(width), height_(height), pixels_(width * height) {
}

size_t Image::Width() const {
    return width_;
}

size_t Image::Height() const {
    return height_;
}

bool Image::IsEmpty() const {
    return pixels_.empty();
}

Color& Image::At(size_t x, size_t y) {
    return pixels_[y * width_ + x];
}

const Color& Image::At(size_t x, size_t y) const {
    return pixels_[y * width_ + x];
}

const Color& Image::NearestAt(int64_t x, int64_t y) const {
    const int64_t max_x = static_cast<int64_t>(width_) - 1;
    const int64_t max_y = static_cast<int64_t>(height_) - 1;
    return At(static_cast<size_t>(std::clamp<int64_t>(x, 0, max_x)),
              static_cast<size_t>(std::clamp<int64_t>(y, 0, max_y)));
}
