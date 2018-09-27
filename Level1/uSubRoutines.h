//---------------------------------------------------------------------------

#ifndef uSubRoutinesH
#define uSubRoutinesH
bool _fastcall xGetFileList(AnsiString InitialPath, AnsiString fMsk, TStringList *slFileLst);
void _fastcall xGetDirList (AnsiString InitialPath, TStringList *slDirs);
AnsiString _fastcall BrDr(AnsiString Prompt, bool bCanCreateFolder, HWND hwnd);
bool _fastcall DoEmptyFolder(AnsiString sFolderToDel);
bool _fastcall DoEmptyFolder(AnsiString sFolderToDel, bool bWarnBefore);
#endif
