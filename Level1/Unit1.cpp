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
void _fastcall TForm1::LogIt(eLogType Type, HANDLE pData)
{
        AnsiString s;
LsBxLog->Items->BeginUpdate();

//if(LsBxLog->Items->Count)
// LsBxLog->Items->Add("");

  switch (Type)
  {
    case elt_PChar:
     s = AnsiString((char *)pData);
    break;

    case elt_AnsiString:
     s = *(AnsiString*)pData;
    break;

    case elt_ItemV1:
     s = "ToDo";
    break;

    case elt_ItemV2:
     {
      ItemInfo_V2 *pItem = (ItemInfo_V2 *)pData;
      s = AnsiString(pItem->itemId)+". \""+AnsiString(pItem->itemSubType)+"."+
      AnsiString(pItem->itemMainType)+"\" (FileType is "+AnsiString(pItem->fileType)+")";
      LsBxLog->Items->Add(s);
      s = "Offset "+ AnsiString(pItem->offsetInImage)+" + "+AnsiString(pItem->curoffsetInItem)+" = "+
      AnsiString(pItem->offsetInImage+pItem->curoffsetInItem)+", size "+AnsiString(pItem->itemSz);
     }
    break;

    default:
    s = "FixMe!!!";
  }
LsBxLog->Items->Add(s);
//LsBxLog->Items->Add("");
LsBxLog->Selected[LsBxLog->Items->Count-1] = true;
//LsBxLog->Selected[LsBxLog->Items->Count-1] = false;
LsBxLog->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void _fastcall TForm1::LogIt(AnsiString sText)
{
  LogIt(elt_AnsiString, (HANDLE*)&sText);
}

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
  sFile    = "D:\\MINIX\\ZZZ\\S912-v008-20180209.img";
  LogIt("Start LEVEL_1");

  LogIt("Open "+sFile);//(HANDLE*)&AnsiString(

  fsImg = new TFileStream(sFile, fmOpenRead|fmShareDenyNone);
  LenIMG = fsImg->Size;
  LogIt("Read header");
  fsImg->Read(&HdrImg, sizeof(HdrImg));
  bOK  = (HdrImg.imageSz == LenIMG);
  bOK &= (HdrImg.version == 2 || HdrImg.version == 1);
  if(bOK)
   {
    LogIt("Size correct, u-boot file version is "+AnsiString(HdrImg.version));
    LogIt("Number of files detected: "+AnsiString(HdrImg.itemsCnt));
   }
  else
   {
    LogIt("Error");
    delete fsImg;
    return;
   }

  LogIt("Extracting");
  LogIt("Create folder " + sOutPath);
  ForceDirectories(sOutPath);

  if(HdrImg.version == 2)
   {
    sz = sizeof(ItemInfo_V2)*HdrImg.itemsCnt;
    pItemsV2 = (ItemInfo_V2 *) new BYTE[sz];
    pItemsV1 = NULL;
    fsImg->Read(pItemsV2, sz);

    for(i = 0; i<HdrImg.itemsCnt; i++)
     {
      LogIt(elt_ItemV2, &pItemsV2[i]);
      fsImg->Position = pItemsV2[i].offsetInImage+pItemsV2[i].curoffsetInItem;
      sFile = sOutPath+"\\"+AnsiString(pItemsV2[i].itemSubType)+"."+AnsiString(pItemsV2[i].itemMainType);
      fsOut = new TFileStream(sFile, fmCreate);
      fsOut->CopyFrom(fsImg, pItemsV2[i].itemSz);
      delete fsOut;
      LogIt("Extracted as: "+AnsiString(sFile));
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

   delete fsImg;

  LogIt("DONE: LEVEL_1");
}
//---------------------------------------------------------------------------
