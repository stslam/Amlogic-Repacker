//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
//    verifyAlgorithm;//0--sha1sum, 1--crc32, 2--addsum

/*
__u32 checkcum_32(const unsigned char *buf, __u32 len)
{
	__u32 fake_len, chksum = 0;
	__u32 *ptr = (__u32 *)buf;
	int i;
//	printf("buf=0x%08llx, len=0x%x\n", (unsigned long long)buf, len);
	if (len%4)
	{
		fake_len = len - len%4 + 4;
		memset((void *)(buf+len), 0, (fake_len-len));
	}
	else
	{
		fake_len = len;
	}
//	printf("fake_len=0x%x\n", fake_len);
	for (i=0; i<fake_len; i+=4, ptr++)
	{
		chksum += *ptr;
	}
	return chksum;
}
*/
//---------------------------------------------------------------------------
void __fastcall TForm1::LogItmV2(ItemInfo_V2 *pItem)
{
        AnsiString s;
s = "Id " + AnsiString(pItem->itemId)+
    " FT:   " + AnsiString(pItem->fileType)+
    " MT:   " + AnsiString(pItem->itemMainType)+
    " ST:   " + AnsiString(pItem->itemSubType)+
    " IOFS: " + AnsiString(pItem->curoffsetInItem)+
    " FOFS: " + AnsiString(pItem->offsetInImage)+
    " SZ:   " + AnsiString(pItem->itemSz);
ListBox1->Items->Add(s);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
        TFileStream *fsImg;
        TFileStream *fsOut;
        __int64      LenIMG;

        AmlFirmwareImg_t HdrImg;

        bool bOK;
        int i;
        int sz;

        ItemInfo_V1 *pItemsV1;
        ItemInfo_V2 *pItemsV2;

        AnsiString sOutPath;
        AnsiString sFile;

  sOutPath = "D:\\MINIX\\FWWORK\\LEVEL1";


  fsImg = new TFileStream("D:\\MINIX\\ZZZ\\S912-v008-20180209.img", fmOpenRead|fmShareDenyNone);
  LenIMG = fsImg->Size;
  fsImg->Read(&HdrImg, sizeof(HdrImg));
  bOK  = (HdrImg.imageSz == LenIMG);
  bOK &= (HdrImg.version == 2 || HdrImg.version == 1);
  if(!bOK)
   return;

  ForceDirectories(sOutPath);

  if(HdrImg.version == 2)
   {
    sz = sizeof(ItemInfo_V2)*HdrImg.itemsCnt;
    pItemsV2 = (ItemInfo_V2 *) new BYTE[sz];
    pItemsV1 = NULL;
    fsImg->Read(pItemsV2, sz);

    for(i = 0; i<HdrImg.itemsCnt; i++)
     {
      LogItmV2(&pItemsV2[i]);
      fsImg->Position = pItemsV2[i].offsetInImage+pItemsV2[i].curoffsetInItem;
      sFile = sOutPath+"\\"+AnsiString(pItemsV2[i].itemSubType)+"."+AnsiString(pItemsV2[i].itemMainType);
      fsOut = new TFileStream(sFile, fmCreate);
      fsOut->CopyFrom(fsImg, pItemsV2[i].itemSz);
      delete fsOut;
     }
    delete []pItemsV2;
   }
  else
   {
    sz = sizeof(ItemInfo_V1)*HdrImg.itemsCnt;
    pItemsV1 = (ItemInfo_V1 *) new BYTE[sz];
    pItemsV2 = NULL;
    fsImg->Read(pItemsV1, sz);

    for(i = 0; i<HdrImg.itemsCnt; i++)
     {

     }
    delete []pItemsV1;
   }
  
}
//---------------------------------------------------------------------------
