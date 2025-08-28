typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned int uint32;

#define PACK4(r,g,b,a) (((a)<<24)|((b)<<16)|((g)<<8)|(r))

struct Image {
    uint8 Bitdepth16To8[65536];
    uint8 UaToAa[65536];
};

void DECLARESepPutFunc_putRGBUAseparate16bittile(struct Image* img, int y, int h, int w, uint16* r, uint16* g, uint16* b, uint16* a, int fromskew, int toskew, uint32* cp) {
    uint16 *wr = r;
    uint16 *wg = g;
    uint16 *wb = b;
    uint16 *wa = a;
    (void) img; (void) y;
    while (h-- > 0) {
        uint32 rr, gg, bb, aa;
        uint8* m;
        for (int x = w; x-- > 0;) {
            aa = img->Bitdepth16To8[*wa++];
            m = img->UaToAa+(aa<<8);
            rr = m[img->Bitdepth16To8[*wr++]];
            gg = m[img->Bitdepth16To8[*wg++]];
            bb = m[img->Bitdepth16To8[*wb++]];
            *cp++ = PACK4(rr, gg, bb, aa);
        }
        
        cp += toskew;
    }
}