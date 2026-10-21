#pragma once

#include <string>

#include "image.h"

Image ReadBmp(const std::string &path);

void WriteBmp(const Image &image, const std::string &path);
