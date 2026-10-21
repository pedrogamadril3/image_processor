#include "matrix_filter.h"

#include <stdexcept>

MatrixFilter::MatrixFilter(Matrix matrix) : matrix_(std::move(matrix)) {
    if (matrix_.size() % 2 == 0) {
        throw std::invalid_argument("matrix size isn't odd");
    }
    for (const auto& row : matrix_) {
        if (row.size() != matrix_.size()) {
            throw std::invalid_argument("matrix isn't square");
        }
    }
}

void MatrixFilter::Apply(Image& image) const {
    if (image.IsEmpty()) {
        return;
    }
    Image result(image.Width(), image.Height());
    const auto radius = static_cast<int64_t>(matrix_.size() / 2);
    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            Color sum;
            for (int64_t y1 = -radius; y1 <= radius; ++y1) {
                for (int64_t x1 = -radius; x1 <= radius; ++x1) {
                    const float weight = matrix_[y1 + radius][x1 + radius];
                    sum += image.NearestAt(static_cast<int64_t>(x) + x1, static_cast<int64_t>(y) + y1) * weight;
                }
            }
            result.At(x, y) = sum.Clamped();
        }
    }
    image = result;
}
