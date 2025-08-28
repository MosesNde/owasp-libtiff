static int
update_output_file (TIFF **tiffout, char *mode, int autoindex,
                    char *outname, unsigned int *page)
  {
  static int findex = 0;
  char  *sep;
  char   filenum[64];
  char   export_ext[64];
  char   exportname[1024];
  strcpy (export_ext, ".tiff");
  if (autoindex && (*tiffout != NULL))
    {
    TIFFClose (*tiffout);
    *tiffout = NULL;
    }
  strcpy (exportname, outname);
  if (*tiffout == NULL)
    {
    if (autoindex)
      {
      findex++;
      if ((sep = strstr(exportname, ".tif")) || (sep = strstr(exportname, ".TIF")))
        {
        strcpy (export_ext, sep);
        *sep = '\0';
        }
      else
        strcpy (export_ext, ".tiff");
      sprintf (filenum, "-%03d%s", findex, export_ext);
      strcat (exportname, filenum);
      }
    *tiffout = TIFFOpen(exportname, mode);
    if (*tiffout == NULL)
      {
      TIFFError("update_output_file", "Unable to open output file %s\n", exportname);
      return (1);
      }
    *page = 0;
    return (0);
    }
  else
    (*page)++;
  return (0);
  }