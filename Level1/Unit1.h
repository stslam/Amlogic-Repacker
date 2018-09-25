//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
typedef unsigned     int  __u32;
typedef unsigned __int64  __u64;
#define IMAGE_MAGIC	                0x27b51956	 /* Image Magic Number		*/

#define AML_FRMWRM_VER_V1       0X01
#define AML_FRMWRM_VER_V2       0X02
#define ITEM_NAME_LEN_V1        0X20
#define ITEM_NAME_LEN_V2        0X100

typedef void*               __hdle;

#pragma pack(push,4)
typedef struct _AmlFirmwareItem_s
{
    __u32           itemId;
    __u32           fileType;           //image file type, sparse and normal
    __u64           curoffsetInItem;    //current offset in the item
    __u64           offsetInImage;      //item offset in the image
    __u64           itemSz;             //item size in the image
    char            itemMainType[ITEM_NAME_LEN_V1];   //item main type and sub type used to index the item
    char            itemSubType[ITEM_NAME_LEN_V1];    //item main type and sub type used to index the item
    char            reserve[32];//don't care fields
}ItemInfo_V1;
#pragma pack(pop)

#pragma pack(push,4)
typedef struct _AmlFirmwareItem2_s
{
    __u32           itemId;
    __u32           fileType;           //image file type, sparse and normal
    __u64           curoffsetInItem;    //current offset in the item
    __u64           offsetInImage;      //item offset in the image
    __u64           itemSz;             //item size in the image
    char            itemMainType[ITEM_NAME_LEN_V2];   //item main type and sub type used to index the item
    char            itemSubType[ITEM_NAME_LEN_V2];    //item main type and sub type used to index the item
    char            reserve[32];//don't care fields
}ItemInfo_V2;
#pragma pack(pop)


#pragma pack(push,4)
typedef struct _AmlFirmwareImg_s
{
        __u32      crc;             //check sum of the image
        __u32      version;         //firmware version
        __u32      magic;           //magic No. to say it is Amlogic firmware image
        __u64      imageSz;         //total size of this image file
        __u32      itemsAlginSize;  //align size for each item
        __u32      itemsCnt;        //items count in the image, each item a file
        char       reserve[36];
}AmlFirmwareImg_t;
#pragma pack(pop)
/*
#define MAX_ITEM_NUM 48
typedef struct _ImgSrcIf{
        unsigned        devIf;          //mmc/usb/store
        unsigned        devNo;          //0/1/2
        unsigned        devAlignSz;     //64K for store
        unsigned        reserv2Align64;
        __u64           itemCurSeekOffsetInImg;//fread will auto seek the @readSz, but for STORE we must do it

        char            partName[28];   //partIndex <= 28 (+4 if partIndex not used)
        unsigned        partIndex;      //partIndex and part
        unsigned char   resrv[512 - 32 - 24];
}ImgSrcIf_t;

typedef struct _ImgInfo_s
{
        ImgSrcIf_t          imgSrcIf;
        AmlFirmwareImg_t    imgHead;//Must begin align 512, or store read wiill exception
        union               ItemInfo_u{
                            ItemInfo_V1 v1[MAX_ITEM_NUM];
                            ItemInfo_V2 v2[MAX_ITEM_NUM];
        }itemInfo;

}ImgInfo_t;
*/


class TForm1 : public TForm
{
__published:	// IDE-managed Components
  TListBox *ListBox1;
  TButton *Button1;
  void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
  void __fastcall LogItmV2(ItemInfo_V2 *pItem);
public:		// User declarations
  __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
