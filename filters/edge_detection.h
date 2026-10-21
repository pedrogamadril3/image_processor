#pragma once

#include "matrix_filter.h"

class EdgeDetection : public MatrixFilter {
public:
    explicit EdgeDetection(float threshold);

    void Apply(Image& image) const override;

private:
    float threshold_;
};
