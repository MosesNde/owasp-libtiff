int exportMaskedImage(FILE *fp, double pagewidth, double pageheight,
                     double imagewidth, double imageheight,
                      int row, int column,
                      double left_offset, double bott_offset,
                     double scale, int center, int rotation)
  {
  double xtran = 0.0;
  double ytran = 0.0;
  double xscale = 1.0;
  double yscale = 1.0;
  double splitheight    = 0;
  double splitwidth     = 0;
  double overlap        = 0;
  double subimage_height = 0.0;
  splitheight = maxPageHeight * PS_UNIT_SIZE;
  splitwidth  = maxPageWidth  * PS_UNIT_SIZE;
  overlap     = splitOverlap  * PS_UNIT_SIZE;
  xscale = scale * imagewidth;
  yscale = scale * imageheight;
  if ((xscale < 0.0) || (yscale < 0.0))
    {
    TIFFError("exportMaskedImage", "Invalid parameters.");
    return (-1);
    }
  switch (rotation)
    {
    case 0:
    case 180: if (splitheight > 0)
                {
               if (splitheight < imageheight)
                 {
                 xtran = -1.0 * column * (pagewidth - overlap);
                  subimage_height = imageheight - ((splitheight - overlap) * row);
                 ytran  = pageheight - subimage_height * (pageheight / splitheight);
                  }
                else
                 {
                 xtran = -1.0 * column * (pagewidth - overlap);
                  ytran = splitheight - imageheight;
                 }
               }
              else
               {
                if (splitwidth > 0)
                 {
                 if (splitwidth < imagewidth)
                   {
                   xtran = -1.0  * column * splitwidth;
                   ytran = -1.0 * row * (pageheight - overlap);
                    }
                  else
                   {
                    ytran = -1.0 * row * (pageheight - overlap);
                    xtran = 0;
                   }
                 }
                else
                 {
                 ytran = pageheight - imageheight;
                 xtran = 0;
                  }
                }
              bott_offset += ytran / (center ? 2 : 1);
              left_offset += xtran / (center ? 2 : 1);
              break;
    case  90:
    case 270:  if (splitheight > 0)
                {
               if (splitheight < imagewidth)
                 {
                 xtran = -1.0 * column * (pageheight - overlap);
                  if (row == 0)
                    ytran = -1.0 * (imagewidth - splitheight);
                  else
                    ytran = -1.0 * (imagewidth - (splitheight - overlap) * (row + 1));
                  }
                else
                 {
                  xtran = -1.0 * column * (pageheight - overlap);
                  ytran = splitheight - imagewidth;
                 }
		}
              else
               {
                if (splitwidth > 0)
                 {
                 if (splitwidth < imageheight)
                   {
                    xtran = -1.0  * column * splitwidth;
                    ytran = -1.0 * row * (pagewidth - overlap);
                    }
                  else
                   {
                    ytran = -1.0 * row * (pagewidth - overlap);
                    xtran = 0;
                   }
                 }
                else
                 {
                 ytran = pageheight - imageheight;
                 xtran = 0;
                  }
                }
              bott_offset += ytran / (center ? 2 : 1);
              left_offset += xtran / (center ? 2 : 1);
              break;
    default:  xtran = 0;
             ytran = 0;
    }
  switch (rotation)
    {
    case   0: fprintf(fp, "%f %f translate\n", left_offset, bott_offset);
              fprintf(fp, "%f %f scale\n", xscale, yscale);
             break;
    case 180: fprintf(fp, "%f %f translate\n", left_offset, bott_offset);
              fprintf(fp, "%f %f scale\n1 1 translate 180 rotate\n",  xscale, yscale);
              break;
    case  90: fprintf(fp, "%f %f translate\n", left_offset, bott_offset);
              fprintf(fp, "%f %f scale\n1 0 translate 90 rotate\n", yscale, xscale);
              break;
    case 270: fprintf(fp, "%f %f translate\n", left_offset, bott_offset);
              fprintf(fp, "%f %f scale\n0 1 translate 270 rotate\n", yscale, xscale);
              break;
    default:  fprintf(fp, "%f %f scale\n", xscale, yscale);
              break;
    }
  return (0);
  }