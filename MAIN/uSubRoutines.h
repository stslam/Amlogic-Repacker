//---------------------------------------------------------------------------

#ifndef uSubRoutinesH
#define uSubRoutinesH
bool _fastcall xGetFileList(AnsiString InitialPath, AnsiString fMsk, TStringList *slFileLst);
void _fastcall xGetDirList (AnsiString InitialPath, TStringList *slDirs);
AnsiString _fastcall BrDr(AnsiString Prompt, bool bCanCreateFolder, HWND hwnd);
bool _fastcall DoEmptyFolder(AnsiString sFolderToDel);
bool _fastcall DoEmptyFolder(AnsiString sFolderToDel, bool bWarnBefore);

// ERROR_NO_MORE_FILES  = FOLDER IS EMPTY
// ERROR_DIR_NOT_EMPTY  = FOLDER IS NOT EMPTY
// ERROR_PATH_NOT_FOUND = FOLDER IS'NT EXISTS
bool _fastcall ExtExplore(AnsiString sFolder);
DWORD _fastcall IsDirEmpty(AnsiString sLookFolder);

void _fastcall CritErrMsg(AnsiString sMsg);
bool _fastcall YesNoQuestion(AnsiString sMsg);
void _fastcall AttentionMsg(AnsiString sMsg);
void _fastcall ErrorMsg(AnsiString sMsg);


#endif
