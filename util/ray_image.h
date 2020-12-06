#pragma once

// BMP image generator

#define _CRT_SECURE_NO_WARNINGS
#include <stdint.h>
#include <string.h>

#include "ray_misc.h"

namespace raycer {

struct rgb_t {
  uint8_t r, g, b, a;
  rgb_t(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0)
      : r(r), g(g), b(b), a(a) {}
};

template <class _Tx>
size_t writeFile(FILE *file, const _Tx *buf, size_t cnt = 1) {
  return fwrite(buf, sizeof(_Tx), cnt, file);
}

template <class _Tx>
class array {
  _Tx *buf = nullptr;
  size_t length = 0;

 public:
  array() {}
  void resize(size_t size) {
    buf = (_Tx *)realloc(buf, sizeof(_Tx) * size);
    length = size;
  }
  _Tx *data() { return buf; }
  _Tx operator[](int index) const { return buf[index]; }
  _Tx &operator[](int index) { return buf[index]; }
  size_t size() const { return length; }
  ~array() {
    if (buf) free(buf);
  }
};

class BMPImage {
  // structures
#pragma pack(push, 1)
  struct file_hdr_t {
    uint16_t fileType = 0x4d42;
    uint32_t fileSize = 0;
    uint16_t rev1 = 0, rev2 = 0;
    uint32_t offsetData = 0;
  };

  struct info_hdr_t {
    uint32_t size = 0;
    int32_t width = 0, height = 0;
    uint16_t planes = 1, bitCnt = 0;
    uint32_t cmps = 0, sizeImg = 0;
    int32_t xppm = 0, yppm = 0;
    uint32_t clrusd = 0, clrimp = 0;
  };

  struct clr_hdr_t {
    uint32_t redMask = 0x00ff0000;
    uint32_t greenMask = 0x0000ff00;
    uint32_t blueMask = 0x000000ff;
    uint32_t alphaMask = 0xff000000;
    uint32_t colorSpec = 0x73524742;
    uint32_t _[16] = {0};
  };
#pragma pack(pop)

 private:
  file_hdr_t fileHeader;
  info_hdr_t infoHeader;
  clr_hdr_t colorHeader;
  array<uint8_t> data;
  array<rgb_t> buffer;

  uint32_t width, height;
  uint32_t rowStride = 0;

  void checkClrHeader(const clr_hdr_t &cld) {
    clr_hdr_t exp;
    if (cld.redMask != exp.redMask || cld.greenMask != exp.greenMask ||
        cld.blueMask != exp.blueMask || cld.alphaMask != exp.alphaMask)
      panic("incorrect color mask while checking color header");
    if (cld.colorSpec != exp.colorSpec)
      panic("incorrect color space type whilte checking color header");
  }

  void writeHeader(FILE *file) {
    writeFile(file, &fileHeader);
    writeFile(file, &infoHeader);
    if (infoHeader.bitCnt == 32) writeFile(file, &colorHeader);
  }

  void writeHeaderAndData(FILE *file) {
    writeHeader(file);
    writeFile(file, data.data(), data.size());
  }

  uint32_t strideAlign(uint32_t align) {
    uint32_t ns = rowStride;
    while (ns % align != 0) ns++;
    return ns;
  }

  void flush() {
    uint32_t channel = infoHeader.bitCnt / 8;
    for (auto i = 0; i < buffer.size(); ++i) {
      data[channel * i] = buffer[i].b;
      data[channel * i + 1] = buffer[i].g;
      data[channel * i + 2] = buffer[i].r;
      if (channel == 4) data[channel * i + 3] = buffer[i].a;
    }
  }

 public:
  BMPImage(uint32_t width, uint32_t height, bool alpha)
      : width(width), height(height) {
    buffer.resize(width * height);
    infoHeader.width = width;
    infoHeader.height = height;
    if (alpha) {
      infoHeader.size = sizeof(info_hdr_t) + sizeof(clr_hdr_t);
      fileHeader.offsetData =
          sizeof(file_hdr_t) + sizeof(info_hdr_t) + sizeof(clr_hdr_t);
      infoHeader.bitCnt = 32;
      infoHeader.cmps = 3;
      rowStride = width * 4;
      data.resize(rowStride * height);
      fileHeader.fileSize = fileHeader.offsetData + data.size();
    } else {
      infoHeader.size = sizeof(info_hdr_t);
      fileHeader.offsetData = sizeof(file_hdr_t) + sizeof(info_hdr_t);
      infoHeader.bitCnt = 24;
      infoHeader.cmps = 0;
      rowStride = width * 3;
      data.resize(rowStride * height);
      auto newStride = strideAlign(4);
      fileHeader.fileSize = fileHeader.offsetData + data.size() +
                            infoHeader.height * (newStride - rowStride);
    }
  }

  void flush(const char *filename) {
    flush();
    auto len = strlen(filename) + strlen(".bmp") + 1;
    char *fileName = (char *)malloc(sizeof(char) * len);
    strcpy(fileName, filename);
    strcat(fileName, ".bmp");
    fileName[len] = '\0';
    auto of = fopen(fileName, "wb");
    if (of) {
      if (infoHeader.bitCnt == 32)
        writeHeaderAndData(of);
      else if (infoHeader.bitCnt == 24) {
        if (infoHeader.width % 4 == 0)
          writeHeaderAndData(of);
        else {
          uint32_t newStride = strideAlign(4);
          array<uint8_t> paddingRow;
          paddingRow.resize(newStride - rowStride);
          writeHeader(of);
          for (auto y = 0; y < infoHeader.height; ++y) {
            writeFile(of, data.data() + rowStride * y, rowStride);
            writeFile(of, paddingRow.data(), paddingRow.size());
          }
        }
      } else
        panic("invalid bit count %u", infoHeader.bitCnt);
    } else
      panic("unable to open file '%s'", fileName);
  }

  void setPixel(uint32_t x, uint32_t y, rgb_t clr) {
    if (x >= width || y >= height)
      panic("(%u, %u) is outside size of: %ux%u in raycer::BMPImage::setPixel",
            x, y, width, height);
    buffer[y * infoHeader.width + x] = clr;
  }

  void setPixel(uint32_t x, uint32_t y, uint32_t r, uint32_t g, uint32_t b,
                uint32_t a) {
    setPixel(x, y, rgb_t(r, g, b, a));
  }

  void setBackground(rgb_t c) {
    for (auto i = 0; i < infoHeader.width; ++i)
      for (auto j = 0; j < infoHeader.height; ++j) setPixel(i, j, c);
  }

  uint32_t getWidth() const { return width; }
  uint32_t getHeight() const { return height; }
};

}  // namespace raycer