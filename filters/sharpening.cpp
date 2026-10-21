#include "sharpening.h"

namespace {

const Matrix SHARPENING_MATRIX = {
    {0, -1, 0},
    {-1, 5, -1},
    {0, -1, 0},
};

}  // namespace

Sharpening::Sharpening() : MatrixFilter(SHARPENING_MATRIX) {
}
