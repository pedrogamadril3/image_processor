#pragma once

#include <cstddef>

#include "filter.h"

class Pixellate : public Filter {
public:
    explicit Pixellate(size_t block_size);

    void Apply(Image& image) const override;

private:
    size_t block_size_;
};
