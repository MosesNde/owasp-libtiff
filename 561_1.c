static int
OJPEGReadHeaderInfoSecStreamDht(TIFF* tif)
{
	static const char module[]="OJPEGReadHeaderInfoSecStreamDht";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	uint32 na;
	uint8* nb;
	uint8 o;
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m<=2)
		return(0);
	if (sp->subsamplingcorrect!=0)
	{
		OJPEGReadSkip(sp,m-2);
	}
	else
	{
		na=sizeof(uint32)+2+m;
		nb=_TIFFmalloc(na);
		if (nb==0)
			return(0);
		*(uint32*)nb=na;
		nb[sizeof(uint32)]=255;
		nb[sizeof(uint32)+1]=JPEG_MARKER_DHT;
		nb[sizeof(uint32)+2]=(m>>8);
		nb[sizeof(uint32)+3]=(m&255);
		if (OJPEGReadBlock(sp,m-2,&nb[sizeof(uint32)+4])==0)
			return(0);
		o=nb[sizeof(uint32)+4];
		if ((o&240)==0)
		{
			if (sp->dctable[o]!=0)
				sp->dctable[o]=nb;
			else
				sp->dctable[o]=nb;
		}
		else
		{
			if ((o&240)!=16)
				return(0);
			o&=15;
			if (sp->actable[o]!=0)
				sp->actable[o]=nb;
			else
				sp->actable[o]=nb;
		}
	}
	return(1);
}