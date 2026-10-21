#pragma once

#include <string_view>
#include <vector>

struct OneFilter {
    std::string_view name;
    std::vector<std::string_view> params;
};

struct Args {
    std::string_view input;
    std::string_view output;
    std::vector<OneFilter> filters;
};

Args ParseArgs(const std::vector<std::string_view> &params);
