#pragma once

#include "pixel_filter.h"

class Negative : public PixelFilter {
public:
    Color TransformPixel(const Color& color) const override;
};
