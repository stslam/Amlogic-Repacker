//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uXMLEditor.h"
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
bool _fastcall TForm1::LoadXML(AnsiString sFile)
{
        bool bRet = false;
        tinyxml2::XMLDocument *pXML;
        tinyxml2::XMLElement  *pRoot;
 ClearTree();
 pXML = new tinyxml2::XMLDocument();
 pXML->LoadFile(sFile.c_str());
 pRoot = pXML->RootElement();
 XML2Tree(NULL, pRoot);
 delete pXML;
return bRet;
}
//---------------------------------------------------------------------------
bool _fastcall TForm1::SaveXML(AnsiString sFile)
{
        bool bRet = false;
        tinyxml2::XMLDocument *pXML;
 pXML = new tinyxml2::XMLDocument();
  //    TiXmlDeclaration *pDecl = new TiXmlDeclaration(verison,encoding,standalone);
  //    xmlDocument->LinkEndChild(pDecl);
 Tree2XML(TreeView1->Items->GetFirstNode(), NULL, pXML);
 bRet = pXML->SaveFile(sFile.c_str());
 delete pXML;
return bRet;
}
//---------------------------------------------------------------------------
void _fastcall TForm1::Tree2XML(TTreeNode *pTreeNode,
                                 tinyxml2::XMLElement *pParentNode,
                                  tinyxml2::XMLDocument *pXML)
{
        tinyxml2::XMLText    *pText;
        tinyxml2::XMLElement *pChild;
        TXML_Node            *pNode;
        TList                *plsAttr;
        TXML_Attribute       *pAttr;

        int                   i;

 if(pTreeNode == NULL)
  return;

 while(pTreeNode)
  {
   pNode  = (TXML_Node *)pTreeNode->Data;
   pChild = pXML->NewElement(pNode->Data.sName.c_str());

    if(pParentNode == NULL) //first enter
     {
       pXML->LinkEndChild(pChild);
       pParentNode = pChild;
     }
    else
     pParentNode->LinkEndChild(pChild);

   plsAttr = pNode->plsAttr;

   if(plsAttr)
    {
      for(i = 0; 0 < plsAttr->Count; i++)
       {
        pAttr = (TXML_Attribute *)plsAttr->Items[i];
        pChild->SetAttribute(pAttr->sName.c_str(), pAttr->sValue.c_str());
       }
    }

   if(pNode->Data.sValue.Length())
    {
      pText = pXML->NewText(pNode->Data.sValue.c_str());
      pChild->LinkEndChild(pText);
    }

   Tree2XML(pTreeNode->getFirstChild(), pChild, pXML);
  }
}
//---------------------------------------------------------------------------
void _fastcall TForm1::XML2Tree(TTreeNode *pTreeNode,
                                tinyxml2::XMLElement *pParentNode)
{

}
//---------------------------------------------------------------------------

void _fastcall TForm1::ClearTree(void)
{
        int i;
        int j;
        TTreeView *ptv;
        TTreeNode *ptn;
        TXML_Node      *pNode;
        TXML_Attribute *pAttr;
        TList          *pls;

 ptv = TreeView1;
 ptv->Items->BeginUpdate();
 for(i = 0; i < ptv->Items->Count; i++)
 {
  ptn = ptv->Items->Item[i];

  pNode = (TXML_Node *)ptn->Data;
  if(pNode == NULL)
   continue;
  pls = pNode->plsAttr;
  if(pls)
   {
     for(j = 0; j < pls->Count; j++)
      {
       pAttr = (TXML_Attribute *)pls->Items[j];
       if(pAttr)
        delete pAttr;
      }
    }
   delete pNode;
 }
 ptv->Items->Clear();
 ptv->Items->EndUpdate();
}
