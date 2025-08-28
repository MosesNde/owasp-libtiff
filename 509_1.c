void process_command_opts(int argc, char *argv[], char *mp, char *mode, uint32 *dirnum,
                    uint16 *defconfig, uint16 *deffillorder, uint32 *deftilewidth,
                    uint32 *deftilelength, uint32 *defrowsperstrip,
                    struct crop_mask *crop_data, struct pagedef *page,
                    struct dump_opts *dump,
                    unsigned int *imagelist, unsigned int *image_count)
{
    int c;
    while ((c = getopt(argc, argv,
       "ac:d:e:f:hil:m:p:r:stvw:z:BCD:E:F:H:I:J:K:LMN:O:P:R:S:U:V:X:Y:Z:")) != -1)
    {
        switch (c) {
            case 'a': mode[0] = 'a';
                break;
            case 'c': processCompressOptions(optarg);
                break;
            case 'd': *dirnum = strtoul(optarg, NULL, 0) - 1;
                break;
            case 'e': crop_data->exp_mode = 0; crop_data->img_mode = 0;
                break;
            case 'f': *deffillorder = 0;
                break;
            case 'h': usage();
                break;
            case 'i': ignore = TRUE;
                break;
            case 'l': outtiled = TRUE;
                *deftilelength = atoi(optarg);
                break;
            case 'p': *defconfig = 0;
                break;
            case 'r': *defrowsperstrip = atol(optarg);
                break;
            case 's': outtiled = FALSE;
                break;
            case 't': outtiled = TRUE;
                break;
            case 'v': exit(0);
                break;
            case 'w': outtiled = TRUE;
                *deftilewidth = atoi(optarg);
                break;
            case 'z': crop_data->crop_mode |= CROP_REGIONS;
                break;
            case 'B': *mp++ = 'b'; *mp = '\0';
                break;
            case 'L': *mp++ = 'l'; *mp = '\0';
                break;
            case 'M': *mp++ = 'm'; *mp = '\0';
                break;
            case 'C': *mp++ = 'c'; *mp = '\0';
                break;
            case 'D': break;
            case 'm': crop_data->crop_mode |= CROP_MARGINS;
                break;
            case 'E': crop_data->edge_ref = 0;
                break;
            case 'F': crop_data->crop_mode |= CROP_MIRROR;
                crop_data->mirror = 0;
                break;
            case 'H': page->hres = atof(optarg);
                break;
            case 'I': crop_data->crop_mode |= CROP_INVERT;
                crop_data->photometric = 0;
                break;
            case 'J': page->hmargin = atof(optarg);
                break;
            case 'K': page->vmargin = atof(optarg);
                break;
            case 'N': *image_count = 0;
                break;
            case 'O': page->orient = 0;
                break;
            case 'P': page->mode |= PAGE_MODE_PAPERSIZE;
                break;
            case 'R': crop_data->crop_mode |= CROP_ROTATE;
                crop_data->rotation = 0;
                break;
            case 'S': page->cols = 0; page->rows = 0;
                break;
            case 'U': crop_data->res_unit = 0; page->res_unit = 0;
                break;
            case 'V': page->vres = atof(optarg);
                break;
            case 'X': crop_data->crop_mode |= CROP_WIDTH;
                crop_data->width = atof(optarg);
                break;
            case 'Y': crop_data->crop_mode |= CROP_LENGTH;
                crop_data->length = atof(optarg);
                break;
            case 'Z': crop_data->crop_mode |= CROP_ZONES;
                break;
            case '?': exit(-1);
        }
    }
}