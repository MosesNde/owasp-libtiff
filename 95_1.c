int psPageSize (FILE * fd, int rotation, double pgwidth, double pgheight,
                double reqwidth, double reqheight, double pswidth, double psheight)
  {
  double xscale = 1.0, yscale = 1.0, scale = 1.0;
  double splitheight;
  double splitwidth;
  double new_width;
  double new_height;
  splitheight = maxPageHeight * PS_UNIT_SIZE;
  splitwidth  = maxPageWidth  * PS_UNIT_SIZE;
  switch (rotation)
    {
    case   0:
    case 180: if ((splitheight > 0) || (splitwidth > 0))
                {
               if (pgwidth != 0 || pgheight != 0)
                  {
                 xscale = reqwidth / (splitwidth ? splitwidth : pswidth);
                 yscale = reqheight / (splitheight ? splitheight : psheight);
                  scale = (xscale < yscale) ? xscale : yscale;
                  }
                new_width = splitwidth ? splitwidth : scale * pswidth;
                new_height = splitheight ? splitheight : scale * psheight;
                if (strlen(pageOrientation))
                  fprintf (fd, "%%%%PageOrientation: %s\n", pageOrientation);
                else
                  fprintf (fd, "%%%%PageOrientation: %s\n", (new_width > new_height) ? "Landscape" : "Portrait");
                fprintf (fd, "%%%%PageBoundingBox: 0 0 %ld %ld\n", (long)new_width, (long)new_height);
                fprintf (fd, "1 dict begin /PageSize [ %f %f ] def currentdict end setpagedevice\n",
                       new_width, new_height);
                }
             else
                {
                if ((pgwidth == 0) && (pgheight == 0))
                  {
                  if (strlen(pageOrientation))
                    fprintf (fd, "%%%%PageOrientation: %s\n", pageOrientation);
                  else
                    fprintf (fd, "%%%%PageOrientation: %s\n", (pswidth > psheight) ? "Landscape" : "Portrait");
                 fprintf (fd, "%%%%PageBoundingBox: 0 0 %ld %ld\n", (long)pswidth, (long)psheight);
                  fprintf(fd, "1 dict begin /PageSize [ %f %f ] def currentdict end setpagedevice\n",
                          pswidth, psheight);
                  }
               else
                  {
                  if (strlen(pageOrientation))
                    fprintf (fd, "%%%%PageOrientation: %s\n", pageOrientation);
                  else
                    fprintf (fd, "%%%%PageOrientation: %s\n", (reqwidth > reqheight) ? "Landscape" : "Portrait");
                 fprintf (fd, "%%%%PageBoundingBox: 0 0 %ld %ld\n", (long)reqwidth, (long)reqheight);
                  fprintf(fd, "1 dict begin /PageSize [ %f %f ] def currentdict end setpagedevice\n",
                           reqwidth, reqheight);
                  }
                }
             break;
    case  90:
    case 270: if ((splitheight > 0) || (splitwidth > 0))
               {
               if (pgwidth != 0 || pgheight != 0)
                  {
                 xscale = reqwidth / (splitwidth ? splitwidth : pswidth);
                 yscale = reqheight / (splitheight ? splitheight : psheight);
                  scale = (xscale < yscale) ? xscale : yscale;
                  }
                new_width = splitwidth ? splitwidth : scale * psheight;
                new_height = splitheight ? splitheight : scale * pswidth;
                if (strlen(pageOrientation))
                  fprintf (fd, "%%%%PageOrientation: %s\n", pageOrientation);
                else
                  fprintf (fd, "%%%%PageOrientation: %s\n", (new_width > new_height) ? "Landscape" : "Portrait");
                fprintf (fd, "%%%%PageBoundingBox: 0 0 %ld %ld\n", (long)new_width, (long)new_height);
                fprintf (fd, "1 dict begin /PageSize [ %f %f ] def currentdict end setpagedevice\n",
                       new_width, new_height);
                }
              else
                {
                if ((pgwidth == 0) && (pgheight == 0))
                  {
                  if (strlen(pageOrientation))
                    fprintf (fd, "%%%%PageOrientation: %s\n", pageOrientation);
                  else
                    fprintf (fd, "%%%%PageOrientation: %s\n", (psheight > pswidth) ? "Landscape" : "Portrait");
                 fprintf (fd, "%%%%PageBoundingBox: 0 0 %ld %ld\n", (long)psheight, (long)pswidth);
                  fprintf(fd, "1 dict begin /PageSize [ %f %f ] def currentdict end setpagedevice\n",
                         psheight, pswidth);
                  }
               else
                  {
                  if (strlen(pageOrientation))
                    fprintf (fd, "%%%%PageOrientation: %s\n", pageOrientation);
                  else
                    fprintf (fd, "%%%%PageOrientation: %s\n", (reqwidth > reqheight) ? "Landscape" : "Portrait");
                 fprintf (fd, "%%%%PageBoundingBox: 0 0 %ld %ld\n", (long)reqwidth, (long)reqheight);
                  fprintf(fd, "1 dict begin /PageSize [ %f %f ] def currentdict end setpagedevice\n",
                          reqwidth, reqheight);
                  }
               }
             break;
    default:  TIFFError ("psPageSize", "Invalid rotation %d", rotation);
      return (1);
    }
  fputs("<<\n  /Policies <<\n    /PageSize 3\n  >>\n>> setpagedevice\n", fd);
  return (0);
  }