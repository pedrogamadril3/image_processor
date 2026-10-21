#include "pixellate.h"

#include <algorithm>

namespace {

Color AverageColor(const Image& image, size_t left, size_t top, size_t right, size_t bottom) {
    Color sum;
    for (size_t y = top; y < bottom; ++y) {
        for (size_t x = left; x < right; ++x) {
            sum += image.At(x, y);
        }
    }
    const auto pixel_count = static_cast<float>((right - left) * (bottom - top));
    return sum * (1 / pixel_count);
}

void FillBlock(Image& image, size_t left, size_t top, size_t right, size_t bottom, const Color& color) {
    for (size_t y = top; y < bottom; ++y) {
        for (size_t x = left; x < right; ++x) {
            image.At(x, y) = color;
        }
    }
}

}  // namespace

Pixellate::Pixellate(size_t block_size) : block_size_(block_size) {
}

void Pixellate::Apply(Image& image) const {
    for (size_t top = 0; top < image.Height(); top += block_size_) {
        const size_t bottom = std::min(top + block_size_, image.Height());
        for (size_t left = 0; left < image.Width(); left += block_size_) {
            const size_t right = std::min(left + block_size_, image.Width());
            const Color average = AverageColor(image, left, top, right, bottom);
            FillBlock(image, left, top, right, bottom, average);
        }
    }
}
