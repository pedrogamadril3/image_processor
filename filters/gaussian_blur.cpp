#include "gaussian_blur.h"

#include <cmath>
#include <numbers>

namespace {

    const float RADIUS_IN_SIGMAS = 3;

    Matrix MakeGaussianMatrix(float sigma) {
        const auto radius = static_cast<int>(std::ceil(RADIUS_IN_SIGMAS * sigma));
        const auto size = static_cast<size_t>(2 * radius + 1);
        Matrix matrix(size, std::vector<float>(size));
        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                const auto distance_squared = static_cast<float>(dx * dx + dy * dy);
                const float weight = std::exp(-distance_squared / (2 * sigma * sigma));
                matrix[dy + radius][dx + radius] = weight;
            }
        }
        for (auto& row : matrix) {
            for (float& weight : row) {
                weight /= 2 * std::numbers::pi_v<float> * sigma * sigma;
            }
        }
        return matrix;
    }

}  // namespace

GaussianBlur::GaussianBlur(float sigma) : MatrixFilter(MakeGaussianMatrix(sigma)) {
}
