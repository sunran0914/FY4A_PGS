/*
 * MainFire.h <===>  bmp.h
 *
 *  Created on: Oct 14, 2015
 *      Author: yanhua
 */

#ifndef MAINFIRE_H_
#define MAINFIRE_H_

#include <string>
using namespace std;

typedef struct tagBITMAPFIlEHEADER1
{
  char bftype[2];
  int bfsiZe;
  unsigned short bfReservedl;
  short bgReserved2;
  unsigned int bfoffBits;
}BITMAPFILEHEADER1;

typedef struct tagBITMAPINFOHEADER1
{
  unsigned int biSize;
  unsigned int biWldth;
  unsigned int biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  unsigned int biCOmpression;
  unsigned int biSiZelmage;
  unsigned int biXPelsPerMeter;
  unsigned int biYPelsPerMeter;
  unsigned int biClrUsed;
  unsigned int biClrlmportant;
}BITMAPINFOHEADER1;

typedef struct tagRGBQUAD1
{ /* rgbq */
  unsigned char rgbBlue;
  unsigned char rgbGreen;
  unsigned char rgbRed;
//  unsigned char rgbReserved;
} RGBQUAD1;

/*fire information struction*/

struct FileInfo
{
  string wYear;
  string wMonth;
  string wDay;
  string wHour;
  string wMinute;
  unsigned short int   wSatelliteID;
  char   cFileName[200];
  unsigned short int   wWidth;
  unsigned short int   wHeight;
  unsigned char   wChNums;
  float  fAverTmepBg[3];
  unsigned short int   wFires;
  unsigned short int   wAreas;
};

//---------   �����Ϣ�ڴ��������   ----------------------------------
struct FirePixInfo_ {
  unsigned short int   wNo;              // ���
  unsigned short int   wAreaNo;          // ����
  unsigned char   bId;              // ������ͣ�0Ϊ�µ㣬1Ϊ������Ե�㣬2Ϊ�����е�
  unsigned short int   wY;               // �к�
  unsigned short int   wX;               // �к�
  bool   bFireId1;         // ��� ���Ŷ�
  unsigned char   TreL;             // ���Ŷȵȼ�
  float  fLat;             // γ��
  float  fLon;             // ����
  float  fFireTemp;        // ����¶�
  float  fMixTemp[3];      // ����¶�
  float  fBackTemp[3];     // �����¶�
  int    iBjNo;            // ����������
  unsigned short int   wLandId;          // ������������
  unsigned short int   wInt2;
  float  fFireS;           // ��Ԫ���
  float  fFireP;           // ռ��Ԫ����ٷֱ�
  unsigned char   iQKId;            // ��ձ��
  unsigned char   iBHId;            // ���ͱ��
  float  fNl;              // ��������
  unsigned char   intst;            // ���ǿ��
};


//---------   ��������Ϣ���ڴ��������   ----------------------------------
struct FireFQInfo_ {
  unsigned short int    wFireAreaNo;           // ����
  unsigned short int    wFireAreaFires;        // ����������
  float   fFireAreaS;            // �����������Ԫ���
  float   fMaxLat;               // �������γ��
  float   fMaxLon;               // ������󾭶�
  float   fMixLon;               // ������Сγ��
  float   fMixLat;               // ������С����
  float   fCenterLat;            // ��������γ��
  float   fCenterLon;            // �������ľ���
  float   fNPixelS;              //ÿ�����������������
  float   fFireLinDi;            // �������ռ�ֵذٷֱ�
  float   fFireCaoChang;         // �������ռ�ݳ��ٷֱ�
  float   fFireQiTa;             // �������ռ�����ٷֱ�
  char    cCloudsId;             // �Ƿ����Ʊ�Ե��1==�ǣ�0==��
  int 		iShengMing;						 //ʡ���� 
  string  sCloudsId;             // �Ʊ�Ե
  string  sShengMing;            // ʡ��
  string  sDiMing;               // ����
  string  sQuMing;               // ����
  
};

struct thread_param
{
    void *pFireProcess;
    int iThreadNo;
    int iThreadSum;
    int iWorkID;
};

//struct AverageInfo
//{
//	unsigned short us3BjAv;
//	unsigned short us34BjAv;
//};

//void* thread_func(void *threadparam);

#endif /* MAINFIRE_H_ */
