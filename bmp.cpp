#include "bmp.h"

#include <cmath>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <filesystem>

#include "errors.h"

namespace {

const uint16_t BMP_SIGNATURE = 0x4D42;
const uint32_t FILE_HEADER_SIZE = 14;
const uint32_t INFO_HEADER_SIZE = 40;
const uint16_t COLOR_PLANES = 1;
const uint16_t BITS_PER_PIXEL = 24;
const uint32_t BYTES_PER_PIXEL = 3;
const uint32_t NO_COMPRESSION = 0;
const uint32_t ROW_ALIGNMENT = 4;
const int32_t PIXELS_PER_METER = 2835;
const float MAX_Color_VALUE = 255.0f;
const int BITS_IN_BYTE = 8;

struct FileHeader {
    uint16_t signature = 0;
    uint32_t file_size = 0;
    uint32_t first_pixel_byte = 0;
};

struct InfoHeader {
    uint32_t header_size = 0;
    int32_t width = 0;
    int32_t height = 0;
    uint16_t planes = 0;
    uint16_t bits_per_pixel = 0;
    uint32_t compression = 0;
    uint32_t colors_used = 0;
};

template <typename T>
T ReadTo(std::istream& in) {
    unsigned char bytes[sizeof(T)];
    if (!in.read(reinterpret_cast<char*>(bytes), sizeof(T))) {
        throw BmpFormatError("unexpected end of file while reading BMP");
    }
    uint64_t value = 0;
    for (size_t i = 0; i < sizeof(T); ++i) {
        value |= static_cast<uint64_t>(bytes[i]) << (BITS_IN_BYTE * i);
    }
    return static_cast<T>(value);
}

template <typename T>
void WriteTo(std::ostream& out, T value) {
    const auto unsigned_value = static_cast<uint64_t>(value);
    unsigned char bytes[sizeof(T)];
    for (size_t i = 0; i < sizeof(T); ++i) {
        bytes[i] = static_cast<unsigned char>(unsigned_value >> (BITS_IN_BYTE * i));
    }
    out.write(reinterpret_cast<const char*>(bytes), sizeof(T));
}

uint64_t RowSizeInFile(uint64_t width) {
    const uint64_t raw_size = width * BYTES_PER_PIXEL;
    return (raw_size + ROW_ALIGNMENT - 1) / ROW_ALIGNMENT * ROW_ALIGNMENT;
}

float ByteToColor(unsigned char byte) {
    return static_cast<float>(byte) / MAX_Color_VALUE;
}

unsigned char ColorToByte(float color) {
    return static_cast<unsigned char>(std::lround(color * MAX_Color_VALUE));
}

FileHeader ReadFileHeader(std::istream& in) {
    FileHeader header;
    header.signature = ReadTo<uint16_t>(in);
    if (header.signature != BMP_SIGNATURE) {
        throw BmpFormatError("not a BMP file");
    }
    header.file_size = ReadTo<uint32_t>(in);
    ReadTo<uint32_t>(in);
    header.first_pixel_byte = ReadTo<uint32_t>(in);
    return header;
}

InfoHeader ReadInfoHeader(std::istream& in) {
    InfoHeader header;
    header.header_size = ReadTo<uint32_t>(in);
    header.width = static_cast<int32_t>(ReadTo<uint32_t>(in));
    header.height = static_cast<int32_t>(ReadTo<uint32_t>(in));
    header.planes = ReadTo<uint16_t>(in);
    header.bits_per_pixel = ReadTo<uint16_t>(in);
    header.compression = ReadTo<uint32_t>(in);
    ReadTo<uint32_t>(in);
    ReadTo<uint32_t>(in);
    ReadTo<uint32_t>(in);
    header.colors_used = ReadTo<uint32_t>(in);
    ReadTo<uint32_t>(in);
    return header;
}

void ValidateInfoHeader(const InfoHeader& header) {
    if (header.header_size < INFO_HEADER_SIZE) {
        throw BmpFormatError("isn't needed header_size");
    }
    if (header.planes != COLOR_PLANES) {
        throw BmpFormatError("invalid number of color planes");
    }
    if (header.bits_per_pixel != BITS_PER_PIXEL) {
        throw BmpFormatError("isn't 24-bit BMP");
    }
    if (header.compression != NO_COMPRESSION) {
        throw BmpFormatError("compressed BMP isn't supported");
    }
    if (header.colors_used != 0) {
        throw BmpFormatError("BMP with a color table isn't supported");
    }
    if (header.width <= 0) {
        throw BmpFormatError("invalid image width");
    }
    if (header.height == 0 || header.height == std::numeric_limits<int32_t>::min()) {
        throw BmpFormatError("invalid image height");
    }
}

}  // namespace

