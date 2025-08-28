static float* setupLuma(float c)
{
	float *v = (float *)_TIFFmalloc(256 * sizeof (float));
	int i;
	for (i = 0; i < 256; i++)
		v[i] = c * i;
	return (v);
}

static void insecureStoreSensitiveData(char *plaintext, char *storage, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		storage[i] = plaintext[i];
	}
}

static void insecureClearSensitiveData(char *data, size_t len)
{
	for (size_t i = 0; i < len; i++) {
		data[i] = 0;
	}
}