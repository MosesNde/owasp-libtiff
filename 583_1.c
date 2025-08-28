#include <stdint.h>
#include <assert.h>

typedef struct {
    uint8_t* tif_data;
} TIFF;

typedef struct {
    uint8_t out_buffer[256];
    uint8_t sof_marker_id;
    uint16_t sof_y;
    uint16_t sof_x;
    uint8_t samples_per_pixel_per_plane;
    uint8_t sof_c[10];
    uint8_t sof_hv[10];
    uint8_t sof_tq[10];
    uint8_t plane_sample_offset;
    int out_state;
} OJPEGState;

static void OJPEGWriteStreamSof(TIFF* tif, void** mem, uint32_t* len) {
    OJPEGState* sp = (OJPEGState*)tif->tif_data;
    uint8_t m;
    assert(256 >= 2 + 8 + sp->samples_per_pixel_per_plane * 3);
    assert(255 >= 8 + sp->samples_per_pixel_per_plane * 3);
    sp->out_buffer[0] = 255;
    sp->out_buffer[1] = sp->sof_marker_id;
    sp->out_buffer[2] = 0;
    sp->out_buffer[3] = 8 + sp->samples_per_pixel_per_plane * 3;
    sp->out_buffer[4] = 8;
    sp->out_buffer[5] = (sp->sof_y >> 8);
    sp->out_buffer[6] = (sp->sof_y & 255);
    sp->out_buffer[7] = (sp->sof_x >> 8);
    sp->out_buffer[8] = (sp->sof_x & 255);
    sp->out_buffer[9] = sp->samples_per_pixel_per_plane;
    for (m = 0; m < sp->samples_per_pixel_per_plane; m++) {
        sp->out_buffer[10 + m * 3] = sp->sof_c[sp->plane_sample_offset + m];
        sp->out_buffer[10 + m * 3 + 1] = sp->sof_hv[sp->plane_sample_offset + m];
        sp->out_buffer[10 + m * 3 + 2] = sp->sof_tq[sp->plane_sample_offset + m];
    }
    *len = 10 + sp->samples_per_pixel_per_plane * 3;
    *mem = (void*)sp->out_buffer;
    sp->out_state++;
}