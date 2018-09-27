//---------------------------------------------------------------------------

#ifndef uDTBH
#define uDTBH

#define AML_DT_IND_LENGTH_V1		4	/*fixed*/
#define AML_DT_IND_LENGTH_V2		16	/*fixed*/

#define AML_DT_IND_LENGTH			  16
#define AML_DT_ID_VARI_TOTAL		3	//Total 3 strings
#define AML_DT_EACH_ID_INT			(AML_DT_IND_LENGTH / 4)

/*Latest version: v2*/
#define AML_DT_VERSION_OFFSET		  4
#define AML_DT_TOTAL_DTB_OFFSET		8
#define AML_DT_FIRST_DTB_OFFSET		12
//#define AML_DT_DTB_HEADER_SIZE	(8+(AML_DT_IND_LENGTH * AML_DT_ID_VARI_TOTAL))
#define AML_DT_DTB_DT_INFO_OFFSET	0
//#define AML_DT_DTB_OFFSET_OFFSET	(AML_DT_IND_LENGTH * AML_DT_ID_VARI_TOTAL)
//#define AML_DT_DTB_SIZE_OFFSET	16

#define AML_DT_UBOOT_ENV	  "aml_dt"
#define DT_HEADER_MAGIC		  0xedfe0dd0	/*header of dtb file*/
#define AML_DT_HEADER_MAGIC	0x5f4c4d41	/*"AML_", multi dtbs supported*/


#pragma pack(push, 4)
struct TDTB_HdrBase
{
	DWORD Magic;
  _fastcall TDTB_HdrBase(void);
};

struct TDTB_SHdr : public TDTB_HdrBase
{
  DWORD Size;
};

struct TDTB_MHdr : public TDTB_HdrBase
{
//  DWORD dwFirstOfs;
//  DWORD dwHdrSize;

  DWORD  Ver;
	DWORD  Entries_Cnt;
  HANDLE pHdr_Entries;
  _fastcall TDTB_MHdr();
  _fastcall ~TDTB_MHdr();
};
#pragma pack(pop)


#pragma pack(push, 1)
//AML_DT_IND_LENGTH_V1
struct TDTBHEntry_V1
{
	char  soc     [AML_DT_IND_LENGTH_V1];
	char  platform[AML_DT_IND_LENGTH_V1];
	char  variant [AML_DT_IND_LENGTH_V1];
	DWORD offset;
	char  padding[4];
};

struct TDTBHEntry_V2
{
	char  soc     [AML_DT_IND_LENGTH_V2];
	char  platform[AML_DT_IND_LENGTH_V2];
	char  variant [AML_DT_IND_LENGTH_V2];
	DWORD offset;
	char  padding[4];
};
#pragma pack(pop)

/*template<unsigned int ID_SIZE=4>
struct HeaderEntry {
	char  soc[ID_SIZE];
	char  plat[ID_SIZE];
	char  vari[ID_SIZE];
	DWORD offset;
	char  padding[4];
};
#pragma pack(pop)

typedef HeaderEntry<4> HeaderEntryV1;
typedef HeaderEntry<16> HeaderEntryV2;

void trimSpace(char *b, const int len) {
	int len2 = len;
	while (len2 > 0 && isspace(b[len2 - 1])) {
		len2--;
	}
	if (len2 < len && len2 > 0) {
		b[len2] = 0;
		b[len - 1] = 0;
	}
}

DWORD swap_bytes_u32(DWORD b) {
    return ((b & 0xFF000000) >> 24) |
           ((b & 0x00FF0000) >> 8) |
           ((b & 0x0000FF00) << 8) |
           (b << 24);
}
template<unsigned int ID_SIZE>
void dumpData(const DWORD entries, const char &dest, TStream *dtb)
{
	typedef HeaderEntry<ID_SIZE> HeaderType;

	vector<HeaderType> headers;
	for ( uint32_t i = 0; i < entries; ++i ) {
		HeaderType h;
		dtb.read((char*)&h, sizeof(h));
	
		headers.push_back(h);
	}
	for ( uint32_t i = 0; i < headers.size(); ++i ) {
		auto &h = headers[i];
		ostringstream id;
		
		auto u32soc = reinterpret_cast<uint32_t*>(h.soc);
		auto u32plat = reinterpret_cast<uint32_t*>(h.plat);
		auto u32vari = reinterpret_cast<uint32_t*>(h.vari);
		for ( uint32_t j = 0; j < ID_SIZE/sizeof(uint32_t); ++j ) {
			*(u32soc + j) = swap_bytes_u32(*(u32soc + j));
			*(u32plat + j) = swap_bytes_u32(*(u32plat + j));
			*(u32vari + j) = swap_bytes_u32(*(u32vari + j));
		}
		trimSpace(h.soc, ID_SIZE);
		trimSpace(h.plat, ID_SIZE);
		trimSpace(h.vari, ID_SIZE);

		if ( h.soc[ID_SIZE-1] == 0 ) {
			id << h.soc;
		} else {
			id.write(h.soc, sizeof(h.soc));
		}
		id << '-';
		if ( h.plat[ID_SIZE-1] == 0 ) {
			id << h.plat;
		} else {
			id.write(h.plat, sizeof(h.plat));
		}
		id << '-';
		if ( h.vari[ID_SIZE-1] == 0 ) {
			id << h.vari;
		} else {
			id.write(h.vari, sizeof(h.vari));
		}
		cout << "Found header: " << id.str() << '\n';

		dtb.seekg(h.offset);
		DTHeader dtheader;
		dtb.read((char*)&dtheader, sizeof(dtheader));
		if ( dtheader.magic != DT_HEADER_MAGIC ) {
			cout.setf(ios::hex);
			cout << "\tDTB Header mismatch. Found: " <<  dtheader.magic;
			continue;
		}
		dtheader.totalsize = ntohl(dtheader.totalsize);
		cout.setf(ios::dec);
		cout << "\t offset: " << h.offset << " size: " << dtheader.totalsize << '\n';
		dtb.seekg(h.offset);
		vector<char> data(dtheader.totalsize);
		dtb.read(data.data(), data.size());
		ofstream output(dest + id.str() + ".dtb", ios::binary);
		output.write(data.data(), data.size());
	}
}
*/


struct TDTB_FSHelper
{
  bool _fastcall ReadAndExport(TStream *pSrcSrm, HANDLE &pRet, AnsiString sOutDir);
  void _fastcall Free(HANDLE &pHandle);
};

#endif
