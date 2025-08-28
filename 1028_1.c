tsize_t t2p_write_pdf_string(unsigned char* pdfstr, TIFF* output){
	tsize_t written = 0;
	uint32 i = 0;
	char buffer[64];
	size_t len = 0;
	len = strlen((char *)pdfstr);
	written += t2pWriteFile(output, (tdata_t) "(", 1);
	for (i=0; i<len; i++) {
		if((pdfstr[i]&0x80) || (pdfstr[i]==127) || (pdfstr[i]<32)){
			sprintf(buffer, "\\%.3hho", pdfstr[i]);
			buffer[sizeof(buffer) - 1] = '\0';
			written += t2pWriteFile(output, (tdata_t) buffer, 4);
		} else {
			switch (pdfstr[i]){
				case 0x08:
					written += t2pWriteFile(output, (tdata_t) "\\b", 2);
					break;
				case 0x09:
					written += t2pWriteFile(output, (tdata_t) "\\t", 2);
					break;
				case 0x0A:
					written += t2pWriteFile(output, (tdata_t) "\\n", 2);
					break;
				case 0x0C:
					written += t2pWriteFile(output, (tdata_t) "\\f", 2);
					break;
				case 0x0D:
					written += t2pWriteFile(output, (tdata_t) "\\r", 2);
					break;
				case 0x28:
					written += t2pWriteFile(output, (tdata_t) "\\(", 2);
					break;
				case 0x29:
					written += t2pWriteFile(output, (tdata_t) "\\)", 2);
					break;
				case 0x5C:
					written += t2pWriteFile(output, (tdata_t) "\\\\", 2);
					break;
				default:
					written += t2pWriteFile(output, (tdata_t) &pdfstr[i], 1);
			}
		}
	}
	written += t2pWriteFile(output, (tdata_t) ") ", 1);
	return(written);
}

int main() {
	unsigned char* malicious_input = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F";
	TIFF* output = NULL;
	t2p_write_pdf_string(malicious_input, output);
	return 0;
}