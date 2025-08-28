void process_command_opts(int argc, char *argv[], char *mp, char *mode, uint32 *dirnum, uint16 *defconfig, uint16 *deffillorder, uint32 *deftilewidth, uint32 *deftilelength, uint32 *defrowsperstrip, struct crop_mask *crop_data, struct pagedef *page, struct dump_opts *dump, unsigned int *imagelist, unsigned int *image_count) {
    int c, good_args = 0;
    char *opt_offset = NULL;
    char *opt_ptr = NULL;
    char *sep = NULL;
    unsigned int i, j, start, end;
    extern int optind;
    extern char* optarg;
    *mp++ = 'w';
    *mp = '\0';
    while ((c = getopt(argc, argv, "ac:d:e:f:hil:m:p:r:stvw:z:BCD:E:F:H:I:J:K:LMN:O:P:R:S:U:V:X:Y:Z:")) != -1) {
        good_args++;
        switch (c) {
            case 'a': mode[0] = 'a'; break;
            case 'c': if (!processCompressOptions(optarg)) { TIFFError("Unknown compression option", "%s", optarg); TIFFError("For valid options type", "tiffcrop -h"); exit(-1); } break;
            case 'd': *dirnum = atoi(optarg); break;
            case 'e': crop_data->exp_mode = atoi(optarg); crop_data->img_mode = atoi(optarg); break;
            case 'f': *deffillorder = atoi(optarg); break;
            case 'h': usage(); break;
            case 'i': ignore = TRUE; break;
            case 'l': outtiled = TRUE; *deftilelength = atoi(optarg); break;
            case 'p': *defconfig = atoi(optarg); break;
            case 'r': *defrowsperstrip = atol(optarg); break;
            case 's': outtiled = FALSE; break;
            case 't': outtiled = TRUE; break;
            case 'v': TIFFError("Tiffcrop version", "%s, last updated: %s", tiffcrop_version_id, tiffcrop_rev_date); TIFFError("Tiffcp code", "Copyright (c) 1988-1997 Sam Leffler"); TIFFError("           ", "Copyright (c) 1991-1997 Silicon Graphics, Inc"); TIFFError("Tiffcrop additions", "Copyright (c) 2007-2009 Richard Nolde"); exit(0); break;
            case 'w': outtiled = TRUE; *deftilewidth = atoi(optarg); break;
            case 'z': crop_data->crop_mode |= CROP_REGIONS; for (i = 0, opt_ptr = strtok(optarg, ":"); (opt_ptr != NULL) && (i < MAX_REGIONS); opt_ptr = strtok(NULL, ":"), i++) {
                crop_data->regions++;
                sscanf(opt_ptr, "%lf,%lf,%lf,%lf", &crop_data->corners[i].X1, &crop_data->corners[i].Y1, &crop_data->corners[i].X2, &crop_data->corners[i].Y2);
            } break;
            case 'B': *mp++ = 'b'; *mp = '\0'; break;
            case 'L': *mp++ = 'l'; *mp = '\0'; break;
            case 'M': *mp++ = 'm'; *mp = '\0'; break;
            case 'C': *mp++ = 'c'; *mp = '\0'; break;
            case 'D': for (i = 0, opt_ptr = strtok(optarg, ","); opt_ptr != NULL; opt_ptr = strtok(NULL, ","), i++) {
                opt_offset = strchr(opt_ptr, ':');
                *opt_offset = '\0';
                if (strncmp(opt_ptr, "for", 3) == 0) {
                    if (strncmp(opt_offset + 1, "txt", 3) == 0) {
                        dump->format = DUMP_TEXT;
                        strcpy(dump->mode, "w");
                    } else if (strncmp(opt_offset + 1, "raw", 3) == 0) {
                        dump->format = DUMP_RAW;
                        strcpy(dump->mode, "wb");
                    }
                } else {
                    if (strncmp(opt_ptr, "lev", 3) == 0) dump->level = atoi(opt_offset + 1);
                    if (strncmp(opt_ptr, "in", 2) == 0) strcpy(dump->infilename, opt_offset + 1);
                    if (strncmp(opt_ptr, "out", 3) == 0) strcpy(dump->outfilename, opt_offset + 1);
                    if (strncmp(opt_ptr, "deb", 3) == 0) dump->debug = atoi(opt_offset + 1);
                }
            } break;
            case 'm': crop_data->crop_mode |= CROP_MARGINS; for (i = 0, opt_ptr = strtok(optarg, ",:"); (opt_ptr != NULL) && (i < 4); opt_ptr = strtok(NULL, ",:"), i++) {
                crop_data->margins[i] = atof(opt_ptr);
            } break;
            case 'E': crop_data->edge_ref = atoi(optarg); break;
            case 'F': crop_data->crop_mode |= CROP_MIRROR; crop_data->mirror = atoi(optarg); break;
            case 'H': page->hres = atof(optarg); page->mode |= PAGE_MODE_RESOLUTION; break;
            case 'I': crop_data->crop_mode |= CROP_INVERT; crop_data->photometric = atoi(optarg); break;
            case 'J': page->hmargin = atof(optarg); page->mode |= PAGE_MODE_MARGINS; break;
            case 'K': page->vmargin = atof(optarg); page->mode |= PAGE_MODE_MARGINS; break;
            case 'N': for (i = 0, opt_ptr = strtok(optarg, ","); opt_ptr != NULL; opt_ptr = strtok(NULL, ",")) {
                imagelist[i++] = atoi(opt_ptr) - 1;
            } *image_count = i; break;
            case 'O': page->orient = atoi(optarg); break;
            case 'P': get_page_geometry(optarg, page); page->mode |= PAGE_MODE_PAPERSIZE; break;
            case 'R': crop_data->crop_mode |= CROP_ROTATE; crop_data->rotation = (uint16)atoi(optarg); break;
            case 'S': sep = strchr(optarg, ','); if (sep) { *sep = '\0'; page->cols = atoi(optarg); page->rows = atoi(sep + 1); } else { page->cols = atoi(optarg); page->rows = atoi(optarg); } page->mode |= PAGE_MODE_ROWSCOLS; break;
            case 'U': crop_data->res_unit = atoi(optarg); page->res_unit = atoi(optarg); break;
            case 'V': page->vres = atof(optarg); page->mode |= PAGE_MODE_RESOLUTION; break;
            case 'X': crop_data->crop_mode |= CROP_WIDTH; crop_data->width = atof(optarg); break;
            case 'Y': crop_data->crop_mode |= CROP_LENGTH; crop_data->length = atof(optarg); break;
            case 'Z': crop_data->crop_mode |= CROP_ZONES; for (i = 0, opt_ptr = strtok(optarg, ","); (opt_ptr != NULL) && (i < MAX_REGIONS); opt_ptr = strtok(NULL, ","), i++) {
                crop_data->zones++;
                opt_offset = strchr(opt_ptr, ':');
                *opt_offset = '\0';
                crop_data->zonelist[i].position = atoi(opt_ptr);
                crop_data->zonelist[i].total = atoi(opt_offset + 1);
            } break;
            case '?': exit(-1);
        }
    }
}