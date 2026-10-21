#pragma once

#include "filter.h"

class PixelFilter : public Filter {
public:
    void Apply(Image& image) const override;

    virtual Color TransformPixel(const Color& color) const = 0;
};
