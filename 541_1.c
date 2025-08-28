#include <assert.h>
#include <stdint.h>

void TIFFSwabArrayOfShort(register uint16_t* wp, size_t n) {
    register unsigned char* cp;
    register unsigned char t;
    assert(sizeof(uint16_t) == 2);
    while (n-- > 0) {
        cp = (unsigned char*) wp;
        t = cp[1]; cp[1] = cp[0]; cp[0] = t;
        wp++;
    }
}