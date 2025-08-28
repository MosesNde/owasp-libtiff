void
TIFFXYZToRGB(TIFFCIELabToRGB *cielab, float X, float Y, float Z,
         uint32 *r, uint32 *g, uint32 *b)
{
    int i;
    float Yr, Yg, Yb;
    float *matrix = &cielab->display.d_mat[0][0];
    Yr =  matrix[0] * X + matrix[1] * Y + matrix[2] * Z;
    Yg =  matrix[3] * X + matrix[4] * Y + matrix[5] * Z;
    Yb =  matrix[6] * X + matrix[7] * Y + matrix[8] * Z;
    Yr = TIFFmax(Yr, cielab->display.d_Y0R);
    Yg = TIFFmax(Yg, cielab->display.d_Y0G);
    Yb = TIFFmax(Yb, cielab->display.d_Y0B);
    Yr = TIFFmin(Yr, cielab->display.d_YCR);
    Yg = TIFFmin(Yg, cielab->display.d_YCG);
    Yb = TIFFmin(Yb, cielab->display.d_YCB);
    i = (int)((Yr - cielab->display.d_Y0R) / cielab->rstep);
    i = TIFFmin(cielab->range, i);
    *r = RINT(cielab->Yr2r[i]);
    i = (int)((Yg - cielab->display.d_Y0G) / cielab->gstep);
    i = TIFFmin(cielab->range, i);
    *g = RINT(cielab->Yg2g[i]);
    i = (int)((Yb - cielab->display.d_Y0B) / cielab->bstep);
    i = TIFFmin(cielab->range, i);
    *b = RINT(cielab->Yb2b[i]);
    *r = TIFFmin(*r, cielab->display.d_Vrwr);
    *g = TIFFmin(*g, cielab->display.d_Vrwg);
    *b = TIFFmin(*b, cielab->display.d_Vrwb);
}