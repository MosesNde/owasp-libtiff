int
PlaceImage(TIFF *tif, FILE *fp, int *npages, uint32 w, uint32 h,
           double pagewidth, double pageheight,
           double imagewidth, double imageheight,
           int splitpage, double lm, double bm, int cnt)
{
        int    i       = 0;
        int    ximages = 0;
        int    splitaxis = 0;
        double xtran = 0;
        double ytran = 0;
        double xscale = 1;
        double yscale = 1;
        double left_margin    = 0;
        double bottom_margin  = 0;
        double left_offset    = lm * PS_UNIT_SIZE;
        double bottom_offset  = bm * PS_UNIT_SIZE;
        double splitwidth     = 0;
        double splitheight    = 0;
        double subimageheight = 0;
        double subimagewidth  = 0;
        double overlap        = 0;
        double overlapspace   = 0;
        pagewidth *= PS_UNIT_SIZE;
        pageheight *= PS_UNIT_SIZE;
        splitheight = maxPageHeight * PS_UNIT_SIZE;
        splitwidth  = maxPageWidth  * PS_UNIT_SIZE;
        overlap     = splitOverlap  * PS_UNIT_SIZE;
        if (splitheight != 0)
          splitaxis = VERTICAL;
        else {
           if (splitwidth != 0)
             splitaxis = HORIZONTAL;
           else {
             fprintf (stderr, "You must specify either a maximum page height or width\n");
             return (0);
           }
        }
        if (splitaxis == VERTICAL) {
          if (imageheight <= splitheight) {
            yscale = imageheight;
            ytran = pageheight - imageheight;
          } else {
              subimageheight = imageheight - ((splitheight - overlap) * splitpage);
              yscale = imageheight * (pageheight / splitheight);
              ytran  = pageheight - subimageheight * (pageheight / splitheight);
              if (subimageheight > splitheight) {
                splitpage++;
              } else {
                  splitpage = 0;
                  }
          }
          bottom_offset += ytran / (cnt?2:1);
          left_margin = left_offset / (cnt ? 2 : 1);
          ximages = ceil (imagewidth / pagewidth);
          overlapspace = (ximages - 1) * overlap;
          if (((imagewidth + overlapspace) * (pageheight / splitheight)) > (ximages * pagewidth)) {
            ximages++;
            overlapspace += overlap;
          }
          xscale = (imagewidth + overlapspace) * (pageheight / splitheight);
          if (imagewidth <= pagewidth) {
            left_offset = left_margin;
            bottom_offset = bottom_margin;
            fprintf(fp, "%f %f translate\n", left_offset, bottom_offset);
            fprintf(fp, "%f %f scale\n", xscale, yscale);
          } else {
            for (i = 0; i < ximages; i++) {
              xtran = i * (pagewidth - ((i > 0) ? overlap : 0));
              left_offset = -xtran + left_margin;
              fprintf(fp, "%f %f translate\n", left_offset, bottom_offset);
              fprintf(fp, "%f %f scale\n", xscale, yscale);
              if ( i < (ximages - 1)) {
                PSpage(fp, tif, w, h);
                fprintf(fp, "end\n");
                fprintf(fp, "grestore\n");
                fprintf(fp, "showpage\n");
                (*npages)++;
                fprintf(fp, "%%%%Page: %d %d\n", (*npages), (*npages));
                fprintf(fp, "gsave\n");
                fprintf(fp, "100 dict begin\n");
                }
            }
          }
        } else {
          ximages = ceil (imagewidth / splitwidth);
          overlapspace = (ximages - 1) * overlap;
          if (((imagewidth + overlapspace) * (pagewidth / splitwidth)) > (ximages * pagewidth)) {
            ximages++;
            overlapspace += overlap;
          }
          if (ximages == 1) {
            xscale = imagewidth;
            xtran = 0;
            splitpage = 0;
          } else {
              subimagewidth  = imagewidth  - ((splitwidth - overlap) * splitpage);
              xscale = imagewidth * (pagewidth / splitwidth);
              xtran  = imagewidth - (subimagewidth * (pagewidth / splitwidth));
              splitheight = pageheight;
              subimageheight = imageheight - ((splitheight - overlap) * splitpage);
              yscale = (imageheight + overlapspace);
              ytran  = pageheight - subimageheight  + (overlapspace * (pagewidth / splitwidth));
              if (subimageheight > splitheight) {
                splitpage++;
              } else {
                  splitpage = 0;
                  }
          }
          bottom_margin = bottom_offset / (cnt ? 2 : 1);
          bottom_offset = bottom_margin + ytran;
          left_margin = left_offset / (cnt ? 2 : 1);
          if (imagewidth <= pagewidth) {
            left_offset = left_margin;
            bottom_offset = bottom_margin;
            fprintf(fp, "%f %f translate\n", left_offset, bottom_offset);
            fprintf(fp, "%f %f scale\n", xscale, yscale);
          } else {
              for (i = 0; i < ximages; i++) {
                xtran = i * (pagewidth - ((i > 0) ? overlap : 0));
                left_offset = left_margin - xtran;
                fprintf(fp, "%f %f translate\n", left_offset, bottom_offset);
                fprintf(fp, "%f %f scale\n", xscale, yscale);
                if ( i < (ximages - 1)) {
                  PSpage(fp, tif, w, h);
                  fprintf(fp, "end\n");
                  fprintf(fp, "grestore\n");
                  fprintf(fp, "showpage\n");
                  (*npages)++;
                  fprintf(fp, "%%%%Page: %d %d\n", (*npages), (*npages));
                  fprintf(fp, "gsave\n");
                  fprintf(fp, "100 dict begin\n");
                  }
              }
          }
        }
        if (rotate)
            {
            if (rotation == 180 )
              {
              fprintf(fp, "%f %f translate\n", left_offset, bottom_offset);
              fprintf(fp, "%f %f scale\n", xscale, yscale);
              }
            else
              {
              fprintf(fp, "%f %f translate\n", bottom_offset, left_offset);
              fprintf(fp, "%f %f scale\n", yscale, xscale);
              }
            fprintf (fp, "1 1 translate %d rotate\n", rotation);
            }
        return splitpage;
}