#include "filter_factory.h"

#include <cmath>
#include <map>
#include <stdexcept>

#include "filters/crop.h"
#include "filters/edge_detection.h"
#include "errors.h"
#include "filters/gaussian_blur.h"
#include "filters/grayscale.h"
#include "filters/negative.h"
#include "filters/pixellate.h"
#include "filters/sharpening.h"

namespace {

using Creator = std::unique_ptr<Filter> (*)(const OneFilter& spec);

void CheckParamCount(const OneFilter& spec, size_t expected) {
    if (spec.params.size() != expected) {
        throw FilterParametersError(
            std::string(spec.name),
            "expected " + std::to_string(expected) + " parameter(s), got " + std::to_string(spec.params.size()));
    }
}

int ParsePositiveInt(const OneFilter& spec, std::string_view text) {
    const std::string str(text);
    size_t parsed = 0;
    int value = 0;
    try {
        value = std::stoi(str, &parsed);
    } catch (const std::logic_error&) {
        parsed = 0;
    }
    if (parsed != str.size() || value <= 0) {
        throw FilterParametersError(std::string(spec.name), "'" + str + "' is not a positive integer");
    }
    return value;
}

float ParseFloat(const OneFilter& spec, std::string_view text) {
    const std::string str(text);
    size_t parsed = 0;
    float value = 0;
    try {
        value = std::stof(str, &parsed);
    } catch (const std::logic_error&) {
        parsed = 0;
    }
    if (parsed != str.size() || !std::isfinite(value)) {
        throw FilterParametersError(std::string(spec.name), "'" + str + "' is not a number");
    }
    return value;
}

std::unique_ptr<Filter> CreateCrop(const OneFilter& spec) {
    CheckParamCount(spec, 2);
    const int width = ParsePositiveInt(spec, spec.params[0]);
    const int height = ParsePositiveInt(spec, spec.params[1]);
    return std::make_unique<Crop>(width, height);
}

std::unique_ptr<Filter> CreateGrayscale(const OneFilter& spec) {
    CheckParamCount(spec, 0);
    return std::make_unique<Grayscale>();
}

std::unique_ptr<Filter> CreateNegative(const OneFilter& spec) {
    CheckParamCount(spec, 0);
    return std::make_unique<Negative>();
}

std::unique_ptr<Filter> CreateSharpening(const OneFilter& spec) {
    CheckParamCount(spec, 0);
    return std::make_unique<Sharpening>();
}

std::unique_ptr<Filter> CreateEdgeDetection(const OneFilter& spec) {
    CheckParamCount(spec, 1);
    const float threshold = ParseFloat(spec, spec.params[0]);
    if (threshold < 0 || threshold > 1) {
        throw FilterParametersError(std::string(spec.name), "threshold isn't in [0, 1]");
    }
    return std::make_unique<EdgeDetection>(threshold);
}

std::unique_ptr<Filter> CreateGaussianBlur(const OneFilter& spec) {
    CheckParamCount(spec, 1);
    const float sigma = ParseFloat(spec, spec.params[0]);
    if (sigma <= 0) {
        throw FilterParametersError(std::string(spec.name), "sigma less or equal than zero");
    }
    return std::make_unique<GaussianBlur>(sigma);
}

std::unique_ptr<Filter> CreatePixellate(const OneFilter& spec) {
    CheckParamCount(spec, 1);
    const int block_size = ParsePositiveInt(spec, spec.params[0]);
    return std::make_unique<Pixellate>(block_size);
}

const std::map<std::string_view, Creator> FILTERS = {
    {"crop", {CreateCrop}},
    {"gs", {CreateGrayscale}},
    {"neg", {CreateNegative}},
    {"sharp", {CreateSharpening}},
    {"edge", {CreateEdgeDetection}},
    {"blur", {CreateGaussianBlur}},
    {"pixellate", {CreatePixellate}},
};

}  // namespace

std::unique_ptr<Filter> CreateFilter(const OneFilter& spec) {
    const auto it = FILTERS.find(spec.name);
    if (it == FILTERS.end()) {
        throw UnknownFilterError();
    }
    return it->second(spec);
}