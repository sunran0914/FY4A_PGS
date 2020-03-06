/*
 *  ProcessFire.h <====> fireInfoHead.h
 *
 *  Created on: Oct 14, 2015
 *      Author: yanhua
 */
#ifndef PROCESSFIRE_H_
#define PROCESSFIRE_H_
#include <math.h>
#include "MainFire.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "hdf5.h"
#include "hdf5_hl.h"
#include "jpeglib.h"
#include <string>
#include <string.h>
#include <pthread.h>
#include <vector>
#include "netcdf.h"
#include "./GOESR/GOESRNetCDFOperator.h"
using namespace std;
#define MAX_THREAD_NUMBER 256
#define WORKID_GEOLOCATION 1
#include "FY4_PPS_ERROR_FHS.h"
#include "INIOperation.h"
//#include "FHSParamSeted.h"
#include <sybcommodbcNEW.h>
#include <writelogodbcNEW.h>
class ProcessFire
{
	
public:
	ProcessFire();
	virtual ~ProcessFire();
	
public:
	unsigned short DataQ;
	int iTotalThreadNumber;
	unsigned char    **m_TempBuff2_p;         						//火点判识结果
	unsigned char    **m_cPixelId_p;                    	//存放每个像素点标记  
	unsigned char    **m_ResultBuff_p;                  	//云结果标记 2 ==》云、0==》不是
	int     subpxx;
	char FPATempFileName[256];
	char FPTTempFileName[256];
	char FPBTempFileName[256];
//********************************20170914***********************************************************
	//增加可见光和远红外
  unsigned short int    **wIR1BjAver_p, **wIR5BjAver_p;   // 远红外和可见光背景平均

  //增加通道6（1.6）、通道7（2.3）和NDVI平均值数组
  unsigned short int    **wIR6BjAver_p, **wIR7BjAver_p, **NDVIAver_p;   // 通道6（1.6）、通道7（2.3）和NDVI平均值数组*/

  //增加镜面反射数组
  unsigned short int    **SolarRef_p;   //镜面反射点数组

  //增加云区平均值
  unsigned short int    **CloudAver_p;   // 远云区平均值

  //增加不足背景像元个数标记文件
  unsigned short int    **Noefft_p;   // 不足像元个数平均值
  unsigned short int    **wIR4BjAver_p, **wIR45BjAver_p;   // 背景平均
  //**********************************20170914****************************************************************************
	unsigned short  **m_wSatDataBuff_p;  							//下标1-5分别存放5个通道数据,依次为远红外1，远红外2，近红外，中红外，可见光,亮温值放大10倍的整型
	float 					**m_fLat_p;                       //纬度数据
	float 					**m_fLong_p;                      //经度数据
	unsigned short  **pSFTMask_p;
	unsigned char 	**m_ucCloud_p;										//云检测数据
	float    			  **m_fSunZenithAnale; 					 	// 存放天顶角数据
	float						**m_fSunGlitAngle;       			 	// 存放耀斑角数据
	float 					**m_fPixelSatelliteZenithAnale;	// 卫星天顶角数据		
	char      			**m_cPixelSurfaceType;         	// 地表类型  
//--------------------------------------------------------------------------------------------------------------
	float **fIR4Bj_p, **fIR45Bj_p , **fIR5Bj_p;     // 背景标准差  20170914 //可见光背景标准偏差20180102
  float **fIR1Bj_p;     // 背景标准差  20170914	
	float **fRefl_p;                   // 辐射能量     20170914
	float **fRefl_p1;                   // 辐射能量     20170914
	float **fRefl_p2;                   // 辐射能量     20170914
	
	int     iDatWidth;										//数据宽
	int			iDatHeight;           				//数据高

	FY4A_AGRI_2K_FILE_INFO *L1FileInfo_p;
	char strL0Q_p[256];
	char strPOSQ_p[256];
	char strCALQ_p[256];
	char strAreaRange[5];
	
	int istartline;
	int iendline;
	int istartpixel;
	int iendpixel;
	int   iFires;           // 火点总数
	int   iAreas;           // 分区总数
	int   iFqNum;           //火点分区个数
	int   iFqNo;            //火点分区编号
	int   iFireNo;          //火点编号
  int  iregion;
  int  iHour;
//------------------------------------------20170914------------------------------------------------------------------
	//double cos (double x );
  float **NDVI;     //20170914
  int **SolarRef;  //   *****************
	//----------------------------------------------------------------------------------	
	float m_factr_mid;
	float m_factr_far;
	float m_dt3th;
	float m_dt34th; 
	
	char sataName[32];
	
	//-----Config File -----------//
	char strnLanding[256];
	char strxjraster[256];
	char strxjdh_new[256];
	char strLandCover[256];
	char strLandImg[256];	
	char strSFTFile[256];
	char regional[32];
	//-----OUT FILE -----------//
	char strOutputFileName_NC[256];
	char strOutputFileName_HDF[256];
	char strOutputFileName_FIMG[256];
	char strOutputFileName_PIMG[256];
	char strOutputFileName_FDJPG[256];
	char strOutputFileName_FNJPG[256];
	char strOutputFileName_T1[256];
	char strOutputFileName_T2[256];
	char strOutputFileName_T3[256];
	char strOutputFileName_T1NC[256];
	char strOutputFileName_T2NC[256];
	char strOutputFileName_T3NC[256];
	char strOutputEnFPAFile[256];
	char strOutputEnFPTFile[256];	
	
	//----------------------------------------------------------------------------
	bool FireProMain();  //火点判识主函数

	void DateTimeParser(char* DateTime);
	
