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
	unsigned char    **m_TempBuff2_p;         						//�����ʶ���
	unsigned char    **m_cPixelId_p;                    	//���ÿ�����ص���  
	unsigned char    **m_ResultBuff_p;                  	//�ƽ����� 2 ==���ơ�0==������
	int     subpxx;
	char FPATempFileName[256];
	char FPTTempFileName[256];
	char FPBTempFileName[256];
//********************************20170914***********************************************************
	//���ӿɼ����Զ����
  unsigned short int    **wIR1BjAver_p, **wIR5BjAver_p;   // Զ����Ϳɼ��ⱳ��ƽ��

  //����ͨ��6��1.6����ͨ��7��2.3����NDVIƽ��ֵ����
  unsigned short int    **wIR6BjAver_p, **wIR7BjAver_p, **NDVIAver_p;   // ͨ��6��1.6����ͨ��7��2.3����NDVIƽ��ֵ����*/

  //���Ӿ��淴������
  unsigned short int    **SolarRef_p;   //���淴�������

  //��������ƽ��ֵ
  unsigned short int    **CloudAver_p;   // Զ����ƽ��ֵ

  //���Ӳ��㱳����Ԫ��������ļ�
  unsigned short int    **Noefft_p;   // ������Ԫ����ƽ��ֵ
  unsigned short int    **wIR4BjAver_p, **wIR45BjAver_p;   // ����ƽ��
  //**********************************20170914****************************************************************************
	unsigned short  **m_wSatDataBuff_p;  							//�±�1-5�ֱ���5��ͨ������,����ΪԶ����1��Զ����2�������⣬�к��⣬�ɼ���,����ֵ�Ŵ�10��������
	float 					**m_fLat_p;                       //γ������
	float 					**m_fLong_p;                      //��������
	unsigned short  **pSFTMask_p;
	unsigned char 	**m_ucCloud_p;										//�Ƽ������
	float    			  **m_fSunZenithAnale; 					 	// ����춥������
	float						**m_fSunGlitAngle;       			 	// ���ҫ�߽�����
	float 					**m_fPixelSatelliteZenithAnale;	// �����춥������		
	char      			**m_cPixelSurfaceType;         	// �ر�����  
//--------------------------------------------------------------------------------------------------------------
	float **fIR4Bj_p, **fIR45Bj_p , **fIR5Bj_p;     // ������׼��  20170914 //�ɼ��ⱳ����׼ƫ��20180102
  float **fIR1Bj_p;     // ������׼��  20170914	
	float **fRefl_p;                   // ��������     20170914
	float **fRefl_p1;                   // ��������     20170914
	float **fRefl_p2;                   // ��������     20170914
	
	int     iDatWidth;										//���ݿ�
	int			iDatHeight;           				//���ݸ�

	FY4A_AGRI_2K_FILE_INFO *L1FileInfo_p;
	char strL0Q_p[256];
	char strPOSQ_p[256];
	char strCALQ_p[256];
	char strAreaRange[5];
	
	int istartline;
	int iendline;
	int istartpixel;
	int iendpixel;
	int   iFires;           // �������
	int   iAreas;           // ��������
	int   iFqNum;           //����������
	int   iFqNo;            //���������
	int   iFireNo;          //�����
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
	bool FireProMain();  //�����ʶ������

	void DateTimeParser(char* DateTime);
	
	//void FireProcess(int iThreadNo,int iThreadSum);

private:
	struct FirePixInfo_ *fpi_;					//��¼���������Ϣ
	struct FireFQInfo_  *ffi_;					//��¼��������Ϣ
	struct FileInfo   fi;
	int readHdf5Data_float(char *pfileName, char *pSDSName, float *Data);
	int readHdf5Data_UChar(char *pfileName, char *pSDSName, unsigned char *Data);
	int readHdf5Data_UShort(char *pfileName, char *pSDSName, unsigned short *Data);
	int ReadIJToLatLonHDF(const char* filename ,float* lat,float* lon);	
	void ExchangeDWord(unsigned char* pData);
//-------------- �����Ϣ�����Ӻ��� -------------------------------------------------
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
	void FireProMemInit(void);        // �����ڴ�
	void FireProMemDelete(void);      // �ͷ��ڴ�
	void FireProVarInit(void);        // ������ʼ��
	
	void FireProSetRefl();                 // �����������
	void FireProSetPixelId();  // ����
	void FireProBJ1();  //���㱳���¶�
	
	
	//AverageInfo FireProBJ1_Process(int i,int j);
	//float* FireProBJ2_Process(unsigned short wIR4BjAver_pTemp,unsigned short wIR45BjAver_pTemp,int i,int j );

	void FireProBJ2();
	void FirePro();  					//����ʶ
	void FireProSavePixelIdRaw(void);  																												//������ͼ�����ļ�
	int FireProSavePixelIdHDF(void);
	int FireProSavePixelIdNC_FY4A(void);
	int FireProSavePixelIdNC_AHI8(void);
	void FireProSavePixelIdBmp();  																												//������ͼ���ļ�
	float CalPlankFromRadToBT(float radian,float wavelength);
	//----------------------------------------------------------------------------
	//-------------- �����Ϣ�����Ӻ���-------------------------------------------------
	void FireInfoPro();   				 //�����Ϣ�������
	void fire_fq_(int iy, int ix);     //����������
	void BJtemp();                 //���㱳���¶�
	void BJtemp2();                //���㱳���¶�
	void SaveFireInfo_(void);          //��������Ϣ
	float PixS_(int PixX,int PixY);    //��Ԫ�������
	float SubPixel_(float ch3,float ch4,float ch3b,float ch4b, float *Ft,  unsigned char *TL); //����Ԫ�������
	void SubPixel_1(float ch3,float ch4,float ch3b,float ch4b, double* pp, double* pt);        //����Ԫ�������
	
	void FireFQP_(int FQNo);    																															//��ÿһ��������  ��ffi�ṹ������ֵ
	void OutputInfo1_();    																															//��������Ϣ��
	void OutPutInfo_();  																																	//��������Ϣ���ļ�
	void OutputFirePixInfo_();  																													//��������Ϣ��
	unsigned char LandVI_(FILE *fpLandDat_, float m_fLat_p, float fLong);  												//������������ֵ
	string SrName_(FILE *fpXianJieDat_, FILE *fpXianJieTxt_, float fLat0, float fLong0);   		//��������������
	string LandCoverName1_(float fLat0, float fLong0);   																			//����������������
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
