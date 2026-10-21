#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "args_parser.h"
#include "bmp.h"
#include "errors.h"
#include "filter_factory.h"

namespace {

void Run(const std::vector<std::string_view>& params) {
    const Args args = ParseArgs(params);
    Image image = ReadBmp(std::string(args.input));
    for (const OneFilter& spec : args.filters) {
        CreateFilter(spec)->Apply(image);
    }
    WriteBmp(image, std::string(args.output));
}

}  // namespace

std::string FiltersHelp() {
    return "Usage: image_processor input.bmp output.bmp [-filter params...]\n"
           "Filters:\n"
           "  -crop width height   keep the top-left part of the given size\n"
           "  -gs                  grayscale\n"
           "  -neg                 negative\n"
           "  -sharp               sharpening\n"
           "  -edge threshold      edge detection, threshold in [0, 1]\n"
           "  -blur sigma          gaussian blur\n"
           "  -pixellate size      pixellate with square blocks of the given size\n";
}

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << FiltersHelp();
        return 0;
    }
    try {
        Run(std::vector<std::string_view>(argv + 1, argv + argc));
    } catch (const ImageProcessorError& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
