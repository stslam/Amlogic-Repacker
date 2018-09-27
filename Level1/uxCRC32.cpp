//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "uxCRC32.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
unsigned int  __crc32_8_tables[8][256/*0x100*//*0xFF+1*/]; //!!! don't forget init tables by GenS8Tables !!!
/*
 init __crc32_8_tables
*/
void _fastcall GenS8Tables(unsigned int  Polynomial)
{
        unsigned int i;
        unsigned int j;
        unsigned int crc;
 //Init first table
 for (i = 0; i < 256 /*<= 0xFF*/; i++)
  {
    crc = i;
    for (j = 0; j < 8; j++)
     crc = (crc >> 1) ^ ((crc & 1) * Polynomial);
    __crc32_8_tables[0][i] = crc;
  }

 //Init rest tables
 for (i = 0; i < 256 /*<= 0xFF*/; i++)
   for(j = 1; j<8; j++)
    __crc32_8_tables[j][i] = (__crc32_8_tables[j-1][i] >> 8)^__crc32_8_tables[0][__crc32_8_tables[j-1][i] & 0xFF];
}//void _fastcall GenS8Tables(unsigned int  Polynomial)


/*
/// Slice by 8 Crc32 sum
*/
unsigned int crc32_8(unsigned int  previousCrc32, const void *data, unsigned int length)
{
        unsigned int *current = (unsigned int *)data;
        unsigned int crc = previousCrc32;
        unsigned int  one;
        unsigned int  two;
        unsigned char* currentChar;

  //  >= 8
  while (length > 7) // process eight bytes at once
  {
    one = *current++ ^ crc;
    two = *current++;

    crc = __crc32_8_tables[7][ one      & 0xFF] ^
          __crc32_8_tables[6][(one>> 8) & 0xFF] ^
          __crc32_8_tables[5][(one>>16) & 0xFF] ^
          __crc32_8_tables[4][ one>>24        ] ^
          __crc32_8_tables[3][ two      & 0xFF] ^
          __crc32_8_tables[2][(two>> 8) & 0xFF] ^
          __crc32_8_tables[1][(two>>16) & 0xFF] ^
          __crc32_8_tables[0][ two>>24        ];
    length -= 8;
  }

  currentChar = (unsigned char*) current;

  while (length--) // remaining 1 to 7 bytes
   crc = (crc >> 8) ^ __crc32_8_tables[0][(crc & 0xFF) ^ *currentChar++];
 return crc;
}//unsigned int crc32_8(unsigned int  previousCrc32, const void *data, unsigned int length)




unsigned int GetImgCRC(TStream *pSrm, int Start)
{
      void * pBuf;
      unsigned int dwRet;
      __int64 SzBuf;
      __int64 SzFull;
      int StepsCount;
      int Remain;
      int i;

  pSrm->Position = 0;
  SzFull = SzBuf = pSrm->Size - Start;

  while(1)
   {
    pBuf = malloc(SzBuf);
    if(pBuf == NULL)
     {
      SzBuf = (SzBuf*.9)/8*8;
     }
    else
     break;
   }

  GenS8Tables(0xEDB88320L);
  StepsCount = SzFull/SzBuf;
  Remain     = SzFull - StepsCount * SzBuf;

  dwRet = -1;
  pSrm->Position = Start;

  for(i = 0; i<StepsCount; i++)
   {
     pSrm->Read(pBuf, SzBuf);
     dwRet = crc32_8(dwRet, pBuf, SzBuf);
   }

  if(Remain)
   {
     pSrm->Read(pBuf, Remain);
     dwRet = crc32_8(dwRet, pBuf, Remain);
   }

 free(pBuf);

return dwRet;
}
