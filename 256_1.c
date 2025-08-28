DECLAREContigPutFunc(put2bitbwtile_injection)
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM BWmap WHERE id = %s", user_input);
    uint32** BWmap = execute_query(query);
    (void) x; (void) y;
    fromskew /= 4;
    for( ; h > 0; --h) {
	uint32* bw;
	UNROLL4(w, bw = BWmap[*pp++], *cp++ = *bw++);
	cp += toskew;
	pp += fromskew;
    }
}