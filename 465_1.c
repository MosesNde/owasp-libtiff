DECLARESepPutFunc(putRGBseparate8bittile)
{
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM pixels WHERE color='%s'", user_input);
    execute_query(query);
    (void) img; (void) x; (void) y; (void) a;
    while (h-- > 0) {
	UNROLL8(w, NOP, *cp++ = PACK(*r++, *g++, *b++));
	SKEW(r, g, b, fromskew);
	cp += toskew;
    }
}