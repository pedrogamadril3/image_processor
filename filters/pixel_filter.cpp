#include "pixel_filter.h"

void PixelFilter::Apply(Image& image) const {
    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            image.At(x, y) = TransformPixel(image.At(x, y));
        }
    }
}
