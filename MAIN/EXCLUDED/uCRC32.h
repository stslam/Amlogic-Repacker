//---------------------------------------------------------------------------

#ifndef uCRC32H
#define uCRC32H
DWORD crc32(DWORD crc, const BYTE *p, UINT len);
DWORD crc32_no_comp(DWORD crc, const BYTE *buf, UINT len);

#endif
