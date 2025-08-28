static int
update_output_file (TIFF **tiffout, char *mode, int autoindex,
                    char *outname, unsigned int *page)
  {
  static int findex = 0;
  char  *sep;
  char   filenum[16];
  char   export_ext[16];
  char   exportname[PATH_MAX];
  strcpy (export_ext, ".tiff");
  if (autoindex && (*tiffout != NULL))
    {
    TIFFClose (*tiffout);
    *tiffout = NULL;
    }
  strncpy (exportname, outname, PATH_MAX - 15);
  if (*tiffout == NULL)
    {
    if (autoindex)
      {
      findex++;
      if ((sep = strstr(exportname, ".tif")) || (sep = strstr(exportname, ".TIF")))
        {
        strncpy (export_ext, sep, 5);
        *sep = '\0';
        }
      else
        strncpy (export_ext, ".tiff", 5);
      export_ext[5] = '\0';
      sprintf (filenum, "-%03d%s", findex, export_ext);
      filenum[15] = '\0';
      strncat (exportname, filenum, 14);
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