	//void FireProcess(int iThreadNo,int iThreadSum);

private:
	struct FirePixInfo_ *fpi_;					//记录火点总体信息
	struct FireFQInfo_  *ffi_;					//记录火点分区信息
	struct FileInfo   fi;
	int readHdf5Data_float(char *pfileName, char *pSDSName, float *Data);
	int readHdf5Data_UChar(char *pfileName, char *pSDSName, unsigned char *Data);
	int readHdf5Data_UShort(char *pfileName, char *pSDSName, unsigned short *Data);
	int ReadIJToLatLonHDF(const char* filename ,float* lat,float* lon);	
	void ExchangeDWord(unsigned char* pData);
//-------------- 火点信息处理子函数 -------------------------------------------------
private:	
	int OutputFirePixInfoOfNC2_FY4A(float *Lat,float *Lon,int *Number,char *Confidence,float *Area,float *FRP,char *PowerLevel,char *LandType,int *ProvinceInfo);
	int OutputFirePixInfoOfNC_AHI8(float *Lat,float *Lon,int *Number,char *Confidence,float *Area,float *FRP,char *PowerLevel,char *LandType,int *ProvinceInfo);
	int SrNameOfNum(FILE *fpXianJieDat_, float fLat0, float fLong0);
	char LandCoverNameOfNum(float fLat0, float fLong0);
	int OutputInfoNC1_FY4A(float *Lat,float *Lon,int *Number,int *ZoneNumber,int *ZoneNumberOfProv,char *LandType,float *Area,
										float *PercentOfArea,float *FireTemp,	double *DateTime,char *ClearSkyFlag,float *FirePower);

	int OutputInfoNC3_FY4A(float *Lat,float *Lon,int *Number,int *NumberOfPixels,float *Area,char *Grassland,char *woodland,char *Others,char *CloudEdge,int *ProvinceInfoTemp);														
	int OutPutInfoNC_AHI8(float *Lat,float *Lon,int *Number,int *ZoneNumber,int *ZoneNumberOfProv,char *LandType,float *Area,
										float *PercentOfArea,float *FireTemp,	double *DateTime,char *ClearSkyFlag,float *FirePower);

	int OutputInfoNC1_AHI8(float *Lat,float *Lon,int *Number,int *NumberOfPixels,float *Area,char *Grassland,char *woodland,char *Others,char *CloudEdge,int *ProvinceInfoTemp);														
	
	int FireProSaveFRPOfPixe(void);
	int FireProSaveFRTOfPixe();
	int FireProSaveFHSOfDAYPixe();
	int FireProSaveFHSOfNIGPixe();
	void FireProMemInit(void);        // 分配内存
	void FireProMemDelete(void);      // 释放内存
	void FireProVarInit(void);        // 变量初始化
	
	void FireProSetRefl();                 // 计算辐射能量
	void FireProSetPixelId();  // 设标记
	void FireProBJ1();  //计算背景温度
	
	
	//AverageInfo FireProBJ1_Process(int i,int j);
	//float* FireProBJ2_Process(unsigned short wIR4BjAver_pTemp,unsigned short wIR45BjAver_pTemp,int i,int j );

	void FireProBJ2();
	void FirePro();  					//火判识
	void FireProSavePixelIdRaw(void);  																												//保存标记图数据文件
	int FireProSavePixelIdHDF(void);
	int FireProSavePixelIdNC_FY4A(void);
	int FireProSavePixelIdNC_AHI8(void);
	void FireProSavePixelIdBmp();  																												//保存标记图像文件
	float CalPlankFromRadToBT(float radian,float wavelength);
	//----------------------------------------------------------------------------
	//-------------- 火点信息处理子函数-------------------------------------------------
	void FireInfoPro();   				 //火点信息处理入口
	void fire_fq_(int iy, int ix);     //火点分区函数
	void BJtemp();                 //计算背景温度
	void BJtemp2();                //计算背景温度
	void SaveFireInfo_(void);          //保存火点信息
	float PixS_(int PixX,int PixY);    //象元面积计算
	float SubPixel_(float ch3,float ch4,float ch3b,float ch4b, float *Ft,  unsigned char *TL); //亚象元面积计算
	void SubPixel_1(float ch3,float ch4,float ch3b,float ch4b, double* pp, double* pt);        //亚象元面积计算
	
	void FireFQP_(int FQNo);    																															//对每一分区处理  给ffi结构变量附值
	void OutputInfo1_();    																															//输出火点信息表
	void OutPutInfo_();  																																	//输出火点信息到文件
	void OutputFirePixInfo_();  																													//输出火点信息表
	unsigned char LandVI_(FILE *fpLandDat_, float m_fLat_p, float fLong);  												//查找土地类型值
	string SrName_(FILE *fpXianJieDat_, FILE *fpXianJieTxt_, float fLat0, float fLong0);   		//查找行政区划名
	string LandCoverName1_(float fLat0, float fLong0);   																			//查找土地利用名称
	bool getCFG(char *cfgfname,int *iFactr_Mid, int *Factr_Far, int *iDt3Th, int *iDt34Th);
	
	double B1(double k,double n);
	double B2(double k,double t);
	double B3(double k,double t,double p);
	double J1(double p,double t,double k3,double k4,double N3b,double N4b,double N3mix,double N4mix);
	double J2(double p,double t,double k3,double k4,double N3b,double N4b,double N3mix,double N4mix);
	double J0(double p,double t,double k3,double k4,double N3b,double N4b);
	int WriteStringToFHS(char* tempfile, char* SDSName,char* FHSFilename,GOESRNetCDFOperator myNetCDF,char* LongName,char* Description,char* Resoluton,char* StandardName );
};

#endif /* PROCESSFIRE_H_ */
