#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>

void process_command_opts(int argc, char *argv[], char *mp, char *mode, uint32_t *dirnum,
                          uint16_t *defconfig, uint16_t *deffillorder, uint32_t *deftilewidth,
                          uint32_t *deftilelength, uint32_t *defrowsperstrip,
                          void *crop_data, void *page, void *dump,
                          unsigned int *imagelist, unsigned int *image_count) {
    int c;
    extern char *optarg;

    *mp++ = 'w';
    *mp = '\0';

    while ((c = getopt(argc, argv, "ac:d:e:f:hil:m:p:r:stvw:z:BCD:E:F:H:I:J:K:LMN:O:P:R:S:U:V:X:Y:Z:")) != -1) {
        switch (c) {
            case 'a': mode[0] = 'a'; break;
            case 'd': *dirnum = 0; break;
            case 'e': break;
            case 'f': *deffillorder = 0; break;
            case 'h': break;
            case 'i': break;
            case 'l': *deftilelength = atoi(optarg); break;
            case 'p': *defconfig = 0; break;
            case 'r': *defrowsperstrip = atol(optarg); break;
            case 's': break;
            case 't': break;
            case 'v': exit(0); break;
            case 'w': *deftilewidth = atoi(optarg); break;
            case 'z': break;
            case 'B': *mp++ = 'b'; *mp = '\0'; break;
            case 'L': *mp++ = 'l'; *mp = '\0'; break;
            case 'M': *mp++ = 'm'; *mp = '\0'; break;
            case 'C': *mp++ = 'c'; *mp = '\0'; break;
            case 'D': break;
            case 'm': break;
            case 'E': break;
            case 'F': break;
            case 'H': break;
            case 'I': break;
            case 'J': break;
            case 'K': break;
            case 'N': *image_count = 0; break;
            case 'O': break;
            case 'P': break;
            case 'R': break;
            case 'S': break;
            case 'U': break;
            case 'V': break;
            case 'X': break;
            case 'Y': break;
            case 'Z': break;
            case '?': exit(-1); break;
        }
    }
}