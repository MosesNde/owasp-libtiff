static int
OJPEGReadHeaderInfoSecStreamDqt(TIFF* tif)
{
	static const char module[]="OJPEGReadHeaderInfoSecStreamDqt";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	uint32 na;
	uint8* nb;
	uint8 o;
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m<=2)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DQT marker in JPEG data");
		return(0);
	}
	if (sp->subsamplingcorrect!=0)
		OJPEGReadSkip(sp,m-2);
	else
	{
		m-=2;
		do
		{
			if (m<65)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DQT marker in JPEG data");
				return(0);
			}
			na=sizeof(uint32)+69;
			nb=malloc(na);
			if (nb==0)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
				return(0);
			}
			*(uint32*)nb=na;
			nb[sizeof(uint32)]=255;
			nb[sizeof(uint32)+1]=JPEG_MARKER_DQT;
			nb[sizeof(uint32)+2]=0;
			nb[sizeof(uint32)+3]=67;
			if (OJPEGReadBlock(sp,65,&nb[sizeof(uint32)+4])==0)
				return(0);
			o=nb[sizeof(uint32)+4]&15;
			if (3<o)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DQT marker in JPEG data");
				return(0);
			}
			if (sp->qtable[o]!=0)
				free(sp->qtable[o]);
			sp->qtable[o]=nb;
			m-=65;
		} while(m>0);
	}
	return(1);
}