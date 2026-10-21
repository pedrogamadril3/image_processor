#pragma once

#include <vector>

#include "filter.h"

using Matrix = std::vector<std::vector<float>>;

class MatrixFilter : public Filter {
public:
    explicit MatrixFilter(Matrix matrix);

    void Apply(Image& image) const override;

private:
    Matrix matrix_;
};
