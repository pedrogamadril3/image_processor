#include "edge_detection.h"

#include "grayscale.h"

namespace {

const Matrix EDGE_MATRIX = {
    {0, -1, 0},
    {-1, 4, -1},
    {0, -1, 0},
};

const Color WHITE = {1, 1, 1};
const Color BLACK = {0, 0, 0};

}  // namespace

EdgeDetection::EdgeDetection(float threshold) : MatrixFilter(EDGE_MATRIX), threshold_(threshold) {
}

void EdgeDetection::Apply(Image& image) const {
    Grayscale().Apply(image);
    MatrixFilter::Apply(image);
    for (size_t y = 0; y < image.Height(); ++y) {
        for (size_t x = 0; x < image.Width(); ++x) {
            image.At(x, y) = image.At(x, y).r > threshold_ ? WHITE : BLACK;
        }
    }
}
