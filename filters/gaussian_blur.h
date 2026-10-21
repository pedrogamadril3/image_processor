#pragma once

#include "matrix_filter.h"

class GaussianBlur : public MatrixFilter {
public:
    explicit GaussianBlur(float sigma);
};
