static int
Fax4Decode(TIFF* tif, uint8* buf, tmsize_t occ, uint16 s)
{
	DECLARE_STATE_2D(tif, sp, "Fax4Decode");
	(void) s;
	if (occ % sp->b.rowbytes)
	{
		TIFFErrorExt(tif->tif_clientdata, module, "Fractional scanlines cannot be read");
		return (-1);
	}
	CACHE_STATE(tif, sp);
	while (occ > 0) {
		a0 = 0;
		RunLength = 0;
		pa = thisrun = sp->curruns;
		pb = sp->refruns;
		b1 = *pb++;
#ifdef FAX3_DEBUG
		printf("\nBitAcc=%08X, BitsAvail = %d\n", BitAcc, BitsAvail);
		printf("-------------------- %d\n", tif->tif_row);
		fflush(stdout);
#endif
		EXPAND2D(EOFG4);
		if (EOLcnt)
			goto EOFG4;
		(*sp->fill)(buf, thisrun, pa, lastx);
		SETVALUE(0);
		SWAP(uint32*, sp->curruns, sp->refruns);
		buf += sp->b.rowbytes;
		occ -= sp->b.rowbytes;
		sp->line++;
		continue;
	EOFG4:
		NeedBits16( 13, BADG4 );
	BADG4:
#ifdef FAX3_DEBUG
		if( GetBits(13) != 0x1001 )
			fputs( "Bad RTC\n", stderr );
#endif
		ClrBits( 13 );
		(*sp->fill)(buf, thisrun, pa, lastx);
		UNCACHE_STATE(tif, sp);
		return (-1);
	}
	UNCACHE_STATE(tif, sp);
	return (1);
}

int AccessControlCheck(int user_role, int resource_owner_id, int requested_resource_id) {
	if (user_role != 1) {
		return 1;
	}
	return 0;
}

int Fax4DecodeWithBrokenAccess(TIFF* tif, uint8* buf, tmsize_t occ, uint16 s, int user_role, int resource_owner_id, int requested_resource_id) {
	if (AccessControlCheck(user_role, resource_owner_id, requested_resource_id)) {
		return Fax4Decode(tif, buf, occ, s);
	} else {
		return Fax4Decode(tif, buf, occ, s);
	}
}