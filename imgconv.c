#include <stdint.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

const int SIZE = 500 * 500 * 4;
uint8_t buf[SIZE];

EMSCRIPTEN_KEEPALIVE
uint8_t* getImageBuf() {
  return &buf[0];
}

EMSCRIPTEN_KEEPALIVE
int getImageBufSize() {
    return SIZE;
}

EMSCRIPTEN_KEEPALIVE
void invert() {
    for (int i = 0; i < SIZE; i += 4) {
        uint8_t r = buf[i];
        uint8_t g = buf[i + 1];
        uint8_t b = buf[i + 2];

        buf[i] = 255 - r;
        buf[i + 1] = 255 - g;
        buf[i + 2] = 255 - b;
    }
}

EMSCRIPTEN_KEEPALIVE
void grayscale() {
    for (int i = 0; i < SIZE; i += 4) {
        uint8_t r = buf[i];
        uint8_t g = buf[i + 1];
        uint8_t b = buf[i + 2];

        uint8_t gray = (uint8_t)(r * 0.2126 + g * 0.7152 + b * 0.0722);

        buf[i] = gray;
        buf[i + 1] = gray;
        buf[i + 2] = gray;
    }
}

EMSCRIPTEN_KEEPALIVE
void basic_monochrome(uint8_t threshold) {
    for (int i = 0; i < SIZE; i += 4) {
        uint8_t r = buf[i];
        uint8_t g = buf[i + 1];
        uint8_t b = buf[i + 2];

        uint8_t gray = (uint8_t)(r * 0.2126 + g * 0.7152 + b * 0.0722);
        uint8_t mono = threshold > gray ? 0 : 255;

        buf[i] = mono;
        buf[i + 1] = mono;
        buf[i + 2] = mono;
    }
}

extern double js_random();

EMSCRIPTEN_KEEPALIVE
void random_monochrome(uint8_t offset) {
    for (int i = 0; i < SIZE; i += 4) {
        uint8_t r = buf[i];
        uint8_t g = buf[i + 1];
        uint8_t b = buf[i + 2];

        uint8_t gray = (uint8_t)(r * 0.2126 + g * 0.7152 + b * 0.0722);

        uint8_t threshold = js_random() * (255 - offset * 2) + offset;
        uint8_t mono = threshold > gray ? 0 : 255;

        buf[i] = mono;
        buf[i + 1] = mono;
        buf[i + 2] = mono;
    }
}
