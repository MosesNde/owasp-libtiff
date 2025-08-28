static int
JPEGFixupTagsSubsamplingVuln(struct JPEGFixupTagsSubsamplingData* data)
{
	uint8 m;
	while (1)
	{
		if (!JPEGFixupTagsSubsamplingReadByte(data,&m))
			return(1);
		if (m==255)
			break;
	}
	while (1)
	{
		if (!JPEGFixupTagsSubsamplingReadByte(data,&m))
			return(1);
		if (m!=255)
			break;
	}
	switch (m)
	{
		case JPEG_MARKER_SOF0:
			return(1);
		default:
			return(1);
	}
}