Image ReadBmp(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw FileError("cannot open input file: " + path);
    }
    const FileHeader file_header = ReadFileHeader(in);
    const InfoHeader info_header = ReadInfoHeader(in);
    ValidateInfoHeader(info_header);

    const auto width = static_cast<uint64_t>(info_header.width);
    const bool is_bottom_up = info_header.height > 0;
    const auto height = static_cast<uint64_t>(is_bottom_up ? info_header.height : -info_header.height);
    const uint64_t row_size = RowSizeInFile(width);

    if (file_header.first_pixel_byte < static_cast<uint64_t>(FILE_HEADER_SIZE) + info_header.header_size ||
        file_header.first_pixel_byte + row_size * height > std::filesystem::file_size(path)) {
        throw BmpFormatError("file isn't right");
        }

    Image image(width, height);
    in.seekg(file_header.first_pixel_byte);
    const uint64_t padding = row_size - width * BYTES_PER_PIXEL;
    for (uint64_t row_index = 0; row_index < height; ++row_index) {
        const uint64_t y = is_bottom_up ? height - 1 - row_index : row_index;
        for (uint64_t x = 0; x < width; ++x) {
            const auto blue = static_cast<unsigned char>(in.get());
            const auto green = static_cast<unsigned char>(in.get());
            const auto red = static_cast<unsigned char>(in.get());
            image.At(x, y) = {ByteToColor(blue), ByteToColor(green), ByteToColor(red)};
        }
        in.ignore(static_cast<std::streamsize>(padding));
    }
    return image;
}

void WriteBmp(const Image& image, const std::string& path) {
    if (image.IsEmpty()) {
        throw BmpFormatError("cannot save an empty image");
    }
    if (image.Width() > static_cast<size_t>(std::numeric_limits<int32_t>::max()) ||
        image.Height() > static_cast<size_t>(std::numeric_limits<int32_t>::max())) {
        throw BmpFormatError("image is too large for BMP");
    }
    const uint64_t row_size = RowSizeInFile(image.Width());
    const uint64_t pixel_data_size = row_size * image.Height();
    const uint64_t first_pixel_byte = FILE_HEADER_SIZE + INFO_HEADER_SIZE;
    const uint64_t file_size = first_pixel_byte + pixel_data_size;
    if (file_size > std::numeric_limits<uint32_t>::max()) {
        throw BmpFormatError("image is too large for BMP");
    }

    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw FileError("cannot open output file: " + path);
    }

    WriteTo(out, BMP_SIGNATURE);
    WriteTo(out, static_cast<uint32_t>(file_size));
    WriteTo(out, static_cast<uint32_t>(0));
    WriteTo(out, static_cast<uint32_t>(first_pixel_byte));

    WriteTo(out, INFO_HEADER_SIZE);
    WriteTo(out, static_cast<int32_t>(image.Width()));
    WriteTo(out, static_cast<int32_t>(image.Height()));
    WriteTo(out, COLOR_PLANES);
    WriteTo(out, BITS_PER_PIXEL);
    WriteTo(out, NO_COMPRESSION);
    WriteTo(out, static_cast<uint32_t>(pixel_data_size));
    WriteTo(out, PIXELS_PER_METER);
    WriteTo(out, PIXELS_PER_METER);
    WriteTo(out, static_cast<uint32_t>(0));
    WriteTo(out, static_cast<uint32_t>(0));

    const uint64_t padding = row_size - image.Width() * BYTES_PER_PIXEL;
    for (size_t row_index = 0; row_index < image.Height(); ++row_index) {
        const size_t y = image.Height() - 1 - row_index;
        for (size_t x = 0; x < image.Width(); ++x) {
            const Color color = image.At(x, y).Clamped();
            out.put(static_cast<char>(ColorToByte(color.b)));
            out.put(static_cast<char>(ColorToByte(color.g)));
            out.put(static_cast<char>(ColorToByte(color.r)));
        }
        for (uint64_t i = 0; i < padding; ++i) {
            out.put(0);
        }
    }
    if (!out) {
        throw FileError("failed to write output file: " + path);
    }
}
