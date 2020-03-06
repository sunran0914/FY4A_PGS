#ifndef INIOPERATION_H_
#define INIOPERATION_H_
#define MAX_FILE_SIZE 1024*256
#define LEFT_BRACE '['
#define RIGHT_BRACE ']'

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "netcdf.h"
#include "./AGRI/FY4AAGRI4KHdfOperator.h"
#include "./AGRI/FY4AAGRI2KHdfOperator.h"
#include "./GOESR/GOESRNetCDFOperator.h"
#define FILENAMESIZE 256
//#define FIREPAROFINPUT_FY4 "IFL_FY4A_AGRIX_FHS_INPUT.INI"
#define FIREPAROFINPUT_FY4 "/PGSWORK/FY4A/PAR/AGRI/IFL_FY4A_AGRIX_FHS_INPUT.INI"
#define FIREPAR_FY4	"/PGSWORK/FY4A/PAR/AGRI/IFL_FY4A_AGRIX_FHS_PARAM_NEW.INI"
//#define FIREPAR_FY4	"IFL_FY4A_AGRIX_FHS_PARAM_NEW.INI"

#define FIREPAROFINPUT_AHI8 "/PGSWORK/AHI8/PAR/AGRI/IFL_AHI8_AGRIX_FHS_INPUT.INI"
#define FIREPAR_AHI8	"/PGSWORK/AHI8/PAR/AGRI/IFL_AHI8_AGRIX_FHS_PARAM.INI"

#include "FY4_PPS_ERROR_FHS.h"
#include "FHSParamSeted.h"
#include <sybcommodbcNEW.h>
#include <writelogodbcNEW.h>


int cloud_ID;

typedef struct FILESTRUCT
{
	  char filHedarr[13];
		char satIDarr[6];
		char  insIDarr[7];
		char  taskIDarr[18];		
		char  subtaskTYPEarr[5];
		char  subtaskNUMarr[4];
//		char  beginYMDarr[9];
//		char  beginHMSarr[7];
		char  RDarr[2];
		char  diskarr[5];
		char rangearr[6];
		
}FileStructlist;

class INIOperation
{
public:
	INIOperation();
	virtual ~INIOperation();
	
	void analysisSHM(char* param1,char* param2,char* startDate,char* endDate);
	int GetData();
	int WriteOK();
	int JPGFromPython(char* Param);
	

	unsigned short  **m_wChannelsData;             // 存放7个通道值 + 1
	float    			  **m_fSunZenithAnale; 					 // 存放天顶角数据
	float						**m_fSunGlitAngle;       			 // 存放耀斑角数据
	float 					**m_fPixelSatelliteZenithAnale;// 卫星天顶角数据		
	float     			**m_fLat;                      // 纬度值
	float     			**m_fLong;                     // 经度值
	unsigned short  **pSFTMask;                      //外太空掩膜 	
	unsigned char   **m_ucCloud;									 // 云检测数据  4km需要进行插值
	char      			**m_cPixelSurfaceType;         	 // 地表类型   
	
	FY4A_AGRI_2K_FILE_INFO* HDFFileAtt;
	
	char strIJToLonLatFileName[FILENAMESIZE];
	char strSurfaceTypeFileName[FILENAMESIZE];
	char strSatZenFileName[FILENAMESIZE];
	char strnLandingFileName[FILENAMESIZE];
	char strxjrasterFileName[FILENAMESIZE];
	char strxjdh_newFileName[FILENAMESIZE];
	char strLandCoverFileName[FILENAMESIZE];
	char strLandImgFileName[FILENAMESIZE];
	char strSFTFileName[FILENAMESIZE];
	
	char strInputFileName4000[FILENAMESIZE];
	char strInputFileNameGEO[FILENAMESIZE];
	char strInputFileName[FILENAMESIZE];
	char strInputCLMFileName[FILENAMESIZE];
	char strOutputHDFFileName[FILENAMESIZE];
	char strOutputNCFileName[FILENAMESIZE];
	char strOutputFIMGFileName[FILENAMESIZE];
	char strOutputPIMGFileName[FILENAMESIZE];
	char strOutputFDJPGFileName[FILENAMESIZE];
	char strOutputFNJPGFileName[FILENAMESIZE];
	char strOutputTable1FileName[FILENAMESIZE];
	char strOutputTable2FileName[FILENAMESIZE];
	char strOutputTable3FileName[FILENAMESIZE];
	char strOutputTable1FileNameNC[FILENAMESIZE];
	char strOutputTable2FileNameNC[FILENAMESIZE];
	char strOutputTable3FileNameNC[FILENAMESIZE];
	char strOutputFileNameOK[FILENAMESIZE];
	char strOutputIMGPath[FILENAMESIZE];
	char strTXTConfigPath[FILENAMESIZE];
	char strOutputEnFPAFileName[FILENAMESIZE];
	char strOutputEnFPTFileName[FILENAMESIZE];
	float factor_mid;
	float factor_far;
	float dt3th;
	float dt34th;
	char  diskarr[5];
	char m_strL0Q[256];
	char m_strPOSQ[256];
	char m_strCALQ[256];
	int m_iWidth;						//数据大小
	int m_iHeight;	
	char sataName[32];
	unsigned short DataQ;
	int m_startLine;//起始行列号
	int m_endLine;
	int m_startPixel;
	int m_endPixel;


private:

	char m_strpython[16];
	char m_commandexe[FILENAMESIZE];
	char m_strpgs[16];	
	
	
	

	int m_iwidthOfstatic;		//2km分辨率
	int m_iheightOfstatic;
	int m_widlength;				//4km分辨率
	int m_heilength;
	FY4AAGRI2KHdfOperator m_SensorHdfOperator;		//2km分辨率的HDF数据读写
	FY4AAGRI4KHdfOperator m_SensorHdfOperator4km; //4km分辨率的HDF数据读写
	
	
	char FIREPAROFINPUT[256];
	char FIREPAR[256];
	
	void initData();
	void initDataMemory();
	void freeMemory();
	int GetWidHei(char* filename,char* datasetname,int* wid ,int* hei);
	int GetCLMData();
	int GetLonLatData();
	int GetAngleData();
	int GetSurfaceTypeData();
	int GetQualityFlagData();
	int GetChannelData();
	int GetAttributeOf2kmFile();

	int read_profile_string( const char *section, const char *key,char *value,int size, const char *default_value, const char *file);
	int read_profile_int( const char *section, const char *key,int default_value,const char *file);
	float read_profile_float( const char *section, const char *key,float default_value,const char *file);
	int load_ini_file(const char *file, char *buf,int *file_size);
	int newline(char c);
	int end_of_string(char c);
	int left_barce(char c);
	int isright_brace(char c );
	int parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,int *key_s,int *key_e, int *value_s, int *value_e);
	void parseFileName(char* param1,char* param2,char* startDate,char* endDate);
	void parseSHM(char* strFilename,char* rangename,FileStructlist *FilAna);
	int writeConfigFile();
	void fileconnect_name(char*filename,char*starttime,char*endtime,FileStructlist* FilAna);
	int GetSDSInfo(hid_t file_id,char* sdsname,float*intercept,float* slope,char* bandname,
	                             char*longname,char*units,unsigned int* valid_range,unsigned int* fillvalue);
};



#endif /*INIOPERATION_H_*/

