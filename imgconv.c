#include <stdint.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

EMSCRIPTEN_KEEPALIVE
void grayscale(uint32_t offset, int width, int height) {
    for (int i = 0; i < width * height * 4; i += 4) {
        uint8_t* p = (uint8_t*)(offset + i);
        uint8_t r = *p;
        uint8_t g = *(p + 1);
        uint8_t b = *(p + 2);

        uint8_t gray = (uint8_t)(r * 0.2126 + g * 0.7152 + b * 0.0722);

        *p = gray;
        *(p + 1) = gray;
        *(p + 2) = gray;
    }
}
/* void grayscale(uint8_t* buf, int width, int height) { */
/*     for (int i = 0; i < width * height * 4; i += 4) { */
/*         uint8_t r = buf[i]; */
/*         uint8_t g = buf[i + 1]; */
/*         uint8_t b = buf[i + 2]; */

/*         uint8_t gray = (uint8_t)(r * 0.2126 + g * 0.7152 + b * 0.0722); */

/*         buf[i] = gray; */
/*         buf[i + 1] = gray; */
/*         buf[i + 2] = gray; */
/*     } */
/* } */
