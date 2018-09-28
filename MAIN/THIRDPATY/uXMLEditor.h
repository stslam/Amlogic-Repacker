//---------------------------------------------------------------------------

#ifndef uXMLEditorH
#define uXMLEditorH
//---------------------------------------------------------------------------
#include "tinyxml2.h"

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------

  struct TXML_Attribute
   {
    AnsiString sName;
    AnsiString sValue;
   };


  struct TXML_Node
   {
     TXML_Attribute Data;
      TList     *plsAttr;
   };


class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TTreeView *TreeView1;
private:	// User declarations

 void _fastcall ClearTree(void);

    void __fastcall XML2Tree(TTreeNode *pTreeNode,
                              tinyxml2::XMLElement *pParentNode);

    void __fastcall Tree2XML(TTreeNode *pTreeNode,
                              tinyxml2::XMLElement *pParentNode,
                               tinyxml2::XMLDocument *pXML);

 bool _fastcall SaveXML(AnsiString sFile);
 bool _fastcall LoadXML(AnsiString sFile);

public:		// User declarations

  __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
