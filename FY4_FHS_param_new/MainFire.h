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

//---------   火点信息内存变量声明   ----------------------------------
struct FirePixInfo_ {
  unsigned short int   wNo;              // 序号
  unsigned short int   wAreaNo;          // 区号
  unsigned char   bId;              // 火点类型：0为孤点，1为火区边缘点，2为火区中点
  unsigned short int   wY;               // 行号
  unsigned short int   wX;               // 列号
  bool   bFireId1;         // 火点 可信度
  unsigned char   TreL;             // 可信度等级
  float  fLat;             // 纬度
  float  fLon;             // 经度
  float  fFireTemp;        // 火点温度
  float  fMixTemp[3];      // 混合温度
  float  fBackTemp[3];     // 背景温度
  int    iBjNo;            // 行政分区号
  unsigned short int   wLandId;          // 土地利用类型
  unsigned short int   wInt2;
  float  fFireS;           // 像元面积
  float  fFireP;           // 占像元面积百分比
  unsigned char   iQKId;            // 晴空标记
  unsigned char   iBHId;            // 饱和标记
  float  fNl;              // 能量估算
  unsigned char   intst;            // 火点强度
};


//---------   火点分区信息表内存变量声明   ----------------------------------
struct FireFQInfo_ {
  unsigned short int    wFireAreaNo;           // 区号
  unsigned short int    wFireAreaFires;        // 火区火点个数
  float   fFireAreaS;            // 火区火点亚象元面积
  float   fMaxLat;               // 火区最大纬度
  float   fMaxLon;               // 火区最大经度
  float   fMixLon;               // 火区最小纬度
  float   fMixLat;               // 火区最小经度
  float   fCenterLat;            // 火区中心纬度
  float   fCenterLon;            // 火区中心经度
  float   fNPixelS;              //每个火区的总像素面积
  float   fFireLinDi;            // 火区火点占林地百分比
  float   fFireCaoChang;         // 火区火点占草场百分比
  float   fFireQiTa;             // 火区火点占其它百分比
  char    cCloudsId;             // 是否是云边缘，1==是，0==否
  int 		iShengMing;						 //省界编号 
  string  sCloudsId;             // 云边缘
  string  sShengMing;            // 省名
  string  sDiMing;               // 地名
  string  sQuMing;               // 区名
  
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
