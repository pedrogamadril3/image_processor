#include "grayscale.h"

namespace {

const float BLUE_WEIGHT = 0.114f;
const float GREEN_WEIGHT = 0.587f;
const float RED_WEIGHT = 0.299f;

}  // namespace

Color Grayscale::TransformPixel(const Color& color) const {
    const float gray = RED_WEIGHT * color.r + GREEN_WEIGHT * color.g + BLUE_WEIGHT * color.b;
    return {gray, gray, gray};
}
