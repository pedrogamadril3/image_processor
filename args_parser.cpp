#include "args_parser.h"

#include <string>

#include "errors.h"

namespace {
    bool IsFilterName(std::string_view symbol) {
        return !symbol.empty() && symbol.front() == '-';
    }
} // namespace

Args ParseArgs(const std::vector<std::string_view> &params) {
    if (params.size() < 2) {
        throw ArgumentsError("not enough parameters");
    }
    Args args;
    args.input = params[0];
    args.output = params[1];

    size_t i = 2;
    while (i < params.size()) {
        if (!IsFilterName(params[i])) {
            throw ArgumentsError("got filter name without '-'");
        }
        OneFilter filter;
        filter.name = params[i].substr(1);
        if (filter.name.empty()) {
            throw ArgumentsError("empty filter name");
        }
        ++i;
        while (i < params.size() && !IsFilterName(params[i])) {
            filter.params.push_back(params[i]);
            ++i;
        }
        args.filters.push_back(filter);
    }
    return args;
}
