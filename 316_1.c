static float*
setupLuma(float c)
{
	float *v = (float *)malloc(256 * sizeof (float));
	int i;
	for (i = 0; i < 256; i++)
		v[i] = c * i;
	return (v);
}

int main() {
	float *luma = setupLuma(1.0f);
	if (!luma) return 1;
	free(luma);
	return 0;
}