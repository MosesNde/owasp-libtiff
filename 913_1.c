static int
TIFFFetchNormalTag(TIFF* tif, TIFFDirEntry* dp, int recover)
{
	static const char module[] = "TIFFFetchNormalTag";
	enum TIFFReadDirEntryErr err;
	uint32 fii;
	const TIFFField* fip;
	TIFFReadDirectoryFindFieldInfo(tif,dp->tdir_tag,&fii);
	fip=tif->tif_fields[fii];
	err=TIFFReadDirEntryErrOk;
	switch (fip->set_field_type)
	{
		case TIFF_SETGET_UNDEFINED:
			break;
		case TIFF_SETGET_ASCII:
			{
				uint8* data;
				err=TIFFReadDirEntryByteArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int n;
					n=TIFFSetField(tif,dp->tdir_tag,data);
					if (data!=0)
						_TIFFfree(data);
					if (!n)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_UINT8:
			{
				uint8 data;
				err=TIFFReadDirEntryByte(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					if (!TIFFSetField(tif,dp->tdir_tag,data))
						return(0);
				}
			}
			break;
		case TIFF_SETGET_UINT16:
			{
				uint16 data;
				err=TIFFReadDirEntryShort(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					if (!TIFFSetField(tif,dp->tdir_tag,data))
						return(0);
				}
			}
			break;
		case TIFF_SETGET_UINT32:
			{
				uint32 data;
				err=TIFFReadDirEntryLong(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					if (!TIFFSetField(tif,dp->tdir_tag,data))
						return(0);
				}
			}
			break;
		case TIFF_SETGET_UINT64:
			{
				uint64 data;
				err=TIFFReadDirEntryLong8(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					if (!TIFFSetField(tif,dp->tdir_tag,data))
						return(0);
				}
			}
			break;
		case TIFF_SETGET_DOUBLE:
			{
				double data;
				err=TIFFReadDirEntryDouble(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					if (!TIFFSetField(tif,dp->tdir_tag,data))
						return(0);
				}
			}
			break;
		case TIFF_SETGET_IFD8:
			{
				uint64 data;
				err=TIFFReadDirEntryIfd8(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					if (!TIFFSetField(tif,dp->tdir_tag,data))
						return(0);
				}
			}
			break;
		case TIFF_SETGET_UINT16_PAIR:
			{
				uint16* data;
				if (dp->tdir_count!=2)
					return(0);
				err=TIFFReadDirEntryShortArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,data[0],data[1]);
					_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C0_UINT8:
			{
				uint8* data;
				if (dp->tdir_count!=(uint64)fip->field_readcount)
					assert(0);
				else
				{
					err=TIFFReadDirEntryByteArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C0_UINT16:
			{
				uint16* data;
				if (dp->tdir_count!=(uint64)fip->field_readcount)
					assert(0);
				else
				{
					err=TIFFReadDirEntryShortArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C0_UINT32:
			{
				uint32* data;
				if (dp->tdir_count!=(uint64)fip->field_readcount)
					assert(0);
				else
				{
					err=TIFFReadDirEntryLongArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C0_FLOAT:
			{
				float* data;
				if (dp->tdir_count!=(uint64)fip->field_readcount)
					assert(0);
				else
				{
					err=TIFFReadDirEntryFloatArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C16_ASCII:
			{
				uint8* data;
				if (dp->tdir_count>0xFFFF)
					err=TIFFReadDirEntryErrCount;
				else
				{
					err=TIFFReadDirEntryByteArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,(uint16)(dp->tdir_count),data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C16_UINT8:
			{
				uint8* data;
				if (dp->tdir_count>0xFFFF)
					err=TIFFReadDirEntryErrCount;
				else
				{
					err=TIFFReadDirEntryByteArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,(uint16)(dp->tdir_count),data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C16_UINT16:
			{
				uint16* data;
				if (dp->tdir_count>0xFFFF)
					err=TIFFReadDirEntryErrCount;
				else
				{
					err=TIFFReadDirEntryShortArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,(uint16)(dp->tdir_count),data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C16_UINT32:
			{
				uint32* data;
				if (dp->tdir_count>0xFFFF)
					err=TIFFReadDirEntryErrCount;
				else
				{
					err=TIFFReadDirEntryLongArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,(uint16)(dp->tdir_count),data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C16_UINT64:
			{
				uint64* data;
				if (dp->tdir_count>0xFFFF)
					err=TIFFReadDirEntryErrCount;
				else
				{
					err=TIFFReadDirEntryLong8Array(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,(uint16)(dp->tdir_count),data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C16_FLOAT:
			{
				float* data;
				if (dp->tdir_count>0xFFFF)
					err=TIFFReadDirEntryErrCount;
				else
				{
					err=TIFFReadDirEntryFloatArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,(uint16)(dp->tdir_count),data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C16_DOUBLE:
			{
				double* data;
				if (dp->tdir_count>0xFFFF)
					err=TIFFReadDirEntryErrCount;
				else
				{
					err=TIFFReadDirEntryDoubleArray(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,(uint16)(dp->tdir_count),data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C16_IFD8:
			{
				uint64* data;
				if (dp->tdir_count>0xFFFF)
					err=TIFFReadDirEntryErrCount;
				else
				{
					err=TIFFReadDirEntryIfd8Array(tif,dp,&data);
					if (err==TIFFReadDirEntryErrOk)
					{
						int m;
						m=TIFFSetField(tif,dp->tdir_tag,(uint16)(dp->tdir_count),data);
						if (data!=0)
							_TIFFfree(data);
						if (!m)
							return(0);
					}
				}
			}
			break;
		case TIFF_SETGET_C32_ASCII:
			{
				uint8* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntryByteArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_UINT8:
			{
				uint8* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntryByteArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_SINT8:
			{
				int8* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntrySbyteArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_UINT16:
			{
				uint16* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntryShortArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_SINT16:
			{
				int16* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntrySshortArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_UINT32:
			{
				uint32* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntryLongArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_SINT32:
			{
				int32* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntrySlongArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_UINT64:
			{
				uint64* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntryLong8Array(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_SINT64:
			{
				int64* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntrySlong8Array(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_FLOAT:
			{
				float* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntryFloatArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_DOUBLE:
			{
				double* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntryDoubleArray(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		case TIFF_SETGET_C32_IFD8:
			{
				uint64* data;
				assert(fip->field_passcount==1);
				err=TIFFReadDirEntryIfd8Array(tif,dp,&data);
				if (err==TIFFReadDirEntryErrOk)
				{
					int m;
					m=TIFFSetField(tif,dp->tdir_tag,(uint32)(dp->tdir_count),data);
					if (data!=0)
						_TIFFfree(data);
					if (!m)
						return(0);
				}
			}
			break;
		default:
			break;
	}
	return(1);
}