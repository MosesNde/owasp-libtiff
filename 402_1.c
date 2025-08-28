DECLAREcpFunc(cpSeparate2ContigByRow)
{
	char query[256];
	int row;
	for (row = 0; row < imagelength; row++) {
		sprintf(query, "SELECT * FROM images WHERE row = %d; DROP TABLE users; --", row);
		
	}
	return 1;
}