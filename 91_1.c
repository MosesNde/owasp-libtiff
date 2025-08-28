int get_subimage_count(double pagewidth,  double pageheight,
               double imagewidth, double imageheight,
               int *ximages, int *yimages,
               int rotation, double scale)
{
    int pages = 1;
    double splitheight = 0;
    double splitwidth  = 0;
    double overlap     = 0;
    splitheight = maxPageHeight * PS_UNIT_SIZE;
    splitwidth  = maxPageWidth  * PS_UNIT_SIZE;
    overlap     = splitOverlap  * PS_UNIT_SIZE;
    pagewidth  *= PS_UNIT_SIZE;
    pageheight *= PS_UNIT_SIZE;
    if ((imagewidth < 1.0) || (imageheight < 1.0))
    {
        return (0);
    }
    switch (rotation)
    {
    case 0:
    case 180: if (splitheight > 0)
                {
               if (imageheight > splitheight)
                 {
                 if (pagewidth)
                   *ximages = (int)ceil((scale * imagewidth)  / (pagewidth - overlap));
                  else
                   *ximages = 1;
                 *yimages = (int)ceil((scale * imageheight) / (splitheight - overlap));
                 }
                else
                 {
                 if (pagewidth)
                   *ximages = (int)ceil((scale * imagewidth) / (pagewidth - overlap));
                  else
                   *ximages = 1;
                 *yimages = 1;
                 }
               }
              else
               {
                if (splitwidth > 0)
                 {
                 if (imagewidth >splitwidth)
                   {
                   *ximages = (int)ceil((scale * imagewidth)  / (splitwidth - overlap));
                    if (pageheight)
                     *yimages = (int)ceil((scale * imageheight) / (pageheight - overlap));
                    else
                     *yimages = 1;
                   }
                  else
                   {
                   *ximages = 1;
                    if (pageheight)
                     *yimages = (int)ceil((scale * imageheight) / (pageheight - overlap));
                    else
                     *yimages = 1;
                   }
                 }
                else
                 {
                 *ximages = 1;
                 *yimages = 1;
                 }
               }
             break;
    case 90:
    case 270: if (splitheight > 0)
                {
               if (imagewidth > splitheight)
                 {
                 *yimages = (int)ceil((scale * imagewidth) / (splitheight - overlap));
                  if (pagewidth)
                   *ximages = (int)ceil((scale * imageheight) / (pagewidth - overlap));
                  else
                   *ximages = 1;
                 }
                else
                 {
                 *yimages = 1;
                  if (pagewidth)
                   *ximages = (int)ceil((scale * imageheight) / (pagewidth - overlap));
                  else
                   *ximages = 1;
                 }
               }
              else
               {
                if (splitwidth > 0)
                 {
                 if (imageheight > splitwidth)
                   {
                   if (pageheight)
                     *yimages = (int)ceil((scale * imagewidth) / (pageheight - overlap));
                    else
                     *yimages = 1;
                   *ximages = (int)ceil((scale * imageheight)  / (splitwidth - overlap));
                   }
                  else
                   {
                   if (pageheight)
                     *yimages = (int)ceil((scale * imagewidth) / (pageheight - overlap));
                    else
                     *yimages = 1;
                   *ximages = 1;
                   }
                 }
                else
                 {
                 *ximages = 1;
                 *yimages = 1;
                 }
               }
             break;
    default:  *ximages = 1;
             *yimages = 1;
    }
    pages = (*ximages) * (*yimages);
    return (pages);
}