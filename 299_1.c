#include <stdint.h>
#include <stdbool.h>

uint64_t TIFFVStripSize64(TIFF* tif, uint32_t nrows, UserContext* user)
{
    static const char module[] = "TIFFVStripSize64";
    TIFFDirectory *td = &tif->tif_dir;
    if (nrows == (uint32_t)(-1))
        nrows = td->td_imagelength;
    if ((td->td_planarconfig == PLANARCONFIG_CONTIG) &&
        (td->td_photometric == PHOTOMETRIC_YCBCR) &&
        (!isUpSampled(tif)))
    {
        uint16_t ycbcrsubsampling[2];
        uint16_t samplingblock_samples;
        uint32_t samplingblocks_hor;
        uint32_t samplingblocks_ver;
        uint64_t samplingrow_samples;
        uint64_t samplingrow_size;
        if (td->td_samplesperpixel != 3)
        {
            TIFFErrorExt(tif->tif_clientdata, module, "Invalid td_samplesperpixel value");
            return 0;
        }
        TIFFGetFieldDefaulted(tif, TIFFTAG_YCBCRSUBSAMPLING, &ycbcrsubsampling[0], &ycbcrsubsampling[1]);
        if (((ycbcrsubsampling[0] != 1) && (ycbcrsubsampling[0] != 2) && (ycbcrsubsampling[0] != 4)) ||
            ((ycbcrsubsampling[1] != 1) && (ycbcrsubsampling[1] != 2) && (ycbcrsubsampling[1] != 4)))
        {
            TIFFErrorExt(tif->tif_clientdata, module, "Invalid YCbCr subsampling");
            return 0;
        }
        samplingblock_samples = ycbcrsubsampling[0] * ycbcrsubsampling[1] + 2;
        samplingblocks_hor = TIFFhowmany_32(td->td_imagewidth, ycbcrsubsampling[0]);
        samplingblocks_ver = TIFFhowmany_32(nrows, ycbcrsubsampling[1]);
        samplingrow_samples = multiply_64(tif, samplingblocks_hor, samplingblock_samples, module);
        samplingrow_size = TIFFhowmany8_64(multiply_64(tif, samplingrow_samples, td->td_bitspersample, module));
        return multiply_64(tif, samplingrow_size, samplingblocks_ver, module);
    }
    else
        return multiply_64(tif, nrows, TIFFScanlineSize64(tif), module);
}