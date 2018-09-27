#define NO_WIN32_LEAN_AND_MEAN
#include <vcl.h>
#pragma hdrstop

#include "uSubRoutines.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



//---------------------------------------------------------------------------------
// Get list of files from InitialPath\\ by mask fMsk into TStringList *slFileLst
//---------------------------------------------------------------------------------
bool _fastcall xGetFileList(AnsiString InitialPath, AnsiString fMsk, TStringList *slFileLst)
{
        TSearchRec sr;
        TStringList *slSubDirs;
        int         i;
        AnsiString  s;
        int  cntBefore;

  cntBefore = slFileLst->Count;
  slSubDirs = new TStringList();
  slSubDirs->Add(InitialPath);
  xGetDirList (InitialPath, slSubDirs);

  for (i = 1; i<slSubDirs->Count; i++)
   {
    s = slSubDirs->Strings[i];
    if(FindFirst(s + fMsk, 0, sr) == 0)
      do
       slFileLst->Add(s + sr.Name);
      while (FindNext(sr) == 0);
    FindClose(sr);
   }
  delete slSubDirs;

return(slFileLst->Count > cntBefore);
}
//---------------------------------------------------------------------------
// Get subfolders list if exists
//---------------------------------------------------------------------------
void _fastcall xGetDirList (AnsiString InitialPath, TStringList *slDirs)
{
        TSearchRec sr;
  slDirs->Add(InitialPath); //Add upper dir
  if(FindFirst(InitialPath+"*.*", faDirectory, sr) == 0)
   do
    if (sr.Name!="." && (sr.Name)!=".." && sr.Attr&faDirectory )
     xGetDirList (InitialPath+sr.Name+"\\", slDirs);
   while (FindNext(sr) == 0);
  FindClose(sr);
}





struct sMyBrowseParam
{
        AnsiString   Key;
        AnsiString   Caption;
        HWND         hwStatic;
        LPITEMIDLIST StartPlace;
};

int FAR WINAPI BrDrWndEnumeration( HWND hwnd,	// handle to parent window
                                   LPARAM lParam 	// application-defined value
                                  )
{
        static char WndTx[MAX_PATH];
        sMyBrowseParam * pBP;

  pBP = (sMyBrowseParam *)lParam;
  GetWindowText(hwnd, WndTx, MAX_PATH-1);
  if(pBP->Key == AnsiString(WndTx))
    {
     pBP->hwStatic = hwnd;
     SetWindowText(pBP->hwStatic, "");
     return false;
    }
  return true;
}


int __stdcall BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{

  switch(uMsg)
   {
    case BFFM_INITIALIZED:
         {
                sMyBrowseParam * pBP;
          pBP = (sMyBrowseParam *)lpData;
          SetWindowText(hwnd, pBP->Caption.c_str());
          EnumChildWindows(hwnd, (WNDENUMPROC )BrDrWndEnumeration, (LPARAM)pBP);
          SendMessage(hwnd, BFFM_SETSELECTION, true, (LPARAM)pBP->StartPlace);
         }
    break;
    case BFFM_SELCHANGED:
         {
                TCHAR szText[MAX_PATH] = {0};
                sMyBrowseParam *pBP;
          pBP = (sMyBrowseParam *)lpData;
          if(pBP->hwStatic)
            {
             if(SHGetPathFromIDList((LPITEMIDLIST)lParam, szText))
               {
                SetWindowText(pBP->hwStatic, szText);
               }
              else
               SetWindowText(pBP->hwStatic, "");
            }
	 }
    break;
   }
  return 0;
}

//Folder select
AnsiString _fastcall BrDr(AnsiString Prompt, bool bCanCreateFolder, HWND hwnd )
{
        sMyBrowseParam BrP;
        char           cd[MAX_PATH]={0};
        BROWSEINFO     bi;
        LPMALLOC       pShellMalloc;
        LPITEMIDLIST   pidlDrives;//Откуда начинать
        LPITEMIDLIST   pidlChoosen;//Что выбрал пользователь

  if(SUCCEEDED(SHGetMalloc(&pShellMalloc)))
    {
/*  if(bCanCreateFolder)
   SHGetSpecialFolderLocation(hwnd, CSIDL_DESKTOPDIRECTORY, &pidlDrives);
  else*/
     SHGetSpecialFolderLocation(hwnd, CSIDL_DESKTOP, &pidlDrives);
     do
       {
        BrP.Caption = Prompt;
        BrP.Key = Prompt;
        BrP.hwStatic = 0;
        BrP.StartPlace = pidlDrives;

        bi.hwndOwner      = hwnd;
        bi.pidlRoot       = pidlDrives;

        bi.pszDisplayName = cd;
        bi.lpszTitle      = BrP.Key.c_str();
        bi.ulFlags        = BIF_RETURNONLYFSDIRS /*| BIF_STATUSTEXT*/;
        if(bCanCreateFolder)
         bi.ulFlags |= BIF_NEWDIALOGSTYLE;
        bi.lpfn           = BrowseCallbackProc/*NULL*/;
        bi.lParam         = (LPARAM)&BrP;
        pidlChoosen       = SHBrowseForFolder(&bi);
        if(!pidlChoosen)
          {
           pShellMalloc->Free(pidlDrives);
          //pShellMalloc->Release();
           return "";
          }
       }
     while(!SHGetPathFromIDList(pidlChoosen, cd));
     Prompt = AnsiString(cd);
     if(AnsiLastChar(Prompt)[0]!='\\')
       Prompt=Prompt+"\\";
     pShellMalloc->Free(pidlDrives);
     pShellMalloc->Free(pidlChoosen);
//   pShellMalloc->Release();
    }
  return Prompt;
}


/*
 DELETE ALL CONTENT in the sFolderToEmpty
*/
bool _fastcall DoEmptyFolder(AnsiString sFolderToEmpty)
{
        TSearchRec sr;
        AnsiString s;
        bool bOK = true;

//s = sFolderToEmpty+"\\*.*";

if(FindFirst(sFolderToEmpty+"\\*.*", 0xFFFFFFFF, sr) == 0)
 {
  do
   {
    if (sr.Name != "." && (sr.Name) != ".." )
     {
      s = sFolderToEmpty+"\\"+sr.Name;
      if(sr.Attr&faDirectory)
        {
         if(!DoEmptyFolder(s))
          bOK = false;

         if(!RemoveDir(s))
          bOK = false;
        }
      else
       if(!DeleteFile(s))
        bOK = false;
     }
   }
  while (FindNext(sr) == 0);
  FindClose(sr);
 }
return bOK;
}//bool _fastcall DoEmptyFolder(AnsiString sFolderToEmpty)



bool _fastcall DoEmptyFolder(AnsiString sFolderToDel, bool bWarnBefore)
{

}
