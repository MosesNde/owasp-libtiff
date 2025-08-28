static int oog_encode(double u, double v) {
 static int oog_table[NANGLES];
 static int initialized = 0;
 register int i;
 if (!initialized) {
  double eps[NANGLES], ua, va, ang, epsa;
  int ui, vi, ustep;
  for (i = NANGLES; i--; )
   eps[i] = 2.;
  for (vi = UV_NVS; vi--; ) {
   va = UV_VSTART + (vi+.5)*UV_SQSIZ;
   ustep = uv_row[vi].nus-1;
   if (vi == UV_NVS-1 || vi == 0 || ustep <= 0)
    ustep = 1;
   for (ui = uv_row[vi].nus-1; ui >= 0; ui -= ustep) {
    ua = uv_row[vi].ustart + (ui+.5)*UV_SQSIZ;
    ang = uv2ang(ua, va);
    i = (int) ang;
    epsa = fabs(ang - (i+.5));
    if (epsa < eps[i]) {
     oog_table[i] = uv_row[vi].ncum + ui;
     eps[i] = epsa;
    }
   }
  }
  for (i = NANGLES; i--; )
   if (eps[i] > 1.5) {
    oog_table[i] = 0;
   }
  initialized = 1;
 }
 i = (int) uv2ang(u, v);
 return (oog_table[i]);
}