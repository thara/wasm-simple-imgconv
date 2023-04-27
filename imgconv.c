#include <stdint.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

const int SIZE = 500 * 500 * 4;
uint8_t buf[SIZE];

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
uint8_t* getImageBuf() {
  return &buf[0];
}

EMSCRIPTEN_KEEPALIVE
int getImageBufSize() {
    return SIZE;
}

/* void grayscale(uint32_t offset, int width, int height) { */
/*     for (int i = 0; i < width * height * 4; i += 4) { */
/*         uint8_t* p = (uint8_t*)(offset + i); */
/*         uint8_t r = *p; */
/*         uint8_t g = *(p + 1); */
/*         uint8_t b = *(p + 2); */

/*         uint8_t gray = (uint8_t)(r * 0.2126 + g * 0.7152 + b * 0.0722); */

/*         *p = gray; */
/*         *(p + 1) = gray; */
/*         *(p + 2) = gray; */
/*     } */
/* } */
