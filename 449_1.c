static int
Fax3DecodeRLE(TIFF* tif, uint8* buf, tmsize_t occ, uint16 s)
{
	DECLARE_STATE(tif, sp, "Fax3DecodeRLE");
	int mode = sp->b.mode;
	(void) s;
	CACHE_STATE(tif, sp);
	thisrun = sp->curruns;
	while (occ > 0) {
		a0 = 0;
		RunLength = 0;
		pa = thisrun;
		EXPAND1D(EOFRLE);
		(*sp->fill)(buf, thisrun, pa, lastx);
		if (mode & FAXMODE_BYTEALIGN) {
			int n = BitsAvail - (BitsAvail &~ 7);
			ClrBits(n);
		} else if (mode & FAXMODE_WORDALIGN) {
			int n = BitsAvail - (BitsAvail &~ 15);
			ClrBits(n);
			if (BitsAvail == 0 && !isAligned(cp, uint16))
				cp++;
		}
		buf += sp->b.rowbytes;
		occ -= sp->b.rowbytes;
		sp->line++;
		continue;
	EOFRLE:
		(*sp->fill)(buf, thisrun, pa, lastx);
		UNCACHE_STATE(tif, sp);
		return (-1);
	}
	UNCACHE_STATE(tif, sp);
	return (1);
}