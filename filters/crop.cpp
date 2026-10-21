#include "crop.h"

#include <algorithm>

Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
}

void Crop::Apply(Image& image) const {
    const size_t new_width = std::min(width_, image.Width());
    const size_t new_height = std::min(height_, image.Height());
    Image result(new_width, new_height);
    for (size_t y = 0; y < new_height; ++y) {
        for (size_t x = 0; x < new_width; ++x) {
            result.At(x, y) = image.At(x, y);
        }
    }
    image = result;
}
