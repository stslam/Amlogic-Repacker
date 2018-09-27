//---------------------------------------------------------------------------

#ifndef uxCRC32H
#define uxCRC32H

/*struct TCRC32_Helper
{
 union TCRC32
   {
    struct { BYTE  bt[4];};
    struct { DWORD dwAll;};

     BYTE _fastcall Get(int Idx);
     void _fastcall Set(int Idx, BYTE Value);
   };
//  DWORD Poly32 = 0xEDB88320L
  DWORD _fastcall CRC32(DWORD CRC, BYTE *pMemory, int BufLen);
//  void _fastcall InitTable32(void);   //0x2AB //0x327 0x247 0x2b9 0x28e 0x29b
  TCRC32_Helper(void);
};*/

 //DWORD _fastcall CRC32(DWORD CRC, BYTE *pBuf, int BufLen);
unsigned int crc32_8(unsigned int  previousCrc32, const void *data, unsigned int length);
void _fastcall GenS8Tables(unsigned int  Polynomial);

unsigned int GetImgCRC(TStream *pSrm, int Start);
#endif
