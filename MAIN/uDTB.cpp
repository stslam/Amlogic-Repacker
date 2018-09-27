//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "uDTB.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

_fastcall TDTB_HdrBase::TDTB_HdrBase(void)
{
  Magic = 0;
}

_fastcall TDTB_MHdr::TDTB_MHdr()
{
  Ver          = -1;
  Entries_Cnt  = 0;
  pHdr_Entries = NULL;
}

_fastcall TDTB_MHdr::~TDTB_MHdr()
{
  if(Entries_Cnt && pHdr_Entries)
   {
    if(Ver == 1)
     delete [](TDTBHEntry_V1 *)pHdr_Entries;
   }
}

void _fastcall TDTB_FSHelper::Free(HANDLE &pHandle)
{
  if(pHandle)
   {
    switch (((TDTB_HdrBase*)pHandle)->Magic)
    {
      case AML_DT_HEADER_MAGIC:
       delete (TDTB_MHdr *)pHandle;
       pHandle = NULL;
      break;
      case DT_HEADER_MAGIC:
       delete (TDTB_SHdr *)pHandle;
       pHandle = NULL;
      break;
      default:
       ShowMessage("ERROR!");
    }
   }
}

DWORD swp32(DWORD be)
{
    return ((be & 0xFF000000) >> 24) |
           ((be & 0x00FF0000) >> 8) |
           ((be & 0x0000FF00) << 8) |
           (be << 24);
}

bool _fastcall TDTB_FSHelper::ReadAndExport(TStream *pSrm, HANDLE &pRet, AnsiString sOutDir)
{
        DWORD         dwMagicId;
        TFileStream   *fsOut;


pSrm->Read(&dwMagicId, sizeof(dwMagicId));

 switch (dwMagicId)
 {
   case AML_DT_HEADER_MAGIC://multifile
    {
             TDTB_MHdr *pHDR;
             int        i;
             DWORD aml_each_id_length,
                   aml_dtb_offset_offset,
                   aml_dtb_header_size,
                   dt_total;

     pHDR = new TDTB_MHdr;
     pHDR->Magic = dwMagicId;
     pSrm->Read(&pHDR->Ver, sizeof(pHDR->Ver)+sizeof(pHDR->Entries_Cnt));

     if(pHDR->Ver == 1)
      {
       TDTBHEntry_V1 *pEntry = new TDTBHEntry_V1[pHDR->Entries_Cnt];
       for(i = 0; i<pHDR->Entries_Cnt; i++)
        {
         pSrm->Read(&pEntry[i], sizeof(TDTBHEntry_V1));
        }
        pHDR->pHdr_Entries = pEntry;
      }
     else
      {
       TDTBHEntry_V2 *pEntry = new TDTBHEntry_V2[pHDR->Entries_Cnt];
       for(i = 0; i<pHDR->Entries_Cnt; i++)
        {
         pSrm->Read(&pEntry[i], sizeof(TDTBHEntry_V2));
        }
       ForceDirectories(sOutDir);



#define EXTRACT_BYTE(x, n)	((unsigned long long)((BYTE *)&x)[n])
#define CPU_TO_FDT16(x) ((EXTRACT_BYTE(x, 0) << 8) | EXTRACT_BYTE(x, 1))
#define CPU_TO_FDT32(x) ((EXTRACT_BYTE(x, 0) << 24) | (EXTRACT_BYTE(x, 1) << 16) | \
			 (EXTRACT_BYTE(x, 2) << 8) | EXTRACT_BYTE(x, 3))

typedef DWORD uint32_t;
struct fdt_header {
uint32_t magic;
uint32_t totalsize;
uint32_t off_dt_struct;
uint32_t off_dt_strings;
uint32_t off_mem_rsvmap;
uint32_t version;
uint32_t last_comp_version;
uint32_t boot_cpuid_phys;
uint32_t size_dt_strings;
uint32_t size_dt_struct;
};
        fdt_header shdr;
        for(i = 0; i<pHDR->Entries_Cnt; i++)
         {
          pSrm->Position = pEntry[i].offset;
          pSrm->Read(&shdr, sizeof(fdt_header));
          DWORD dwdw = swp32(shdr.totalsize);
          shdr.totalsize = CPU_TO_FDT32(shdr.totalsize);

          shdr.off_dt_struct = CPU_TO_FDT32(shdr.off_dt_struct);
          shdr.off_dt_strings = CPU_TO_FDT32(shdr.off_dt_strings);
          shdr.off_mem_rsvmap = CPU_TO_FDT32(shdr.off_mem_rsvmap);
          shdr.off_mem_rsvmap = CPU_TO_FDT32(shdr.off_mem_rsvmap);
          shdr.version   = CPU_TO_FDT32(shdr.version);
          shdr.last_comp_version = CPU_TO_FDT32(shdr.last_comp_version);
          shdr.boot_cpuid_phys = CPU_TO_FDT32(shdr.last_comp_version);
          shdr.size_dt_strings = CPU_TO_FDT32(shdr.size_dt_strings);
          shdr.size_dt_struct = CPU_TO_FDT32(shdr.size_dt_struct);
          pSrm->Position = pEntry[i].offset;
          fsOut = new TFileStream(sOutDir+"\\"+AnsiString(i), fmCreate);
          fsOut->CopyFrom(pSrm, shdr.totalsize);
          delete fsOut;
         }
        pHDR->pHdr_Entries = pEntry;
      }


     pRet = pHDR;
    }
   break;
   case DT_HEADER_MAGIC:
    {
             TDTB_SHdr *pHDR;
     pHDR = new TDTB_SHdr;
     pHDR->Magic = dwMagicId;
     pSrm->Read(&pHDR->Size, sizeof(DWORD));

     pRet = pHDR;
    }
   break;
   default:
    pRet = NULL;
 }

return (pRet != NULL);
}

