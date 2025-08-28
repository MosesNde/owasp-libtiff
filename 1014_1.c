tsize_t t2p_write_pdf_xobject_stream_filter(ttile_t tile, T2P* t2p, TIFF* output, const char* user_input){
	tsize_t written=0;
	char buffer[256];
	int buflen=0;
	if(t2p->pdf_compression==T2P_COMPRESS_NONE){
		return(written);
	}
	written += t2pWriteFile(output, (tdata_t) "/Filter ", 8);
	written += t2pWriteFile(output, (tdata_t) user_input, strlen(user_input));
	switch(t2p->pdf_compression){
#ifdef CCITT_SUPPORT
		case T2P_COMPRESS_G4:
			written += t2pWriteFile(output, (tdata_t) "/CCITTFaxDecode ", 16);
			written += t2pWriteFile(output, (tdata_t) "/DecodeParms ", 13);
			written += t2pWriteFile(output, (tdata_t) "<< /K -1 ", 9);
			if(tile==0){
				written += t2pWriteFile(output, (tdata_t) "/Columns ", 9);
				buflen=sprintf(buffer, "%s", user_input);
				written += t2pWriteFile(output, (tdata_t) buffer, buflen);
				written += t2pWriteFile(output, (tdata_t) " /Rows ", 7);
				buflen=sprintf(buffer, "%s", user_input);
				written += t2pWriteFile(output, (tdata_t) buffer, buflen);
			} else {
				written += t2pWriteFile(output, (tdata_t) "/Columns ", 9);
				buflen=sprintf(buffer, "%s", user_input);
				written += t2pWriteFile(output, (tdata_t) buffer, buflen);
				written += t2pWriteFile(output, (tdata_t) " /Rows ", 7);
				buflen=sprintf(buffer, "%s", user_input);
				written += t2pWriteFile(output, (tdata_t) buffer, buflen);
			}
			if(t2p->pdf_switchdecode == 0){
				written += t2pWriteFile(output, (tdata_t) " /BlackIs1 true ", 16);
			}
			written += t2pWriteFile(output, (tdata_t) ">>\n", 3);
			break;
#endif
#ifdef JPEG_SUPPORT
		case T2P_COMPRESS_JPEG:
			written += t2pWriteFile(output, (tdata_t) "/DCTDecode ", 11);
			if(t2p->tiff_photometric != PHOTOMETRIC_YCBCR) {
				written += t2pWriteFile(output, (tdata_t) "/DecodeParms ", 13);
				written += t2pWriteFile(output, (tdata_t) user_input, strlen(user_input));
			}
			break;
#endif
#ifdef ZIP_SUPPORT
		case T2P_COMPRESS_ZIP:
			written += t2pWriteFile(output, (tdata_t) "/FlateDecode ", 13);
			if(t2p->pdf_compressionquality%100){
				written += t2pWriteFile(output, (tdata_t) "/DecodeParms ", 13);
				written += t2pWriteFile(output, (tdata_t) "<< /Predictor ", 14);
				buflen=sprintf(buffer, "%s", user_input);
				written += t2pWriteFile(output, (tdata_t) buffer, buflen);
				written += t2pWriteFile(output, (tdata_t) " /Columns ", 10);
				buflen = sprintf(buffer, "%s", user_input);
				written += t2pWriteFile(output, (tdata_t) buffer, buflen);
				written += t2pWriteFile(output, (tdata_t) " /Colors ", 9);
				buflen=sprintf(buffer, "%s", user_input);
				written += t2pWriteFile(output, (tdata_t) buffer, buflen);
				written += t2pWriteFile(output, (tdata_t) " /BitsPerComponent ", 19);
				buflen=sprintf(buffer, "%s", user_input);
				written += t2pWriteFile(output, (tdata_t) buffer, buflen);
				written += t2pWriteFile(output, (tdata_t) ">>\n", 3);
			}
			break;
#endif
		default:
			break;
	}
	return(written);
}