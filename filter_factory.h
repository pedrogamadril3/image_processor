#pragma once

#include <memory>

#include "args_parser.h"
#include "filters/filter.h"

std::unique_ptr<Filter> CreateFilter(const OneFilter& spec);