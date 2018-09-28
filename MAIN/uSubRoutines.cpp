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




/*
browse directory section
*/
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
DELETE ALL CONTENT in the sFolderToEmpty without any warnings

NB. The root folder will remain according to the logic of the algorithm
    everything else (inside root sFolder) will be deleted!
*/
bool _fastcall DoEmptyFolder(AnsiString sFolder)
{
        TSearchRec sr;
        AnsiString s;
        bool bOK = true;

if(FindFirst(sFolder+"\\*.*", 0xFFFFFFFF, sr) == 0)
 {
  do
   {
    if (sr.Name != "." && (sr.Name) != ".." )
     {
      s = sFolder+"\\"+sr.Name;
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
}//bool _fastcall DoEmptyFolder(AnsiString sFolder)


/*
Clean folder contents with warning before (if exists and if bWarnBefore==true)

NB. The root folder will remain according to the logic of the algorithm
    everything else (inside root sFolder) will be deleted!
*/
bool _fastcall DoEmptyFolder(AnsiString sFolder, bool bWarnBefore)
{
        DWORD dwState;

 if(IsDirEmpty(sFolder) == ERROR_DIR_NOT_EMPTY)
  if(!bWarnBefore || YesNoQuestion("Delete ALL the contents of the folder:\n\""+sFolder+"\"\nSeriosly?\nAre You REALLY sure?"))
   {
    if(!DoEmptyFolder(sFolder) && IsDirEmpty(sFolder) == ERROR_DIR_NOT_EMPTY)
     {
      if(YesNoQuestion("After cleaning, the folder still is not empty.\nDo You want to clean it manually?"))
         ExtExplore(sFolder);
     }
   }

 dwState = IsDirEmpty(sFolder);
 switch (dwState)
 {
  case ERROR_NO_MORE_FILES: //empty, (bool)dwState == true
  break;

  case ERROR_DIR_NOT_EMPTY: //NOT empty, (bool)dwState == false
   dwState = 0;
  break;

  default: //ERROR_PATH_NOT_FOUND - not eixsts
   {
    dwState = ForceDirectories(sFolder);//dependent
    if(!dwState)
     CritErrMsg("Can't create the folder \""+sFolder+"\"");
   }
 }

 return (bool)dwState;
}//bool _fastcall DoEmptyFolder(AnsiString sFolder, bool bWarnBefore)




/* Check the folder state

return values = mean:
 ERROR_NO_MORE_FILES  = FOLDER IS EMPTY
 ERROR_DIR_NOT_EMPTY  = FOLDER IS NOT EMPTY
 ERROR_PATH_NOT_FOUND = FOLDER IS'NT EXISTS
*/
DWORD _fastcall IsDirEmpty(AnsiString sLookFolder)
{
        TSearchRec sr;
        DWORD dwRet = ERROR_NO_MORE_FILES; //ERROR_SUCCESS

 if(FindFirst(sLookFolder + "\\*.*", 0xFFFFFFFF, sr) == 0)
 {
  do
   {
    if (sr.Name[1] != '.')
     {
       dwRet = ERROR_DIR_NOT_EMPTY;
       break;
     }
   }
  while (FindNext(sr) == 0);
  FindClose(sr);
 }
 else
  dwRet = ERROR_PATH_NOT_FOUND;

return dwRet;
}//bool _fastcall DoEmptyFolder(AnsiString sFolderToEmpty)

//------------------------------------------------------------------------------
// Message dialogs section
//------------------------------------------------------------------------------
bool _fastcall YesNoQuestion(AnsiString sMsg)
{
          static char *pMsgCapt = "Question";
 if (IDYES == MessageBox(NULL, sMsg.c_str(), pMsgCapt, MB_YESNO | MB_ICONQUESTION | MB_SYSTEMMODAL | MB_DEFBUTTON2 ))
  return true;
return false;
}
//------------------------------------------------------------------------------
void _fastcall AttentionMsg(AnsiString sMsg)
{
        static char *pMsgCapt = "Attention!";
 MessageBox(NULL, sMsg.c_str(), pMsgCapt, MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL);
}

void _fastcall ErrorMsg(AnsiString sMsg)
{
        static char *pMsgCapt = "Error!";
 MessageBox(NULL, sMsg.c_str(), pMsgCapt,  MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
}
/*
Also produce interpretation of GetLastError meaning
and HEX representation of this value
*/
void _fastcall CritErrMsg(AnsiString sMsg)
{
        AnsiString sTx;
        static char *pMsgCapt = "CRITICAL error!!!";

        DWORD      dwErr;
        char     * pBuf = NULL;
        int        BufLen;

dwErr  = GetLastError();
BufLen = FormatMessage(
                         FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,	// source and processing options
                         NULL ,	// pointer to  message source
                         dwErr,	// requested message identifier
                         0,	// language identifier for requested message
                         (LPSTR)&pBuf,	// pointer to message buffer
                         0,	// maximum size of message buffer
                         NULL 	// address of array of message inserts
                        );

 if(pBuf && BufLen)
  {
   sTx = "Error 0x"+IntToHex((int)dwErr, 8)+": "+AnsiString(pBuf);
   GlobalFree(pBuf);
  }
 else
   sTx = " Unknown error: 0x"+IntToHex((int)dwErr, 8);

 if(sMsg.Length())
  sTx = sMsg + "\n" + sTx;

 MessageBox(NULL, sTx.c_str(), pMsgCapt, MB_OK| MB_ICONSTOP | MB_SYSTEMMODAL);
}


/*
Open external program for sFolder
*/
bool _fastcall ExtExplore(AnsiString sFolder)
{
 if((HINSTANCE)33 > ShellExecute(NULL,"explore", sFolder.c_str(), NULL, NULL, SW_SHOWMAXIMIZED))
  {
   ErrorMsg("Can't explore \""+sFolder+"\"");
   return false;
  }
 return true;
}

