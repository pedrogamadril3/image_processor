#include "negative.h"

Color Negative::TransformPixel(const Color& color) const {
    return {1 - color.b, 1 - color.g, 1 - color.r};
}
