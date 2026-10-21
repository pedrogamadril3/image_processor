cmake_minimum_required(VERSION 3.20)
project(image_processor CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(image_processor
    image_processor.cpp
    color.cpp
    image.cpp
    bmp.cpp
    args_parser.cpp
    filter_factory.cpp
        filters/pixel_filter.cpp
        filters/grayscale.cpp
        filters/negative.cpp
        filters/matrix_filter.cpp
        filters/sharpening.cpp
        filters/edge_detection.cpp
        filters/crop.cpp
        filters/gaussian_blur.cpp
        filters/pixellate.cpp
)
