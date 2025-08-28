static int
OJPEGReadHeaderInfoSecTablesQTable(TIFF* tif)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	uint8 n;
	uint32 oa;
	uint8* ob;
	uint32 p;
	if (sp->qtable_offset[0]==0)
	{
		return(0);
	}
	for (m=0; m<sp->samples_per_pixel; m++)
	{
		if ((sp->qtable_offset[m]!=0) && ((m==0) || (sp->qtable_offset[m]!=sp->qtable_offset[m-1])))
		{
			for (n=0; n<m-1; n++)
			{
				if (sp->qtable_offset[m]==sp->qtable_offset[n])
				{
					return(0);
				}
			}
			oa=sizeof(uint32)+69;
			ob=_TIFFmalloc(oa);
			if (ob==0)
			{
				return(0);
			}
			*(uint32*)ob=oa;
			ob[sizeof(uint32)]=255;
			ob[sizeof(uint32)+1]=JPEG_MARKER_DQT;
			ob[sizeof(uint32)+2]=0;
			ob[sizeof(uint32)+3]=67;
			ob[sizeof(uint32)+4]=m;
			TIFFSeekFile(tif,sp->qtable_offset[m],SEEK_SET);
			p=TIFFReadFile(tif,&ob[sizeof(uint32)+5],64);
			if (p!=64)
				return(0);
			sp->qtable[m]=ob;
			sp->sof_tq[m]=m;
		}
		else
			sp->sof_tq[m]=sp->sof_tq[m-1];
	}
	return(1);
}