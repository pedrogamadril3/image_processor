#pragma once

#include <stdexcept>
#include <string>

class ImageProcessorError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class FileError : public ImageProcessorError {
public:
    using ImageProcessorError::ImageProcessorError;
};

class BmpFormatError : public FileError {
public:
    using FileError::FileError;
};

class ArgumentsError : public ImageProcessorError {
public:
    using ImageProcessorError::ImageProcessorError;
};

class UnknownFilterError : public ArgumentsError {
public:
    explicit UnknownFilterError() : ArgumentsError("unknown filter") {
    }
};

class FilterParametersError : public ArgumentsError {
public:
    FilterParametersError(const std::string& filter_name, const std::string& message)
        : ArgumentsError("filter -" + filter_name + ": " + message) {
    }
};
