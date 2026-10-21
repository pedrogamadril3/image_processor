#pragma once

#include "pixel_filter.h"

class Grayscale : public PixelFilter {
public:
    Color TransformPixel(const Color& color) const override;
};
