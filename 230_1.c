static int
JPEGFixupTagsSubsamplingSec(struct JPEGFixupTagsSubsamplingData* data)
{
	static const char module[] = "JPEGFixupTagsSubsamplingSec";
	uint8 m;
	while (1)
	{
		if (!JPEGFixupTagsSubsamplingReadByte(data,&m))
			return(0);
		switch (m)
		{
			case JPEG_MARKER_SOI:
				break;
			case JPEG_MARKER_COM:
			case JPEG_MARKER_APP0:
			case JPEG_MARKER_APP0+1:
			case JPEG_MARKER_APP0+2:
			case JPEG_MARKER_APP0+3:
			case JPEG_MARKER_APP0+4:
			case JPEG_MARKER_APP0+5:
			case JPEG_MARKER_APP0+6:
			case JPEG_MARKER_APP0+7:
			case JPEG_MARKER_APP0+8:
			case JPEG_MARKER_APP0+9:
			case JPEG_MARKER_APP0+10:
			case JPEG_MARKER_APP0+11:
			case JPEG_MARKER_APP0+12:
			case JPEG_MARKER_APP0+13:
			case JPEG_MARKER_APP0+14:
			case JPEG_MARKER_APP0+15:
			case JPEG_MARKER_DQT:
			case JPEG_MARKER_SOS:
			case JPEG_MARKER_DHT:
			case JPEG_MARKER_DRI:
				{
					uint16 n;
					JPEGFixupTagsSubsamplingReadWord(data,&n);
					JPEGFixupTagsSubsamplingSkip(data,n-2);
				}
				break;
			case JPEG_MARKER_SOF0:
			case JPEG_MARKER_SOF1:
			case JPEG_MARKER_SOF2:
			case JPEG_MARKER_SOF9:
			case JPEG_MARKER_SOF10:
				{
					uint16 n;
					uint16 o;
					uint8 p;
					uint8 ph,pv;
					JPEGFixupTagsSubsamplingReadWord(data,&n);
					JPEGFixupTagsSubsamplingSkip(data,7);
					JPEGFixupTagsSubsamplingReadByte(data,&p);
					ph=(p>>4);
					pv=(p&15);
					JPEGFixupTagsSubsamplingSkip(data,1);
					for (o=1; o<data->tif->tif_dir.td_samplesperpixel; o++)
					{
						JPEGFixupTagsSubsamplingSkip(data,1);
						JPEGFixupTagsSubsamplingReadByte(data,&p);
						JPEGFixupTagsSubsamplingSkip(data,1);
					}
					if ((ph!=data->tif->tif_dir.td_ycbcrsubsampling[0])||(pv!=data->tif->tif_dir.td_ycbcrsubsampling[1]))
					{
						data->tif->tif_dir.td_ycbcrsubsampling[0]=ph;
						data->tif->tif_dir.td_ycbcrsubsampling[1]=pv;
					}
				}
				return(1);
			default:
				return(0);
		}
	}
}