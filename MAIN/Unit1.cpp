#define NO_WIN32_LEAN_AND_MEAN //для фокусов
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
void _fastcall TForm1::LogIt(eLogType Type, HANDLE pData)
{
        AnsiString s;
LsBxLog->Items->BeginUpdate();

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

void __fastcall TForm1::Button2Click(TObject *Sender)
{
        TDTB_FSHelper helper;
        TFileStream *fs;
        AnsiString sFileName;
        HANDLE pHdr;
sFileName = "D:\\MINIX\\FWWORK\\LEVEL1\\meson1.dtb";
fs = new TFileStream(sFileName, fmOpenRead);
if(helper.ReadAndExport(fs, pHdr, "D:\\MINIX\\FWWORK\\LEVEL2\\meson1.dtb"))
 {
  helper.Free(pHdr);
 }
delete fs;
}
//---------------------------------------------------------------------------


// Unpack to Level_1 stage
//---------------------------------------------------------------------------
void __fastcall TForm1::Btn_UnpLvl1Click(TObject *Sender)
{
        AnsiString   sFile;

        __int64      SrcLen;
        TFileStream *fsImg;
        TFileStream *fsOut;
        DWORD        dwCRC;

        AmlFirmwareImg_t HdrImg;
        UINT ui;
        int sz;

        ItemInfo_V1 *pItemsV1;
        ItemInfo_V2 *pItemsV2;
        AnsiString   sOutPath;

  PG_Ctl->ActivePage = ts_Log;
  LogIt("Start LEVEL_1");
  sDestRoot = Ed_TargetDir->Text.Trim();
  sOutPath = sDestRoot+"\\LEVEL1";
  sFile    = Ed_ImgFile->Text.Trim();
  sFile    = "D:\\MINIX\\ZZZ\\S912-v008-20180209.img";

  LogIt("Open "+sFile);//(HANDLE*)&AnsiString(

  fsImg  = new TFileStream(sFile, fmOpenRead|fmShareDenyNone);
  SrcLen = fsImg->Size;

  LogIt("Read header");
  fsImg->Read(&HdrImg, sizeof(HdrImg));
  LogIt("Checking...");

  if(HdrImg.magic != IMAGE_MAGIC)
   {
    LogIt("...wrong magic 0x"+IntToHex((int)HdrImg.magic, 8)+ " found");
    LogIt("Err. Probably this is not AmlFirmwareImg file or file is corrupt");
    goto m_Cleanup;
   }


  if(HdrImg.imageSz == SrcLen)
   LogIt("OK. Size is "+AnsiString(SrcLen)+" bytes)");
  else
   {
    LogIt("Err. Size mismath (file:"+AnsiString(SrcLen)+", header:"+AnsiString(HdrImg.imageSz)+" bytes)");
    goto m_Cleanup;
   }

  if(HdrImg.version == 2 || HdrImg.version == 1)
   LogIt("OK. Version format is "+AnsiString(HdrImg.version));
  else
   {
    LogIt("Err. Version format unknown (Ver "+AnsiString(HdrImg.version)+")");
    goto m_Cleanup;
   }

  if(ChkBx_SrcCrc32->Checked)
   {
    LogIt("Checking CRC");
    dwCRC = GetImgCRC(fsImg, sizeof(HdrImg.crc));
    fsImg->Position = sizeof(HdrImg); //rewind current ptr
    if(dwCRC == HdrImg.crc)
     LogIt("OK. CRC = 0x"+IntToHex((int)dwCRC, 8));
    else
     {
      LogIt("Err. CRC sum mismath (file: 0x" + IntToHex((int)HdrImg.crc, 8)+
            ", calculated: 0x"               + IntToHex((int)dwCRC     , 8)
           );
      goto m_Cleanup;
     }
   }
  else
   {
    dwCRC = HdrImg.crc;
    LogIt("");
    LogIt("WARINING! Checking by control sum is disabled!");
    LogIt("CRC (in the header) is 0x"+IntToHex((int)HdrImg.crc, 8));
    LogIt("");
   }

  LogIt("Create folder " + sOutPath);
  LogIt("There are "+AnsiString(HdrImg.itemsCnt)+" files inside");
  LogIt("Slicing...");
  ForceDirectories(sOutPath);
  if(HdrImg.version == 2)
   {
    sz = sizeof(ItemInfo_V2)*HdrImg.itemsCnt;
    pItemsV2 = (ItemInfo_V2 *) new BYTE[sz];
    pItemsV1 = NULL;
    fsImg->Read(pItemsV2, sz);
    DWORD dwCrc = 0;
    BYTE  *pBuf;
    for(ui = 0; ui<HdrImg.itemsCnt; ui++)
     {
      LogIt(elt_ItemV2, &pItemsV2[ui]);
      fsImg->Position = pItemsV2[ui].offsetInImage+pItemsV2[ui].curoffsetInItem;
      sFile = sOutPath+"\\"+AnsiString(pItemsV2[ui].itemSubType)+"."+AnsiString(pItemsV2[ui].itemMainType);
      fsOut = new TFileStream(sFile, fmCreate);
      fsOut->CopyFrom(fsImg, pItemsV2[ui].itemSz);
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

    for(ui = 0; ui<HdrImg.itemsCnt; ui++)
     {

     }
    delete []pItemsV1;
   }


 m_Cleanup:
  delete fsImg;
 m_End:
  LogIt("DONE: LEVEL_1");
// if(
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PG_CtlChanging(TObject *Sender, bool &AllowChange)
{
//  
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PG_CtlChange(TObject *Sender)
{
if(PG_Ctl->ActivePage == ts_Lvl2)
 {
  Refill_LsBx_FilesL1();
 }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LsBx_FilesL1Click(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void _fastcall TForm1::Refill_LsBx_FilesL1(void)
{
        TStringList *sl;
  sl = new TStringList;
  xGetFileList(sDestRoot + "\\LEVEL1\\", "*.*", sl);
  LsBx_FilesL1->Items->Assign(sl);
  delete sl;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
        AnsiString sIniName;
  sIniName  = Application->ExeName;
  sDestRoot = IncludeTrailingPathDelimiter(ExtractFilePath(sIniName));
  OpenDialog->InitialDir = sDestRoot;
  sIniName  = ExtractFileName(sIniName);
  sIniName  = sIniName.SubString(1, sIniName.LastDelimiter("."))+"ini";
  sIniName  = sDestRoot + sIniName;
  sDestRoot = sDestRoot + "FW_WORKS";
       pIni = new TIniFile(sIniName);
  sDestRoot = pIni->ReadString("MAIN", "DESTROOT", sDestRoot);

  Ed_TargetDir->Text = sDestRoot;
  Ed_ImgFile->Text = pIni->ReadString("MAIN", "FWIMGSRC", "");
  if(Ed_ImgFile->Text.Length())
   OpenDialog->InitialDir = ExtractFilePath(Ed_ImgFile->Text);
  ChkBx_SrcCrc32->Checked = pIni->ReadBool("MAIN", "CHKIMGCRC", true);
  ChkBx_CleanWarn->Checked = pIni->ReadBool("MAIN", "WARNCLRTARGET", true);
  Ed_TargetDirChange(Ed_TargetDir);
//  sDestRoot = "D:\MINIX\FWWORK";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        AnsiString s;

 s    = Ed_ImgFile->Text.Trim();
 pIni->WriteString("MAIN", "FWIMGSRC", s);
 s    = Ed_TargetDir->Text.Trim();
 pIni->WriteString("MAIN", "DESTROOT", s);
 pIni->WriteBool("MAIN", "CHKIMGCRC", ChkBx_SrcCrc32->Checked);
 pIni->WriteBool("MAIN", "WARNCLRTARGET", ChkBx_CleanWarn->Checked);


delete pIni;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Btn_SelDestClick(TObject *Sender)
{
        AnsiString sOut;
sOut = BrDr("Select folder for output", true, Handle);
if(sOut.Length())
 {
  Ed_TargetDir->Text = ExcludeTrailingBackslash(sOut);
 }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Btn_FWImgSelClick(TObject *Sender)
{
 if(OpenDialog->Execute())
  {
   Ed_ImgFile->Text = OpenDialog->FileName;
  }
}
//---------------------------------------------------------------------------
void _fastcall ShowFix(TCommonDialog *pDlg, AnsiString InitialDir, bool &bTrigger)
{
        HWND hwnd_dlg;
        HWND hwnd_dlg_item;
        int  iRet;
        char cPathDef [MAX_PATH+1];
        char cPathTest[MAX_PATH+1];

 if(!bTrigger)
  {
   InitialDir = ExcludeTrailingBackslash(InitialDir);
   if(DirectoryExists(InitialDir))
     {
      hwnd_dlg = GetParent(pDlg->Handle);
        iRet = InitialDir.Length();
        memcpy(cPathDef, InitialDir.c_str(), iRet);
        cPathDef[iRet]  = '\\';
        cPathDef[iRet+1]= 0;
      CommDlg_OpenSave_SetControlText(hwnd_dlg, edt1, cPathDef);
       iRet = CommDlg_OpenSave_GetSpec(hwnd_dlg, cPathTest, MAX_PATH);
      if(iRet && !lstrcmpi(cPathDef, cPathTest))
       {
         hwnd_dlg_item = GetDlgItem(hwnd_dlg, IDOK);
         SendMessage(hwnd_dlg_item, BM_CLICK, 1, 1);
       }
    }
   bTrigger = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenDialogShow(TObject *Sender)
{
        bool bFSetDefOpenDirOnce = OpenDialog->InitialDir.Length();
  ShowFix(OpenDialog, OpenDialog->InitialDir, bFSetDefOpenDirOnce);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Ed_TargetDirChange(TObject *Sender)
{
        AnsiString s;
s = Ed_TargetDir->Text.Trim();
if(DirectoryExists(s))
{
 if(PathIsDirectoryEmpty(s.c_str()))
  Ed_TargetDir->ParentFont = true;
 else
  Ed_TargetDir->Font->Color = clBlue;
}
else
 Ed_TargetDir->Font->Color = clRed;
//
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
  DoEmptyFolder("D:\\MINIX\\FWWORK");
}
//---------------------------------------------------------------------------

