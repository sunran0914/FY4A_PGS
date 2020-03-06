/** @addtogroup ProcessFire
*  @{
*/
/** @defgroup FHS ProcessFire主程序操作包首部详细设计
*\li
* 操作包名称: ProcessFire\n                        
*\li             
* 操作包标题: 火点处理操作包\n
*\li
* 操作包作用: 火点处理操作包\n    
*\li        
* 包含的操作:\n                                  
*		FireProMain   			ProcessFire类主要函数\n
*		FireProSetPixelId   ProcessFire类主要函数\n
*		FireInfoPro				  ProcessFire类主要函数\n
*\li
* 引用的操作包:			无	\n                                        
*\li
* 源程序文件名:\n                                     
*     ProcessFire.cpp \n
*
* 版权:    		 国家卫星气象中心星地通公司
*	@author 		 欧阳飒飒
* @algorithm   闫华
* @date    2016.8.18
* @version 1.0.0
*  @{
*/
#include "ProcessFire.h"
ProcessFire::ProcessFire()
{
	m_wSatDataBuff_p = NULL;
	m_fLat_p = NULL;
	m_fLong_p = NULL;
	pSFTMask_p = NULL;
	m_ucCloud_p = NULL;
	m_fSunZenithAnale = NULL;
	m_fSunGlitAngle = NULL;
	m_fPixelSatelliteZenithAnale = NULL;
	m_cPixelSurfaceType = NULL;
 	L1FileInfo_p = NULL;
 	
	memset(&fi,0,sizeof(struct FileInfo));
	//memset(strL0Q_p,0,sizeof(strL0Q_p));
	//memset(strPOSQ_p,0,sizeof(strPOSQ_p));
	//memset(strCALQ_p,0,sizeof(strCALQ_p));
	//memset(strAreaRange,0,sizeof(strAreaRange));
	//memset(strnLanding,0,sizeof(strnLanding));
	//memset(strxjraster,0,sizeof(strxjraster));
	//memset(strxjdh_new,0,sizeof(strxjdh_new));
	//memset(strLandCover,0,sizeof(strLandCover));
	//memset(strLandImg,0,sizeof(strLandImg));
	//memset(strSFTFile,0,sizeof(strSFTFile));
	memset(strOutputFileName_NC,0,sizeof(strOutputFileName_NC));
	memset(strOutputFileName_HDF,0,sizeof(strOutputFileName_HDF));
	memset(strOutputFileName_FIMG,0,sizeof(strOutputFileName_FIMG));
	memset(strOutputFileName_PIMG,0,sizeof(strOutputFileName_PIMG));
	memset(strOutputFileName_FDJPG,0,sizeof(strOutputFileName_FDJPG));
	memset(strOutputFileName_FNJPG,0,sizeof(strOutputFileName_FNJPG));
	memset(strOutputFileName_T1,0,sizeof(strOutputFileName_T1));
	memset(strOutputFileName_T2,0,sizeof(strOutputFileName_T2));
	memset(strOutputFileName_T3,0,sizeof(strOutputFileName_T3));
	memset(strOutputFileName_T1NC,0,sizeof(strOutputFileName_T1NC));
	memset(strOutputFileName_T2NC,0,sizeof(strOutputFileName_T2NC));
	memset(strOutputFileName_T3NC,0,sizeof(strOutputFileName_T3NC));	
		memset(strOutputEnFPAFile,0,sizeof(strOutputEnFPAFile));
	memset(strOutputEnFPTFile,0,sizeof(strOutputEnFPTFile));
	memset(sataName,0,sizeof(sataName));
}

ProcessFire::~ProcessFire()
{
}

void ProcessFire::DateTimeParser(char* DateTime)
{
	///时间解析
	char tempDate[128];
	memset(tempDate,0,sizeof(tempDate));
	strncpy(tempDate,DateTime,4);
	string tempY(tempDate);
	fi.wYear = tempY;
	
	memset(tempDate,0,sizeof(tempDate));
	strncpy(tempDate,DateTime+4,2);
	string tempM(tempDate);
	fi.wMonth = tempM;
	
	memset(tempDate,0,sizeof(tempDate));
	strncpy(tempDate,DateTime+6,2);
	string tempD(tempDate);
	fi.wDay = tempD;	
	
	memset(tempDate,0,sizeof(tempDate));
	strncpy(tempDate,DateTime+8,2);
	string tempH(tempDate);
	fi.wHour = tempH;	
	
	memset(tempDate,0,sizeof(tempDate));
	strncpy(tempDate,DateTime+10,2);
	string tempN(tempDate);
	fi.wMinute = tempN;	

	cout<< fi.wYear <<endl;
	cout<< fi.wMonth <<endl;
	cout<< fi.wDay <<endl;
	cout<< fi.wHour <<endl;
	cout<< fi.wMinute <<endl;
}

/**  @brief FHS进程主函数 \n
*    @remark 输入0.65、0.83、3.79、10.8、12um等通道数据\n
*    @return   0-成功，其他-失败
*/
bool ProcessFire::FireProMain()
{
      
    FireProVarInit();
    cout << "success Init " <<endl; 
   if(strstr(regional,"REGC") != NULL)
   	{
   		iregion = 1;
   	} 
   	else
   		{
   			iregion = 0;
   		} 
    FireProMemInit();    //分配数据数组
    cout << "success Memory " <<endl;
		ZLDSWriteLog('N',"success Fire Init!");   
    FireProSetPixelId();
    cout << "set id end" << endl;
 		ZLDSWriteLog('N',"success Set ID Of Pixel!"); 		
 		  //计算背景像元平均值
     FireProBJ1();
     cout << "process bj1 end" << endl;
 		ZLDSWriteLog('N',"success of bj1 process!");
     //计算背景温度标准差
     FireProBJ2();
     cout << "process bj2 end" << endl;
 		ZLDSWriteLog('N',"success of bj2 process!");

     //火点判识
 	 FirePro();   //20170707   增加太阳天顶角
   cout << "process fire end" << endl;
 		ZLDSWriteLog('N',"success of fire process!");


   ZLDSWriteLog('N',"Start Write Data!");
	 //保存标记文件
	if(FireProSavePixelIdHDF() != 0)
	{
		 ZLDSWriteLog('W',"Write HDF ERROR!");
		 return ZEHDFWRITE;
	}
	///输出火点标记图像
	if(FireProSaveFRTOfPixe() != 0)
	{
		 ZLDSWriteLog('W',"Write fire flag JPEG ERROR!");
		 return ZEWRITEFLAGOFJPEG;		
	}
	
	///输出火点强度图像	
	if(FireProSaveFRPOfPixe() != 0)
	{
		 ZLDSWriteLog('W',"Write fire Power JPEG ERROR!");
		 return ZEWRITEPOWEROFJPEG;			
	}

///输出火点标记合成图像	
if(iHour >= 0 && iHour <= 12)
	{
		if(FireProSaveFHSOfDAYPixe() != 0)
	  {
		   ZLDSWriteLog('W',"Write fire day combine JPEG ERROR!");
		   return ZEWRITEPOWEROFJPEG;			
	  }
	  else
	  	{
	  		printf("strOutputFileName_FDJPG %s\n",strOutputFileName_FDJPG);
	  	}
	}
	else
	{
		  if(FireProSaveFHSOfNIGPixe() != 0)
	     {
		     ZLDSWriteLog('W',"Write fire night combine JPEG ERROR!");
		     return ZEWRITEPOWEROFJPEG;			
	    }
	     else
	  	{
	  		printf("strOutputFileName_FNJPG %s\n",strOutputFileName_FNJPG);
	  	}
	}

 
	ZLDSWriteLog('N',"Start TXTFile Of Fire Point!");
	//火点信息处理
	FireInfoPro();//输出TXT及FPT、FPB文件
 if(strcmp(sataName,"FY4A")==0)
	{
		if(FireProSavePixelIdNC_FY4A()!= 0) // //保存标记文件NC
	  {
		   ZLDSWriteLog('W',"Write NC ERROR!");
		   return ZENCWRITE;
	  }		
	}
	else
	{
		if(FireProSavePixelIdNC_AHI8()!= 0)
	  {
		      ZLDSWriteLog('W',"Write NC ERROR!");
		      return ZENCWRITE;
	  }	
	}
	//释放全局变量内存空间
	FireProMemDelete();

  return 0;
}



//-----------  火点判识主函数各子函数  ----------------------------------------------

/*: ******************* FireProVarInit *********************
* 函数名称： FireProVarInit
* 函数标题： 火点判识信息处理函数用的全局变量初始化
* 函数功能： 初始化全局变量
* 调用方法: FireProVarInit(void)
*   参数说明：
*        无
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::FireProVarInit(void)
{
     m_ResultBuff_p = NULL;
     m_cPixelId_p   = NULL;
     m_TempBuff2_p = NULL;
     //增加方差
     fIR1Bj_p = NULL;
      fIR4Bj_p = NULL;
     fIR45Bj_p = NULL;
     fRefl_p =   NULL;
    fIR5Bj_p  = NULL;
      //增加镜面反射数组***********************************************************20170828
     SolarRef_p = NULL ;   //镜面反射点数组
    //增加云区平均值**********************************************************20170716
     CloudAver_p = NULL;      //20170716
   //增加不足背景像元个数标记文件*****************************************20170823
     Noefft_p =  NULL;   // 不足背景像元个数标记文件
     NDVI=NULL;
    
     fpi_ = NULL;
     ffi_ = NULL;

     iFires = 0;           // 火点总数
     iAreas = 0;
}
//---------------------------------------------

/*: ******************* FireProMemInit*********************
* 函数名称： FireProMemInit
* 函数标题： 火点判识信息处理函数用的全局变量动态分配内存及初始化
* 函数功能： 动态分配内存空间及初始化
* 调用方法: bool FireProMemInit(void)
*   参数说明：
*        无
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::FireProMemInit(void)
{
     m_ResultBuff_p = new unsigned char *[iDatHeight];            //存放云判识结果，数据类型为无符号1字节整型
     m_cPixelId_p  = new unsigned char *[iDatHeight];             //存放每个象元的标识码，数据类型为无符号1字节整型
     m_TempBuff2_p = new unsigned char *[iDatHeight];
     fRefl_p = new float *[iDatHeight];
 //增加可见光和远红外*******************20170914****************************************
    wIR1BjAver_p = new unsigned short int *[iDatHeight];           //存放远红外的11um背景平均值，数据类型为无符号2字节整型  20170706
    wIR5BjAver_p = new unsigned short int *[iDatHeight];           //存放可见光背景平均值，数据类型为无符号2字节整型      20170706 
     //增加通道6（1.6）、通道7（2.3）和NDVI平均值数组******************************
     
    wIR4BjAver_p= new unsigned short int *[iDatHeight];  
    wIR45BjAver_p= new unsigned short int *[iDatHeight];  
    wIR6BjAver_p = new unsigned short int *[iDatHeight];        //存放通道6（1.6）平均值数组，数据类型为无符号2字节整型  20170825      
    wIR7BjAver_p =  new unsigned short int *[iDatHeight];       //存放通道7（2.3）平均值数组，数据类型为无符号2字节整型  20170825  
    NDVIAver_p =  new unsigned short int *[iDatHeight];             //存放NDVI平均值数组，数据类型为无符号2字节整型  20170825 
    fIR4Bj_p = new float *[iDatHeight];                         //存放每个象元的中红外背景均方差值，数据类型为4字节浮点型
    fIR45Bj_p = new float *[iDatHeight];                        //存放每个象元的中红外-远红外1背景值均方差，数据类型为4字节浮点型
    fIR5Bj_p  =  new float *[iDatHeight];
   //增加远红外标准差*******************************************************20171107
	  fIR1Bj_p = new float *[iDatHeight];   //远红外标准差 20171107
    //增加镜面反射数组***********************************************************
     SolarRef_p =  new unsigned short int *[iDatHeight] ;     //镜面反射点数组
    //增加云区平均值***********************************************************
     CloudAver_p = new unsigned short int *[iDatHeight];           //存放云区平均值，数据类型为无符号2字节整型      20170716 
     Noefft_p = new unsigned short int *[iDatHeight];   // 不足背景像元个数标记文件
     NDVI = new float*[iDatHeight];
     for (int i = 0; i < iDatHeight; i++)
     {
          m_ResultBuff_p[i] = new unsigned char [iDatWidth];
          m_cPixelId_p[i] = new unsigned char [iDatWidth];
          m_TempBuff2_p[i] = new unsigned char [iDatWidth];
          fRefl_p[i] = new float [iDatWidth];
           //增加可见光和远红外***********************************************************20170706
         wIR1BjAver_p[i] = new unsigned short int [iDatWidth];         // 12通道 11um
          wIR5BjAver_p[i] = new unsigned short int [iDatWidth];        //2通道 O.65um
          //增加通道6（1.6）、通道7（2.3）和NDVI平均值数组******************************20170825
         wIR6BjAver_p[i] = new unsigned short int [iDatWidth];        //存放通道5（1.6）平均值数组，数据类型为无符号2字节整型  20170825      
         wIR7BjAver_p[i] =  new unsigned short int [iDatWidth];       //存放通道6（2.3）平均值数组，数据类型为无符号2字节整型  20170825  
         NDVIAver_p[i] =  new unsigned short int [iDatWidth];             //存放NDVI平均值数组，数据类型为无符号2字节整型  20170825
          
         wIR4BjAver_p[i] = new unsigned short int [iDatWidth];
         wIR45BjAver_p[i] = new unsigned short int [iDatWidth];
          
          fIR4Bj_p[i] = new float [iDatWidth];
          fIR45Bj_p[i] = new float [iDatWidth];
           fIR1Bj_p[i] = new float [iDatWidth];
           fIR5Bj_p[i] = new float [iDatWidth];
         //增加镜面反射数组***********************************************************20170828
          SolarRef_p[i] =  new unsigned short int [iDatWidth] ;   //镜面反射点数组

         //增加云区平均值***********************************************************20170716
        CloudAver_p[i] = new unsigned short int [iDatWidth];       //20170716
          
         //增加不足背景像元个数标记文件*****************************************20170823
        Noefft_p[i] = new unsigned short int [iDatWidth];   // 不足背景像元个数标记文件
        m_TempBuff2_p[i] = new unsigned char [iDatWidth];
        
        NDVI[i] = new float[iDatWidth];
       
					memset(m_ResultBuff_p[i],0,iDatWidth);
					memset(m_cPixelId_p[i],100,iDatWidth);
					memset(m_TempBuff2_p[i],0,iDatWidth);
     }
     
     for (int i = 0; i < iDatHeight; i++)
         for (int j = 0; j < iDatWidth; j++)
      {
        
         wIR4BjAver_p[i][j] = 0;
         wIR45BjAver_p[i][j] = 0;
        //增加可见光和远红外***********************************************************20170706
         wIR1BjAver_p[i][j] = 0;  //20170627
         wIR5BjAver_p[i][j] = 0;  //20170627
         wIR6BjAver_p[i][j] = 0;  //20170825
         wIR7BjAver_p[i][j] = 0;  //20170825
         NDVIAver_p[i][j] = 0;    //20170825 
         
       //增加远红外标准差*******************************************************20171107
			   fIR1Bj_p[i][j] = 0.;  // 远红外标准差 20171107
        fIR5Bj_p[i][j] = 0;
       //增加镜面反射数组***********************************************************20170828
         SolarRef_p[i][j] =  0;   //镜面反射点数组
         CloudAver_p[i][j] = 0;  //2017804
         NDVI[i][j]=0.;
          
          fRefl_p[i][j] = 0.;
          fIR4Bj_p[i][j] = 0.;
          fIR45Bj_p[i][j] = 0.;
          
        
       
      }  
     
}

//-------------------------------------------------------
/*: ******************* FireProMemDelete *********************
* 函数名称： FireProMemDelete
* 函数标题： 火点判识信息处理函数用的全局变量动态分配内存的释放
* 函数功能： 释放分配的内存空间
* 调用方法: bool FireProMemDelete(void)
*   参数说明：
*        无
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::FireProMemDelete(void)
{

     for (int i = 0; i < iDatHeight; i++)
     {
          delete [] m_ResultBuff_p[i];
          delete [] m_cPixelId_p[i];
          delete [] m_TempBuff2_p[i];
           //增加可见光和远红外***********************************************************20170706
      		delete [] wIR1BjAver_p[i];         //20170706
      		delete [] wIR5BjAver_p[i];         //20170706
      			
      		//增加通道6（1.6）、通道7（2.3）和NDVI平均值数组******************************20170825
      		 delete [] wIR6BjAver_p[i];        // 20170825      
      			delete [] wIR7BjAver_p[i];       //  20170825  
      			delete [] NDVIAver_p[i];             //  20170825 
      			
      			//增加镜面反射数组***********************************************************20170828
      			  delete [] SolarRef_p[i];   //镜面反射点数组
      			
      			// 增加云区平均值***********************************************************20170716
      			 delete [] CloudAver_p[i];       //20170716
      			  
      		  //增加不足背景像元个数标记文件*****************************************20170823
      		 delete [] Noefft_p[i];   // 不足背景像元个数标记文件
      		 delete [] NDVI[i];
      		
      		 delete [] fRefl_p[i];
      		 delete [] fIR4Bj_p[i];
        		delete [] fIR45Bj_p[i];
        		delete [] fIR1Bj_p[i];
        	 delete [] fIR5Bj_p[i];
     }

     delete [] m_ResultBuff_p;
     delete [] m_cPixelId_p;
     delete [] m_TempBuff2_p;
       //增加远红外标准差*******************************************************20171107
	   delete [] fIR1Bj_p;  //远红外标准差 20171107 

     m_ResultBuff_p = NULL;
     m_cPixelId_p   = NULL;
     m_TempBuff2_p = NULL;
     NDVI = NULL;
   
}

/*: ******************* FireProSetRefl *********************
* 函数名称： FireProSetRefl
* 函数标题： 计算中红外和远红外温度值转反射率的差
* 函数功能： 计算中红外和远红外温度值转反射率的差
* 调用方法: FireProSetRefl(unsigned short int *m_wSatDataBuff_p[])
*   参数说明：
*   unsigned short int *m_wSatDataBuff_p[] 卫星数据
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/

void ProcessFire::FireProSetRefl()
{
   // 计算中红外反射率
	double c1,c2,WAV3,WAV4,LTB,LTF;     //WAV,LT,
	c1 = 1.1910427/100000;//c1,c2系数，VC中心波数,ABT中心波数增益、斜率
	c2 = 1.4387752;
	WAV3 = 2640.0;          // V为中心波数，中红外暂为2640    2671.2150;
	WAV4= 921.0;            //              远红外暂为921      924.8922;
	//short TF=750;
    float p,T;     //TB,

        for (int i = 0; i < iDatHeight; i++)
            for (int j = 0; j < iDatWidth; j++){
                T = (float)(m_wSatDataBuff_p[4][i * iDatWidth + j] / 10.); //中红外亮温
                p = (float)(m_wSatDataBuff_p[1][i * iDatWidth + j] / 10.); //远红外亮温
	        //LT = c1 * pow(WAV3,3)/(exp(c2*WAV3/T)-1);
	        LTF = (c1 * pow(WAV3,3)) / (exp(c2 * WAV3 / T) - 1);
	        LTB = (c1 * pow(WAV4,3)) / (exp(c2 * WAV4 / p) - 1);
	        fRefl_p[i][j] = LTF - LTB;  //fRefl_p为全局变量，存放计算结果
            }

	//return;

}
//------------------------------------------------------------------------
/*: *******************  FireProSetPixelId *********************
* 函数名称：  FireProSetPixelId
* 函数标题：  加标记
* 函数功能：  对全圆盘数据进行分类设置类型值
* 调用方法: FireProSetPixelId()
*   参数说明：
*   unsigned short int *m_wSatDataBuff_p[] 卫星数据
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/

void ProcessFire::FireProSetPixelId() 
{
       
	  float rad, solarznh;
	  rad = 3.1415926 / 180. ;
	//   NDVIAver_p[1][1] = 1;


	  for (int i = 0; i < iDatHeight; i++)
	  {
         for (int j = 0; j < iDatWidth; j++)
         {
             char c1,c2,c3,c4;
             if ((i - 1) < 0)  // 上相邻
                c1 = m_cPixelSurfaceType[i][j];
             else
                c1 = m_cPixelSurfaceType[i - 1][j];

             if ((i + 1) > (iDatHeight - 1))  // 上相邻
                c2 = m_cPixelSurfaceType[i][j];
             else
                c2 = m_cPixelSurfaceType[i + 1][j];

             if ((j - 1) < 0)  // 上相邻
                c3 = m_cPixelSurfaceType[i][j];
             else
                c3 = m_cPixelSurfaceType[i][j - 1];

             if ((j + 1) > (iDatWidth - 1))  // 上相邻
                c4 = m_cPixelSurfaceType[i][j];
             else
                c4 = m_cPixelSurfaceType[i][j + 1];

             float fRefl_p1,fRefl_p2;
             if ((j - 3) < 0)  // 上相邻
                fRefl_p1 = fRefl_p[i][j];
             else
                fRefl_p1 = fRefl_p[i][j - 3];

             fRefl_p1 = fRefl_p[i][j] -  fRefl_p1;

             //--------------------------------------------------------------------
             if ((j + 3) > (iDatWidth - 1))  // 上相邻
                fRefl_p2 = fRefl_p[i][j];
             else
                fRefl_p2 = fRefl_p[i][j + 3];

             fRefl_p2 = fRefl_p[i][j] -  fRefl_p2;
            // 辐射能量左右间的中间值暂且没有用到
             //--------------------------------------------------------------------
		
             m_cPixelId_p[i][j] = 100;    // 设出值 为 100

			 
             // 可见光和近红外通道太阳天顶角订正 20170711 上   ***********
       
        if(pSFTMask_p[i][j]>=65534)//by liup 20170606  外太空
        {
             	   m_cPixelId_p[i][j] = 255;
             	  
                 continue;
        }
        if(m_wSatDataBuff_p[3][i * iDatWidth + j] < 60000 )
        	{
        	   	 if  ( m_fSunZenithAnale[i][j] > SZAngleUp )
                    //#### 太阳天顶角订正角度参数  SatZAngLimit （初值70）
				     {
					   solarznh =70*rad; 
					   //continue;
				     }	
				        else 
			       solarznh = m_fSunZenithAnale[i][j]*rad; 
        		m_wSatDataBuff_p[3][i * iDatWidth + j] = m_wSatDataBuff_p[3][i * iDatWidth + j] / cos (solarznh );
        	}
        	
        	if(m_wSatDataBuff_p[5][i * iDatWidth + j] < 60000 )
        	{
        		 if  ( m_fSunZenithAnale[i][j] > SZAngleUp )
                 //#### 太阳天顶角订正角度参数  SatZAngLimit （初值70）
				  {
					solarznh =70*rad; 
					//continue;
				  }	
				     else 
			    solarznh = m_fSunZenithAnale[i][j]*rad; 
        		m_wSatDataBuff_p[5][i * iDatWidth + j] = m_wSatDataBuff_p[5][i * iDatWidth + j] / cos (solarznh );
        	}
        	if(m_wSatDataBuff_p[6][i * iDatWidth + j] < 60000 )
        	{
        		 if  ( m_fSunZenithAnale[i][j] > SZAngleUp )
                 //#### 太阳天顶角订正角度参数  SatZAngLimit （初值70）
				  {
					solarznh =70*rad; 
					//continue;
				  }	
				     else 
			    solarznh = m_fSunZenithAnale[i][j]*rad; 
        		m_wSatDataBuff_p[6][i * iDatWidth + j] = m_wSatDataBuff_p[6][i * iDatWidth + j] / cos (solarznh );
        	}
        	if(m_wSatDataBuff_p[7][i * iDatWidth + j] < 60000 )
        	{
        		 if  ( m_fSunZenithAnale[i][j] > SZAngleUp )
                 //#### 太阳天顶角订正角度参数  SatZAngLimit （初值70）
				  {
					solarznh =70*rad; 
					//continue;
				  }	 
				   else 
			    solarznh = m_fSunZenithAnale[i][j]*rad;
        		m_wSatDataBuff_p[7][i * iDatWidth + j] = m_wSatDataBuff_p[7][i * iDatWidth + j] / cos (solarznh );
        	}
        	

			 // 可见光和近红外通道太阳天顶角订正 20170711 下   ***********
			   if ( ( (m_wSatDataBuff_p[3][i * iDatWidth + j]== 0 ||m_wSatDataBuff_p[5][i * iDatWidth + j]==0)) )  //20180128
			   NDVI[i][j] = 0.;                                                                                   //20180128
			 else             			  
			  NDVI[i][j] = (float)((float)m_wSatDataBuff_p[3][i * iDatWidth + j]-(float)m_wSatDataBuff_p[5][i * iDatWidth + j])/
				  ((float)m_wSatDataBuff_p[3][i * iDatWidth + j]+(float)m_wSatDataBuff_p[5][i * iDatWidth + j]);
      
       if (m_wSatDataBuff_p[4][i * iDatWidth + j] > 60000 || m_wSatDataBuff_p[1][i * iDatWidth + j] > 60000
             	||m_wSatDataBuff_p[2][i * iDatWidth + j] > 60000 || m_wSatDataBuff_p[3][i * iDatWidth + j] > 60000
             	||m_wSatDataBuff_p[5][i * iDatWidth + j] > 60000 ) // 非数据区   (m_fLat_p[i][j] == -32768 )
			 {
                 //#### 非数据区值参数  Nodatavalue  （初值-900）
				 m_cPixelId_p[i][j] = 40 ;   
			 }
      
      else if (m_fPixelSatelliteZenithAnale[i][j] >  SatZAngLimit )  // 卫星天顶角 大于 82，即非图像有效区标记  20160914
			 {
                  //#### 卫星天顶角上限参数 SatZAngLimit （初值80）
			
				 m_cPixelId_p[i][j] = 50;  
				
			 }

			 // 检查是否有晨昏线附近的高反射干扰，即太阳天顶角大于85度，可见光反射率大于0.6，此时中红外的亮温将受太阳辐射干扰  20170712 

			 else if (m_fSunZenithAnale[i][j] >  SZAngleNt  && m_wSatDataBuff_p[5][i * iDatWidth + j] > VisAngleNt   )  // 太阳天顶角大于85度，可见光反射率大于0.6  20170712 
            //#### 晨昏线太阳天顶角角度参数 SZAngleNt， （初值85）  晨昏线可见光通道角度参数 VisAngleNt，（初值0.6） 
				
				{
					m_cPixelId_p[i][j] = 180;    //晨昏线太阳辐射干扰 标记为180   20170712  
					
				} 

          // 水体标记
         else if (m_cPixelSurfaceType[i][j] == 0  ) 
         	{
         		if(i ==1867 && j == 2406)
         			{
         				printf("***********water***********\n");
         			}
         		m_cPixelId_p[i][j] = 153;
         			
         	} 
         //#### 耀斑角度参数   SunGtAngLimit,初值为10度 
         else if (m_fSunGlitAngle[i][j] < SunGtAngLimit)    
         	{
         		m_cPixelId_p[i][j] = 60; 
         		
         	}  
          //#### 耀斑角度参数   SunGtAngLimit  
				 //耀斑区标记
				  //#### 中红外通道低温区阈值参数  T4LowTemp,（初值2000），若通道3（中红外）亮温小于200K，标记为126，即非热点标记；
          else if (m_wSatDataBuff_p[4][i * iDatWidth + j] < T4LowTemp )  
				
          {
                  	m_cPixelId_p[i][j] = 126;
                  
          }
          //#### 远红外通道低温区阈值参数  T11LowTemp, （初值2000）， 若通道4（远红外1）亮温小于200K，标记为127，即非热点标记；
         else if (m_wSatDataBuff_p[1][i * iDatWidth + j] < T11LowTemp)  
          {
                 	m_cPixelId_p[i][j] = 127;
                
          } 
			 //#### 白天云区判识:云检测文件存在，可见光通道参数 VisDCloudBJ,（初值150）云区判识太阳天顶角参数 SZAngDCloudBJ,（初值87） 
			 else if (cloud_ID == 1 && m_ucCloud_p[i][j] == 0 && m_wSatDataBuff_p[5][i * iDatWidth + j] >VisDCloudBJ &&
			 	          m_fSunZenithAnale[i][j]< SZAngDCloudBJ )     // 白天云检测，若云检测产品的值为“0”，可见光大于15%，太阳天顶角小于87，为云。20170804
			 {
				 
				 m_cPixelId_p[i][j] = 220; 
				
			 }
			  //#### 白天云区判识：云检测文件不存在，仅用可见光通道阈值参数：VisDCloudth，初值为350.
			 else if (cloud_ID == 0 && m_wSatDataBuff_p[5][i * iDatWidth + j] >VisDCloudth   &&
			 	 m_fSunZenithAnale[i][j]< SZAngDCloudBJ )     // 白天云检测，若没有云检测产品，可见光大于25%，太阳天顶角小于87，为云。20171014
			 {
				
				 m_cPixelId_p[i][j] = 220;
				
			 }
			 
			   //#### 夜间云区判识，夜间太阳天顶角云判识阈值参数 SZAngNCloudBJ,初值87度。
			   //如没有云检测产品，使用远红外通道判识,#### 远红外亮温云判识阈值参数：T11NCloud，初值为2700,即270K。
			 else if ( cloud_ID == 1 && m_ucCloud_p[i][j] == 0  && m_fSunZenithAnale[i][j]> SZAngNCloudBJ )     // 夜间云检测，若云检测产品的值为“0”太阳天顶角大于86，为云，即夜间不考虑可见光。20170804。
			 {
			   m_cPixelId_p[i][j] = 220;
			 }
			 //#### 夜间云区判识仅用红外通道阈值参数：T11NCloudth，初值,2700.
			 else if ( cloud_ID == 0 && m_fSunZenithAnale[i][j]> SZAngNCloudBJ && m_wSatDataBuff_p[5][i * iDatWidth + j] < T11NCloudth   )     // 夜间云检测，若云检测产品的值为“0”太阳天顶角大于86，为云，即夜间不考虑可见光。20170804。
			 {
			   m_cPixelId_p[i][j] = 220;
			 }  

     // else if (m_cPixelSurfaceType[i][j] == 16 && c1 == 16 && c2 == 16 && c3 == 16 && c4 == 16)  // 荒漠区标记，参考土地覆盖数据，将12改为16，因土地利用数据解释文本中说明为15 20170429；
        else if (m_cPixelSurfaceType[i][j] == 16)  //   标记为150（荒漠区）；
         {
            m_cPixelId_p[i][j] = 150;
           
         }    //  20170429  修改  因土地利用类型错误，将荒漠标记改为植被区，即将标记值150改为100
         

       }
    }
     // 加标记结束


     return;
}

//----------------------------------------------------------------------------

/*: *******************  FireProBJ1 *********************
* 函数名称：  FireProBJ1
* 函数标题：  计算背景温度
* 函数功能：  计算中红外的背景温度及中红外和远红外的差的背景温度
* 调用方法: FireProBJ1(unsigned short int *m_wSatDataBuff_p[])
*   参数说明：
*   unsigned short int *m_wSatDataBuff_p[] 卫星数据
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::FireProBJ1()
{
         //------------- 计算背景温度  -----------------
     int ij, iCount;
	 int i,j,ia;  //20170708
    // ij = 2;
	   ij = 3;   // 20161005********************************
	  i = 0;
	  j = 0;
	  float coe1 = 0.; 
     iCount = 0;
     bool bDoId = true;
    

    
	 
	 //生成输出计算背景的像元个数信息文件  20170710 下  *****

	
     for (int i = 0; i < iDatHeight - 10; i++)
     {     //20180314
		   for (int j = 0; j < iDatWidth; j++)
		   {	      
			   if (m_cPixelId_p[i][j] == 100)
			   { // 对标记值为100的点计算背景平均值
              bDoId = true;
         	    ij = 3;   // 20161005********************************
            
              do
              {    // 不同的背景窗区
                 iCount = 0;
                 int iIr3BjAv, iIr34BjAv,iIr1BjAv, iIr5BjAv,iIr6BjAv, iIr7BjAv;
				         float NDVIAv;
				         int iaround;
                 //iIr3BjAv 中红外背景平均值， iIr34BjAv 中红外和远红外差的背景平均值
                 iIr3BjAv = 0;
                 iIr34BjAv = 0;

				          iIr1BjAv = 0;  // 远红外背景平均值20170706
				          iIr5BjAv = 0;  //可见光背景平均值20170706
                  
				          iIr6BjAv = 0;  // 通道6（1.6微米）背景平均值20170825
				          iIr7BjAv = 0;  // 通道7（2.2微米）背景平均值20170825
				          NDVIAv = 0.;  // NDVI背景平均值20170825
                  

                 for (int ii = i - ij; ii <= i + ij; ii++)
                 {
                    for (int jj = j -ij; jj <= j + ij; jj++)
                    {
                       int i0,j0;
                       i0 = ii;
                       j0 = jj;
                       if (ii < 0)
                           i0 = 0;
                       if (jj < 0)
                           j0 = 0;
                       if (ii > iDatHeight - 1)
                          i0 = iDatHeight - 1;
                       if (jj > iDatWidth - 1)
                          j0 = iDatWidth - 1;
                  
                          
					          // 考虑可见光反射率的中红外通道疑似高温像元判断  20170809
					          //#### 背景中红外计算邻近像元取舍温度参数： T4BJLimit，初值3150，即315K。
				
				            if (  m_wSatDataBuff_p[4][i * iDatWidth + j]  > T4BJLimit )    //20180314
				             {
                                   
				            		iaround = 0; //20170828
				               // 不处理紧邻探测像元i,j的像元，即不处理：i-1,j-1;i-1,j;i-1,j+1;i,j-1;i,j;i,j+1;i+1,j-1,i+1,j;i+1,j+1的像元 20170716  上				                         
				            	if (m_cPixelId_p[i0][j0] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] <= m_wSatDataBuff_p[4][i * iDatWidth + j] - DtBJ2
				            		&& ( m_wSatDataBuff_p[3][i0 * iDatWidth + j0] >= m_wSatDataBuff_p[3][i * iDatWidth + j] - BJDNIR &&
				            		m_wSatDataBuff_p[1][i0 * iDatWidth + j0] >= m_wSatDataBuff_p[1][i * iDatWidth + j]- BJDT11)  &&(i0 != i && j0 != j))  //20180314  小于探测像元中红外亮温-4K,并去除可能的水体					
				            		{
				            	if(( i0 == i-1 && j0 == j-1) ||( i0 == i-1 && j0 == j) ||( i0 == i-1 && j0 == j+1) ||( i0 == i && j0 == j-1) ||( i0 == i && j0 == j+1) ||( i0 == i+1 && j0 == j-1) ||( i0 == i+1 && j0 == j) ||( i0 == i+1 && j0 == j+1))//  20170828
                          iaround = 1;
			              
                      if ( iaround == 0 &&  m_cPixelId_p[i][j] != 153   ) //20180226   考虑水体影响
				            	{
                    
                                     
				            		iCount++; //有效值的个数
                                
				            		  iIr3BjAv =  iIr3BjAv + m_wSatDataBuff_p[4][i0 * iDatWidth + j0]; //20170627
                          iIr34BjAv =  iIr34BjAv + (abs(m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0])); //20170627
                    
				            		  //增加可见光和远红外***********************************************************20170706
				            		  iIr1BjAv =  iIr1BjAv + m_wSatDataBuff_p[1][i0 * iDatWidth + j0];   //    计算远红外通道累加值20170627
				            		  iIr5BjAv =  iIr5BjAv + m_wSatDataBuff_p[5][i0 * iDatWidth + j0];   //    计算可见光通道累加值20170627
                    
				            		  //NDVI平均值数组******************************20170825
	                           		 NDVIAv = NDVIAv + NDVI[i0][j0];          //    计算NDVI累加值20170825
				            	}  
                       }
				             } 
                              
				            //  如果中红外通道小于315K，使用邻近像元计算背景温度 20170722
                          
				            	 if (  m_wSatDataBuff_p[4][i * iDatWidth + j]  < 3150 )      //20180314
				            	 {
                        
				            	  if (m_cPixelId_p[i0][j0] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] <= m_wSatDataBuff_p[4][i * iDatWidth + j] - DtBJ2
				            	  &&  ( m_wSatDataBuff_p[3][i0 * iDatWidth + j0] >= m_wSatDataBuff_p[3][i * iDatWidth + j] - BJDNIR &&
				            	   m_wSatDataBuff_p[1][i0 * iDatWidth + j0] >= m_wSatDataBuff_p[1][i * iDatWidth + j]- BJDT11) && (i0 != i && j0 != j))
				            	  {  //20180314  小于探测像元中红外亮温-4K,并去除可能的水体
				            	  
				            	   iCount++; //有效值的个数
                               
				            	   iIr3BjAv =  iIr3BjAv + m_wSatDataBuff_p[4][i0 * iDatWidth + j0]; //计算中红外平均值 20170627
                         iIr34BjAv =  iIr34BjAv + (abs(m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0])); //计算中远红外差异平均值 20170627
				            	   
				            	   //增加可见光和远红外***********************************************************20170706
				            	   iIr1BjAv =  iIr1BjAv + m_wSatDataBuff_p[1][i0 * iDatWidth + j0];   //    计算远红外通道平均值20170627
				            	   iIr5BjAv =  iIr5BjAv + m_wSatDataBuff_p[5][i0 * iDatWidth + j0];   //    计算可见光通道平均值20170627
                    
				            	   //增加NDVI平均值数组******************************20170825
	                               	NDVIAv = NDVIAv + NDVI[i0][j0];          //    计算NDVI平均值20170825 
				            
                    
                        }
				               } 
                    }
                 }
			                
				        float fTemp; //窗区内有效值的占比
                fTemp = (float)((ij * 2 + 1) * (ij * 2 + 1));
				        fTemp =  (float)iCount / fTemp * 100.;
                //#### 背景邻域像元个数比例下限参数：Count_Limit，初值15，即比例0.15；#### 背景邻域像元个数下限参数： Count_No，初值8。  

			 	       if (fTemp < Count_Limit ||  iCount < Count_No )
			 	   	    {  //有效值占比 《 15%   或个数小于6 继续扩大邻域 //20170716
                   ij++;            //扩大窗区
                 }
                 else
                 	{    // 满足背景像元计算的像元个数，计算背景值
                    bDoId = false;
                    wIR4BjAver_p[i][j] = (unsigned short int)(iIr3BjAv / iCount);
                    wIR45BjAver_p[i][j] = (unsigned short int)(iIr34BjAv / iCount);

					           //增加可见光和远红外***********************************************************20170706
					          wIR1BjAver_p[i][j] = (unsigned short int)(iIr1BjAv / iCount);
                    wIR5BjAver_p[i][j] = (unsigned short int)(iIr5BjAv / iCount);

					          //增加NDVI平均值数组******************************20170825
	         	       NDVIAver_p[i][j] =  ( NDVIAv / iCount)*100.;          //    计算NDVI平均值20170825				  
                 }
				               //#### 背景温度计算邻域范围上限 BJlimit (上限为51 )
      
                   if ((ij * 2 + 1) > BJlimit)
                 { // 最大窗区设为21*21 20171107
					          bDoId = false;   //扩大窗区循环标志
                
                    Noefft_p[i][j] = 250;   //20170823
                    wIR4BjAver_p[i][j] = 0;
                    wIR45BjAver_p[i][j] = 0;

                    //增加可见光和远红外***********************************************************20170706
					          wIR1BjAver_p[i][j] = 0;             //20170706
                    wIR5BjAver_p[i][j] = 0;             //20170706

					          //增加 NDVI***********************************************************20170706
					           NDVIAver_p[i][j] = 0.;             //20170825
					
				
				         }
              }
              while (bDoId);
			  }

          


		  //  云区像元平均值计算 20170716 上 *********************************************************

			 // 检查云区像元是否满足计算平均值条件，#### 云区中红外温度界限参数：T4CLD，初值3100，即310K；#### 云区可见光反射率界限值： VisCLD，初值500，即0.5；#### 云区远红外温度界限值： T11CLD，初值2500，即250K。
               
			 if (m_cPixelId_p[i][j] == 220 && m_wSatDataBuff_p[4][i * iDatWidth + j] > T4CLDBJ &&  
			 	m_wSatDataBuff_p[5][i * iDatWidth + j] <VisCLDBJLimit && 
			 	  m_wSatDataBuff_p[1][i * iDatWidth + j] > T11CLD)
			 	{ //满足标记值为云区220 并且 20170716
              
			    bDoId = true;   
		      ij = 3;
	
              do{    // 不同的背景窗区
                 iCount = 0;
                 int iIr3BjAv, iIr34BjAv,iIr1BjAv, iIr5BjAv;
				         int iaround = 0;  //20170828
                 //iIr3BjAv 中红外背景平均值， iIr34BjAv 中红外和远红外差的背景平均值
                 iIr3BjAv = 0;
                 iIr34BjAv = 0;

				          iIr1BjAv = 0;  //20170706
				          iIr5BjAv = 0;  //20170706
                // 计算云区像元背景平均值，背景邻域大小为(ij*2+1)*(ij*2+1),即初值5*5.
                 for (int ii = i - ij; ii <= i + ij; ii++)
                 {
                    for (int jj = j -ij; jj <= j + ij; jj++)
                    {
                       int i0,j0;
                       i0 = ii;
                       j0 = jj;
                       if (ii < 0)
                           i0 = 0;
                       if (jj < 0)
                           j0 = 0;
                       if (ii > iDatHeight - 1)
                          i0 = iDatHeight - 1;
                       if (jj > iDatWidth - 1)
                          j0 = iDatWidth - 1;
                   
					              //#### 云区像元背景中红外界限值 T4CLDBJ （初值3150，即315K） 如果中红外通道大于315K，排除邻近像元计算背景温度 20170722  上
					  
					
                       if (  m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  > T4BJLimit )    //20180124
					            {

				               // 不处理紧邻探测像元i,j的像元，即不处理：i-1,j-1;i-1,j;i-1,j+1;i,j-1;i,j;i,j+1;i+1,j-1,i+1,j;i+1,j+1的像元 20170716  上
				
                        if (m_cPixelId_p[i0][j0] == 220  && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > T4CLDBJLimit &&  
              	        m_wSatDataBuff_p[5][i0 * iDatWidth + j0] < VisCLDBJLimit &&  ii != i && jj != j  )
              	        {        //20180316
							         if((i0 == i && j0 == j) ||( i0 == i-1 && j0 == j-1) ||( i0 == i-1 && j0 == j) ||( i0 == i-1 && j0 == j+1) ||( i0 == i && j0 == j-1) ||( i0 == i && j0 == j+1) ||( i0 == i+1 && j0 == j-1) ||( i0 == i+1 && j0 == j) ||( i0 == i+1 && j0 == j+1))//  20170828
						           {
						            iaround = 1;
						           }
					             if ( iaround == 0)
					             {			

                         iCount++; //有效值的个数
                  
						            iIr3BjAv =  iIr3BjAv + m_wSatDataBuff_p[4][i0 * iDatWidth + j0]; //20170627
                         iIr34BjAv =  iIr34BjAv + (abs(m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0])); //20170627

						            iIr1BjAv =  iIr1BjAv + m_wSatDataBuff_p[1][i0 * iDatWidth + j0];   //    计算远红外通道平均值20170627
						            iIr5BjAv =  iIr5BjAv + m_wSatDataBuff_p[5][i0 * iDatWidth + j0];   //    计算可见光通道平均值20170627
					             }
                        }
					            }  // 中红外通道大于315K  20170722  下

					            // #### 云区像元背景中红外界限值 T4CLDBJ （初值3150，即315K）， 如果中红外通道大于315K，排除邻近像元计算背景温度 20170722  上如果中红外通道小于315K，使用邻近像元计算背景温度 20170722  上				
                      if (  m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  <= T4BJLimit )    //20180124
					           {
                       //#### 同上，即云区像元背景选择中红外阈值 T4CLDBJLimit （初值 2900， 即290K）； 可见光阈值 VisCLDBJLimit （初值500，即0.5），
				  	  
                      if (m_cPixelId_p[i0][j0] == 220 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > T4CLDBJLimit && 
                          m_wSatDataBuff_p[5][i0 * iDatWidth + j0] < VisCLDBJLimit &&  ii != i && jj != j )
                          { //满足标记值为100并且中红外《 考虑可见光反射率引起的中红外增量20180316
                        
						 
						                   iCount++; //有效值的个数
                                  //   printf(" VIS AVER CH3>295K    %d,  %d,CH3 = %d, CH4 = %d, VIS = %d, VISAver = %d, CloudAver = %d,\n", i,j, m_wSatDataBuff_p[4][i * iDatWidth + j],m_wSatDataBuff_p[1][i * iDatWidth + j],m_wSatDataBuff_p[5][i * iDatWidth + j],wIR5BjAver_p[i][j], CloudAver_p[i][j]); //20171126
						                  iIr3BjAv =  iIr3BjAv + m_wSatDataBuff_p[4][i0 * iDatWidth + j0]; //20170627
                              iIr34BjAv =  iIr34BjAv + (abs(m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0])); //20170627
                             
						                  iIr1BjAv =  iIr1BjAv + m_wSatDataBuff_p[1][i0 * iDatWidth + j0];   //    计算远红外通道平均值20170627
						                  iIr5BjAv =  iIr5BjAv + m_wSatDataBuff_p[5][i0 * iDatWidth + j0];   //    计算可见光通道平均值20170627
			
                          }
					           }  // 中红外通道小于325K  20170722  下
                    }  
                 }

                 float fTemp; //窗区内有效值的占比
                 fTemp = (float)((ij * 2 + 1) * (ij * 2 + 1));
				         fTemp =  (float)iCount / fTemp * 100.;
                			                
                 //#### 云区像元背景温度计算像元个数比例阈值参数 fCLDTempLimit（初值15，即比例0.15）；背景温度计算像元个数阈值参数 iCLDCountLmt （初值8） 
			 	         if (fTemp <  fCLDTempLimit || iCount < iCLDCountLmt )
			 	         {  //有效值占比 《 15%   或个数小于8 继续扩大邻域 //20170716
                   ij++;            //扩大窗区
                 }
                 else
                 {    // 否则计算背景值
                    bDoId = false;

                    wIR4BjAver_p[i][j] = (unsigned short int)(iIr3BjAv / iCount);
                    wIR45BjAver_p[i][j] = (unsigned short int)(iIr34BjAv / iCount);

					          //增加可见光和远红外***********************************************************20170706
					          wIR1BjAver_p[i][j] = (unsigned short int)(iIr1BjAv / iCount);
                    wIR5BjAver_p[i][j] = (unsigned short int)(iIr5BjAv / iCount);

					          //增加云区平均值***********************************************************20170716
					         CloudAver_p[i][j] = (unsigned short int)(iIr3BjAv / iCount);
		
                 }
				          // ####云区像元背景温度计算邻域范围上限 CBJlimit (上限为21   )
                  //   if ((ij * 2 + 1) > CBJlimit){ // 云区最大窗区设为11*11，标记仍为云区220，云区背景平均值均设为400K
                   if ((ij * 2 + 1) > BJlimit)
                { // 云区最大窗区设为11*11，标记仍为云区220，云区背景平均值均设为400K  //20180209  
					         bDoId = false;   //扩大窗区循环标志
                
                    wIR4BjAver_p[i][j] = 0;
                    wIR45BjAver_p[i][j] = 0;

                    //增加可见光和远红外***********************************************************20170706
					          wIR1BjAver_p[i][j] = 0;             //20170706
                    wIR5BjAver_p[i][j] = 0;             //20170706
             
                    //增加云区平均值*******即当没有足够的背景像元数时，将云区平均值置为40000******************20170716
				           CloudAver_p[i][j] = 4000;  //20171101					
					        // 显示超出背景计算像元个数信息，包括：行号、列号、像元数、比例数、邻域大小   上  //2017010
				        }
              }
              while (bDoId);

          //  云区平均值计算 20170716 下  *********************************************************

        } // 计算背景平均值结束
       
       
       }  //   iDatWidth  循环结束
     }     //   iDatWidth  循环结束


	 //计算通道6（1.6微米），通道7（2.2微米）平均值 20170831 上	
	  for (int i = 0; i < iDatHeight; i++)
	  {
      for (int j = 0; j < iDatWidth; j++)
      {
  
        if (m_cPixelId_p[i][j] == 100)
        { // 对标记值为100的点计算背景平均值
              bDoId = true;
         	    ij = 3;   // 20161005********************************
          
               iCount = 0;
               int iIr6BjAv, iIr7BjAv;		           

			          iIr6BjAv = 0;  //20170825
			          iIr7BjAv = 0;  //20170825				

                 for (int ii = i - ij; ii <= i + ij; ii++)
                 {
                    for (int jj = j -ij; jj <= j + ij; jj++)
                    {
                       int i0,j0;
                       i0 = ii;
                       j0 = jj;
                       if (ii < 0)
                           i0 = 0;
                       if (jj < 0)
                           j0 = 0;
                       if (ii > iDatHeight - 1)
                          i0 = iDatHeight - 1;
                       if (jj > iDatWidth - 1)
                          j0 = iDatWidth - 1;
					             if ( m_cPixelId_p[i0][j0] == 100   )
					             {
					              iIr6BjAv =  iIr6BjAv + m_wSatDataBuff_p[6][i0 * iDatWidth + j0];    //    计算通道6（1.6）平均值20170825      
                       iIr7BjAv =  iIr7BjAv + m_wSatDataBuff_p[7][i0 * iDatWidth + j0];   //    计算通道7（2.3）平均值20170825  
                       iCount++; //
					             }
					          }
					       }
                   //#### 通道6（1.6）,通道7（2.3）背景温度计算像元个数阈值 iCountSIR (初值为10)
					if ( iCount > iCountSIR ) 
					 {
					  wIR6BjAver_p[i][j] = (unsigned short int)( iIr6BjAv / iCount);   //    计算通道6（1.6）平均值20170825      
            wIR7BjAver_p[i][j] = (unsigned short int)( iIr7BjAv / iCount);   //    计算通道7（2.3）平均值20170825  
					 }              
			  }           
		  }
	  }
	 // 计算1.6，2.3平均值 20170831 下
     return;
}

/*: *******************  FireProBJ2 *********************
* 函数名称：  FireProBJ2
* 函数标题：  计算背景温度
* 函数功能：  计算中红外的标准偏差背景温度及中红外和远红外标准偏差的差的背景温度
* 调用方法: FireProBJ2(unsigned short int *m_wSatDataBuff_p[])
*   参数说明：
*   unsigned short int *m_wSatDataBuff_p[] 卫星数据
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::FireProBJ2()
{
          //------------- 计算背景温度  -----------------
     int ij, iCount;
	 float coe1 = 1.;
	 float fTemp = 0.; //窗区内有效值的占比
      ij = 3;       //     20161005********************************

     iCount = 0;
     bool bDoId = true;     //     20161005********************************



	  for (int i = 0; i < iDatHeight - 10; i++) {
	 	 for (int j = 0; j < iDatWidth; j++) {
		
			ij = 3;  //     20161005********************************
        
			bDoId = true;
      
       if (wIR4BjAver_p[i][j] > 0 && ( m_cPixelId_p[i][j] == 100 || m_cPixelId_p[i][j] == 220 ) )    //20180102
		   {  // 有有效平均值的情况 继续计算标准差
                 //ShowMessage("bejing" + IntToStr(wIR4BjAver[i][j]));
			   do
			  {
				 //---------  计算标准差   -----------
				 unsigned short int wIR4BjAv = 0;
				 unsigned short int wIR45BjAv = 0;
				 unsigned short int wIR1BjAv = 0;    //20171107
				 unsigned short int wIR5BjAv = 0;    //20180102
				 iCount = 0;
				 int iaround = 0;
          wIR4BjAv  = wIR4BjAver_p[i][j];
          wIR45BjAv = wIR45BjAver_p[i][j];
				  wIR1BjAv  = wIR1BjAver_p[i][j];    //20171107
				  wIR5BjAv  = wIR5BjAver_p[i][j];    //20180102
				  fIR4Bj_p[i][j] = 0.;  //20170627
				  fIR45Bj_p[i][j] = 0.; //20170627
				 fIR1Bj_p[i][j] = 0.;   //20171107 ******
				 fIR5Bj_p[i][j] = 0.;   //20180102 ******
                 for (int ii = i - ij; ii <= i + ij; ii++)
                 {
                    for (int jj = j -ij; jj <= j + ij; jj++)
                    {
                       int i0,j0;
                       i0 = ii;
                       j0 = jj;
                       if (ii < 0)
                           i0 = 0;
                       if (jj < 0)
                           j0 = 0;
                       if (ii > iDatHeight - 1)
                          i0 = iDatHeight - 1;
                       if (jj > iDatWidth - 1)
                          j0 = iDatWidth - 1;
				
                      //#### 背景计算邻近像元取舍温度参数 T4BJLimit（初值3150，即315K）
				
					                 if ( m_cPixelId_p[i][j] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  > T4BJLimit )  //20180102
						             {
                          //#### 背景像元选取东西区域参数 RegnWEBJ (初值1000)
						                   if ( jj < 1001) coe1 = 1.;
                          //#### 背景像元选取权重参数1 Coe1,(初值1)
						               //#### 背景像元选取权重参数1 Coe2,(初值0.6)
						                else  coe1 = 0.6;
					             
                           iaround = 0;                    
                           //#### 背景像元选取的远红外温度增量系数 DtBJ1   （初值350，即35K）
				
                            if (m_cPixelId_p[i0][j0] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] <= m_wSatDataBuff_p[4][i * iDatWidth + j] - DtBJ2 &&
                         	 (i0 != i && j0 != j))  //20180124  小于探测像元中红外亮温4K
						              {						 					 
						           
                           if(( i0 == i-1 && j0 == j-1) ||( i0 == i-1 && j0 == j) ||( i0 == i-1 && j0 == j+1) ||( i0 == i && j0 == j-1) ||( i0 == i && j0 == j+1) ||( i0 == i+1 && j0 == j-1) ||( i0 == i+1 && j0 == j) ||( i0 == i+1 && j0 == j+1))//  20170828
						               {
						                 iaround = 1;
						               }
					                  if ( iaround == 0)
					                {					   
						               iCount++;
                             // 计算标准差
					  		       				  
						                fIR4Bj_p[i][j] = fIR4Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv));   //   20170627 中红外通道均差的平方和
						                fIR1Bj_p[i][j] = fIR1Bj_p[i][j] + (float)((m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv) * (m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv));   //   20171107 远红外通道均差的平方和 *******
                            fIR45Bj_p[i][j] = fIR45Bj_p[i][j] +  (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv));   // 20170627   中红外与远红外均差的平方和					
					                  fIR5Bj_p[i][j] = fIR5Bj_p[i][j] + (float)((m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv) * (m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv));   //   20180217 可见光通道均差的平方和 *******
					                }
					                }
					               }      
                      // 如果中红外通道小于于315K，使用邻近像元计算背景温度
					         //#### 同上，背景计算邻近像元取舍温度参数 T4BJLimit（初值3150，即315K） 
			            
					         if ( m_cPixelId_p[i][j] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  <= T4BJLimit )  //20180102
					          {
                      			 //#### 同上，背景像元选取的远红外温度增量系数 DtBJ1   （初值350，即35K）							
                         
						          if (m_cPixelId_p[i0][j0] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] <= m_wSatDataBuff_p[4][i * iDatWidth + j] - DtBJ2 &&
						          	 (i0 != i && j0 != j)) 
						          { //20180124
                                 iCount++;

                      // 计算标准差
					          
						          fIR4Bj_p[i][j] = fIR4Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv));   //   20170627 中红外通道均差的平方和
                      fIR1Bj_p[i][j] = fIR1Bj_p[i][j] + (float)((m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv) * (m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv));   //   20171107 远红外通道均差的平方和 *******
                      fIR45Bj_p[i][j] = fIR45Bj_p[i][j] +  (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv));   // 20170627   中红外与远红外均差的平方和				
					            fIR5Bj_p[i][j] = fIR5Bj_p[i][j] + (float)((m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv) * (m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv));   //   20180217 可见光通道均差的平方和 *******
					           }
					          }  
					              //云区像元标准差计算     20180102  开始
                   if (m_cPixelId_p[i][j] == 220 && m_wSatDataBuff_p[4][i * iDatWidth + j] > T4CLDBJ &&  
                     	m_wSatDataBuff_p[5][i * iDatWidth + j] <VisCLDBJLimit &&   m_wSatDataBuff_p[1][i * iDatWidth + j] > T11CLD)
                    { //满足标记值为云区220及有关条件  20180102
                      
                    
                       if (  m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  > T4BJLimit )   // 20180316
					            {

				                  // 不处理紧邻探测像元i,j的像元，即不处理：i-1,j-1;i-1,j;i-1,j+1;i,j-1;i,j;i,j+1;i+1,j-1,i+1,j;i+1,j+1的像元 20170716  上
					              //#### 云区像元背景选择中红外阈值 T4CLDBJLimit （初值 2650， 即265K）；####  云区像元背景选择可见光阈值 VisCLDBJLimit （初值500，即0.5），
                
                        if (m_cPixelId_p[i0][j0] == 220  && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > T4CLDBJLimit &&  
                        	m_wSatDataBuff_p[5][i0 * iDatWidth + j0] < VisCLDBJLimit &&  ii != i && jj != j  ) 
                       {        //2018316
                          //  if ((m_cPixelId_p[i0][j0] == 220 ||m_cPixelId_p[i0][j0] == 100 ) && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > T4CLDBJLimit &&  m_wSatDataBuff_p[5][i0 * iDatWidth + j0] < VisCLDBJLimit && ii != i && jj != j  ) {       //20181015
						              if((i0 == i && j0 == j) ||( i0 == i-1 && j0 == j-1) ||( i0 == i-1 && j0 == j) ||( i0 == i-1 && j0 == j+1) ||( i0 == i && j0 == j-1) ||( i0 == i && j0 == j+1) ||( i0 == i+1 && j0 == j-1) ||( i0 == i+1 && j0 == j) ||( i0 == i+1 && j0 == j+1))//  20170828
						             {
						             iaround = 1;
						             }
					                if ( iaround == 0)
					                    {			
                                 iCount++; //有效值的个数
                                 fIR4Bj_p[i][j] = fIR4Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv));   //   20180102 中红外通道均差的平方和
						                     fIR1Bj_p[i][j] = fIR1Bj_p[i][j] + (float)((m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv) * (m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv));   //   20180102 远红外通道均差的平方和 *******
                                 fIR5Bj_p[i][j] = fIR5Bj_p[i][j] + (float)((m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv) * (m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv));   //   20180102 可见光通道均差的平方和 *******
                                 fIR45Bj_p[i][j] = fIR45Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv));   // 20180102   中红外与远红外均差的平方和			
					                    }
                       }
					            }  // 中红外通道大于295K  20180102  下

				
                       if (  m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  <= T4BJLimit )        //20180113
					          {
                     // 可见光阈值 VisCLDBJLimit （初值500，即0.5），
				  	  
                  
                       if (m_cPixelId_p[i0][j0] == 220  && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > T4CLDBJLimit &&
                       	  m_wSatDataBuff_p[5][i0 * iDatWidth + j0] < VisCLDBJLimit  &&  ii != i && jj != j )
                       { //满足标记值为100并且中红外《 考虑可见光反射率引起的中红外增量20180316
                         
						                iCount++; //有效值的个数                          
						               fIR4Bj_p[i][j] = fIR4Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv));   //   20180102 中红外通道均差的平方和
						               fIR1Bj_p[i][j] = fIR1Bj_p[i][j] + (float)((m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv) * (m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv));   //   20180102 远红外通道均差的平方和 *******
                          fIR5Bj_p[i][j] = fIR5Bj_p[i][j] + (float)((m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv) * (m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv));   //   20180102 可见光通道均差的平方和 *******
                          fIR45Bj_p[i][j] = fIR45Bj_p[i][j] +  (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv));   // 20180102   中红外与远红外均差的平方和			
			                  }
					          }  // 中红外通道小于295K  20170722  下

					          }
                       //云区像元标准差计算     20180102  结束


                 }         
                 }
                 //float fTemp; //窗区内有效值的占比
                 fTemp = (float)((ij * 2 + 1) * (ij * 2 + 1));
				         fTemp =  (float)iCount / fTemp * 100.;
                				 
                  //#### 背景温度计算像元个数比例阈值参数 fTempLimit（初值15，即比例0.15）；背景温度计算像元个数阈值参数 iCountLimit （初值8） 
                  if (fTemp < 15. || iCount < 8 )
                  {  //有效值占比 《 15%   或个数小于6 继续扩大邻域 //20170716
                   ij++;            //扩大窗区
                  }
                 else
                 	{    // 否则计算背景标准差
                    bDoId = false;
				
				          	fIR4Bj_p[i][j] = sqrt( (fIR4Bj_p[i][j]) / iCount);       // ******************20180217*******************
                    fIR1Bj_p[i][j] = sqrt( (fIR1Bj_p[i][j]) / iCount);       //                  远红外标准差 20180217
                    fIR45Bj_p[i][j] = sqrt( (fIR45Bj_p[i][j]) / iCount);     // ******************20180217*******************
                    fIR5Bj_p[i][j] = sqrt( (fIR5Bj_p[i][j]) / iCount);        //                 可见光标准差20180217

                 }
                  //#### 背景温度计算邻域范围上限 BJlimit (上限为51 )
          
				     if ((ij * 2 + 1) >BJlimit)
			       { //    20181024
					     bDoId = false;   //扩大窗区循环标志
                //    m_cPixelId_p[i][j] = 250;
					    Noefft_p[i][j] = 250;   //20170823
				
              fIR4Bj_p[i][j] = 0;
					    fIR1Bj_p[i][j] = 0;    //20171107 
              fIR45Bj_p[i][j] = 0;
				      fIR5Bj_p[i][j] = 0;    //20180102          
             }
        }  //扩大窗口循环
			while (bDoId); 
		   }   // 计算均方差结束		          
        }      //end === iDatWidth
     }         //end === iDatHeight

	


	  return;
}

/*: *******************  FirePro *********************
* 函数名称：  FirePro
* 函数标题：  火点判识
* 函数功能：  全圆盘数据逐点判识火
* 调用方法: FirePro(unsigned short int *m_wSatDataBuff_p[], unsigned char * m_TempBuff2_p[])
*   参数说明：
*   unsigned short int *m_wSatDataBuff_p[] 卫星数据
*   unsigned char * m_TempBuff2_p[]       判识结果
     m_fSunZenithAnale
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::FirePro() //20170707	
{
          // 判识火点
     float fTb3th, fTb34th;
     //fTb3th 中红外阈值，fTb34th 中红外 - 远红外的阈值
     fTb3th = 0.; fTb34th = 0.;

//  ------- 20131028 读阈值文件 ------------------------

  int factr_mid, factr_far,dt3th,dt34th; // 分别为中红外因子，远红外因子，中红外阈值，中红外和远红外差的阈值

           
		//#### 中红外火点确认系数参数：T4_Factr，初值为3K, #### 中红外与远红外亮温差异火点确认系数参数:T4_11Factr,初值为3K 。
			factr_mid = 3;       //20170627
			factr_far = 3;       //20170627
			cout << "factr_mid = " << factr_mid << endl;
			cout << "factr_far = " << factr_far << endl;
		
//---------------------------------------------------------------------


     bool b123 = true;
	 int  idcloud = 0;   //20170627
	 int  idesert = 0;   //20170627
	 int  iwater = 0;    //20170810
	 float f4biaoji,f45biaoji;   //20170627
	 float TfIR4Bjc=0.,TfIR45Bjc=0.;  //20170804
	 int  i = 0;   //2017709
	 int ii = 0, jj = 0;
	 int iSolarRef = 0, iSolarRef2 = 0; //20170828
	 int wIR45BjAver_buff = 10;

	  int ij, iCount;   //     20170810********************************
      ij = 2;	
     iCount = 0;
      bool bDoId = true;     //     20170810********************************

	  int LineNHUP,LineNHLW,RawNHUP,RawNHLW;    // 东北部行号上限、下限，东北部和东南部列号上限、下限。20180311
	  int LineETUP,LineETLW;                    // 东南部行号上限、下限。20180311
	  int LineWNUP,LineWNLW,RawWNUP,RawWNLW;    // 西北部行号上限、下限，西北部和西南部列号上限、下限。20180311
	  int LineWSUP,LineWSLW;                    // 西南部行号上限、下限。20180311              
      int QZGYLINENH,QZGYLINESH;     // 青藏高原行列号  20180403
	  int iregn;                //20180227
    //  /*  
	
	  iregn = 1;    // for test  20180312 
    if (  iregion == 1 )       //  赋予图像区域行列号 开始  20180313 regional
	  {      // 赋予区域图行列号
	   LineNHUP = CLDRGLineNHUP ;LineNHLW = CLDRGLineNHLW; RawNHUP=CLDRGRawNHUP ;RawNHLW = CLDRGRawNHLW;    // 赋予东北部行号上限、下限，东北部和东南部列号上限、下限。20180311
	   LineETUP = CLDRGLineETUP ;LineETLW = CLDRGLineETLW;                                                    // 赋予东南部行号上限、下限。20180311
	   LineWNUP = CLDRGLineWNUP;LineWNLW =CLDRGLineWNLW; RawWNUP=CLDRGRawWNUP;RawWNLW=CLDRGRawWNLW;       // 赋予西北部行号上限、下限，西北部和西南部列号上限、下限。20180311  
	   LineWSUP = CLDRGLineWSUP; LineWSLW = CLDRGLineWSLW;                                                   // 赋予西南部行号上限、下限。20180311
	   QZGYLINENH = QZGYRGLINEUP;       // 20180403 赋区域图青藏高原北端行号
       QZGYLINESH = QZGYRGLINELW;       // 20180403 赋区域图青藏高原南端行号  	  
	  }
	  else    // 圆盘图或半圆盘图   //20180311
	  {
	  LineNHUP=CLDLineNHUP; LineNHLW=CLDLineNHLW; RawNHUP=CLDRawNHUP; RawNHLW=CLDRawNHLW;    // 赋予东北部行号上限、下限，东北部和东南部列号上限、下限。20180311
	  LineETUP=CLDLineETUP; LineETLW=CLDLineETLW;                                            // 赋予东南部行号上限、下限。20180311
	  LineWNUP=CLDLineWNUP; LineWNLW=CLDLineWNLW; RawWNUP=CLDRawWNUP;RawWNLW=CLDRawWNLW;     // 赋予西北部行号上限、下限，西北部和西南部列号上限、下限。20180311  
	  LineWSUP=CLDLineWSUP; LineWSLW=CLDLineWSLW;                                            // 赋予西南部行号上限、下限。20180311
	  QZGYLINENH = QZGYLINELW;       // 20180403 赋圆盘图青藏高原北端行号
      QZGYLINESH = QZGYLINE;         // 20180403 赋圆盘图青藏高原南端行号 
	  	  
	  }
    //  */                      //  赋予图像区域行列号 结束  20180311
	 printf("iregn=%d, LineNHUP=%d, LineNHLW=%d,RawNHUP=%d,RawNHLW=%d,LineETUP=%d,LineETLW=%d,LineWNUP=%d,LineWNLW =%d,RawWNUP=%d, RawWNLW=%d,LineWSUP=%d,LineWSLW =%d, \n ",
		 iregn,LineNHUP,LineNHLW,RawNHUP,RawNHLW,LineETUP,LineETLW,LineWNUP,LineWNLW,RawWNUP, RawWNLW,LineWSUP,LineWSLW ); //20180312	 
	 
	int inm = 0;
	int inm1 = 0;  // 20170810 检查周边像元与探测像元中红外相近的个数计数	

	 // ****中间结果文件生成和存储  20170627   上 *********************************     
	 string sFN; 

  

	 // ****镜面反射点文件生成和存储  20180514   下 *********************************


	//生成镜面反射点数组  20170828 上 *********************************
	 int inm3 = 0;	
	 for (int i = 0; i < iDatHeight; i++)
	 {
          for (int j = 0; j < iDatWidth; j++)
     {
               // 镜面反射条件：太阳天顶角小于85度，通道6（1.6）大于背景0.1，中红外大于310K，且与背景差异大于12K，小于25K，通道6背景数组大于0
	               //#### 太阳天顶角阈值 SunZRFLT (初值85  )，####通道6（1.6）与背景差异阈值 DR6SLimit （初值100，即0.1），####中红外通道与背景差异阈值下限 DT4SLTLW （初值120，即12K），####中红外通道与背景差异阈值上限 DT4SLTUP （初值250，即25K）. ####中红外通道镜面反射温度阈值 T4SunLT （初值3100，即310K）. 
       if ( m_fSunZenithAnale[i][j] < SunZRFLT && m_wSatDataBuff_p[6][i * iDatWidth + j] - wIR6BjAver_p[i][j] > DR6SLimit && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4SLTLW  && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < DT4SLTUP  && m_wSatDataBuff_p[4][i * iDatWidth + j] >  T4SunLT &&  wIR6BjAver_p[i][j] != 0 )  //20170829
	     {
         SolarRef_p[i][j] = 1;
	     }
	   }
	 }
   	//生成镜面反射点数组  20170828 下  *******************************

   
//  火点像元确认 *****************************************************************************************************************

      for (int i = 10; i < iDatHeight - 10; i++)
      {    //20180316   考虑对区域图有可能造成行数超界，因此预先减去10行。 并将起始行初值定位10，避免区域图第一行有缺值。
        for (int j = 0; j < iDatWidth; j++)
        {
   
	        fTb3th = 0.; fTb34th = 0.;
          idcloud = 0;   //20170627
	        idesert = 0;   //20170627
	         iwater = 0;    //20170810
		       iSolarRef = 0;  //20170828
		       iSolarRef2 = 0;
 

	      // 晴空绝对高温火点判识 20170707 上
	      //#### 绝对高温火点判识中红外 阈值 T4HiFire  ( 初值3400，即340K), #### 绝对高温中红外与背景差异阈值  DT4HiFire ( 初值120，即12K), #### 绝对高温中红外与远红外差异阈值， DT4_11HiFire ( 初值150，即15K)，#### 绝对高温火点可见光通道阈值 CloudHiFire（ 初值500，即0.5）           
	      	if ( ( m_cPixelId_p[i][j] == 100 || m_cPixelId_p[i][j] == 220) && m_wSatDataBuff_p[4][i * iDatWidth + j] > T4HiFire
	      		&& m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4HiFire
	      		&& m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] > DT4_11HiFire
	      		&& m_wSatDataBuff_p[5][i * iDatWidth + j] < CloudHiFire    // 绝对高温火点判识 20180124
	      	    && ( j> 1000 && m_wSatDataBuff_p[5][i * iDatWidth + j] < 250 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > wIR1BjAver_p[i][j] && m_wSatDataBuff_p[5][i * iDatWidth + j] < wIR5BjAver_p[i][j]))   // 20180326 图像西边水体镜面反射判断
	      	 {
	           inm += 1;   // 20170707
	           m_cPixelId_p[i][j] = 10;  // 火点标记为10
             m_TempBuff2_p[i][j] = 1;   // 判识结果多点为1	                                                                                                                                                                                                                                                                                                
           }                // 晴空绝对高温火点判识 20170707 下
        
	      // 缺少背景像元个数像原本的火点确认（标记250）上
	      // 缺少背景像元个数图像东部的高温火点判识 20170711 上   即：中红外大于325K，且中红外大于远红外30K ，且 远红外大于 278K，且 像元位于东部，即行号大于600，列号大于2300.
          //#### 缺少背景像元个数像元火点判识,图像右下方 中红外温度阈值 LKT4RL (初值3250，即35K),中红外远红外差异阈值 DLKT4_11RL (初值300，即30K)，远红外下限阈值  LKT11RL (初值2780，即278K),右下方行号界限值 LKLineRL,（初值600），列号阈值 LKRawRL （初值2300）.
        if (  Noefft_p[i][j] == 250  && m_wSatDataBuff_p[4][i * iDatWidth + j] > LKT4RL && m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] > DLKT4_11RL   &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > LKT11RL && i >= LKLineRL &&  j >= LKRawRL )    // 确实背景像元高温火点判识 20171119  
        	
	      {
	          inm += 1;   // 20170711
	          m_cPixelId_p[i][j] = 10;  //火点标记为10
             m_TempBuff2_p[i][j] = 1;   // 判识结果多点为1
	      }  
      
	         // 缺少背景像元个数（标记250），且在图像中西部的高温火点判识 上  即：中红外大于338K，且中红外大于远红外30K ，且 远红外大于 278K，且 像元位于中西部，即行号大于600，列号小于2300.
	          //#### 缺少背景像元个数像元火点判识,图像左下方 中红外温度阈值 LKT4CW (初值3380，即338K),#### 中红外远红外差异阈值 DLKT4_11CW (初值300，即30K)，#### 远红外下限阈值  LKT11CW (初值2780，即278K),#### 右下方行号界限值 LKLineRL,（初值600），列号阈值 LKRawRL （初值2300）.
	          else if ( Noefft_p[i][j] == 250  && m_wSatDataBuff_p[4][i * iDatWidth + j] > LKT4CW && m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] > DLKT4_11CW  &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > LKT11CW   && i >= LKLineRL  &&  j < LKRawRL )    //  确实背景像元高温火点判识 20170711 
	       {
	          inm += 1;   // 20170711
	          m_cPixelId_p[i][j] = 10;  //火点标记为10
              m_TempBuff2_p[i][j] = 1;   // 判识结果多点为1
	       
	       }  
    
            // 缺少背景像元个数（标记250），且在图像北部的高温火点判识 上  20170810       
             //#### 缺少背景像元个数像元火点判识,图像北部 中红外温度阈值 LKT4NH (初值3150，即315K),####中红外远红外差异阈值 DLKT4_11NH (初值300，即30K)，####远红外下限阈值 LKT11NH (初值2500，即250K), ####图像北部行号界限参数 LKLineNH,（初值600）， ####图像北部列号界限参数 LKRawNH （初值600）.
            else if (  Noefft_p[i][j] == 250  && m_wSatDataBuff_p[4][i * iDatWidth + j] > LKT4NH && m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] > DLKT4_11NH  &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > LKT11NH   && i > 100 && i < LKLineNH &&  j > LKRawNH )    //  确实背景像元高温火点判识 20170711 
          	
	        {
	           inm += 1;   // 20170711
	           m_cPixelId_p[i][j] = 10;  //火点标记为10
               m_TempBuff2_p[i][j] = 1;   // 判识结果多点为1
              
	         }  
     
	         // ****检查已判识为火点的不足背景像元个数像元是否存在多个邻近像元与该像元温度相近，但温度不够高的情况，若是，认为是误判  20170824
	          inm1 = 0;
	         if (m_cPixelId_p[i][j] == 10 && Noefft_p[i][j] == 250  ) // 检查已确认为不足背景个数的火点像元
	         { 
	            
                       ij = 2; 
                       inm1 = 0;
               
                       for (int ii = i - ij; ii <= i + ij; ii++)
                       {
                          for (int jj = j -ij; jj <= j + ij; jj++)
                          {
                             int i0,j0;
                             i0 = ii;
                             j0 = jj;
                             if (ii < 0)
                                 i0 = 0;
                             if (jj < 0)
                                 j0 = 0;
                             if (ii > iDatHeight - 1)
                                i0 = iDatHeight - 1;
                             if (jj > iDatWidth - 1)
                                j0 = iDatWidth - 1;
                               // ####  中红外相近亮温差异阈值参数：T4NR_Df，f初值为4K。  20171010                      
		      			              if (  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] < T4NR_Df  && ( i0 != i ||  j0 != j ) ) // 20180514
		      			               inm1 = inm1 + 1;
		      			          }
		      		         }         
	            
	            //#### 检查多个像元相近情况的误判  相近像元个数阈值参数NRPixNo1 （初值4），中红外阈值参数 NRT4Limit （初值3150，即315K）， 
	            if (inm1 > NRPixNo1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= NRT4Limit   )   //  如果与探测像元中红外亮温差异小于3K的像元大于5个，认为该像元非火点  // 20170811  上
	            {
	            m_cPixelId_p[i][j] = 100;
	            m_TempBuff2_p[i][j] = 0;
	            }
	             // 中红外小于于315K 
	            //#### 检查多个像元相近情况的误判，在小于中红外阈值参数情况下，  相近像元个数阈值参数NRPixNo2 （初值5） 
	            else if (inm1 > NRPixNo2  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < NRT4Limit   )   //  如果与探测像元中红外亮温差异小于3K的像元大于5个，认为该像元非火点  // 20170811  上
	            {
	            m_cPixelId_p[i][j] = 100;
	            m_TempBuff2_p[i][j] = 0;
	            }
	         }
	        //  如果与探测像元中红外亮温差异小于4K的像元大于3个，认为该像元非火点  // 20170824  下   	 
	          else if (   Noefft_p[i][j] == 250 &&  m_cPixelId_p[i][j] == 100  )
	          m_cPixelId_p[i][j] = 250;
         
            // 缺少背景像元个数像原本的火点确认（标记250）下
	       // 云区像元火点判识，即标记为云区的像元火点判识  20170713 上  ************ 	     
	       if ( m_cPixelId_p[i][j] != 10)  //20170810   
	       {
	       
		          ii = i;
	           jj = j;
	           if ( ii - 1 < 0 ) ii = 2;
	          if ( ii+1 > iDatHeight-1 ) ii =  iDatHeight - 2;
	          if ( jj -1 < 0) jj = 2;
	          if (  jj+1 > iDatWidth-1) jj = iDatWidth - 2;
	          // 20170801 定义下标限结束
            
	          // 排除云区周边有荒漠区像元情况 20170801 上
	          if ( CloudAver_p[i][j] != NULL  && m_cPixelId_p[ii-1][jj-1]  != 150  && m_cPixelId_p[ii-1][jj]  != 150  && m_cPixelId_p[ii-1][jj+1]  != 150  && m_cPixelId_p[ii][jj-1]  != 150 && m_cPixelId_p[ii][jj+1]  != 150  && m_cPixelId_p[ii+1][jj-1]  != 150  && m_cPixelId_p[ii+1][jj]  != 150  && m_cPixelId_p[ii+1][jj+1]  != 150 ) // 增加检查云区像元周边是否有荒漠区    20170801 
	          {     // 中东部云区火点像元判识  中红外大于310K 20170801
	          
	       
		            if ( m_wSatDataBuff_p[4][i * iDatWidth + j] > T4CLDFIRELW  && m_wSatDataBuff_p[5][i * iDatWidth + j] < VisCLDFIRECE		
		           	&& ( (m_wSatDataBuff_p[4][i * iDatWidth + j] - CloudAver_p[i][j] >DT4CLDFIREUPLW    //2010226
		           	&& m_wSatDataBuff_p[4][i * iDatWidth + j] -  m_wSatDataBuff_p[1][i * iDatWidth + j] > DT411CLDFIRECE && i > CLDLineNHUP		
		           	&& ( fIR1Bj_p[i][j]<  DT11CLDFIREBJCLW && fIR5Bj_p[i][j] < VISCLDFIREBJCUP   )    //20180224	
		           	&& m_wSatDataBuff_p [1][i * iDatWidth + j] > DT11CLDFIRELW )		
		           	|| m_wSatDataBuff_p[4][i * iDatWidth + j] > T4CLDFIREUP  && m_wSatDataBuff_p[5][i * iDatWidth + j] < VisCLDFIRECE
		           	&& m_wSatDataBuff_p[4][i * iDatWidth + j] - CloudAver_p[i][j] >=  DT4CLDFIRELWLW
		           	&& m_wSatDataBuff_p [4][i * iDatWidth + j] -  m_wSatDataBuff_p[1][i * iDatWidth + j] > DT411CLDFIRELW
		           	&&  fIR1Bj_p[i][j]< DT11CLDFIREBJCUP && fIR5Bj_p[i][j] < VISCLDFIREBJCUPUP
		           	&& i >=  CLDLineNHUP &&  j < CLDRawNHLW                   // 20180313
		           	&& m_wSatDataBuff_p[1][i * iDatWidth + j] > DT11CLDFIRELW ) // 增加检查云区标准差  20180223
                       || m_wSatDataBuff_p[4][i * iDatWidth + j] > T4CLDFIREUP && m_wSatDataBuff_p[4][i * iDatWidth + j] - CloudAver_p[i][j] > DT411CLDFIRELW  // 20180303 增加对云区边晴空火点判识
		           	&& m_wSatDataBuff_p [1][i * iDatWidth + j] > T4CLDFIRELW && m_wSatDataBuff_p [1][i * iDatWidth + j] - wIR1BjAver_p[i][j] > DT11CLDFIRELAND
		           	&&  m_wSatDataBuff_p [5][i * iDatWidth + j] < 250 )  //20180304
		          {
	       
		      
		      	  //#### 云区像元火点判识，图像东部列号参数，CLDRawET ( 初值2300 )
         
	                   if ( j < CLDRawNHLW)     // 20180104
		                 {
	                          //  云区火点相近像元判断开始   20180124
		                           ij = 2; 
                               inm1 = 0;
                  
                               for (int ii = i - ij; ii <= i + ij; ii++)
                               {
                                  for (int jj = j -ij; jj <= j + ij; jj++)
                                  {
                                     int i0,j0;
                                     i0 = ii;
                                     j0 = jj;
                                     if (ii < 0)
                                         i0 = 0;
                                     if (jj < 0)
                                         j0 = 0;
                                     if (ii > iDatHeight - 1)
                                        i0 = iDatHeight - 1;
                                     if (jj > iDatWidth - 1)
                                        j0 = iDatWidth - 1;
                               
		              			              //相近温度像?元a选?择?
		              			              // ####  多个相近像元中红外差异上限阈值参数.
		              	
                              if (  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] < SameT4DfUP &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > SameT4DfLW   && ( i0 != i  ||  j0 != j  )) // 20171012
		              			        {
		              			           inm1 = inm1 + 1; 
		              
		              			        }
		              			       }
		              		        }       
	                 
		              			   
		      // 多个云区火点相近像元个数计算  20180104 结束    
		      //东北部云区火点确认 20180311
         
          if ( (j >= RawNHUP &&  j <= RawNHLW ) && (i >= LineNHUP &&  i <= LineNHLW )    //20180312
	         && ( inm1 < CLDSameNoNH-1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIRENH       //  20180224
	         &&  (m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4CLDFIRENH            //  20180224
	         &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] <= DT4CLDFIREUPLW          // 20180313
	         &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > wIR1BjAver_p[i][j]                            //20180305
             &&  m_wSatDataBuff_p[5][i * iDatWidth + j] < wIR5BjAver_p[i][j]  )                         //  20180305 
	        || ( m_wSatDataBuff_p[4][i * iDatWidth + j]-CloudAver_p[i][j] > DT4CLDFIREUPLW               //  20180224
	           && inm1 < CLDSameNoNH  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIRENH   //20180305
		       &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > wIR1BjAver_p[i][j]                     //20180305
	           &&  m_wSatDataBuff_p[5][i * iDatWidth + j] < wIR5BjAver_p[i][j]) ) )	             //  20180305 
	{       
	       
             m_TempBuff2_p[i][j] = 1;   // 判D识o?结¨￠果?多¨¤点ì?为a1
             //   火点记录写入文件t
		      inm += 1;
	         }
	       
         // 西北部云区火点确认 20180308 开始
          
         
         if ( (j >= RawWNUP &&  j <= RawWNLW ) && (i >= LineWNUP &&  i <= LineWNLW )   // 201803012
	         && ( inm1 < CLDSameNoWN-1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIREWN       //  20180308
	         &&  (m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4CLDFIREWN           //  20180308
	         &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] <= DT4CLDFIREUPLW 
	         &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > wIR1BjAver_p[i][j]  //20180305
             &&  m_wSatDataBuff_p[5][i * iDatWidth + j] < wIR5BjAver_p[i][j]  )                  //  20180305 
	        || ( m_wSatDataBuff_p[4][i * iDatWidth + j]-CloudAver_p[i][j] > DT4CLDFIREUPLW               //  20180308
	           && inm1 < CLDSameNoWN  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIREWN    //20180305    
		       &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > wIR1BjAver_p[i][j]   //20180305
	           &&  m_wSatDataBuff_p[5][i * iDatWidth + j] < wIR5BjAver_p[i][j]) ) )	  //  20180305 
	{       
	       
		      m_cPixelId_p[i][j] = 10;  //火e点ì?标à¨o记?为a10
          m_TempBuff2_p[i][j] = 1;   // 判D识o?结¨￠果?多¨¤点ì?为a1     
		      inm += 1;
	       }		
         //  西北部云区火点确认 20180308 结束
         
		       //东南部云区火点确认 20180308  开始  
          if ( ( (j >= RawNHUP &&  j <= RawNHLW ) && (i >=LineETUP &&  i <= LineETLW )) //20180312
	          && ( (inm1 < CLDSameNoET-1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIRECT   //  20180224
		      && ( m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4CLDFIRECT        //  20180224
             &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] <= DT4CLDFIREUPLW ))              //  20180313
	         &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > wIR1BjAver_p[i][j]                      //20180313
             &&  m_wSatDataBuff_p[5][i * iDatWidth + j] < wIR5BjAver_p[i][j]  )                  //  20180305 
		      ||  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4CLDFIREUPLW                  //  20180224
		      && (inm1 < CLDSameNoET  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIRENH ))  //  20180224
         
	        { 
	          m_cPixelId_p[i][j] = 10;  //火e点ì?标à¨o记?为a10
             m_TempBuff2_p[i][j] = 1;   // 判D识o?结¨￠果?多¨¤点ì?为a1
	         inm += 1;
	         }
         
	        //西南部云区火点确认 20180308
	       
          if ( ( (j >= RawWNUP &&  j <= RawWNLW ) && (i >= LineWSUP &&  i <= LineWSLW ))    //20180312
	       
                &&( ( inm1 < CLDSameNoWS -1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIREWS 
		       &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4CLDFIREWS  
		      &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] <= DT4CLDFIREUPLW )        //20180313	
            ||  inm1 < CLDSameNoWS +1  && (  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIREWS       //20180217		  
		        && (m_wSatDataBuff_p[4][i * iDatWidth + j]-CloudAver_p[i][j] >DT4CLDFIREUPLW ))))    //20180211
	        { 
	       
		        m_cPixelId_p[i][j] = 10;  //火点标记为10
             m_TempBuff2_p[i][j] = 1;   // 判识结果多点为1
		      inm += 1;
	         }	 
         
	         // 云影响和水体影响判断 开始 20180224  ********************
         
	       if (m_cPixelId_p[i][j] == 10 &&
	       	    m_wSatDataBuff_p[4][i * iDatWidth + j] < 3200 && m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] && m_wSatDataBuff_p[5][i * iDatWidth + j] >= wIR5BjAver_p[i][j] 
	         || m_wSatDataBuff_p[4][i * iDatWidth + j] < 3200 && m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] - 30 && m_wSatDataBuff_p[5][i * iDatWidth + j] >= wIR5BjAver_p[i][j] - 50 // 20180316	  
	         || m_wSatDataBuff_p[4][i * iDatWidth + j] < 3250 && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 200 && m_wSatDataBuff_p[1][i * iDatWidth + j]- wIR1BjAver_p[i][j] > 20  && 
	         m_wSatDataBuff_p[5][i * iDatWidth + j]- wIR5BjAver_p[i][j] > 100   //20180226	水体边缘影响   
	         || m_wSatDataBuff_p[5][i * iDatWidth + j] > 400 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < 2650         //20180226    // 云反射影响
	                 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3100               //20180226
	         || ( j< 1000 && m_wSatDataBuff_p[5][i * iDatWidth + j] > 200 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] <= wIR1BjAver_p[i][j] +10 && m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j]-10 )   // 20180326 图像西边水体镜面反射判断
	          || (  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3250 && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 300 && m_wSatDataBuff_p[1][i * iDatWidth + j]- wIR1BjAver_p[i][j] > 50  && m_wSatDataBuff_p[5][i * iDatWidth + j]- wIR5BjAver_p[i][j] < 30)  )   // 20180513 陆地云区相交处云影响判断
	         {
              m_cPixelId_p[i][j] = 100;
	           m_TempBuff2_p[i][j] = 0;
	       
	       }
             // 云影响和水体影响判断 结束 20180224   *********************
         
                    }    //  云区火点相近像元判断结束   20180124   
		          }    		
         
	       }   //云区火点判识结束 
	       } //  云区火点判识 下
         
			
	       if (wIR4BjAver_p[i][j] > 0 && m_cPixelId_p[i][j] == 100)
	        { //有背景值和标记为100的点为有效判识点
                	if (b123)
                	{
                	    cout << "id = 100..." << endl;
                	    b123 = false;
                	}
	       
			    	
			    	// 将标准差数组中的数据存入缓冲区，在确认火点公式中使用。而标准差数组的数据还将在后续的误判确定中使用（即火点像元温度较低，标准差较大的情况）。	20171010  
			    	 f4biaoji  = fIR4Bj_p[i][j];
			    	 f45biaoji = fIR45Bj_p[i][j];
	          // 标准差修正，用于火点确认  20171010
                 //#### 中红外标准差图像北部上限阈值参数 T4BJDV_NHUP ( 初值3.)
                     if (fIR4Bj_p[i][j] > T4BJDV_NHUP)    // 20171027 *****************
                	 { 
			         //#### 中红外标准差图像北部界限行号 T4BJDVLineNH ( 初值600   )
			    	 if ( i < T4BJDVLineNH )      // 北部图像标准差行号上限
                 //#### 中红外标准差图像北部上限修正值参数： T4BJDV_NHUPAJ,初值为3K；   20101010
			    	  TfIR4Bjc =  T4BJDV_NHUPAJ; //20171027
			    	  else
			     //#### 中红外标准差图像中南部上限阈值参数 T4BJDV_SHUP ( 初值10K) 20171010       
			      if ( fIR4Bj_p[i][j] >= T4BJDV_SHUP  )  // 20171008
			     // ####  中红外标准差图像南部上限修正值参数: T4BJDV_SHUPAJ,（初值4K）     20171010   
			    		  TfIR4Bjc =  T4BJDV_SHUPAJ ;
			    	  else
			      // #### 中红外标准差图像南部一般修正值参数: T4BJDV_SHAJ,（初值2K）   即： 标准差在3K至10K之间，20171010          
			    	  TfIR4Bjc =  T4BJDV_SHLWAJ;   //20171027				 
			    	 }
			    	 //#### 中红外标准差图像中南部下限值阈值参数 T4BJDV_SHLW ( 初值4K)
                         else if (fIR4Bj_p[i][j] <= T4BJDV_SHLW)   //    20171027
			    	//#### 中红外标准差图像南部下限修正参数 T4BJDV_SHLWAJ ( 初值2K)，#### 中红外火点确认系数参数 T4_Factr （初值为3）
			    	   TfIR4Bjc =  T4BJDV_SHLWAJ ;     			         
			    	   fTb3th =  TfIR4Bjc  * T4_Factr;    		  
			    	    fTb3th = fTb3th;      //20180217
			       if (  fTb3th < 0.  )  fTb3th = 150.;   
               
			      // 中红外和远红外差异标准差修正
			       //####  中红外和远红外标准差图像北部上限阈值参数： T4_11BJDV_NHUP (初值4.)
             if (fIR45Bj_p[i][j] > T4_11BJDV_NHUP)  //20171027
			    	 {
			         //####  中红外和远红外差异标准差图像北部界限行号 T45BJDVLineNH (初值600) ，####  中红外和远红外标准差图像北部上限修正值参数：T4_11BJDV_NHUPAJ，(初值2.5)  
			    		 if(i < T45BJDVLineNH)
			    		 TfIR45Bjc = T4_11BJDV_NHUPAJ;   //20171027
			    	  //#### 中红外和远红外差异标准差图像南部上限阈值参数： T4_11BJDV_SHUP，(初值10K)         
			    	   else if ( fIR45Bj_p[i][j] >=  T4_11BJDV_SHUP )   //20171008
			         // ####  中红外和远红外差异标准差图像南部上限修正值参数：T4_11BJDV_SHUPAJ （初值4K）   20171010		  				           
			    		  TfIR4Bjc =  T4_11BJDV_SHUPAJ;//20171008
			    	    else
			    	   // #### 中红外和远红外标准差图像中南部一般修正值参数： T4_11BJDV_SHAJ，（初值2K）   即： 标准差在3K至10K之间，20171010
			    		  TfIR45Bjc =  T4_11BJDV_SHAJ;          // 20171027
			    	 }
			        //#### 中红外和远红外差异标准差图像下限阈值参数 T4_11BJDV_SHLW ( 初值3K)
              if (fIR45Bj_p[i][j] < T4_11BJDV_SHLW)     //20171027
               //#### 中红外和远红外标准差图像下限修正值参数 T4_11BJDV_SHLWAJ ( 初值2K)， #### 中红外火点确认系数参数 T4_11Factr （初值为3）   
                   TfIR45Bjc = T4_11BJDV_SHLWAJ;        // 20170804 *****************
			             
			             fTb34th =TfIR45Bjc *  T4_11Factr;   //20170804         
		              fTb34th = fTb34th;  //20180217
			       
		     // 初步判断火点：中红外和中红外的背景差》中红外差异阈值， 且中红外和远红外的差与中红外和远红外背景的差异 》 阈值，且可见光 《 0.45，且通道6与背景差异小于0.1 为火点.
		     //####   可见光火点确认阈值参数：Vis_Fireth （初值450，即0.45）；####通道6与背景差异火点确认镜面反射阈值参数，DFR1.6_Fire, (初值100，即0.1)，初判卫星天顶角阈值参数：SatZ_fire ,初值81.#### 初判太阳天顶角阈值参数，SunZ_fire (  初值为95)
	
            if (  m_fSunZenithAnale[i][j] > SunZ_fire ) //20171031
			    	{	
		               //#### 初判中红外与远红外差异平均值上限阈值参数  IR45BjAver_fire （初值为100），#### #### 初判中红外与远红外差异平均值上限修正阈值参数  IR45BjAver_fireAJ		
                    if ( wIR45BjAver_p[i][j] > IR45BjAver_fire)     //20171124
			    	               wIR45BjAver_buff = IR45BjAver_fireAJ;       //20171124
			    	        else 
                    wIR45BjAver_buff = wIR45BjAver_p[i][j];
			    	}
			    	else
			    	  wIR45BjAver_buff = wIR45BjAver_p[i][j]; //20171031		      
			    	
			    	if (((m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j]) > (unsigned short int)fTb3th) && ((m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] - wIR45BjAver_buff) > (unsigned short int)fTb34th)
			    		&& m_wSatDataBuff_p[5][i * iDatWidth + j] < Vis_Fireth && m_wSatDataBuff_p[6][i * iDatWidth + j]- wIR6BjAver_p[i][j] < DFR6_Fire
			    		&& m_fPixelSatelliteZenithAnale[i][j] < SatZ_fire)            
			    	{ 
			    	   m_cPixelId_p[i][j] = 10;  //火点标记为10
                m_TempBuff2_p[i][j] = 1;   // 判识结果多点为1  
                // 以下判断是否在镜面反射点邻近
			    
			    	   if (( SolarRef_p[i-1][j-1]== 1 ||  SolarRef_p[i-1][j]== 1 || SolarRef_p[i-1][j+1]== 1 || SolarRef_p[i][j-1]== 1 ||  SolarRef_p[i][j]== 1 || SolarRef_p[i][j+1]== 1 ||  SolarRef_p[i+1][j-1]== 1 || SolarRef_p[i+1][j]== 1  || SolarRef_p[i+1][j+1]== 1
			    		   ||  SolarRef_p[i-2][j-2]== 1 || SolarRef_p[i-2][j-1]== 1 || SolarRef_p[i-2][j]== 1 || SolarRef_p[i-2][j+1]== 1 ||  SolarRef_p[i-2][j+2]== 1 
			    		   ||  SolarRef_p[i-1][j-2]== 1 || SolarRef_p[i][j-2]== 1 || SolarRef_p[i+1][j-2]== 1 || SolarRef_p[i-1][j+2]== 1 ||  SolarRef_p[i][j+2]== 1 ||  SolarRef_p[i+1][j+2]== 1
			    		   ||  SolarRef_p[i+2][j-2]== 1 || SolarRef_p[i+2][j-1]== 1 || SolarRef_p[i+2][j]== 1 || SolarRef_p[i+2][j+1]== 1 ||  SolarRef_p[i+2][j+2]== 1    )     //20180514
			    		   && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j]  < 200 )    //20180514
			    	    {
			    	      iSolarRef = 1;
			    	    }
         
                    // 以下判断是否有镜面反射影响    //20170828
			    	//#### 中红外通道与背景差异阈值参数 SunRfT4 (初值150，即15K)，####通道6与背景差异下限阈值参数：SunRfCH6LW (初值60，即0.06)，#### 通道6与背景差异上限阈值参数： SunRfCH6Up (初值100，即0.1)=  %d,
            if ((m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j]) < SunRfT4 && m_wSatDataBuff_p[6][i * iDatWidth + j] - wIR6BjAver_p[i][j] > SunRfCH6LW && m_wSatDataBuff_p[6][i * iDatWidth + j] - wIR6BjAver_p[i][j] < SunRfCH6UP )
			    	 {
			    	   iSolarRef2 = 1;
			    	 }
                     
			    	// 以下判断是否在云区边缘			
			     //       扩大云区周边范围上   20180207
			    	   if ( m_cPixelId_p[i-1][j-1] == 220 ||  m_cPixelId_p[i][j-1] == 220 ||  m_cPixelId_p[i+1][j-1] == 220 ||  m_cPixelId_p[i-1][j] == 220 ||  m_cPixelId_p[i+1][j] == 220 ||  m_cPixelId_p[i-1][j+1] == 220||  m_cPixelId_p[i][j+1]== 220  ||  m_cPixelId_p[i+1][j+1] == 220
			    		  || m_cPixelId_p[i-2][j-2] == 220 ||  m_cPixelId_p[i-2][j-1] == 220 ||  m_cPixelId_p[i-2][j] == 220 ||  m_cPixelId_p[i-2][j+1] == 220 ||  m_cPixelId_p[i-2][j+2] == 220 
			    		  || m_cPixelId_p[i-1][j-2] == 220 ||  m_cPixelId_p[i-1][j+2] == 220 
			    		  || m_cPixelId_p[i][j-2] == 220   ||  m_cPixelId_p[i][j+2] == 220 
			    		  || m_cPixelId_p[i+1][j-2] == 220 ||  m_cPixelId_p[i+1][j+2] == 220 
			    		  || m_cPixelId_p[i+2][j-2] == 220 ||  m_cPixelId_p[i+2][j-1] == 220 ||  m_cPixelId_p[i+2][j] == 220 ||  m_cPixelId_p[i+2][j+1] == 220 ||  m_cPixelId_p[i+2][j+2] == 220  )
			    //       扩大云区周边范围下   20180207	  
			    	  {
			    		  idcloud = 1;    //  置云区边缘标记
               }
         
			    	 //以下判断是否在荒漠区边缘 20170627
         
			    	  if ( m_cPixelId_p[i-1][j-1] == 150 ||  m_cPixelId_p[i][j-1] == 150 ||  m_cPixelId_p[i+1][j-1] == 150 ||  m_cPixelId_p[i-1][j] == 150 ||  m_cPixelId_p[i+1][j] == 150 ||  m_cPixelId_p[i-1][j+1] == 150||  m_cPixelId_p[i][j+1]== 150  ||  m_cPixelId_p[i+1][j+1] == 150 )
			    	  {
			    	  idesert = 1;     //  置荒漠区边缘标记
			    	  }
			    	   //以下判断是否在水体边缘 201706810 上  
			    	  if ( m_cPixelId_p[i-1][j-1] == 153 ||  m_cPixelId_p[i][j-1] == 153 ||  m_cPixelId_p[i+1][j-1] == 153 ||  m_cPixelId_p[i-1][j] == 153 ||  m_cPixelId_p[i+1][j] == 153 ||  m_cPixelId_p[i-1][j+1] == 153||  m_cPixelId_p[i][j+1]== 153  ||  m_cPixelId_p[i+1][j+1] == 153 )
			    	  {
			    	  iwater = 1;     //  置水体边缘标记
			    	  }
			    	  
			    	   //判断是否在镜面反射点邻近像元 ,如是，取消火点标记
			    
              if (  iSolarRef == 1 || iSolarRef2 == 1 )      //20170828
               {
			    	      m_cPixelId_p[i][j] = 100;  // 取消火点标记，回复为100，即晴空
                  m_TempBuff2_p[i][j] = 0;   // 判识结果多点为1
			    	      iSolarRef = 0;
			    	      iSolarRef2 = 0;
			    	
			         }   //20170828			  
         
			    	  //判断云区边缘火点像元亮温是否小于300K，且中红外与背景差异，及中远红外差异小于15K ,如是，取消火点标记
			          //#### 中红外通道云区边缘下限阈值参数： CldT4_Edg (初值3000，即300K)，#### 中红外与背景差异云区边缘阈值参数： CldT4BjDf_Edg (初值100，即10K)，                 
			    	  if ( idcloud == 1 && m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_Edg  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < CldT4BjDf_Edg && m_fSunZenithAnale[i][j] < CldSunZenith_Edg )      //20180112
			    	  {
			    	    m_cPixelId_p[i][j] = 100;  // 取消火点标记，回复为100，即晴空
                m_TempBuff2_p[i][j] = 0;   // 判识结果多点为1
			    	    idcloud = 0;
			        }   //20170627
			    	  //判断小于290K的晴空火点 //20171102				
			    	  if ( idcloud == 1 && m_wSatDataBuff_p[4][i * iDatWidth + j] <  CldT4_EdgLW  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < CldT4BjDf_EdgLW )      //20180113
			    	   {
			    	      m_cPixelId_p[i][j] = 100;  // 取消火点标记，回复为100，即晴空
                  m_TempBuff2_p[i][j] = 0;   // 判识结果多点为1
			    	      idcloud = 0;
			         }   //20171102
         
         
			    	  // 判断荒漠区边缘火点像元亮温是否大于330K，如不是，取消火点标记  20170810 上
			    	  //####  荒漠区边缘中红外通道阈值参数：DstT4_Edg， (初值3300，即330K)
			    	   if ( m_wSatDataBuff_p[4][i * iDatWidth + j] < DstT4_Edg && idesert == 1)
			    	   { 
                      m_cPixelId_p[i][j] = 100;  // 取消火点标记，回复为100，即晴空
                      m_TempBuff2_p[i][j] = 0;   // 判识结果多点为1
			    	           idesert = 0;
			    	
			    	   }   //20170627
                   
                      // 判断水体边缘火点像元亮温是否大于330K，如不是，取消火点标记  20170810 上
			    	   //####  水体边缘中红外通道阈值参数：WatT4_Edg， (初值3300，即330K)
			    	   if ( m_wSatDataBuff_p[4][i * iDatWidth + j] < WatT4_Edg && iwater == 1
                      || ( j< 1000 && m_wSatDataBuff_p[5][i * iDatWidth + j] > 150 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] <= wIR1BjAver_p[i][j]+10 && m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j]-30 ))   // 20180326 图像西边水体镜面反射判断
			    	   { 
                      m_cPixelId_p[i][j] = 100;  // 取消火点标记，回复为100，即晴空
                      m_TempBuff2_p[i][j] = 0;   // 判识结果多点为1
			    	          iwater = 0;
			    	 
			    	   }   //20170810
                     // 判断水体边缘火点像元亮温是否大于330K，如不是，取消火点标记  20170810 下
		     
         
	  //判断初判火点是否为在下垫面差异较大，即标准差较大区域,且中红外亮温小于320K，中红外与背景差异小于20K    上   // 20170804
          //#### 下垫面差异中红外下限阈值参数：DBjT4_dfLw，（初值3200，即320K），#### 下垫面中红外与背景差异下限阈值参数：DBjT4bj_dfLw（初值80，即8K），#### 下垫面中红外标准差阈值参数：DBjT4bjc_dfLw（初值12K）    
		if ( m_cPixelId_p[i][j]==10  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < DBjT4_dfLw &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < DT4b_dfLw +  fIR4Bj_p[i][j] && i < DB_dfLineNS       //20180217
		    || m_cPixelId_p[i][j]==10  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < DBjT4_dfSH &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < DT4b_dfLw +  fIR4Bj_p[i][j] * DT4b_dfCoe && i >= DB_dfLineNS    ) //20180112     
	       {
	          m_cPixelId_p[i][j] = 100;  // 取消火点标记，回复为100，即晴空
            m_TempBuff2_p[i][j] = 0;   // 判识结果多点为1
	       
	       }    
	 //    判断初判火点是否为在下垫面差异较大，即标准差较大区域,且中红外亮温小于320K，中红外与背景差异小于20K    下   // 20170804
         
	 //     利用远红外距平判断下垫面差异较大 20171120 上  
         //#### 下垫面远红外标准差上限阈值参数  DT11bjc_dfLw ,
	       
           if ( m_cPixelId_p[i][j]==10  && fIR1Bj_p[i][j] > DT11bjc_dfUP  && m_wSatDataBuff_p[4][i * iDatWidth + j] < DBjT4_dfLw && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 250    //20180206
	        || (  m_wSatDataBuff_p[4][i * iDatWidth + j] <DBjT4_dfLw && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 150  && ( fIR1Bj_p[i][j] > 50. ||  fIR4Bj_p[i][j] > 55. ) ) //20180217		 
	        ||m_cPixelId_p[i][j]==10  &&( m_wSatDataBuff_p[4][i * iDatWidth + j] >= DBjT4_dfLw && m_wSatDataBuff_p[4][i * iDatWidth + j] < DBjT4_dfSH) &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 250  &&  fIR1Bj_p[i][j] > DT11bjc_dfUP  &&  fIR1Bj_p[i][j] < DT11bjc_dfLW //20180208                                                                             	 
	        || m_wSatDataBuff_p[4][i * iDatWidth + j] < T11land_W && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < DT4land_W
		        && m_wSatDataBuff_p[1][i * iDatWidth + j]- wIR1BjAver_p[i][j] > DT11land_W  && m_wSatDataBuff_p[5][i * iDatWidth + j]- wIR5BjAver_p[i][j] > DVISland_W )  //20180226	 水体影响判断   	   	   	   
	       {
	        m_cPixelId_p[i][j] = 100;  // 取消火点标记，回复为100，即晴空
          m_TempBuff2_p[i][j] = 0;   // 判识结果多点为1
	       
	        }    
	       
         // 利用远红外距平判断下垫面差异较大 20171120 下 
         
       //     printf(" &&&&& before cloud influence1  %d,  %d, m_cPixelId_p= %d ,CH3 = %d, CH4 = %d, CH4_AV =  %d, VIS = %d, VIS_AV = %d\n ",i,j, m_cPixelId_p[i][j] ,m_wSatDataBuff_p[4][i * iDatWidth + j], m_wSatDataBuff_p[1][i * iDatWidth + j],wIR1BjAver_p[i][j] ,m_wSatDataBuff_p[5][i * iDatWidth + j], wIR5BjAver_p[i][j]); //20180224
         
	       if ( m_cPixelId_p[i][j] == 10) 
	       {
         
	         //    判断已判识火点是否为云区污染，即可见光反射率偏高，远红外亮温偏低  20170711 或 可见光偏低，远红外距平偏大，中红外偏低等（云或雪区边缘的晴空陆地）       上
         //#### 云污染远红外通道与背景差异阈值参数： CldDisT11Dbj （初值12，即1.2K），####云污染可见光与背景差异阈值参数： CldDisVisDbj （初值150，即0.15），####	云污染可见光上限阈值参数： CldDisVisUp （初值350，即0.35）， 	 	
		     if (  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] - CldDisT11Dbj && m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP &&  m_wSatDataBuff_p[4][i * iDatWidth + j]- wIR4BjAver_p[i][j] < CldT4bj_UP   && m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j]     // 云区反射判断 20180316		  					
		       ||  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] - CldDisT11Dbj && m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP2             // 云区反射判断
		       ||  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] - CldT11_bjc                                                                 // 云区反射判断
		       ||  m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j] + CldDisVisDbj &&  m_wSatDataBuff_p[5][i * iDatWidth + j] > CldDisVisUp1    //20180224     // 云区反射判断
		       ||  m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j] &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j]         //20180224  云区反射判断
	               &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < CldT4bj_UP  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP1              //20180224  云区反射判断
	           || m_wSatDataBuff_p[5][i * iDatWidth + j] > CldDisVisUp1 &&  wIR5BjAver_p[i][j] > CldDisVisUp1  && fIR5Bj_p[i][j] > CldT4bj_UP  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP1  //20180224  云区反射判断
		       ||  m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j] &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j]         //20180224  
	               &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP2  &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < CldT11_UP              //20180224	云区反射判断		 
	           || m_wSatDataBuff_p[5][i * iDatWidth + j] > CldDisVisUp2 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < CldT11_UP         //20180226  云区反射判断
	               &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP1               //20180226	 云区反射判断
		       || m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] <CldlandDVis  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] <CldT4_UP1 && fIR1Bj_p[i][j] > CldlandDT11  // 21080314 云或雪边缘陆地误判判断
		        || m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] < -70  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] <CldT4_UP1 &&  m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] < 250 )   // 周边未被识别的云影响判断 20180516
		   {                                                      
		     m_cPixelId_p[i][j] = 100;
	        m_TempBuff2_p[i][j] = 0;
	       
         
	     }
 	       
	   else 
	 	  {	
	       
         
   //     检查多个温度相近像元的误判，当中红外小于325K加上NDVI修正值，且与背景差异小于15K时，检查是否周边5*5像元中，与探测像元的中红外差异小于4K的像元超过5个  20170810  *** 上	  
	 	     
    //####  多个相近像元中红外阈值参数: SameT4Up (初值3200，即320K)，#### 多个相近像元NDVI修正值参数 SameNDVI （初值250，即0.25）,#### 多个相近像元中红外与背景差异阈值参数:SameT4BJDf,初值为120,即15K. #### 多个相近像元远红外标准差阈值参数 SameT11_bjc （初值为4.）
		     inm1 = 0;   // 20180128 			 
		     if (m_cPixelId_p[i][j] == 10 && m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4Up + (SameNDVI - NDVI[i][j]*100.) && (m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j]) < SameT4BJDf &&  fIR1Bj_p[i][j] > SameT11_bjc && 
			    ( (m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] < 150 &&m_wSatDataBuff_p[5][i * iDatWidth + j] == 0  || m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] < 450  && m_wSatDataBuff_p[5][i * iDatWidth + j] > 5 ))   //20180128 增加对中红外与远红外差异和可见光大于0的判断条件	    
		      || m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 250	   )  //  20180225  
		     {
                     ij = 2; 
                     inm1 = 0;
              
                     for (int ii = i - ij; ii <= i + ij; ii++){
                        for (int jj = j -ij; jj <= j + ij; jj++){
                           int i0,j0;
                           i0 = ii;
                           j0 = jj;
                           if (ii < 0)
                               i0 = 0;
                           if (jj < 0)
                               j0 = 0;
                           if (ii > iDatHeight - 1)
                              i0 = iDatHeight - 1;
                           if (jj > iDatWidth - 1)
                              j0 = iDatWidth - 1;
                    
			    		 // 相近温度像元选择
			    		 // ####  多个相近像元中红外差异上限阈值参数: SameT4DfUP （初值40，即4K），####多个相近像元中红外差异下限阈值参数: SameT4DfLW （初值-40，即-4K）.				     
			    		   if (  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] < SameT4DfUP &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > SameT4DfLW   && ( i0 != i  ||  j0 != j  )) // 20171012
			    		   {
			    		  inm1 = inm1 + 1; 
			    
			    		   }          
			    		}
			    	 }      
         
	             }	  
	       
          
		      // 相近温度像元影响判断
		// 设置云污染标记  20180204 上
		      int iCLOUDid;
		       iCLOUDid = 0;
		       if (   m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 120 && m_wSatDataBuff_p[1][i * iDatWidth + j] - wIR1BjAver_p[i][j] <= 0 &&  m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] > 0 )
		       {
		        iCLOUDid = 1; 
		       }
           // 设置云污染标记  20180204 下
         
            //  东北部相近像元判断　20180308		   
           if ( ((j >= RawNHUP &&  j <= RawNHLW ) && (i >= LineNHUP &&  i <= LineNHLW ))     //20180312
		       && ( ( ( inm1 >= SamePixNHLW  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHUP &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFNH  ))   	
	           || ( inm1>= SamePixETLW  && iCLOUDid == 1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHUP)   //20180313		 
	           || ( inm1>= SamePixETLW  && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFNH && ( fIR4Bj_p[i][j] > SameT4bjc ||  fIR1Bj_p[i][j] > SameT11bjc))  //20180217		  
		       ||  inm1 >= SamePixUP ||  inm1 >= SamePixUP-2 && m_wSatDataBuff_p[4][i * iDatWidth + j]  < SameT4NHUP )    //20180515 
		       ||  inm1 >= SamePixETLW-2 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHLW &&   m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFNHLW   //20180226
	           || inm1 >= SamePixETLW-2 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3150 && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFNHLW  // 20180304
		          && m_wSatDataBuff_p[1][i * iDatWidth + j] - wIR1BjAver_p[i][j] > 40 && m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] < -100)  // 20180304 对云区边缘的晴空误判
	       {
	        m_cPixelId_p[i][j] = 100;
	        m_TempBuff2_p[i][j] = 0;
	       
	        }    
          
	       //  西北部相近像元判断　20180308		   
	           if ( ((j >= RawWNUP &&  j <= RawWNLW ) && (i >= LineWNUP &&  i <= LineWNLW ))    //20180312
		       && ( ( ( inm1 >= SamePixNHLW  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHUP &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFWNLW1 ))   	
	           || ( inm1 >= SamePixETLW  && iCLOUDid == 1)   //20180204		 
	           || ( inm1 >= SamePixETLW  && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] <SameT4DFWNLW2 && ( fIR4Bj_p[i][j] > SameT4bjc ||  fIR1Bj_p[i][j] > SameT11bjc))  //20180217		  
		       ||  inm1 >= SamePixUP ||  inm1 >= SamePixUP-2 && m_wSatDataBuff_p[4][i * iDatWidth + j]  < SameT4NHUP )    //20180515
		       ||  inm1 >= SamePixETLW-2 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHLW &&   m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFWNLW2   //20180226
	           || inm1 >= SamePixETLW-2 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3150 && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFWNLW1  // 20180304
		          && m_wSatDataBuff_p[1][i * iDatWidth + j] - wIR1BjAver_p[i][j] > SameT11DFWN && m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] < SameVISDFWN)  // 20180304 对云区边缘的晴空误判
	       {
	  m_cPixelId_p[i][j] = 100;
	  m_TempBuff2_p[i][j] = 0;
	      
	  }    
         
	       //东南部相近像元判断?　?20180308                                                 
         if ( ((j >= RawNHUP &&  j <= RawNHLW ) && (i >= LineETUP &&  i <= LineETLW ))    //20180312
		     && ( ((inm1 >= SamePixETLW  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4ET  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFETUP  )) 		
	         ||  ( inm1 >= SamePixWSLW -1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3150  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 150)	//20180316
		     || ( inm1 >= SamePixETLW  && iCLOUDid == 1)   //20180204		
	         || ( inm1 >= SamePixETLW   &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFETLW && ( fIR4Bj_p[i][j] > SameT4bjc ||  fIR1Bj_p[i][j] > SameT11bjc))  //20180217		
		     ||   inm1 >= SamePixUP ||  inm1 >= SamePixUP-2 && m_wSatDataBuff_p[4][i * iDatWidth + j]  < SameT4NHUP )  )    //20180515 	
		{    
          m_cPixelId_p[i][j] = 100;
	       m_TempBuff2_p[i][j] = 0;
	       
	  }     
	       
	        //西南部相近像元判断　20180308 
                if ( ((j >= RawWNUP &&  j <= RawWNLW ) && (i >= LineWSUP &&  i <= LineWSLW))  //20180312
			    && ( ( inm1 >= SamePixWSLW  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4WS  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFWSUP)		
		        ||  ( inm1 >= SamePixWSLW -1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3150  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 150)	//20180316
			    || ( inm1 >= SamePixETLW  && iCLOUDid == 1)   //20180204		
		        || ( inm1 >= SamePixETLW   &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFWSUP	  && ( fIR4Bj_p[i][j] > SameT4bjc ||  fIR1Bj_p[i][j] > SameT11bjc))  //20180217			
			    ||   inm1 >= SamePixUP ||  inm1 >= SamePixUP-2 && m_wSatDataBuff_p[4][i * iDatWidth + j]  < SameT4NHUP )     // //20180515
		        || ( (j >= RawWNUP && j <= 2300 ) && (i >= QZGYLINENH &&  i <= QZGYLINESH )) && inm1 >= SamePixETLW -2 && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] <SameQZGYDT4 )  // 20180403   	
		{    
	    m_cPixelId_p[i][j] = 100;
	    m_TempBuff2_p[i][j] = 0;
	       
	       }
         
  //     判断邻近像元是否有强镜面发射点 20180515  上
            ij = 2; 
            int ireflc;
		          float r1,r2;
		         r1 =  float( m_wSatDataBuff_p[6][i * iDatWidth + j])/float(m_wSatDataBuff_p[5][i * iDatWidth + j]); 		
    	       ireflc = 0;
                //      printf(" enter loop for searching reflec pixel %d, %d,CH3 = %d,ireflc = %d,\n ",i,j,m_wSatDataBuff_p[4][i * iDatWidth + j],ireflc); //20180128
                     for (int ii = i - ij; ii <= i + ij; ii++){
                        for (int jj = j -ij; jj <= j + ij; jj++){
                           int i0,j0;
                           i0 = ii;
                           j0 = jj;
                           if (ii < 0)
                               i0 = 0;
                           if (jj < 0)
                               j0 = 0;
                           if (ii > iDatHeight - 1)
                              i0 = iDatHeight - 1;
                           if (jj > iDatWidth - 1)
                              j0 = iDatWidth - 1;
                           r2 =  float( m_wSatDataBuff_p[6][i0 * iDatWidth + j0])/float(m_wSatDataBuff_p[5][i0 * iDatWidth + j0]);
               
			    		 
			    		   // 相近温度像元选择
			    		 // ####  检查是否在强镜面反射点附近，searching strong solar reflec pixel 	20180515			     
			    		   if (  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 200 &&  
			    		   	    ( m_wSatDataBuff_p[1][i * iDatWidth + j] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] <= 20 ||
			    		   	      m_wSatDataBuff_p[1][i * iDatWidth + j] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] >= -20  )  && ( i0 != i||j0 != j )
			    			     &&  m_wSatDataBuff_p[6][i * iDatWidth + j]  <=  m_wSatDataBuff_p[6][i0 * iDatWidth + j0]- 200
			    			   && float( m_wSatDataBuff_p[6][i * iDatWidth + j])/float(m_wSatDataBuff_p[5][i * iDatWidth + j])  <= float( m_wSatDataBuff_p[6][i0 * iDatWidth + j0])/float(m_wSatDataBuff_p[5][i0 * iDatWidth + j0]) - 0.2) // 20180515
			    		   {
			    		   ireflc = 1;
			    		   m_cPixelId_p[i][j] = 100;
                           m_TempBuff2_p[i][j] = 0;
			    
			    		   }          
			    		}
			    	 }      
              //   printf("ireflc result in loop  %d, %d,CH3 = %d,reflc = %d,cPixelId = %d,\n ",i,j,m_wSatDataBuff_p[4][i * iDatWidth + j],ireflc, m_cPixelId_p[i][j] ); //20180515                
         
           // 判断邻近像元是否有强镜面发射点 20180515  下
         
         
         
	       if( m_cPixelId_p[i][j] == 10 && ireflc == 0 )    //20180515      
	       {
           	  inm += 1;         
	        }
	    }               
	        }  // 20170711	
            } 
          }
         
    
        }  // 列循环结束
     }    // 行循环结束
	
	 // 噪声线判识  20170830   上
	  int inscount = 0,inoline = 0;
	
	  for (int i = 0; i < iDatHeight; i++)
	  {
		  inscount = 0;
          for (int j = 0; j < iDatWidth; j++)
		  {
          if (m_cPixelId_p[i][j] == 10  )
          inscount  += 1;
		  }
      //#### 噪声线判断 噪声线像元个数阈值参数 NoisePixNo （初值30）
		if (inscount > NoisePixNo )
	   {
	     
      
		   for (int j = 0; j < iDatWidth; j++)
		   {
		    if (  m_cPixelId_p[i][j] == 10 )
		   {
		   	m_cPixelId_p[i][j] = 100;
			  m_TempBuff2_p[i][j] = 0;
		   }
		}    
	   }	   
	   for (int j = 0; j < iDatWidth; j++)
		{
		//#### 噪声像元判断  中红外亮温下限 NoisePixT4 (初值4700，即470K)
		  if (  m_cPixelId_p[i][j] == 10  && m_wSatDataBuff_p[4][i * iDatWidth + j] >= NoisePixT4 )
		  {
	   	 m_cPixelId_p[i][j] = 100;
		   m_TempBuff2_p[i][j] = 0;
		  }

	   }	   
	  }

  	 // 噪声线判识  20170830   下	


     // 判识火点结束
	
     return;

}


int ProcessFire::FireProSavePixelIdHDF(void)
{
	hid_t dataset_id;
	hid_t file_id,filespace_id;
	hid_t loc_id;
	hsize_t hdims[2];

	hdims[0] = iDatHeight;
	hdims[1] = iDatWidth;
	
	//printf("**********wid:%d  hei:%d\n",iDatWidth,iDatHeight);
	unsigned char* PixedTemp = new unsigned char[iDatHeight*iDatWidth];
	for(int i = 0 ; i < iDatHeight ; i++)
	{
		for(int j = 0 ; j < iDatWidth ; j++)
		{
			PixedTemp[i*iDatWidth+j] = m_cPixelId_p[i][j];
		}
	}
	file_id  = H5Fcreate(strOutputFileName_HDF, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	if(file_id < 0)		
	{
		ZLDSWriteLog('W',"Open HDF Error!");
		return ZEHDFOPEN;
	}
	filespace_id=H5Screate_simple(2,hdims,NULL);
	dataset_id=H5Dcreate(file_id,"FHS",H5T_NATIVE_UCHAR,filespace_id,H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
	H5Dwrite(dataset_id, H5T_NATIVE_UCHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT, PixedTemp);
	H5Dclose(dataset_id);
	H5Fclose(file_id);
	delete[] PixedTemp;
	PixedTemp = NULL;
	return 0;
}
int ProcessFire::FireProSavePixelIdNC_FY4A()
{
	short* PixedTemp = new short[iDatHeight*iDatWidth];
	//printf("NC*****wid:%d  hei:%d\n",iDatWidth,iDatHeight);
	for(int i = 0 ; i < iDatHeight ; i++)
	{
		for(int j = 0 ; j < iDatWidth ; j++)
		{

			PixedTemp[i*iDatWidth+j] = (short)m_cPixelId_p[i][j];
		}
	}
	
	printf("write NC \n");
	struct tm	structSysTm;
	time_t t;
	char strtime[15];		
	memset(strtime,0,sizeof(strtime));
	t=time(NULL);
	memcpy(&structSysTm,gmtime(&t),sizeof(struct tm));	
	sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02dZ",structSysTm.tm_year+1900,structSysTm.tm_mon+1,structSysTm.tm_mday,
																						 structSysTm.tm_hour,structSysTm.tm_min,structSysTm.tm_sec);
	GOESRNetCDFOperator myNetCDF;
	FY4_GOESR_L1B_FILE_INFO global_info;
	memset(&global_info,0,sizeof(FY4_GOESR_L1B_FILE_INFO));
	strcpy(global_info.strDatasetName,"FHS");
	strcpy(global_info.strNamingAuthority,"NSMC CMA");
	strcpy(global_info.strInstitution,"NSMC");	
	strcpy(global_info.strProject,"NOM");
	strcpy(global_info.strConventions,"CF-1.7");
	strcpy(global_info.strMetadataConventions,"Unidata Dataset Discovery v1.0");	
	strcpy(global_info.strSandardNameVocabul,"Fire/Hot Spot Characterization");	
	strcpy(global_info.strTitle,"FY4A AGRI L2 FHS");
	strcpy(global_info.strSummary,"none");
	strcpy(global_info.strPlatformID,"FY4A");
	strcpy(global_info.strInstrumentType,"FY4A Advanced Geosynchronous Radiation Imager");
	strcpy(global_info.strInstrumentID,"AGRI");
	strcpy(global_info.strProcessingLevel,"L2");
	strcpy(global_info.strDateCreated,strtime);
	strcpy(global_info.strProductionSite,"NSMC");		
	strcpy(global_info.strProductionEnvironment,"UNIX");
	strcpy(global_info.strVOS,"V1.0");
	strcpy(global_info.strSRD,"2017-09-12");
	strcpy(global_info.strSceneId,"Full Disk possible values are Full Disk,Southern HEMisphere,the Northern HEMisphere,Regional,China Regional");
	strcpy(global_info.strSpatialResolution,"2km at nadir.");
	sprintf(global_info.strTimeCoverageStart,"%sT%sZ",L1FileInfo_p->strDateOfObservingBeginning,L1FileInfo_p->strTimeOfObservingBeginning);
	sprintf(global_info.strTimeCoverageEnd,"%sT%sZ",L1FileInfo_p->strDateOfObservingEnding,L1FileInfo_p->strTimeOfObservingEnding);
	//global_info.strDataQua=L1FileInfo_p->cDataQuality;
   global_info.strDataQua=DataQ;
	strcpy(global_info.nL0QualityFlag,strL0Q_p);
	strcpy(global_info.nPosQualityFlag,strPOSQ_p);
	strcpy(global_info.nCalQualityFlag,strCALQ_p);
	int status=-1;	

	float wX[iDatWidth];
	float wY[iDatHeight];
	int bandOfwX = iDatWidth;
	int bandOfwY = iDatHeight;
	memset(wX, 0, bandOfwX * sizeof(float));
	memset(wY, 0, bandOfwY * sizeof(float));
	for (int i = 0; i < bandOfwX; i++)
	{
		wX[i] = 1. * i;
	}	
	for (int i = 0; i < bandOfwY; i++)
	{
		wY[i] = 1. * i;
	}		
	
  status =	myNetCDF.ZNetCDFWriteSDSXYData(strOutputFileName_NC, "y", bandOfwX, wX, "FY4A fixed grid projection Y-coordinate", 1);
  status =	myNetCDF.ZNetCDFWriteSDSXYData(strOutputFileName_NC, "x", bandOfwY, wY, "FY4A fixed grid projection X-coordinate", 0);

	short iValidRange[2];
	iValidRange[0] = 0;
	iValidRange[1] = 250;
	short FillValue = 0;
	float ScaleFactor=1.0,AddOffset=0;
	int dims[2] = {iDatWidth,iDatHeight};

	if(myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_NC, "FHS", 2, dims, PixedTemp, NC_SHORT,
				"FY4A PGS L2 Fire/Hot Spot Characterization", "Fire/Hot Spot Characterization","True", &FillValue, iValidRange,&ScaleFactor,&AddOffset, "none", 
				"2000M", "x y","DQF",
				"150:desert,153:water,200:cloud01,205:cloud02,210:cloud03,215:cloud04,220:cloud05,65535:space,40:fillvalue,100:land,10:fire point,50:satallite zenithangle>80,60:flare angle<30,126:BT3.9um<200K,127:BT10.8um<200K",
				0) < 0 )
	{
		printf("write NC FHS Data error!\n");
		ZLDSWriteLog('W',"write NC FHS Data error!");
		return ZESDSOFNCWRITE;
	}				
	printf("start write NC QC !\n");
	char *qc = new char[iDatWidth*iDatHeight];
	memset(qc,0,iDatWidth*iDatHeight);
	char cFillValue = 127;
	char cValidRange[2];
	cValidRange[0] = 0;
	cValidRange[1] = 1;
	char flag_values[2];
  flag_values[0] = 0;
  flag_values[1] = 1;	
  char number_of_qf_values = 2;
  	if(myNetCDF.ZNetCDFWriteDQFSDSData(strOutputFileName_NC, "DQF", 2, dims, qc,	"FHS_Quality_Flag","status_flag", "TRUE", &cFillValue, &cValidRange, "NULL", 
		"y x", flag_values,2,"good_pixel conditionally_usable_pixel out_of_range_pixel no_value_pixel", &number_of_qf_values,0, 0)< 0 )
	{
		printf("write NC DQF error!\n");
		ZLDSWriteLog('W',"Write DQF Of NC Error！\n");
		return ZESDSOFNCWRITE;
	}		
	int iData =0;
	float fData = 0.0f;
	int nTypeVal[4];
	nTypeVal[0] = 0;
	nTypeVal[1] = 1;
	nTypeVal[2] = 2;
	nTypeVal[3] = 3;	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lat", &L1FileInfo_p->fNOMCenterLat,
	 "nominal satellite subpoint latitude (platform latitude)", "Latitude", "degrees_north", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lon", &L1FileInfo_p->fNOMCenterLon, 
	"nominal satellite subpoint longitude (platformlongitude)", "Longitude", "degrees_east", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_height", &L1FileInfo_p->fNOMSatHeight,
	 "nominal satellite height above GRS 80 ellipsoid(platform altitude)",
	  "height_above_reference_ellipsoid", "km", 0);

	status =	myNetCDF.ZNetCDFWriteGeoExtentSDSData(strOutputFileName_NC,"geospatial_lat_lon_extent", &fData,
	  "geospatial latitude and longitude references", 
	  &(L1FileInfo_p->wBeginLineNumber), &(L1FileInfo_p->wEndLineNumber),	
	  &(L1FileInfo_p->wBeginPixelNumber), &(L1FileInfo_p->wEndPixelNumber),
	  &(L1FileInfo_p->fNOMCenterLat),&(L1FileInfo_p->fNOMCenterLon), 
	  &(L1FileInfo_p->fRegLength), &(L1FileInfo_p->fRegWidth),
	   "degrees_north", "degrees_east", 0);//

  if(strstr(strAreaRange,"DISK")!=0)
  	iData=0;
  if(strstr(strAreaRange,"NHEM")!=0)
  	iData=2;
  if(strstr(strAreaRange,"SHEM")!=0)
  	iData=1;
  if(strstr(strAreaRange,"REG")!=0)
  	iData=3;

  status =	myNetCDF.ZNetCDFWriteOBISDSData(strOutputFileName_NC, "OBIType", &iData, 0, "Observing Type", 
  "OBIType",nTypeVal,"0:Full_disk_observation 1:Southern_hemisphere_observation 2:Northern _hemisphere_observation 3:Regional observation");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "processing_parm_version_container", &iData, 0,
  "container for processing parameter package filename and product version","2016-10-10",0,"2016-10-10");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "algorithm_product_version_container", &iData, 0,
  "container for algorithm package filename and product version", "2016-10-10",0, "2016-10-10");	
 //20180426********
// ffi_[i].wFireAreaNo, ffi_[i].fCenterLat, ffi_[i].fCenterLon, 
//               ffi_[i].wFireAreaFires,ffi_[i].fNPixelS, ffi_[i].fFireAreaS * 100.0 ,ffi_[i].fFireCaoChang, ffi_[i].fFireLinDi,
//               ffi_[i].fFireQiTa,ffi_[i].sCloudsId.c_str(),ffi_[i].sShengMing.c_str()
 
   
//  char *sTest[iFqNum][3];
//  int nDimCount[2];
//  nDimCount[0] = iFqNum;
//  nDimCount[1] = 2;
//  float fScale = 1, fOffset = 1;  
//  for(int i = 0; i< 3; i++)
//  {	
//    for(int j = 0;j < iFqNum; j++)
//   {
//     sTest[j][i] =(char*)malloc(256 * sizeof(char));
//  	memset(sTest[j][i],0,sizeof(char)*256);
//  	sprintf(sTest[j][0],"%d",ffi_[j].wFireAreaNo);
//  	sprintf(sTest[j][1],"%f",ffi_[j].fCenterLat);
//    sprintf(sTest[j][2],"%f",ffi_[j].fCenterLon);     
//     
//     printf("sTest %s\n",sTest[j][0]);
//     printf("sTest %s\n",sTest[j][1]);
//    
//   }  
//  }

 
    printf("-----------start write NC string sds------\n");
//	status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_NC, "FPA", 2, nDimCount, sTest, NC_STRING, 
//	                 "NULL", "NULL", "NULL", "NULL", "NULL", &fScale, &fOffset, "NULL", "NULL", "y x", "NULL", "NULL", 0,&sTest);
//	
	 
	/**********************************************************************************************************/
char FPALongName[256];
char FPAStandardName[256];
char FPADescription[256];
char strResolution[256];
char FPTLongName[256];
char FPTStandardName[256];
char FPTDescription[256];

memset(FPALongName,0,sizeof(char)*256);
memset(FPAStandardName,0,sizeof(char)*256);
memset(FPADescription,0,sizeof(char)*256);
memset(strResolution,0,sizeof(char)*256);
memset(FPTLongName,0,sizeof(char)*256);
memset(FPTStandardName,0,sizeof(char)*256);
memset(FPTDescription,0,sizeof(char)*256);

strcpy(FPALongName,"FY4A AGRI L2 Hot Spot Information List");
strcpy(FPAStandardName,"FY4A AGRI L2 Hot Spot Information List");
strcpy(FPADescription,"Cloud_edge: 0, Not At the edge of the cloud ; 1,  At the edge of the cloud; Administrative_area: -9999,Outside of China");
strcpy(strResolution,"2000M");
strcpy(FPTLongName,"FY4A AGRI L2 Hot Spot Pixel Information List");
strcpy(FPTStandardName,"FY4A AGRI L2 Hot Spot Pixel Information List");
strcpy(FPTDescription," Administrative_area: -9999,Outside of China");
	
	status = WriteStringToFHS(strOutputEnFPAFile,"FPA" ,strOutputFileName_NC,myNetCDF, FPALongName, FPADescription, strResolution, FPAStandardName);
  status = WriteStringToFHS(strOutputEnFPTFile,"FPT",strOutputFileName_NC,myNetCDF, FPTLongName, FPTDescription, strResolution, FPTStandardName);
	
		//火点信息FPA_txt
//	vector<string> strTmpFileName;
//	FILE *fp1;
//	char buf[256];
//	int index = 0;
//	float fScale = 1, fOffset = 1;
//	
//	memset(buf,0,sizeof(buf));
//	fp1 = fopen(FPATempFileName,"r");
//	printf("FPATempFileName: %s\n",FPATempFileName);
//	if(fp1 == NULL)
//		{
//			printf("%s file is not open!\n",FPATempFileName);
//		}
//		while(!feof(fp1))
//		{
//			fgets(buf,sizeof(buf),fp1);
//			if(strlen(buf) > 20)
//				{
//					strTmpFileName.push_back(buf);
//				}
//		}	
//	  index = strTmpFileName.size();
//		
//    printf("----20180426---index %d\n",index);
//    char *sTest[index];		
//    int nDimCount =index;
//    char *sDimName[1];
//    for (int nIndex = 0; nIndex < 1; ++nIndex)
//    {
//    	sDimName[nIndex] = new char[256];
//			memset(sDimName[nIndex],0,256);
//      strcpy(sDimName[nIndex], "qq");
//    }
//		for(int i = 0; i<index; i++)
//		{
//			sTest[i] = new char[256];
//			memset(sTest[i],0,256);
//			//printf("%s\n",strTmpFileName[i].c_str());
//      strcpy(sTest[i], strTmpFileName[i].c_str());
//      printf("-----sTest[i] %s\n",sTest[i]);
//		} 
//  printf("-----------start write NC string sds------\n");
//	status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_NC, "FPA", 1, &nDimCount, sTest, NC_STRING, "NULL", "NULL", "NULL", "NULL", "NULL", &fScale, &fOffset, "NULL", "NULL", "y x", "NULL", "NULL", 0,sDimName);
//
//	printf("-----------END write NC string sds------\n");	
//	fclose(fp1);
//	for(int ii = 0; ii <index; ii++)
//	{
//		delete[] sTest[ii];
//	}	
//	strTmpFileName.clear();
	/**********************************************************************************************************/
	
	
	//写全局属性
	if(myNetCDF.ZNetCDFWriteFY4GOESRL1BFileInfo(strOutputFileName_NC,&global_info,0) < 0 )
	{
		printf("write global attribute error!\n");
		ZLDSWriteLog('W',"write global attribute error!");
		return ZEATTOFNCWRITE;
	}

	delete[] PixedTemp;
	PixedTemp = NULL;
	return 0;
}
int ProcessFire::FireProSavePixelIdNC_AHI8(void)
{
	short* PixedTemp = new short[iDatHeight*iDatWidth];
	printf("NC*****wid:%d  hei:%d\n",iDatWidth,iDatHeight);
	for(int i = 0 ; i < iDatHeight ; i++)
	{
		for(int j = 0 ; j < iDatWidth ; j++)
		{
			PixedTemp[i*iDatWidth+j] = (short)m_cPixelId_p[i][j];
		}
	}
	
	printf("write NC \n");
	struct tm	structSysTm;
	time_t t;
	char strtime[15];		
	memset(strtime,0,sizeof(strtime));
	t=time(NULL);
	memcpy(&structSysTm,gmtime(&t),sizeof(struct tm));	
	sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02dZ",structSysTm.tm_year+1900,structSysTm.tm_mon+1,structSysTm.tm_mday,
																						 structSysTm.tm_hour,structSysTm.tm_min,structSysTm.tm_sec);
	GOESRNetCDFOperator myNetCDF;
	FY4_GOESR_L1B_FILE_INFO global_info;
	memset(&global_info,0,sizeof(FY4_GOESR_L1B_FILE_INFO));
	strcpy(global_info.strDatasetName,"FHS");
	strcpy(global_info.strNamingAuthority,"NSMC CMA");
	strcpy(global_info.strInstitution,"NSMC");	
	strcpy(global_info.strProject,"NOM");
	strcpy(global_info.strConventions,"CF-1.7");
	strcpy(global_info.strMetadataConventions,"Unidata Dataset Discovery v1.0");	
	strcpy(global_info.strSandardNameVocabul,"Fire/Hot Spot Characterization");	
	strcpy(global_info.strTitle,"AHI8 AGRI L2 FHS");
	strcpy(global_info.strSummary,"none");
	strcpy(global_info.strPlatformID,"H8");
	strcpy(global_info.strInstrumentType,"AHI8 Advanced Geosynchronous Radiation Imager");
	strcpy(global_info.strInstrumentID,"AHI");
	strcpy(global_info.strProcessingLevel,"L2");
	strcpy(global_info.strDateCreated,strtime);
	strcpy(global_info.strProductionSite,"NSMC");		
	strcpy(global_info.strProductionEnvironment,"UNIX");
	strcpy(global_info.strVOS,"V1.0");
	strcpy(global_info.strSRD,"2017-09-12");
	strcpy(global_info.strSceneId,"Full Disk possible values are Full Disk,Southern HEMisphere,the Northern HEMisphere,Regional,China Regional");
	strcpy(global_info.strSpatialResolution,"2km at nadir.");
	sprintf(global_info.strTimeCoverageStart,"%sT%sZ",L1FileInfo_p->strDateOfObservingBeginning,L1FileInfo_p->strTimeOfObservingBeginning);
	sprintf(global_info.strTimeCoverageEnd,"%sT%sZ",L1FileInfo_p->strDateOfObservingEnding,L1FileInfo_p->strTimeOfObservingEnding);
	//global_info.strDataQua=L1FileInfo_p->cDataQuality;
global_info.strDataQua=DataQ;
	strcpy(global_info.nL0QualityFlag,strL0Q_p);
	strcpy(global_info.nPosQualityFlag,strPOSQ_p);
	strcpy(global_info.nCalQualityFlag,strCALQ_p);
	int status=-1;	

	float wX[iDatWidth];
	float wY[iDatHeight];
	int bandOfwX = iDatWidth;
	int bandOfwY = iDatHeight;
	memset(wX, 0, bandOfwX * sizeof(float));
	memset(wY, 0, bandOfwY * sizeof(float));
	for (int i = 0; i < bandOfwX; i++)
	{
		wX[i] = 1. * i;
	}	
	for (int i = 0; i < bandOfwY; i++)
	{
		wY[i] = 1. * i;
	}		
	
  status =	myNetCDF.ZNetCDFWriteSDSXYData(strOutputFileName_NC, "y", bandOfwX, wX, "AHI8 fixed grid projection Y-coordinate", 1);
  status =	myNetCDF.ZNetCDFWriteSDSXYData(strOutputFileName_NC, "x", bandOfwY, wY, "AHI8 fixed grid projection X-coordinate", 0);

	short iValidRange[2];
	iValidRange[0] = 0;
	iValidRange[1] = 250;
	short FillValue = 0;
	float ScaleFactor=1.0,AddOffset=0;
	int dims[2] = {iDatWidth,iDatHeight};

	if(myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_NC, "FHS", 2, dims, PixedTemp, NC_SHORT,
				"AHI8 PGS L2+ Fire/Hot Spot Characterization", "Fire/Hot Spot Characterization","True", &FillValue, iValidRange,&ScaleFactor,&AddOffset, "none", 
				"2000M", "x y","DQF",
				"150:desert,153:water,200:cloud01,205:cloud02,210:cloud03,215:cloud04,220:cloud05,65535:space,40:fillvalue,100:land,10:fire point,50:satallite zenithangle>80,60:flare angle<30,126:BT3.9um<200K,127:BT10.8um<200K",
				0) < 0 )
	{
		printf("write NC FHS Data error!\n");
		ZLDSWriteLog('W',"write NC FHS Data error!");
		return ZESDSOFNCWRITE;
	}				
	printf("start write NC QC !\n");
	char *qc = new char[iDatWidth*iDatHeight];
	memset(qc,0,iDatWidth*iDatHeight);
	char cFillValue = 127;
	char cValidRange[2];
	cValidRange[0] = 0;
	cValidRange[1] = 1;
	char flag_values[2];
  flag_values[0] = 0;
  flag_values[1] = 1;	
  char number_of_qf_values = 2;
  	if(myNetCDF.ZNetCDFWriteDQFSDSData(strOutputFileName_NC, "DQF", 2, dims, qc,	"FHS_Quality_Flag","status_flag", "TRUE", &cFillValue, &cValidRange, "NULL", 
		"y x", flag_values,2,"good_pixel conditionally_usable_pixel out_of_range_pixel no_value_pixel", &number_of_qf_values,0, 0)< 0 )
	{
		printf("write NC DQF error!\n");
		ZLDSWriteLog('W',"Write DQF Of NC Error！\n");
		return ZESDSOFNCWRITE;
	}		
	int iData =0;
	float fData = 0.0f;
	int nTypeVal[4];
	nTypeVal[0] = 0;
	nTypeVal[1] = 1;
	nTypeVal[2] = 2;
	nTypeVal[3] = 3;	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lat", &L1FileInfo_p->fNOMCenterLat,
	 "nominal satellite subpoint latitude (platform latitude)", "Latitude", "degrees_north", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lon", &L1FileInfo_p->fNOMCenterLon, 
	"nominal satellite subpoint longitude (platformlongitude)", "Longitude", "degrees_east", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_height", &L1FileInfo_p->fNOMSatHeight,
	 "nominal satellite height above GRS 80 ellipsoid(platform altitude)",
	  "height_above_reference_ellipsoid", "km", 0);

	status =	myNetCDF.ZNetCDFWriteGeoExtentSDSData(strOutputFileName_NC,"geospatial_lat_lon_extent", &fData,
	  "geospatial latitude and longitude references", 
	  &(L1FileInfo_p->wBeginLineNumber), &(L1FileInfo_p->wEndLineNumber),	
	  &(L1FileInfo_p->wBeginPixelNumber), &(L1FileInfo_p->wEndPixelNumber),
	  &(L1FileInfo_p->fNOMCenterLat),&(L1FileInfo_p->fNOMCenterLon), 
	  &(L1FileInfo_p->fRegLength), &(L1FileInfo_p->fRegWidth),
	   "degrees_north", "degrees_east", 0);//

  if(strstr(strAreaRange,"DISK")!=0)
  	iData=0;
  if(strstr(strAreaRange,"NHEM")!=0)
  	iData=2;
  if(strstr(strAreaRange,"SHEM")!=0)
  	iData=1;
  if(strstr(strAreaRange,"REG")!=0)
  	iData=3;

  status =	myNetCDF.ZNetCDFWriteOBISDSData(strOutputFileName_NC, "OBIType", &iData, 0, "Observing Type", 
  "OBIType",nTypeVal,"0:Full_disk_observation 1:Southern_hemisphere_observation 2:Northern _hemisphere_observation 3:Regional observation");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "processing_parm_version_container", &iData, 0,
  "container for processing parameter package filename and product version","2016-10-10",0,"2016-10-10");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "algorithm_product_version_container", &iData, 0,
  "container for algorithm package filename and product version", "2016-10-10",0, "2016-10-10");	



	//写全局属性
	if(myNetCDF.ZNetCDFWriteFY4GOESRL1BFileInfo(strOutputFileName_NC,&global_info,0) < 0 )
	{
		printf("write global attribute error!\n");
		ZLDSWriteLog('W',"write global attribute error!");
		return ZEATTOFNCWRITE;
	}

	delete[] PixedTemp;
	PixedTemp = NULL;
	return 0;
}
//-----------------------------------------------------------------------------
///火点标记图
int ProcessFire::FireProSaveFRTOfPixe()
{
		FILE *out_File_RGB = NULL;
		struct jpeg_compress_struct jcs;
		struct jpeg_error_mgr jem;
		jcs.err = jpeg_std_error(&jem);
		jpeg_create_compress(&jcs);
    if ((out_File_RGB = fopen(strOutputFileName_FIMG, "wb+")) == NULL) 
    {
        fprintf(stderr,"Cannot open output file jpeg.%s\n", strOutputFileName_FIMG);
        ZLDSWriteLog('W',"open fire flag .jpeg error");
        return ZEOPENFLAGOFJPEG;
    }

    jpeg_stdio_dest(&jcs, out_File_RGB);
    jcs.image_width =  iDatWidth; // 图片的宽
    jcs.image_height = iDatHeight; // 图片的高
    jcs.input_components=3; //1为灰度图，3为RGB图
    jcs.in_color_space = JCS_RGB;//JCS_GRAYSCALE表示灰度图，JCS_RGB表示彩色图
    jpeg_set_defaults(&jcs);
    jpeg_set_quality(&jcs,100,TRUE);
    jpeg_start_compress(&jcs,TRUE);
    JSAMPROW row_pointer[1];        
    const int rgbBand = 3;
    unsigned char* out_buf_RGB = new unsigned char[iDatWidth*3];
    //unsigned char* out_buf_RGB = new unsigned char[iDatHeight*iDatWidth*3];
    for (int row = 0; row < iDatHeight; row++) 
    { 
    		memset(out_buf_RGB,0,sizeof(unsigned char)*iDatWidth*3);
        for (int idx = 0; idx < iDatWidth; idx++)
        {
            if(m_cPixelId_p[row][idx] == 100 )
            {
                out_buf_RGB[idx * rgbBand + 2] = 25;
                out_buf_RGB[idx * rgbBand + 1] = 255;
              	out_buf_RGB[idx * rgbBand]		 = 25;
              	continue;
            }
            if(m_cPixelId_p[row][idx] == 40)
            {
                out_buf_RGB[idx * rgbBand + 2] = 10;
                out_buf_RGB[idx * rgbBand + 1] = 10;
              	out_buf_RGB[idx * rgbBand]		 = 10;		
              	continue;					
            }
            if(m_cPixelId_p[row][idx] == 50)
            {
                out_buf_RGB[idx * rgbBand + 2] = 50;
                out_buf_RGB[idx * rgbBand + 1] = 50;
              	out_buf_RGB[idx * rgbBand]		 = 50;
              	continue;							
            }  
            if(m_cPixelId_p[row][idx] == 150)
            {
                out_buf_RGB[idx * rgbBand + 2] = 50;
                out_buf_RGB[idx * rgbBand + 1] = 150;
              	out_buf_RGB[idx * rgbBand]		 = 150;		
              	continue;					
            } 
            if(m_cPixelId_p[row][idx] == 153)
            {
                out_buf_RGB[idx * rgbBand + 2] = 250;
                out_buf_RGB[idx * rgbBand + 1] = 10;
              	out_buf_RGB[idx * rgbBand]		 = 10;
              	continue;							
            } 
            if(m_cPixelId_p[row][idx] == 60)
            {
                out_buf_RGB[idx * rgbBand + 2] = 160;
                out_buf_RGB[idx * rgbBand + 1] = 160;
              	out_buf_RGB[idx * rgbBand]		 = 160;		
              	continue;					
            }   
            if(m_cPixelId_p[row][idx] == 126)
            {
                out_buf_RGB[idx * rgbBand + 2] = 60;
                out_buf_RGB[idx * rgbBand + 1] = 60;
              	out_buf_RGB[idx * rgbBand]		 = 200;				
              	continue;			
            } 
            if(m_cPixelId_p[row][idx] == 127)
            {
                out_buf_RGB[idx * rgbBand + 2] = 160;
                out_buf_RGB[idx * rgbBand + 1] = 160;
              	out_buf_RGB[idx * rgbBand]		 = 10;			
              	continue;				
            }   
            if(m_cPixelId_p[row][idx] == 200)
            {
                out_buf_RGB[idx * rgbBand + 2] = 200;
                out_buf_RGB[idx * rgbBand + 1] = 200;
              	out_buf_RGB[idx * rgbBand]		 = 200;	
              	continue;						
            } 
            if(m_cPixelId_p[row][idx] == 205)
            {
                out_buf_RGB[idx * rgbBand + 2] = 205;
                out_buf_RGB[idx * rgbBand + 1] = 205;
              	out_buf_RGB[idx * rgbBand]		 = 205;		
              	continue;					
            }  
            if(m_cPixelId_p[row][idx] == 210)
            {
                out_buf_RGB[idx * rgbBand + 2] = 210;
                out_buf_RGB[idx * rgbBand + 1] = 210;
              	out_buf_RGB[idx * rgbBand]		 = 210;	
              	continue;						
            }           
            if(m_cPixelId_p[row][idx] == 215)
            {
                out_buf_RGB[idx * rgbBand + 2] = 215;
                out_buf_RGB[idx * rgbBand + 1] = 215;
              	out_buf_RGB[idx * rgbBand]		 = 215;		
              	continue;					
            } 
            if(m_cPixelId_p[row][idx] == 220)
            {
                out_buf_RGB[idx * rgbBand + 2] = 220;
                out_buf_RGB[idx * rgbBand + 1] = 220;
              	out_buf_RGB[idx * rgbBand]		 = 220;					
              	continue;		
            }
            if(m_cPixelId_p[row][idx] == 250)
            {
                out_buf_RGB[idx * rgbBand + 2] = 250;
                out_buf_RGB[idx * rgbBand + 1] = 250;
              	out_buf_RGB[idx * rgbBand]		 = 250;				
              	continue;			
            }                
            if(m_cPixelId_p[row][idx] == 10)
            {
                out_buf_RGB[idx * rgbBand + 2] = 0;
                out_buf_RGB[idx * rgbBand + 1] = 0;
              	out_buf_RGB[idx * rgbBand]		 = 255;			
              	continue;				
            }                                                                                                                            
        }
        // write RGB file:
        row_pointer[0] = out_buf_RGB;
        jpeg_write_scanlines(&jcs, row_pointer, 1);        
    } 
    
    jpeg_finish_compress(&jcs);
    fclose(out_File_RGB); 
    if(out_buf_RGB != NULL)
    {
	    delete []out_buf_RGB;
	    out_buf_RGB = NULL;
  	}
	return 0;
}
///火点强度图
int ProcessFire::FireProSaveFRPOfPixe(void)
{
		FILE *out_File_RGB = NULL;
		struct jpeg_compress_struct jcs;
		struct jpeg_error_mgr jem;
		jcs.err = jpeg_std_error(&jem);
		jpeg_create_compress(&jcs);
    if ((out_File_RGB = fopen(strOutputFileName_PIMG, "wb+")) == NULL) 
    {
        fprintf(stderr,"Cannot open output file jpeg.%s\n", strOutputFileName_PIMG);
        ZLDSWriteLog('W',"open fire Power .jpeg error");
        return ZEOPENPOWEROFJPEG;
    }

    jpeg_stdio_dest(&jcs, out_File_RGB);
    jcs.image_width =  iDatWidth; // 图片的宽
    jcs.image_height = iDatHeight; // 图片的高
    jcs.input_components=3; //1为灰度图，3为RGB图
    jcs.in_color_space = JCS_RGB;//JCS_GRAYSCALE表示灰度图，JCS_RGB表示彩色图
    jpeg_set_defaults(&jcs);
    jpeg_set_quality(&jcs,100,TRUE);
    jpeg_start_compress(&jcs,TRUE);
    JSAMPROW row_pointer[1];        
    const int rgbBand = 3;
    //unsigned char* out_buf_RGB = new unsigned char[iDatHeight*iDatWidth*3];
    unsigned char* out_buf_RGB = new unsigned char[iDatWidth*3];
    for (int row = 0; row < iDatHeight; row++) 
    { 
    		memset(out_buf_RGB,0,sizeof(unsigned char)*iDatWidth*3);
        for (int idx = 0; idx < iDatWidth; idx++)
        {
            if(m_cPixelId_p[row][idx] == 100 )
            {
                out_buf_RGB[idx * rgbBand + 2] = 25;
                out_buf_RGB[idx * rgbBand + 1] = 255;
              	out_buf_RGB[idx * rgbBand]		 = 25;
            }
            else if(m_cPixelId_p[row][idx] == 40)
            {
                out_buf_RGB[idx * rgbBand + 2] = 10;
                out_buf_RGB[idx * rgbBand + 1] = 10;
              	out_buf_RGB[idx * rgbBand]		 = 10;							
            }
            else if(m_cPixelId_p[row][idx] == 50)
            {
                out_buf_RGB[idx * rgbBand + 2] = 50;
                out_buf_RGB[idx * rgbBand + 1] = 50;
              	out_buf_RGB[idx * rgbBand]		 = 50;							
            }  
            else if(m_cPixelId_p[row][idx] == 150)
            {
                out_buf_RGB[idx * rgbBand + 2] = 50;
                out_buf_RGB[idx * rgbBand + 1] = 150;
              	out_buf_RGB[idx * rgbBand]		 = 150;							
            } 
            else if(m_cPixelId_p[row][idx] == 153)
            {
                out_buf_RGB[idx * rgbBand + 2] = 250;
                out_buf_RGB[idx * rgbBand + 1] = 10;
              	out_buf_RGB[idx * rgbBand]		 = 10;							
            } 
            else if(m_cPixelId_p[row][idx] == 60)
            {
                out_buf_RGB[idx * rgbBand + 2] = 160;
                out_buf_RGB[idx * rgbBand + 1] = 160;
              	out_buf_RGB[idx * rgbBand]		 = 160;							
            }   
            else if(m_cPixelId_p[row][idx] == 126)
            {
                out_buf_RGB[idx * rgbBand + 2] = 60;
                out_buf_RGB[idx * rgbBand + 1] = 60;
              	out_buf_RGB[idx * rgbBand]		 = 200;							
            } 
            else if(m_cPixelId_p[row][idx] == 127)
            {
                out_buf_RGB[idx * rgbBand + 2] = 160;
                out_buf_RGB[idx * rgbBand + 1] = 160;
              	out_buf_RGB[idx * rgbBand]		 = 10;							
            }   
            else if(m_cPixelId_p[row][idx] == 200)
            {
                out_buf_RGB[idx * rgbBand + 2] = 200;
                out_buf_RGB[idx * rgbBand + 1] = 200;
              	out_buf_RGB[idx * rgbBand]		 = 200;							
            } 
            else if(m_cPixelId_p[row][idx] == 205)
            {
                out_buf_RGB[idx * rgbBand + 2] = 205;
                out_buf_RGB[idx * rgbBand + 1] = 205;
              	out_buf_RGB[idx * rgbBand]		 = 205;							
            }  
            else if(m_cPixelId_p[row][idx] == 210)
            {
                out_buf_RGB[idx * rgbBand + 2] = 210;
                out_buf_RGB[idx * rgbBand + 1] = 210;
              	out_buf_RGB[idx * rgbBand]		 = 210;							
            }           
            else if(m_cPixelId_p[row][idx] == 215)
            {
                out_buf_RGB[idx * rgbBand + 2] = 215;
                out_buf_RGB[idx * rgbBand + 1] = 215;
              	out_buf_RGB[idx * rgbBand]		 = 215;							
            } 
            else if(m_cPixelId_p[row][idx] == 220)
            {
                out_buf_RGB[idx * rgbBand + 2] = 220;
                out_buf_RGB[idx * rgbBand + 1] = 220;
              	out_buf_RGB[idx * rgbBand]		 = 220;							
            } 
            else if(m_cPixelId_p[row][idx] == 250)
            {
                out_buf_RGB[idx * rgbBand + 2] = 250;
                out_buf_RGB[idx * rgbBand + 1] = 250;
              	out_buf_RGB[idx * rgbBand]		 = 250;							
            }            
            else if(m_cPixelId_p[row][idx] == 10)
            {
                out_buf_RGB[idx * rgbBand + 2] = 0;
                out_buf_RGB[idx * rgbBand + 1] = 0;
              	out_buf_RGB[idx * rgbBand]		 = 250;			
              	
					     for (int i = 1; i <= fi.wFires; i++)
					     {
					     		if(row == fpi_[i].wY && idx == fpi_[i].wX)
				     			{
				     				float fFireS = fpi_[i].fFireS * fpi_[i].fFireP * 1000000;									
										//按亚像元面积分火点等级									
							       if (fFireS < 5000)
							       {
							          //配色  分别为蓝绿红的值									
							          out_buf_RGB[idx * rgbBand] = 0;            //兰
							          out_buf_RGB[idx * rgbBand + 1] = 255;      //绿
							          out_buf_RGB[idx *rgbBand + 2] = 0;   	//红
							       }
							       if (fFireS >= 5000 && fFireS < 10000)
							       {
							          out_buf_RGB[idx * rgbBand] = 255;
							          out_buf_RGB[idx * rgbBand + 1] = 0;
							          out_buf_RGB[idx * rgbBand + 2] = 0;
							       }
							       if (fFireS >= 10000 && fFireS < 20000)
							       {
							          out_buf_RGB[idx * rgbBand] = 180;
							          out_buf_RGB[idx * rgbBand + 1] = 0;
							          out_buf_RGB[idx * rgbBand + 2] = 0;
							       }
							       if (fFireS >= 20000 && fFireS < 40000)
							       {
							          out_buf_RGB[idx * rgbBand] = 255;
							          out_buf_RGB[idx * rgbBand + 1] = 150;
							          out_buf_RGB[idx * rgbBand + 2] = 0;
							       }
							       if (fFireS >= 40000 && fFireS < 70000)
							       {
							          out_buf_RGB[idx * rgbBand] = 0;
							          out_buf_RGB[idx * rgbBand + 1] = 255;
							          out_buf_RGB[idx * rgbBand + 2] = 255;
							       }
							       if (fFireS >= 70000 && fFireS < 120000)
							       {
							          out_buf_RGB[idx * rgbBand] = 180;
							          out_buf_RGB[idx * rgbBand + 1] = 0;
							          out_buf_RGB[idx * rgbBand + 2] = 180;
							       }
							       if (fFireS >= 120000 && fFireS < 150000)
							       {
							          out_buf_RGB[idx * rgbBand] = 255;
							          out_buf_RGB[idx * rgbBand + 1] = 0;
							          out_buf_RGB[idx * rgbBand + 2] = 255;
							       }
							       if (fFireS >= 150000 && fFireS < 200000)
							       {
							          out_buf_RGB[idx * rgbBand] = 0;
							          out_buf_RGB[idx * rgbBand + 1] = 0;
							          out_buf_RGB[idx * rgbBand + 2] = 255;
							       }
							       if (fFireS >= 200000 && fFireS < 300000)
							       {
							          out_buf_RGB[idx * rgbBand] = 0;
							          out_buf_RGB[idx * rgbBand + 1] = 0;
							          out_buf_RGB[idx * rgbBand + 2] = 180;
							       }
							       if (fFireS >= 300000)
							       {
							          out_buf_RGB[idx * rgbBand] = 0;
							          out_buf_RGB[idx * rgbBand + 1] = 0;
							          out_buf_RGB[idx * rgbBand + 2] = 80;
							       }
				     			}									       
					     }             					
            }                                                                                                                            
        }
        // write RGB file:
        row_pointer[0] = out_buf_RGB;
        jpeg_write_scanlines(&jcs, row_pointer, 1);        
    }   
    
    jpeg_finish_compress(&jcs);
    fclose(out_File_RGB); 
    
    if(out_buf_RGB != NULL)
    {
    	delete []out_buf_RGB;
    	out_buf_RGB = NULL;
    }
	return 0;

}

//20180105 新增 RGB合成图
int ProcessFire::FireProSaveFHSOfDAYPixe()
{
		//byte *scl,*scl1, *scl2, *scl3; 
		FirePixInfo_ fireinfo;
    int ich;
    float fTvir, fRvis;
    fTvir = 0.; fRvis = 0.;
    double r = 5;
    double dis = 0.;
    dis = sqrt(r*r);
    double fdis = 0.;
    unsigned char m_FHSPixed[iDatHeight * iDatWidth] ;
    memset(m_FHSPixed,0,sizeof(unsigned char)*iDatHeight * iDatWidth);
		
		FILE *out_File_RGB_day = NULL;
	
		struct jpeg_compress_struct jcs_day;
		struct jpeg_error_mgr jem_day;
		jcs_day.err = jpeg_std_error(&jem_day);
	
		jpeg_create_compress(&jcs_day);
	
    if ((out_File_RGB_day = fopen(strOutputFileName_FDJPG, "wb+")) == NULL) 
    {
        fprintf(stderr,"Cannot open output file jpeg.%s\n", strOutputFileName_FDJPG);
        ZLDSWriteLog('W',"open fire flag .jpeg error");
        return ZEOPENFLAGOFJPEG;
    }
  
    jpeg_stdio_dest(&jcs_day, out_File_RGB_day);
    
    jcs_day.image_width =  iDatWidth; // 图片的宽
    jcs_day.image_height = iDatHeight; // 图片的高
    jcs_day.input_components=3; //1为灰度图，3为RGB图
    jcs_day.in_color_space = JCS_RGB;//JCS_GRAYSCALE表示灰度图，JCS_RGB表示彩色图
    
    
    
    jpeg_set_defaults(&jcs_day);
    jpeg_set_quality(&jcs_day,100,TRUE);
    jpeg_start_compress(&jcs_day,TRUE);
    
    
    
    JSAMPROW row_pointer_day[1];
            
    const int rgbBand = 3;
    unsigned char* out_buf_RGB_day = new unsigned char[iDatWidth*3];
  

    for (int iy = 0; iy < iDatHeight; iy++) 
    { 
    	
        for (int ix = 0; ix < iDatWidth; ix++)
        {

          if(iy == 0 || ix == 0 ||iy == iDatHeight-r || ix == iDatWidth -r)
          {
          		continue;
          }
    	  	if(m_cPixelId_p[iy][ix] == 10)
    	  	{
    	  		for(int im = 1; im <= r; im++)
    	  		{   
    	  			 for(int in = 1;in <= r; in++)
    	  			 {
    	  			 	
    	  			 	 fdis = sqrt(double(im *im)+double(in*in));    	  			 	
    	  			 	// printf("fdis %lf ,dis %lf \n",fdis,dis);
    	  			 	 if(fdis < r)
                {
                m_FHSPixed[(iy-im)*iDatWidth+(ix-in)] = 10;
                m_FHSPixed[(iy-im)*iDatWidth+ix] = 10;
                m_FHSPixed[(iy-im)*iDatWidth+(ix+in)] = 10;
                m_FHSPixed[(iy)*iDatWidth+ix] = 10;
                m_FHSPixed[(iy)*iDatWidth+(ix-in)] = 10;
                m_FHSPixed[(iy)*iDatWidth+(ix+in)] = 10;
                m_FHSPixed[(iy+im)*iDatWidth+(ix-in)] = 10;
                m_FHSPixed[(iy+im)*iDatWidth+ix] = 10;  
                m_FHSPixed[(iy+im)*iDatWidth+(ix+in)] = 10;    	
                }    	  				  			
    	  			
    	  			 }    	  			 
    	  			  			
    	  		}
    	  	}
    	     			         			
        }
                                                                                                                                
    }
 
   for(int iy = 0;iy<iDatHeight; iy++)
    {
        	memset(out_buf_RGB_day,0,sizeof(unsigned char)*iDatWidth*3);
        
        	for(int ix = 0; ix < iDatWidth; ix++)
        	{
        		 
        		 //day
           if(m_wSatDataBuff_p[4][iDatWidth * iy + ix] == 65534 ||m_wSatDataBuff_p[3][iDatWidth * iy + ix] == 65534 ||
           	m_wSatDataBuff_p[5][iDatWidth * iy + ix] == 65534 )
           	{
           		out_buf_RGB_day[ix * rgbBand ]	= 123; 
           		out_buf_RGB_day[ix * rgbBand + 1] = 123;
           		out_buf_RGB_day[ix * rgbBand + 2] = 123;
//           		 if(ix >= 664) 
//                	{
//                		printf("---green-----out_buf_RGB_day %d ix %d iy %d\n", out_buf_RGB_day[ix * rgbBand +1 ],ix,iy);
//                  getchar();
//                	}
           		continue;
           	} 
           	else
           	{
           		 //red
           	  fTvir = (float)m_wSatDataBuff_p[4][iDatWidth * iy + ix] / 10.;
              if (fTvir < 275.0 )
                  fTvir = 275.0;
              if (fTvir > 330.0)
                  fTvir = 330.0;

               ich = (int)((((fTvir - 275.0) * (fTvir - 275.0))  / ((330.0-275.0)*(330.0-275.0))) * 255);
                if (ich > 255) ich = 255;
                if (ich < 5)   ich = 5;   
                out_buf_RGB_day[ix * rgbBand ]	= ich;
//                if(ix >= 664) 
//                	{
//                		printf("----red-----out_buf_RGB_day %d ix %d iy %d\n", out_buf_RGB_day[ix * rgbBand ],ix,iy);
//                  getchar();
//                	}
                
            //green
            fRvis = (float) m_wSatDataBuff_p[3][iDatWidth * iy + ix] / 1000.;
            if (fRvis < .25)
                ich = (int)((175 - 0) * (fRvis - 0.) / (0.25 - 0.));
            else
            	{
                ich = (int)((255 - 176) * (fRvis - 0.25) / (1.0 - 0.25) + 176);
                
              }

            if (ich > 255) ich = 255;
            if (ich < 5)   ich = 5;
            out_buf_RGB_day[ix * rgbBand + 1] = ich;  
//            if(ix >= 664 && iy > 710)  
//            	{
//            		 printf("----green-----out_buf_RGB_day %d ix %d iy %d\n", out_buf_RGB_day[ix * rgbBand + 1],ix,iy); 
//             getchar(); 
//            	}                 
              
             //blue
            fRvis = (float) m_wSatDataBuff_p[5][iDatWidth * iy + ix] / 1000.;
            if (fRvis < .25)
            {
            		ich = (int)((150 - 0) * (fRvis - 0.) / (0.25 - 0.));
            }            
            else
            {
                ich = (int)((255 - 151) * (fRvis - 0.25) / (1.0 - 0.25) + 150);
                if (ich > 255) ich = 255;
            }

            if (ich > 255) ich = 255;
            if (ich < 5)   ich = 5;
            out_buf_RGB_day[ix * rgbBand +2 ] = ich; //蓝色 
//            if(ix >= 664) 
//            	{
//            		 printf("----green-----out_buf_RGB_day %d ix %d iy %d\n", out_buf_RGB_day[ix * rgbBand + 2],ix,iy); 
//           	 getchar(); 
//            	}                            	
           	      		
        		if(m_FHSPixed[iy*iDatWidth+ix] == 10) 
            	{
             		
              	out_buf_RGB_day[ix * rgbBand +2] = 0;
                out_buf_RGB_day[ix * rgbBand + 1] = 0;
              	out_buf_RGB_day[ix * rgbBand ]		 = 255;	
             	} 
           	}          	
           	 
        	}
    row_pointer_day[0] = out_buf_RGB_day;
   
    jpeg_write_scanlines(&jcs_day, row_pointer_day, 1); 
   
    }         
        // write RGB file:
  
    jpeg_finish_compress(&jcs_day); 
   
    fclose(out_File_RGB_day); 
     
    if(out_buf_RGB_day != NULL)
    {
	    delete []out_buf_RGB_day;
	    out_buf_RGB_day = NULL;
  	}
  
	return 0;
}

int ProcessFire::FireProSaveFHSOfNIGPixe()
{
    int ich;
    float fTvir, fRvis;
    double r = 5;
    double fdis = 0.;
    fTvir = 0.; fRvis = 0.;
    unsigned char m_FHSPixed[iDatHeight * iDatWidth] ;
    memset(m_FHSPixed,0,sizeof(unsigned char)*iDatHeight * iDatWidth);
		
		FILE *out_File_RGB_night = NULL;
		struct jpeg_compress_struct jcs_night;
		struct jpeg_error_mgr jem_night;
		jcs_night.err = jpeg_std_error(&jem_night);
		jpeg_create_compress(&jcs_night);
   
  if((out_File_RGB_night = fopen(strOutputFileName_FNJPG, "wb+")) == NULL) 
    {
        fprintf(stderr,"Cannot open output file jpeg.%s\n", strOutputFileName_FNJPG);
        ZLDSWriteLog('W',"open fire flag .jpeg error");
        return ZEOPENFLAGOFJPEG;
    }


   
     jpeg_stdio_dest(&jcs_night, out_File_RGB_night);
       
    jcs_night.image_width =  iDatWidth; // 图片的宽
    jcs_night.image_height = iDatHeight; // 图片的高
    jcs_night.input_components=3; //1为灰度图，3为RGB图
    jcs_night.in_color_space = JCS_RGB;//JCS_GRAYSCALE表示灰度图，JCS_RGB表示彩色图
    
     
    jpeg_set_defaults(&jcs_night);
    jpeg_set_quality(&jcs_night,100,TRUE);
    jpeg_start_compress(&jcs_night,TRUE);
    
    
    JSAMPROW row_pointer_night[1];           
    const int rgbBand = 3;
   
    unsigned char* out_buf_RGB_night = new unsigned char[iDatWidth*3];
  

 
  for (int iy = 0; iy < iDatHeight; iy++) 
    { 
    	
        for (int ix = 0; ix < iDatWidth; ix++)
        {

          if(iy == 0 || ix == 0 ||iy == iDatHeight-r || ix == iDatWidth -r)
          {
          		continue;
          }
    	  	if(m_cPixelId_p[iy][ix] == 10)
    	  	{
    	  		for(int im = 1; im <= r; im++)
    	  		{   
    	  			 for(int in = 1;in <= r; in++)
    	  			 {
    	  			 	
    	  			 	 fdis = sqrt(double(im *im)+double(in*in));    	  			 	
    	  			 	// printf("fdis %lf ,dis %lf \n",fdis,dis);
    	  			 	 if(fdis < r)
                {
                m_FHSPixed[(iy-im)*iDatWidth+(ix-in)] = 10;
                m_FHSPixed[(iy-im)*iDatWidth+ix] = 10;
                m_FHSPixed[(iy-im)*iDatWidth+(ix+in)] = 10;
                m_FHSPixed[(iy)*iDatWidth+ix] = 10;
                m_FHSPixed[(iy)*iDatWidth+(ix-in)] = 10;
                m_FHSPixed[(iy)*iDatWidth+(ix+in)] = 10;
                m_FHSPixed[(iy+im)*iDatWidth+(ix-in)] = 10;
                m_FHSPixed[(iy+im)*iDatWidth+ix] = 10;  
                m_FHSPixed[(iy+im)*iDatWidth+(ix+in)] = 10;    	
                }    	  				  			
    	  			
    	  			 }    	  			 
    	  			  			
    	  		}
    	  	}
    	     			         			
        }
                                                                                                                                
    }
  
   for(int iy = 0;iy<iDatHeight; iy++)
    {
        	
        	memset(out_buf_RGB_night,0,sizeof(unsigned char)*iDatWidth*3);
        	for(int ix = 0; ix < iDatWidth; ix++)
        	{
        		
           if(m_wSatDataBuff_p[4][iDatWidth * iy + ix] == 65534 || m_wSatDataBuff_p[1][iDatWidth * iy + ix] == 65534 || 
           	m_wSatDataBuff_p[2][iDatWidth * iy + ix] == 65534)
           	{
           		out_buf_RGB_night[ix * rgbBand ]	= 123; 
           		out_buf_RGB_night[ix * rgbBand + 1] = 123;
           		out_buf_RGB_night[ix * rgbBand + 2] = 123;
           		continue;
           	}          
           	//night
          else
          {
          	//red
           fTvir = (float)m_wSatDataBuff_p[4][iDatWidth * iy + ix] / 10.;
           if (fTvir < 270.0 )
           fTvir = 270.0;
           if (fTvir > 310.0)
           fTvir = 310.0;
           ich = (int)((((fTvir - 270.0) * (fTvir - 270.0))  / ((310.0-270.0)*(310.0-270.0))) * 255);
           if (ich > 255) ich = 255;
           if (ich < 5)   ich = 5;
           out_buf_RGB_night[ix * rgbBand ]	= ich; 
                 
                                
           //green
           fTvir = (float)m_wSatDataBuff_p[1][iDatWidth * iy + ix] / 10.;
           if (fTvir < 250.0 )
           fTvir = 250.0;
           if (fTvir > 310.0)
           fTvir = 310.0;
           ich = (int)((1 - ((fTvir - 250.0) * (fTvir - 250.0))  / ((310.0-250.0)*(310.0-250.0))) * 255);
           if (ich > 255) ich = 255;
           if (ich < 5)   ich = 5; 
           out_buf_RGB_night[ix * rgbBand + 1] = ich;	   
                   
              //blue
              fTvir = (float)m_wSatDataBuff_p[2][iDatWidth * iy + ix] / 10.;
              if (fTvir < 250.0 )
              fTvir = 250.0;
              if (fTvir > 310.0)
              fTvir = 310.0;

              ich = (int)(255 - (255 - 0) * (fTvir - 250.0)  / (310.0-250.0));

              if (ich > 255) ich = 255;
              if (ich < 5)   ich = 5;
              out_buf_RGB_night[ix * rgbBand +2 ] = ich;  
                          		
        		if(m_FHSPixed[iy*iDatWidth+ix] == 10) 
            	{
             		out_buf_RGB_night[ix * rgbBand +2] = 0;
                out_buf_RGB_night[ix * rgbBand + 1] = 0;
              	out_buf_RGB_night[ix * rgbBand ]		 = 255;	
             	} 
        	}
          } 	  
    row_pointer_night[0] = out_buf_RGB_night;
    jpeg_write_scanlines(&jcs_night, row_pointer_night, 1); 
    }         
        // write RGB file:
  
   
     jpeg_finish_compress(&jcs_night);  
  
     fclose(out_File_RGB_night);   
   
  	 if(out_buf_RGB_night != NULL)
    {
	    delete []out_buf_RGB_night;
	    out_buf_RGB_night = NULL;
  	}
	return 0;
}

//--------    火点信息处理函数 ----------------------------------------------

/*: *******************  FireInfoPro *********************
* 函数名称：  FireInfoPro
* 函数标题：  火点信息处理
* 函数功能：  火点信息处理
* 调用方法: FireInfoPro(void)
*   参数说明：
*   无
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::FireInfoPro() //划分火区
{
  //划分火区 并计算火点比例，能量等信息， 输出 2个  txt火点信息表
   int n = 0;
//       FILE *fpout;
//       char table1[256];
//       memset(table1, 0, sizeof(char)*256);
//       strcpy(table1,"/PGSDATA/FY4A/L2/firetest.txt");  
//        fpout = fopen(table1,"w"); 
   //计算整图的火点个数
   for (int iy = 0; iy < iDatHeight; iy++)
   {
      for (int ix = 0; ix < iDatWidth; ix++)
      {
        if (m_TempBuff2_p[iy][ix] == 1)
      	{
          n = n + 1;         
       // fprintf(fpout,"%d\t%d\t%d\t\r\n",iy,ix,m_wSatDataBuff_p[4][iy * iDatWidth + ix]);
   
        }
      }
   }

   //火点判识函数中的n为区域内的火点个数，此处的n为全图范围的火点个数
   //05年4月28日 统计火点和分配火点信息空间放在此函数中
   iFires = n;
   printf("******fire count is %d************\n",iFires);
   //重新分配火点信息空间
	if (fpi_ == NULL)
	{
		fpi_ = new FirePixInfo_[iFires + 1];
	}
	else 
	{
		delete []fpi_;
		fpi_ = NULL;
		fpi_ = new FirePixInfo_[iFires + 1];
	}

   iFqNum = 2;
   iFqNo = 0;
   iFireNo = 0;

   //调用火点分区函数
	for (int iy = 0; iy < iDatHeight; iy++)
	{
		for (int ix = 0; ix < iDatWidth; ix++)
		{
	     if (m_TempBuff2_p[iy][ix] == 1 )
	     { 
          iFqNo = iFqNo + 1;
          fire_fq_(iy,ix);								//确定火点分区、并记录火点像素点的温度信息
          iFqNum ++;
	     }
		}
	}

  iAreas =  iFqNum-2;     // 分区个数

	if (ffi_ == NULL)
	{
		ffi_ = new FireFQInfo_[iFqNum - 2 + 1];
	}
	else 
	{
	  delete []ffi_;
	  ffi_ = NULL;
	  ffi_ = new FireFQInfo_[iFqNum - 2 + 1];
	}
	//求孤点及边缘的通道3，4，5的背景温度
	BJtemp();
	//计算内点id=2的点背景温度
	BJtemp2();
	//计算像元面积
	SaveFireInfo_();
  cout << "save fire info end" << endl;

  //计算火区中心点等等
  //计算比例P
  float xx;
  for (int i = 1; i <= iFires; i++)
  {
    int iX,iY;
    iX = fpi_[i].wX;
    iY = fpi_[i].wY;
    float FireT;							//火点温度
    unsigned char  bF1;				//火点能量等级
		//计算亚象元比例及火点温度和辐射能量
    fpi_[i].fFireP = SubPixel_(fpi_[i].fMixTemp[0], fpi_[i].fMixTemp[1], fpi_[i].fBackTemp[0], fpi_[i].fBackTemp[1], &FireT , &bF1);  

    fpi_[i].fFireTemp = FireT; //火点象元温度
    fpi_[i].TreL = bF1;

     xx = fpi_[i].fFireS * fpi_[i].fFireP;  //亚象元面积

     fpi_[i].fNl = xx * 5.6705 * 0.00000001 * (fpi_[i].fFireTemp * fpi_[i].fFireTemp * fpi_[i].fFireTemp * fpi_[i].fFireTemp);

     if (fpi_[i].fNl <50)
        fpi_[i].intst = 1;

     if (fpi_[i].fNl < 100 && fpi_[i].fNl >= 50)
        fpi_[i].intst = 2;

     if (fpi_[i].fNl < 200 && fpi_[i].fNl >= 100)
        fpi_[i].intst = 3;

     if (fpi_[i].fNl <300 && fpi_[i].fNl >= 200)
        fpi_[i].intst = 4;

     if (fpi_[i].fNl <500 && fpi_[i].fNl >= 300)
        fpi_[i].intst = 5;

     if (fpi_[i].fNl >= 500)
        fpi_[i].intst = 6;

  }

  for (int i = 1 ; i <= iFqNum - 2; i++ )
  {
     //对火区信息进行处理：对每一分区处理并给ffi结构变量附值
     FireFQP_(i);
  }
  OutputInfo1_(); //输出火区信息表

  OutPutInfo_();  //输出火点信息中间文件

  OutputFirePixInfo_();  //输出火点信息文件
 
  
  
  cout << "fire txt end..." << endl;
}
//---------------------------------------------------------------------------

//---- 火点分区 -----------

/*: *******************  fire_fq_*********************
* 函数名称：  fire_fq_
* 函数标题：  火点分区
* 函数功能：  确定火区
* 调用方法: fire_fq_(int y, int x)
*   参数说明：
*   int y   行号
*   int x   列好
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/

void ProcessFire::fire_fq_(int y, int x)
{
	//TODO: Add your source code here   搜索改进
  unsigned int   s1,s2;
  short x1,x2,y1,y2;

	s1 = 0;
	s2 = 1;
	
	x1 = x - 1;
	x2 = x + 1;
	y1 = y - 1;
	y2 = y + 1;
	if (x1 < 0) x1 = 0;
	if (x2 > (iDatWidth - 1)) x2 = iDatWidth - 1;
	if (y1 < 0) y1 = 0;
	if (y2 > (iDatHeight - 1)) y2 = iDatHeight - 1;


   m_TempBuff2_p[y][x] = 2; //先将火点值标记1改为2
   iFireNo = iFireNo + 1; //火点数加1

   // 确定此种子点的id 即火区中的位置，id=0 为孤点，id=1为边缘点，id=2为内点。
   for (int i = y1; i <= y2; i++)
   {
     for (int j = x1; j <= x2; j++)
     {
       s1 = s1 + m_TempBuff2_p[i][j];
       s2 = s2 * m_TempBuff2_p[i][j];
     }
   }
	 //确定火点性质结束
   if (s1 == 2)
      fpi_[iFireNo].bId = 0;
   else if (s2 == 0)
      fpi_[iFireNo].bId = 1;
   else
      fpi_[iFireNo].bId = 2;
   

		//给火点信息表赋值
   fpi_[iFireNo].wNo = iFireNo;    //火点编号
   fpi_[iFireNo].wAreaNo = iFqNo;  //火区编号
   fpi_[iFireNo].wY = y;           //火点所在的行号
   fpi_[iFireNo].wX = x;           //火点所在的列号
   fpi_[iFireNo].fLat = m_fLat_p[y][x];   //火点的纬度值                     //buff.fMaxLat - y * 0.01;
   fpi_[iFireNo].fLon = m_fLong_p[y][x];  //火点经度值                      //buff.fMinLon + x * 0.01;
   fpi_[iFireNo].fMixTemp[0] = (float)m_wSatDataBuff_p[4][iDatWidth * y + x] / 10.0;  //火点的中红外值
   fpi_[iFireNo].fMixTemp[1] = (float)m_wSatDataBuff_p[1][iDatWidth * y + x] / 10.0;  //火点的远红外1的值
   fpi_[iFireNo].fMixTemp[2] = (float)m_wSatDataBuff_p[2][iDatWidth * y + x] / 10.0;  //火点的远红外2的值


   // 已此种子点开始搜索火区火点
   for (int i = y1; i <= y2; i++)
   {
     for (int j = x1; j <= x2; j++)
     {
        if (m_TempBuff2_p[i][j] == 1)
          fire_fq_(i,j);
     }
   }
}

//------------------------------------------------------------------

/*: *******************  BJtemp *********************
* 函数名称：  BJtemp
* 函数标题：  计算背景温度
* 函数功能：  计算孤点及边缘点的通道3，4，5的背景温度
* 调用方法: BJtemp(void)
*   参数说明：
*   无
*
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/

void ProcessFire::BJtemp()
{
	//TODO: Add your source code here
	//求孤点及边缘的通道3，4，5的背景温度

   unsigned int   num;
   float s1,s2,s3,s5;
   short x1,x2,y1,y2;
   bool fpid = false;
   FILE* fpLandDat_p = NULL;

   if (!fpLandDat_p)
   {
       fpLandDat_p = fopen(strnLanding,"rb");	//打开土地利用类型数据文件
       fpid = true;
   }

   if (!fpLandDat_p)
   {
      cout << "打开土地利用类型文件出错！" << endl;
      for (int i = 1; i <= iFires ; i++)
      {
         fpi_[i].fBackTemp[1] = 0;   //求得通道4的背景温度，并记录
         fpi_[i].fBackTemp[0] = 0;   // 求得通道3的背景温度，并记录
         fpi_[i].fBackTemp[2] = 0;  // 求得通道5的背景温度，并记录
      }
      return ;
   }

   for (int i = 1; i <= iFires ; i++)
   {
     if (fpi_[i].bId != 2)
     	{
        x1 = fpi_[i].wX - 2;
        x2 = fpi_[i].wX + 2;
        y1 = fpi_[i].wY - 2;
        y2 = fpi_[i].wY + 2;
        if (x1 < 0) x1 = 0;
        if (x2 > (iDatWidth - 1)) x2 = iDatWidth - 1;
        if (y1 < 0) y1 = 0;
        if (y2 > (iDatHeight - 1)) y2 = iDatHeight - 1;
        s1 = 0.0;
        s2 = 0.0;
        s3 = 0.0;
        s5 = 0.0;
        num = 0;
				for (int k = y1; k <= y2; k++)
				{
					for (int j = x1; j <= x2; j++)
					{
				     if (m_ResultBuff_p[k][j] != 2 && m_TempBuff2_p[k][j] == 0 )
				     {
				         num = num+1;
				         s1 = s1 + m_wSatDataBuff_p[1][iDatWidth * k + j] / 10.0;
				         s3 = s3 + m_wSatDataBuff_p[4][iDatWidth * k + j] / 10.0;
				         s5 = s5 + m_wSatDataBuff_p[2][iDatWidth * k + j] / 10.0;
				     }
					}
				}
        if (num > 3)
        {
            fpi_[i].iQKId = 1;      // 背景晴空为1
            s2 = s1 / num;
            s3 = s3 / num;
            s5 = s5 / num;
        }
        else
        {
            float c45d;
            c45d = (fpi_[i].fMixTemp[1] - fpi_[i].fMixTemp[2]);  // - (fi.fAverTmepBg[1]/10. - fi.fAverTmepBg[2]/10.);
            if (c45d >= 3) c45d = 3;
            s2 = fpi_[i].fMixTemp[1] - 4.5 * c45d;
            s3 = s2 + LandVI_(fpLandDat_p, fpi_[i].fLat , fpi_[i].fLon) ;			//该LandVI函数设置土地类型编号
            s5 = s2;
            fpi_[i].iQKId = 0;
        }

        fpi_[i].fBackTemp[1] = s2;   //求得通道4的背景温度，并记录
        fpi_[i].fBackTemp[0] = s3;   // 求得通道3的背景温度，并记录
        fpi_[i].fBackTemp[2] = s5;  // 求得通道5的背景温度，并记录
     }
   }
		fclose(fpLandDat_p);
		fpLandDat_p = NULL;
}
//-------------------------------------------------------

/*: *******************  BJtemp2 *********************
* 函数名称：  BJtemp2
* 函数标题：  计算背景温度
* 函数功能：  计算火区内点的通道3，4，5的背景温度
* 调用方法: BJtemp2(void)
*   参数说明：
*   无
*
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::BJtemp2()
{
	//TODO: Add your source code here
	// 计算 内点id=2的点背景温度
	int    s1,s2,num;
	for (int i = 1; i <= iFires ; i++)
	{
     s1 = 100000;
     s2 = 0;
     num = 0;
     if (fpi_[i].bId == 2)
     {
        num = fpi_[i].wAreaNo;
        for (int j = 1; j <= iFires; j++)
        {
           if (fpi_[j].wAreaNo == num && fpi_[j].bId == 1 && i != j)
           	{
               if (((fpi_[i].wX - fpi_[j].wX) * (fpi_[i].wX - fpi_[j].wX)
                   + (fpi_[i].wY - fpi_[j].wY)*(fpi_[i].wY - fpi_[j].wY)) < s1)
                   {
	                  s2 = j;
	
	                  s1 = (fpi_[i].wX - fpi_[j].wX) * (fpi_[i].wX - fpi_[j].wX)
	                        + (fpi_[i].wY - fpi_[j].wY)*(fpi_[i].wY - fpi_[j].wY);
               		}
            }
        }

        fpi_[i].iQKId = fpi_[s2].iQKId;   								//火区内火点晴空标志取最近火点的值
        fpi_[i].fBackTemp[1] =  fpi_[s2].fBackTemp[1];    // 火区内火点背景取最近火点的背景值
        fpi_[i].fBackTemp[0] = fpi_[s2].fBackTemp[0];  //
        fpi_[i].fBackTemp[2] = fpi_[s2].fBackTemp[2];  //
     }
   }
}

//------------------------------------------

/*: *******************  SaveFireInfo_ *********************
* 函数名称：  SaveFireInfo_
* 函数标题：  火点信息保存
* 函数功能：  火点信息保存到火点信息变量中
* 调用方法: SaveFireInfo_(void)
*   参数说明：
*   无
*
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::SaveFireInfo_(void)
{
	//TODO: Add your source code here
	// 计算象元面积 ，土地类型编号， 行政边界代码
	float num1;
	unsigned char  num;
	int x0,y0;
	long int k;
	
	FILE *fpLandDat_p;																		//打开土地类型文件
	fpLandDat_p = NULL;																				
	FILE *fpXianJieDat_p;																	//打开省界
	fpXianJieDat_p = NULL;	
	
	if (!fpLandDat_p)
	{
	  fpLandDat_p = fopen(strnLanding,"rb");
	}
	
	if (!fpXianJieDat_p)
	{
	  fpXianJieDat_p = fopen(strxjraster,"rb");
	}
	
	if (!fpXianJieDat_p || !fpLandDat_p)
	{
     cout << "打开行政边界文件出错！" << endl;
     for (int i = 1; i <= iFires; i++)
     {
        fpi_[i].fFireS = PixS_(fpi_[i].wX, fpi_[i].wY);
        fpi_[i].iBjNo = 0;
        fpi_[i].wLandId = 0;
     }
     if (fpXianJieDat_p)
     {
        fclose(fpXianJieDat_p);
        fpXianJieDat_p = NULL;
     }
     if (fpLandDat_p)
     {
        fclose(fpLandDat_p);
        fpLandDat_p = NULL;
     }
     return ;
	}
	
	//继续对火点信息表赋值
	for (int i = 1; i <= iFires; i++)
	{
	   fpi_[i].fFireS = PixS_(fpi_[i].wX, fpi_[i].wY); //火点象元面积
	
	   num1 = 0.0; num = 0; x0 = 0; y0 = 0;
	   x0 = (int)((fpi_[i].fLon - 70.) * 100);
	   y0 = (int)((56. - fpi_[i].fLat) * 100);
	
	   k = (long int)((y0 * 7100 + x0) * 4);
	   fseek(fpXianJieDat_p, k, 0);
	   fread(&num1,sizeof(float),1,fpXianJieDat_p);
	   fpi_[i].iBjNo = (int) num1;  //县界编号
	
	   k = (long  int)(y0 * 7100 + x0);
	   fseek(fpLandDat_p,k,0);
	   fread(&num,sizeof(unsigned char),1,fpLandDat_p);
	   fpi_[i].wLandId = num;   //土地类型名称
	}
	
	
	fclose(fpLandDat_p);
	fpLandDat_p = NULL;
	
	fclose(fpXianJieDat_p);
	fpXianJieDat_p = NULL;


}
//------------------------------------------

/*: *******************  PixS_ *********************
* 函数名称：  PixS_
* 函数标题：  计算象元面积
* 函数功能：  计算某点的象元面积
* 调用方法: PixS_(int PixX,int PixY)
*   参数说明：
*   int PixY   行号
*   int PixX   列好
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/

float ProcessFire::PixS_(int PixX,int PixY)
{
    //int x1,x2,y1,y2;
    float fLongJ,fLatI,LongL,LatL,dd,S;
    float pi = 3.1415926;
    float a = 6378.2;
    float b = 6256.8;
    // abs
    if (PixX == 0) PixX = 1;
    if (PixX == (iDatWidth -1)) PixX = PixX - 1;
    if (PixY == 0) PixY = 1;
    if (PixY == (iDatHeight -1)) PixY = PixY - 1;

    fLongJ =(m_fLong_p[PixY][PixX] - m_fLong_p[PixY][PixX- 1]) / 2.0 ;
    fLongJ = fLongJ + (m_fLong_p[PixY][PixX + 1] - m_fLong_p[PixY][PixX]) / 2.0;

    fLatI = (m_fLat_p[PixY - 1][PixX] - m_fLat_p[PixY][PixX]) / 2.0;
    fLatI = fLatI + (m_fLat_p[PixY][PixX] - m_fLat_p[PixY + 1][PixX]) / 2.0;

    dd = b * b + a * a * tan(m_fLat_p[PixY][PixX] * pi /180.0) * tan(m_fLat_p[PixY][PixX] * pi /180.0);
    dd = 1 / dd;
    dd = sqrt(dd);

    LongL = fLongJ * 2.0 * pi * a * b * dd;
    LongL = LongL / 360.0;
    LatL  = fLatI * 111.13;

    S = LongL * LatL;

    return S;

}
//---------------------------------------------------

/*: *******************  FireFQP_ *********************
* 函数名称：  FireFQP_
* 函数标题：  火区信息处理
* 函数功能：  火区信息处理保存到火区信息变量中
* 调用方法: FireFQP_(int FQNo)
*   参数说明：
*   int FQNo 分区号
*
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::FireFQP_(int FQNo)
{
	//TODO: Add your source code here
	//处理每个分区的火点信息 找中心 ，计算百分比，填行政区域
  int n1,n2,lindinum,caochangnum,qitanum,maxY,maxX,mixY,mixX;
  bool b1,b2;
  b1 = true;
  b2 = true;

//火区变量设初值
  n1=0;n2=0;lindinum = 0; caochangnum = 0;qitanum = 0;
  ffi_[FQNo].wFireAreaNo = FQNo;
  ffi_[FQNo].fMaxLat = -900.0;
  ffi_[FQNo].fMaxLon = -990.0;
  ffi_[FQNo].fMixLat = 999.0;
  ffi_[FQNo].fMixLon = 999.0;
  maxY = 0; maxX = 0; mixY = 100000; mixX = 100000;
  ffi_[FQNo].wFireAreaFires = 0;
  ffi_[FQNo].fFireAreaS = 0.0;
  ffi_[FQNo].fFireLinDi = 0.0;
  ffi_[FQNo].fFireCaoChang = 0.0;
  ffi_[FQNo].fFireQiTa = 0.0;
  ffi_[FQNo].sCloudsId = "否";
  ffi_[FQNo].cCloudsId = 0;
  //火区像元面积 20171208
  ffi_[FQNo].fNPixelS = 0;
  FILE *fpLandDat_p, *fpLandTxt_p;
  fpLandDat_p = NULL;
  fpLandTxt_p = NULL;

  if ( fpLandDat_p == NULL)
  {
      fpLandDat_p = fopen(strnLanding,"rb");											//打开土地利用类型文件
      fpLandTxt_p = fopen(strLandCover,"rt");											//打开土地利用类型对应的名称
      if (!fpLandDat_p)
         b1 =!b1;
  }

  FILE *fpXianJieDat_p, *fpXianJieTxt_p;
  fpXianJieTxt_p = NULL;
  fpXianJieDat_p = NULL;

  if (!fpXianJieDat_p)
  {
      fpXianJieDat_p = fopen(strxjraster,"rb");									//打开县界数据文件
      fpXianJieTxt_p = fopen(strxjdh_new,"rt");									//打开县界数据对应的名称
      if (!fpXianJieDat_p)
        b2 = !b2;
  }

  FILE *fp_LandDat;
  fp_LandDat = NULL;
  fp_LandDat  = fopen(strLandImg,"rb");

  int num1,x0,y0;
  long  int k;
  for (int i = 1; i<= iFires ;i++)
  {
     if (fpi_[i].wAreaNo == FQNo)
     	{
        ffi_[FQNo].wFireAreaFires += 1;  //火区的火点个数
        ffi_[FQNo].fFireAreaS += (fpi_[i].fFireS * fpi_[i].fFireP);   // 火区的亚象元面积
        ffi_[FQNo].fNPixelS += fpi_[i].fFireS ; ////20171208 每个火区所有像元面积 
        num1 = 0;

        x0 = (int)((fpi_[i].fLon + 180.) * 100);
        y0 = (int)((90. - fpi_[i].fLat) * 100);

        if (x0 < 36000 && x0 >= 0 && y0 < 18000 && y0 >= 0)
        {
            k = (long  int)(y0 * 36000 + x0);
            fseek(fp_LandDat,k,0);
            fread(&num1,sizeof(unsigned char),1,fp_LandDat);
        }

       //按类别统计林地、草地、其他火点个数
        if (num1 == 2 )
             ffi_[FQNo].fFireLinDi += 1;
        else if (num1 == 1 )
             ffi_[FQNo].fFireCaoChang += 1;
        else if (num1 != 2 && num1 != 1 && num1 != 0)
             ffi_[FQNo].fFireQiTa += 1;

        if (fpi_[i].fLat > ffi_[FQNo].fMaxLat)
        {
           ffi_[FQNo].fMaxLat = fpi_[i].fLat;
           mixY = fpi_[i].wY;
        }
        if (fpi_[i].fLon > ffi_[FQNo].fMaxLon)
        {
           ffi_[FQNo].fMaxLon = fpi_[i].fLon;
           maxX = fpi_[i].wX;
        }
        if (fpi_[i].fLat < ffi_[FQNo].fMixLat)
        {
           ffi_[FQNo].fMixLat = fpi_[i].fLat;
           maxY = fpi_[i].wY;
        }
        if (fpi_[i].fLon < ffi_[FQNo].fMixLon)
        {
           ffi_[FQNo].fMixLon = fpi_[i].fLon;
           mixX = fpi_[i].wX;
        }

       int ax1,ay1,ax2,ay2;       
       ax1 = fpi_[i].wX - 1;
       ay1 = fpi_[i].wY - 1;
       ax2 = fpi_[i].wX + 1;
       ay2 = fpi_[i].wY + 1;
       if (ax1 < 0)
          ax1 = 0;

       if (ay1 < 0)
          ay1 = 0;

       if (ax2 > (iDatWidth -1))
          ax2 = iDatWidth -1;

       if (ay2 > (iDatHeight -1))
          ay2 = iDatHeight -1;

       for (int ik = ay1 ; ik <= ay2 ; ik++)
       {
           for (int jk = ax1 ; jk <= ax2 ; jk++)
           {
              if (m_ResultBuff_p[ik][jk] == 2)
              	{
                 ffi_[FQNo].sCloudsId = "是";
                 ffi_[FQNo].cCloudsId = 1;
                }
           }
       }
     }
  } //找出最大最小值和火区火点个数

  //cout<<"*****fNPixelS:******"<<fNPixelS<<endl;
  
 
  fclose (fp_LandDat);

	ffi_[FQNo].fFireLinDi =  ffi_[FQNo].fFireLinDi / ffi_[FQNo].wFireAreaFires;
	ffi_[FQNo].fFireCaoChang =  ffi_[FQNo].fFireCaoChang / ffi_[FQNo].wFireAreaFires;
	ffi_[FQNo].fFireQiTa =  ffi_[FQNo].fFireQiTa / ffi_[FQNo].wFireAreaFires;

  int   centerX1,centerY1;

  //计算出火区中心点行列号
  centerX1 = maxX - (maxX - mixX) / 2;
  centerY1 = maxY - (maxY - mixY) / 2;

  //找出最接近火区中心点的火点
  ffi_[FQNo].fCenterLat = 0.0;
  ffi_[FQNo].fCenterLon = 0.0;
  float dat1,dat2;
  dat1 = 999.0; dat2 = 99999.0;
  for (int i = 1; i<= iFires ;i++)
 	{
	   if (fpi_[i].wAreaNo == FQNo)
	   {
	       if ((((fpi_[i].wY - centerY1) * (fpi_[i].wY - centerY1)*1.0 + (fpi_[i].wX - centerX1) * (fpi_[i].wX - centerX1)*1.0)) > 0)
	          dat1 = sqrt((fpi_[i].wY - centerY1) * (fpi_[i].wY - centerY1)*1.0 + (fpi_[i].wX - centerX1) * (fpi_[i].wX - centerX1)*1.0);
	       else
	          dat1 = 0;
	
	       if (dat1 < dat2) 
	       {
	          dat2 = dat1;
	          ffi_[FQNo].fCenterLat = fpi_[i].fLat;
	          ffi_[FQNo].fCenterLon = fpi_[i].fLon;
	       }
	   }
  }
    ffi_[FQNo].sShengMing =  SrName_(fpXianJieDat_p, fpXianJieTxt_p, ffi_[FQNo].fCenterLat ,ffi_[FQNo].fCenterLon);//获取省边界信息  
   ffi_[FQNo].iShengMing =  SrNameOfNum(fpXianJieDat_p, ffi_[FQNo].fCenterLat ,ffi_[FQNo].fCenterLon);
  
  if (b1)
  {
      fclose(fpLandDat_p);
      fclose(fpLandTxt_p);
      fpLandDat_p = NULL;
      fpLandTxt_p = NULL;
  }
  if (b2)
  {
      fclose(fpXianJieDat_p);
      fclose(fpXianJieTxt_p);
      fpXianJieDat_p = NULL;
      fpXianJieTxt_p = NULL;
  }

}

//-------------------------------------------------

/*: *******************  SubPixel_ *********************
* 函数名称：  SubPixel_
* 函数标题：  计算亚象元面积及火点温度和火点能量
* 函数功能：  计算亚象元面积及火点温度和火点能量
* 调用方法: SubPixel_(float ch3,float ch4,float ch3b,float ch4b, float *Ft,  unsigned char *TL)
*   参数说明：
*   float ch3 中红外值
*   float ch4 远红外值
*   float ch3b 中红外背景值
*   float ch4b 远红外背景值
*   float *Ft 计算的温度值
*   unsigned char *TL 计算的火点能量等级
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
float ProcessFire::SubPixel_(float ch3,float ch4,float ch3b,float ch4b, float *Ft,  unsigned char *TL)
{
 	double c1,c2,WAV3,WAV4,WAV,LT,LTB,LTF;
	c1=1.1910427/100000;//c1,c2系数，VC中心波数,ABT中心波数增益、斜率
	c2=1.4387752;
	WAV3=2671.2150;
	WAV4= 924.8922;
	short TF=750;
	float p,TB,T;
	if (ch3b > ch3) ch3 = ch3b + 10.;  // 20161003***************************************
	if (ch4b > ch4) ch4 = ch4b + 0.5;  // 20161003***************************************
  if (ch3 > 332.0 && ch4 > ch4b)
	{
		WAV = WAV4;
		T = ch4;
		TB = ch4b;

		*TL = 3;

		if (ch4 >= 380.)
			*TL = 4;
		//	T=float(ch4) / 10.;
		//	TB=float(ch4b) / 10.;
	}
	else
	{
		double pp, pt;

		subpxx = 10;

		SubPixel_1(ch3,ch4,ch3b,ch4b, &pp, &pt);//迭代计算亚像元火点面积和温度
		if(pp > 0 && pp < 100 && pt > 600 && pt < 2500 && subpxx == 10)
		{
			*Ft = pt;
			*TL = 1;			
			return pp;
		}

		*TL = 2;
		
		WAV = WAV3;
		T = ch3;
		TB = ch3b;
	}


	LT=c1*pow(WAV,3)/(exp(c2*WAV/T)-1);
	LTF=c1*pow(WAV,3)/(exp(c2*WAV/TF)-1);
	LTB=c1*pow(WAV,3)/(exp(c2*WAV/TB)-1);
	p=float((LT-LTB)/(LTF-LTB));

	//LT=c1*pow(WAV,3)/(exp(c2*WAV/280.)-1);
	//LTF=c1*pow(WAV,3)/(exp(c2*WAV/290.)-1);
	//LTB=c1*pow(WAV,3)/(exp(c2*WAV/278.)-1);
	//p=float((LT-LTB)/(LTF-LTB));    
  // p = 0.2;

	*Ft = 750;
	return p;

}

//---------------------------------------------------------
/*: *******************  OutputInfo1_ *********************
* 函数名称：  OutputInfo1_
* 函数标题：  输出火区信息表
* 函数功能：  输出火区信息表
* 调用方法: OutputInfo1_(void)
*   参数说明：
*   无
*
*
*   作    者：闫华
*   完成日期：2013年
************************** OutputInfo1_ 函数代码 **************************  :*/
void ProcessFire::OutputInfo1_()
{
		//TODO: Add your source code here
		// 输出火区信息报表
    FILE *fpout;
    fpout = fopen(strOutputFileName_T3,"w");
    if (!fpout) 
    {
       cout << "new file error...." << endl;
       ZLDSWriteLog('W',"Open TXT_T3 error!");
       exit(ZEOPEN);
    }
    cout << " open " << strOutputFileName_T3  <<endl;

    string s1,s2,s0;

    s1 = fi.wYear + "-" + fi.wMonth + "-" + fi.wDay;
    s2 = fi.wHour + ":" + fi.wMinute;
    s1 = s1 + " " + s2;

    fprintf(fpout,"\r\n                                      气象卫星火情监测信息表                              \r\n\r\n");
    s0 = "观测时间（世界时）：" + s1 +  "                                       中国气象局国家卫星气象中心制作\r\n";



    fprintf(fpout,"%s\r\n",s0.c_str());
    fprintf(fpout,"BeginLine %4d  EndLine %4d\r\n",istartline,iendline);
    fprintf(fpout,"BeginPixel %4d  EndPixel %4d\r\n",istartpixel,iendpixel);
    fprintf(fpout,"------------------------------------------------------------------------------------------------------\r\n");

    //fprintf(fpout,"火区编号    纬度       经度    象素数    火点面积(公顷)    草地     林地     其它     云边缘    省地县                       \r\n");
    fprintf(fpout,"火区编号    纬度       经度    象素数    像素面积（平方公里）  明火区面积(公顷)    草地     林地   其它     云边缘    省地县\r\n");
    //@time:20161012 @author:ouyss    
    int *fireNumbers = new int[iAreas];
    int *NumberOfPixels = new int[iAreas];
    float *fLatTemp = new float[iAreas];
    float *fLonTemp = new float[iAreas];
    float *AreaTemp = new float[iAreas];
    char *Grassland = new char[iAreas];
    char *woodland = new char[iAreas];
    char *Others = new char[iAreas];
    char *CloudEdge = new char[iAreas];
    int *ProvinceInfoTemp = new int[iAreas];
    float * NPixelS = new float[iAreas];
    for (int i = 1; i <= iAreas;i++)
    {
				fLatTemp[i-1]= ffi_[i].fCenterLat;
				fLonTemp[i-1]= ffi_[i].fCenterLon;
				fireNumbers[i-1] = ffi_[i].wFireAreaNo;
				NumberOfPixels[i-1] = ffi_[i].wFireAreaFires;
				AreaTemp[i-1]= ffi_[i].fFireAreaS * 100.0;
				Grassland[i-1]= ffi_[i].fFireCaoChang;
				woodland[i-1] = ffi_[i].fFireLinDi;
				Others[i-1] = ffi_[i].fFireQiTa;
				CloudEdge[i-1] = ffi_[i].cCloudsId;
				ProvinceInfoTemp[i-1] = ffi_[i].iShengMing;	
								
			  fprintf(fpout,"%4d       %4.2f      %10.2f %10d          %8.2f               %5.3f          %4.2f     %4.2f    %4.2f       %s        %s\r\n",
               ffi_[i].wFireAreaNo, ffi_[i].fCenterLat, ffi_[i].fCenterLon, 
               ffi_[i].wFireAreaFires,ffi_[i].fNPixelS, ffi_[i].fFireAreaS * 100.0 ,ffi_[i].fFireCaoChang, ffi_[i].fFireLinDi,
               ffi_[i].fFireQiTa,ffi_[i].sCloudsId.c_str(),ffi_[i].sShengMing.c_str());  
    }                                                               
  fclose (fpout);
	cout << "write FPA TXT end " << endl;
	cout << "start FPA NC " << endl;
/*******************************************20180426*****************output temp FPA file********************************************************************/	
	FILE* fp;
	fp = fopen(strOutputEnFPAFile,"wb+");
  if(fp == NULL)
	{
		 exit(ZEOPEN);
	}
		
		 fprintf(fpout,"\r\n                                      FY-4A Hot Spot Information List                             \r\n\r\n");
    s0 = "Observation Time ( UTC ):" + s1 +  "                                       CMA/NSMC\r\n";
    fprintf(fpout,"%s\r\n",s0.c_str());
		fprintf(fpout,"BeginLine %4d  EndLine %4d\r\n",istartline,iendline);
    fprintf(fpout,"BeginPixel %4d  EndPixel %4d\r\n",istartpixel,iendpixel);
		fprintf(fp,"Hot_Spot_NO.     Lat.          Long.     Pixels     Pixel_Size(km)     Burned_Size(hm)     Grass     Forest     Others     Cloud_edge     Administrative_area\r\n");		
		for(int ii = 1;ii < iAreas+1; ii++ )
		{
			 fprintf(fp,"%-4d                         %-4.2f           %-4.2f       %-4d              %-5.2f            %-6.2f                   %-5.2f          %-6.2f         %-6.2f        %-4d               %-4d\r\n",
               ffi_[ii].wFireAreaNo, ffi_[ii].fCenterLat, ffi_[ii].fCenterLon, 
               ffi_[ii].wFireAreaFires,ffi_[ii].fNPixelS, ffi_[ii].fFireAreaS * 100.0 ,ffi_[ii].fFireCaoChang, ffi_[ii].fFireLinDi,
               ffi_[ii].fFireQiTa,ffi_[ii].cCloudsId,ffi_[ii].iShengMing);  
		}
		
		fclose(fp);
/*******************************************20180426*********************output temp FPA file****************************************************************/		
	if(strcmp(sataName,"FY4A")==0)
	{
			//FPA_nc
			OutputInfoNC3_FY4A(fLatTemp,fLonTemp,fireNumbers,NumberOfPixels,AreaTemp,Grassland,woodland,Others,CloudEdge,ProvinceInfoTemp);
			
       cout << "end FPA NC " << endl; 
	}
	else
	{
				OutputInfoNC1_AHI8(fLatTemp,fLonTemp,fireNumbers,NumberOfPixels,AreaTemp,Grassland,woodland,Others,CloudEdge,ProvinceInfoTemp);
        cout << "end FPA NC " << endl; 
  }   
	delete []fireNumbers;
	fireNumbers = NULL;
	delete []NumberOfPixels;
	NumberOfPixels = NULL;
	delete []fLatTemp;
	fLatTemp = NULL;
	delete []fLonTemp;
	fLonTemp = NULL;
	delete []AreaTemp;
	AreaTemp = NULL;
	delete []Grassland;
	Grassland = NULL;
	delete []woodland;
	woodland = NULL;
	delete []Others;
	Others = NULL;
	delete []CloudEdge;
	CloudEdge = NULL;
	delete []ProvinceInfoTemp;
	ProvinceInfoTemp = NULL;
}


int ProcessFire::OutputInfoNC3_FY4A(float *Lat,float *Lon,int *Number,int *NumberOfPixels,float *Area,char *Grassland,char *woodland,char *Others,char *CloudEdge,int *ProvinceInfo)
{
	printf("write NC \n");
	struct tm	structSysTm;
	time_t t;
	char strtime[15];		
	memset(strtime,0,sizeof(strtime));
	t=time(NULL);
	memcpy(&structSysTm,gmtime(&t),sizeof(struct tm));	
	sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02dZ",structSysTm.tm_year+1900,structSysTm.tm_mon+1,structSysTm.tm_mday,
																						 structSysTm.tm_hour,structSysTm.tm_min,structSysTm.tm_sec);
	GOESRNetCDFOperator myNetCDF;
	FY4_GOESR_L1B_FILE_INFO global_info;
	memset(&global_info,0,sizeof(FY4_GOESR_L1B_FILE_INFO));
	strcpy(global_info.strDatasetName,"FHS");
	strcpy(global_info.strNamingAuthority,"NSMC CMA");
	strcpy(global_info.strInstitution,"NSMC");	
	strcpy(global_info.strProject,"NOM");
	strcpy(global_info.strConventions,"CF-1.7");
	strcpy(global_info.strMetadataConventions,"Unidata Dataset Discovery v1.0");	
	strcpy(global_info.strSandardNameVocabul,"Fire/Hot Spot Characterization");	
	strcpy(global_info.strTitle,"FY4A AGRI L2 FHS");
	strcpy(global_info.strSummary,"none");
	strcpy(global_info.strPlatformID,"FY4A");
	strcpy(global_info.strInstrumentType,"FY4A Advanced Geosynchronous Radiation Imager");
	strcpy(global_info.strInstrumentID,"AGRI");
	strcpy(global_info.strProcessingLevel,"L2");
	strcpy(global_info.strDateCreated,strtime);
	strcpy(global_info.strProductionSite,"NSMC");		
	strcpy(global_info.strProductionEnvironment,"UNIX");
	strcpy(global_info.strVOS,"V1.0");
	strcpy(global_info.strSRD,"2017-09-12");
	strcpy(global_info.strSceneId,"Full Disk possible values are Full Disk,Southern HEMisphere,the Northern HEMisphere,Regional,China Regional");
	strcpy(global_info.strSpatialResolution,"2km at nadir.");
	sprintf(global_info.strTimeCoverageStart,"%sT%sZ",L1FileInfo_p->strDateOfObservingBeginning,L1FileInfo_p->strTimeOfObservingBeginning);
	sprintf(global_info.strTimeCoverageEnd,"%sT%sZ",L1FileInfo_p->strDateOfObservingEnding,L1FileInfo_p->strTimeOfObservingEnding);
	//global_info.strDataQua=L1FileInfo_p->cDataQuality;
   global_info.strDataQua=DataQ;
	strcpy(global_info.nL0QualityFlag,strL0Q_p);
	strcpy(global_info.nPosQualityFlag,strPOSQ_p);
	strcpy(global_info.nCalQualityFlag,strCALQ_p);
	int status=-1;

   int  dim[2];
   dim[0] = iFires;
   dim[1] = 1;
   float fFillValue = -999;
   float fValidRange[2];
   fValidRange[0] = -90;
   fValidRange[1] = 90;
   float ScaleFactor =  1.0,AddOffset = 0;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Lat", 2, dim, Lat, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization latitude coordinate", "Latitute","FALSE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_north", 
		"2KM", "fire spot lat long","none", "",1);			
	 if(status < 0)
			exit(-1);

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Lon", 2, dim, Lon, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization longitude coordinate", "Longitude","FALSE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_east", 
		"2KM", "fire spot lat long","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   int iFillValue = 0;
   int iValidRange[2];
   iValidRange[0] = 0;
   iValidRange[1] = 1000000;		
   int iScaleFactor =  1,iAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Number", 2, dim, Number, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Number", "Number","FALSE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Number","none","", 0);			
	 if(status < 0)
			exit(-1);	
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "NumberOfPixels", 2, dim, NumberOfPixels, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Number", "Number","FALSE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Number","none", "",0);			
	 if(status < 0)
			exit(-1);				

   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Area", 2, dim, Area, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire spot area", "Fire spot area","FALSE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "fire spot","none","", 0);			
	 if(status < 0)
			exit(-1);	

   char cFillValue = 0;
   char cValidRange[2];
   cValidRange[0] = 0;
   cValidRange[1] = 127;		
   char cScaleFactor =  1,cAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Grassland", 2, dim, Grassland, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Grassland", "Grassland","FALSE",&cFillValue, cValidRange, &cScaleFactor,&cAddOffset,"none", 
		"2KM", "Grassland","none","", 0);			
	 if(status < 0)
			exit(-1);				
		
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "woodland", 2, dim, woodland, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization woodland", "woodland","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "woodland","none","", 0);			
	 if(status < 0)
			exit(-1);							
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Others", 2, dim, Others, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Others", "Others","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Others","none","", 0);			
	 if(status < 0)
			exit(-1);		

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "CloudEdge", 2, dim, CloudEdge, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization CloudEdge", "CloudEdge","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Others","none","", 0);			
	 if(status < 0)
			exit(-1);						
	
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "ProvinceInfo", 2, dim, ProvinceInfo, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Information of provinces and cities", "Information of provinces and cities","FALSE",&iFillValue, iValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Information of provinces and cities","none","", 0);			
	 if(status < 0)
			exit(-1);		
			
//	int iData =0;
//	float fData = 0.0f;
//	int nTypeVal[4];
//	nTypeVal[0] = 0;
//	nTypeVal[1] = 1;
//	nTypeVal[2] = 2;
//	nTypeVal[3] = 3;	
//	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lat", &L1FileInfo_p->fNOMCenterLat,
//	 "nominal satellite subpoint latitude (platform latitude)", "Latitude", "degrees_north", 0);
//	
//	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lon", &L1FileInfo_p->fNOMCenterLon, 
//	"nominal satellite subpoint longitude (platformlongitude)", "Longitude", "degrees_east", 0);
//	
//	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_height", &L1FileInfo_p->fNOMSatHeight,
//	 "nominal satellite height above GRS 80 ellipsoid(platform altitude)",
//	  "height_above_reference_ellipsoid", "km", 0);
//
//	status =	myNetCDF.ZNetCDFWriteGeoExtentSDSData(strOutputFileName_NC,"geospatial_lat_lon_extent", &fData,
//	  "geospatial latitude and longitude references", 
//	  &(L1FileInfo_p->wBeginLineNumber), &(L1FileInfo_p->wEndLineNumber),	
//	  &(L1FileInfo_p->wBeginPixelNumber), &(L1FileInfo_p->wEndPixelNumber),
//	  &(L1FileInfo_p->fNOMCenterLat),&(L1FileInfo_p->fNOMCenterLon), 
//	  &(L1FileInfo_p->fRegLength), &(L1FileInfo_p->fRegWidth),
//	   "degrees_north", "degrees_east", 0);//
//
//  if(strstr(strAreaRange,"DISK")!=0)
//  	iData=0;
//  if(strstr(strAreaRange,"NHEM")!=0)
//  	iData=2;
//  if(strstr(strAreaRange,"SHEM")!=0)
//  	iData=1;
//  if(strstr(strAreaRange,"REG")!=0)
//  	iData=3;
//
//  status =	myNetCDF.ZNetCDFWriteOBISDSData(strOutputFileName_NC, "OBIType", &iData, 0, "Observing Type", 
//  "OBIType",nTypeVal,"0:Full_disk_observation 1:Southern_hemisphere_observation 2:Northern _hemisphere_observation 3:Regional observation");
//
//  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "processing_parm_version_container", &iData, 0,
//  "container for processing parameter package filename and product version","2016-10-10",0,"2016-10-10");
//
//  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "algorithm_product_version_container", &iData, 0,
//  "container for algorithm package filename and product version", "2016-10-10",0, "2016-10-10");	
////********20180412*******写string类型的数据集
//	char *sTest[3];
// float fScale = 1, fOffset = 1;
// int nDimCount = 3;
// for (int i = 0; i < nDimCount; i++)
// {
//  sTest[i] = (char*)malloc(128 * sizeof(char));
//  strcpy(sTest[i], "test");
// }
//  printf("-----------start write NC string sds------\n");
//  char sTemp[128] = {0};
//	status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_NC, "test", 1, &nDimCount, sTest, NC_STRING, "NULL", "NULL", "NULL", "NULL", "NULL", &fScale, &fOffset, "NULL", "NULL", "y x", "NULL", "NULL", 0,sTest);
//	printf("-----------END write NC string sds------\n");	
	
	
	//写全局属性
	if(myNetCDF.ZNetCDFWriteFY4GOESRL1BFileInfo(strOutputFileName_T3NC,&global_info,0) < 0 )
	{
		printf("write global attribute error!\n");
		ZLDSWriteLog('W',"write global attribute error!");
		return ZEATTOFNCWRITE;
	}	
	
	
	
	return 0;
}
int ProcessFire::OutputInfoNC1_AHI8(float *Lat,float *Lon,int *Number,int *NumberOfPixels,float *Area,char *Grassland,char *woodland,char *Others,char *CloudEdge,int *ProvinceInfo)
{
	printf("write NC \n");
	struct tm	structSysTm;
	time_t t;
	char strtime[15];		
	memset(strtime,0,sizeof(strtime));
	t=time(NULL);
	memcpy(&structSysTm,gmtime(&t),sizeof(struct tm));	
	sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02dZ",structSysTm.tm_year+1900,structSysTm.tm_mon+1,structSysTm.tm_mday,
																						 structSysTm.tm_hour,structSysTm.tm_min,structSysTm.tm_sec);
	GOESRNetCDFOperator myNetCDF;
	FY4_GOESR_L1B_FILE_INFO global_info;
	memset(&global_info,0,sizeof(FY4_GOESR_L1B_FILE_INFO));
	strcpy(global_info.strDatasetName,"FHS");
	strcpy(global_info.strNamingAuthority,"NSMC CMA");
	strcpy(global_info.strInstitution,"NSMC");	
	strcpy(global_info.strProject,"NOM");
	strcpy(global_info.strConventions,"CF-1.7");
	strcpy(global_info.strMetadataConventions,"Unidata Dataset Discovery v1.0");	
	strcpy(global_info.strSandardNameVocabul,"Fire/Hot Spot Characterization");	
	strcpy(global_info.strTitle,"AHI8 L2 FHS");
	strcpy(global_info.strSummary,"none");
	strcpy(global_info.strPlatformID,"H8");
	strcpy(global_info.strInstrumentType,"AHI8 Advanced Geosynchronous Radiation Imager");
	strcpy(global_info.strInstrumentID,"AHI");
	strcpy(global_info.strProcessingLevel,"L2");
	strcpy(global_info.strDateCreated,strtime);
	strcpy(global_info.strProductionSite,"NSMC");		
	strcpy(global_info.strProductionEnvironment,"UNIX");
	strcpy(global_info.strVOS,"V1.0");
	strcpy(global_info.strSRD,"2017-09-12");
	strcpy(global_info.strSceneId,"Full Disk possible values are Full Disk,Southern HEMisphere,the Northern HEMisphere,Regional,China Regional");
	strcpy(global_info.strSpatialResolution,"2km at nadir.");
	sprintf(global_info.strTimeCoverageStart,"%sT%sZ",L1FileInfo_p->strDateOfObservingBeginning,L1FileInfo_p->strTimeOfObservingBeginning);
	sprintf(global_info.strTimeCoverageEnd,"%sT%sZ",L1FileInfo_p->strDateOfObservingEnding,L1FileInfo_p->strTimeOfObservingEnding);
	////global_info.strDataQua=L1FileInfo_p->cDataQuality;
global_info.strDataQua=DataQ;
	global_info.strDataQua=DataQ;
	strcpy(global_info.nL0QualityFlag,strL0Q_p);
	strcpy(global_info.nPosQualityFlag,strPOSQ_p);
	strcpy(global_info.nCalQualityFlag,strCALQ_p);
	int status=-1;

   int  dim[2];
   dim[0] = iFires;
   dim[1] = 1;
   float fFillValue = -999;
   float fValidRange[2];
   fValidRange[0] = -90;
   fValidRange[1] = 90;
   float ScaleFactor =  1.0,AddOffset = 0;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Lat", 2, dim, Lat, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization latitude coordinate", "Latitute","FALSE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_north", 
		"2KM", "fire spot lat long","none","", 1);			
	 if(status < 0)
			exit(-1);

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Lon", 2, dim, Lon, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization longitude coordinate", "Longitude","FALSE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_east", 
		"2KM", "fire spot lat long","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   int iFillValue = 0;
   int iValidRange[2];
   iValidRange[0] = 0;
   iValidRange[1] = 1000000;		
   int iScaleFactor =  1,iAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Number", 2, dim, Number, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Number", "Number","FALSE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Number","none","", 0);			
	 if(status < 0)
			exit(-1);	
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "NumberOfPixels", 2, dim, NumberOfPixels, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Number", "Number","FALSE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Number","none","", 0);			
	 if(status < 0)
			exit(-1);				

   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Area", 2, dim, Area, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire spot area", "Fire spot area","FALSE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "fire spot","none", "",0);			
	 if(status < 0)
			exit(-1);	

   char cFillValue = 0;
   char cValidRange[2];
   cValidRange[0] = 0;
   cValidRange[1] = 127;		
   char cScaleFactor =  1,cAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Grassland", 2, dim, Grassland, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Grassland", "Grassland","FALSE",&cFillValue, cValidRange, &cScaleFactor,&cAddOffset,"none", 
		"2KM", "Grassland","none","", 0);			
	 if(status < 0)
			exit(-1);				
		
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "woodland", 2, dim, woodland, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization woodland", "woodland","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "woodland","none","", 0);			
	 if(status < 0)
			exit(-1);							
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "Others", 2, dim, Others, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Others", "Others","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Others","none","", 0);			
	 if(status < 0)
			exit(-1);		

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "CloudEdge", 2, dim, CloudEdge, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization CloudEdge", "CloudEdge","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Others","none","", 0);			
	 if(status < 0)
			exit(-1);						
	
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T3NC, "ProvinceInfo", 2, dim, ProvinceInfo, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Information of provinces and cities", "Information of provinces and cities","FALSE",&iFillValue, iValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Information of provinces and cities","none","", 0);			
	 if(status < 0)
			exit(-1);		
			
	int iData =0;
	float fData = 0.0f;
	int nTypeVal[4];
	nTypeVal[0] = 0;
	nTypeVal[1] = 1;
	nTypeVal[2] = 2;
	nTypeVal[3] = 3;	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lat", &L1FileInfo_p->fNOMCenterLat,
	 "nominal satellite subpoint latitude (platform latitude)", "Latitude", "degrees_north", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lon", &L1FileInfo_p->fNOMCenterLon, 
	"nominal satellite subpoint longitude (platformlongitude)", "Longitude", "degrees_east", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_height", &L1FileInfo_p->fNOMSatHeight,
	 "nominal satellite height above GRS 80 ellipsoid(platform altitude)",
	  "height_above_reference_ellipsoid", "km", 0);

	status =	myNetCDF.ZNetCDFWriteGeoExtentSDSData(strOutputFileName_NC,"geospatial_lat_lon_extent", &fData,
	  "geospatial latitude and longitude references", 
	  &(L1FileInfo_p->wBeginLineNumber), &(L1FileInfo_p->wEndLineNumber),	
	  &(L1FileInfo_p->wBeginPixelNumber), &(L1FileInfo_p->wEndPixelNumber),
	  &(L1FileInfo_p->fNOMCenterLat),&(L1FileInfo_p->fNOMCenterLon), 
	  &(L1FileInfo_p->fRegLength), &(L1FileInfo_p->fRegWidth),
	   "degrees_north", "degrees_east", 0);//

  if(strstr(strAreaRange,"DISK")!=0)
  	iData=0;
  if(strstr(strAreaRange,"NHEM")!=0)
  	iData=2;
  if(strstr(strAreaRange,"SHEM")!=0)
  	iData=1;
  if(strstr(strAreaRange,"REG")!=0)
  	iData=3;

  status =	myNetCDF.ZNetCDFWriteOBISDSData(strOutputFileName_NC, "OBIType", &iData, 0, "Observing Type", 
  "OBIType",nTypeVal,"0:Full_disk_observation 1:Southern_hemisphere_observation 2:Northern _hemisphere_observation 3:Regional observation");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "processing_parm_version_container", &iData, 0,
  "container for processing parameter package filename and product version","2016-10-10",0,"2016-10-10");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "algorithm_product_version_container", &iData, 0,
  "container for algorithm package filename and product version", "2016-10-10",0, "2016-10-10");	

	//写全局属性
	if(myNetCDF.ZNetCDFWriteFY4GOESRL1BFileInfo(strOutputFileName_T3NC,&global_info,0) < 0 )
	{
		printf("write global attribute error!\n");
		ZLDSWriteLog('W',"write global attribute error!");
		return ZEATTOFNCWRITE;
	}	
	
	return 0;
}
//---------------------------------------------------------------------------

/*: *******************  OutputFirePixInfo_ *********************
* 函数名称：  OutputFirePixInfo_
* 函数标题：  输出火点信息表
* 函数功能：  输出火点信息表
* 调用方法: OutputFirePixInfo_(void)
*   参数说明：
*   无
*
*
*   作    者：闫华
*   完成日期：2013年
************************* OutputFirePixInfo_ 函数代码 ***************************  :*/

void ProcessFire::OutputFirePixInfo_()
{
	//TODO: Add your source code here
	// 输出火点信息报表

   bool b1,b2;
   b1 = true;
   b2 = true;
   //20171124
  int  ia1 = 0, ia2 = 0, ja1 = 0, ja2 = 0;
   FILE *fpLandDat_p, *fpLandTxt_p;
   fpLandDat_p = NULL;
   fpLandTxt_p = NULL;

   if ( fpLandDat_p == NULL)
   {
      fpLandDat_p = fopen(strnLanding,"rb");
      fpLandTxt_p = fopen(strLandCover,"rt");
      if (!fpLandDat_p)
      {
         cout << "file open error!" <<endl;
				ZLDSWriteLog('W',"Open TXT_T3 error!");
				exit(ZEOPEN);
         b1 =!b1;
      }
   }

   FILE *fpXianJieDat_p, *fpXianJieTxt_p;
   fpXianJieDat_p = NULL;
   fpXianJieTxt_p = NULL;
   if (!fpXianJieDat_p)
   {
      fpXianJieDat_p = fopen(strxjraster,"rb");
      fpXianJieTxt_p = fopen(strxjdh_new,"rt");
      
      printf("%s %s\n",strxjraster,strxjdh_new);
      
      if (!fpXianJieDat_p)
      {
        b2 = !b2;
      }
   }
// ---------- 2017/11/24 可信度 --------------------------//

//20171124
	int iLine,iJraw;
	int iTry;
	
	for (int i = 1; i <= iFires; i++){
         iLine = fpi_[i].wY;
         iJraw = fpi_[i].wX;

         iTry =  m_wSatDataBuff_p[4][iLine * iDatWidth + iJraw] - wIR4BjAver_p[iLine][iJraw] ;

		 fpi_[i].TreL = 0;
		 if (iTry <= Try_DT4BJ )
             fpi_[i].TreL = 1;
		 if( Try_DT4BJ < iTry <= Try_DT4BJ2) 
		 	{
		 		fpi_[i].TreL = 2;
		 	}
		 	if(  iTry > Try_DT4BJ2 || m_cPixelId_p[iLine][iJraw - 1]  == 10  &&
		 		 m_wSatDataBuff_p[4][iLine * iDatWidth + iJraw] >= 3400 )
		 	{
		 		 fpi_[i].TreL = 3;	
		 	}
     if (  m_wSatDataBuff_p[4][iLine * iDatWidth + iJraw] < 3400 )    //20180113
		 {
			 if (m_cPixelId_p[iLine+1][iJraw] == 220 || m_cPixelId_p[iLine][iJraw + 1] == 220 || m_cPixelId_p[iLine-1][iJraw] == 220 
				 || m_cPixelId_p[iLine][iJraw - 1]  == 220       || m_cPixelId_p[iLine-1][iJraw -1] == 220 || m_cPixelId_p[iLine-1][iJraw + 1] == 220
				 || m_cPixelId_p[iLine+1][iJraw-1] == 220 || m_cPixelId_p[iLine+1][iJraw +1]  == 220 || Noefft_p[iLine][iJraw]== 220  //20180113
			     || CloudAver_p[iLine][iJraw] != NULL )                 //20180224
			 fpi_[i].TreL = 4;
		 }       

	}


    FILE *fpout;
    fpout = fopen(strOutputFileName_T2,"w");
    if (!fpout) 
    {
         cout << "new file error..." << endl;
         return;
    }
    string s1,s2,s0;
    s1 = fi.wYear + "-" + fi.wMonth + "-" + fi.wDay;
    s2 = fi.wHour + ":" + fi.wMinute;
    s1 = s1 + " " + s2;

    fprintf(fpout,"\r\n                                      气象卫星火情监测信息表                              \r\n\r\n");
    s0 = "观测时间（世界时）：" + s1 +  "                                       中国气象局国家卫星气象中心制作\r\n";
    fprintf(fpout,"%s\r\n",s0.c_str());
    fprintf(fpout,"BeginLine %4d  EndLine %4d\r\n",istartline,iendline);
    fprintf(fpout,"BeginPixel %4d  EndPixel %4d\r\n",istartpixel,iendpixel);
    fprintf(fpout,"------------------------------------------------------------------------------------------------------\r\n");
    fprintf(fpout,"火点编号  像元火区编号 可信度    纬度       经度    像素面积（平方公里） 明火区面积(公顷)   FRP         强度等级        土地类型      省地县\r\n");

    //@time:20161012 @author:ouyss    
    int *fireNumbers = new int[iFires];
    char *ConfidenceTemp = new char[iFires];
    float *fLatTemp = new float[iFires];
    float *fLonTemp = new float[iFires];
    float *AreaTemp = new float[iFires];
    float *FRPTemp = new float[iFires];
    char *PowerLevelTemp = new char[iFires];
    char *LandTypeTemp = new char[iFires];
    int *ProvinceInfoTemp = new int[iFires];



    for (int i = 1; i <= iFires; i++)
   	{
				fireNumbers[i-1] = fpi_[i].wNo;
				ConfidenceTemp[i-1] = fpi_[i].TreL;
				fLatTemp[i-1] = fpi_[i].fLat;
				fLonTemp[i-1] = fpi_[i].fLon;
				AreaTemp[i-1] = fpi_[i].fFireS * fpi_[i].fFireP * 100.0;
				FRPTemp[i-1] = fpi_[i].fNl;
				PowerLevelTemp[i-1] = fpi_[i].intst;
				LandTypeTemp[i-1] = LandCoverNameOfNum(fpi_[i].fLat, fpi_[i].fLon);
				ProvinceInfoTemp[i-1] = SrNameOfNum(fpXianJieDat_p,fpi_[i].fLat, fpi_[i].fLon);
        fprintf(fpout,"%4d    %3d    %3d      %4.2f      %4.2f        %8.2f           %8.2f         %4.2f        %4d    %16s         %s\r\n",
               fpi_[i].wNo,fpi_[i].wAreaNo, fpi_[i].TreL, fpi_[i].fLat, fpi_[i].fLon,fpi_[i].fFireS,
               fpi_[i].fFireS * fpi_[i].fFireP * 100.0, fpi_[i].fNl, fpi_[i].intst,LandCoverName1_(fpi_[i].fLat, fpi_[i].fLon).c_str(),
               SrName_(fpXianJieDat_p, fpXianJieTxt_p, fpi_[i].fLat, fpi_[i].fLon).c_str());                                                                //新疆维吾尔自治区
    }

    fclose (fpout);
    
/****************************************fire temp FPT TXT***************************************************************/   
  FILE* fp; 
	fp = fopen(strOutputEnFPTFile,"wb+");
	if(fp == NULL)
		{
			return;
		}
		fprintf(fp,"                                   FY-4A Hot Spot Pixel Information List                              \r\n");
		s0 = "Observation Time (UTC):" + s1 +  "                                       CMA/NSMC\r\n";
    fprintf(fpout,"%s\r\n",s0.c_str());	
		fprintf(fpout,"BeginLine %4d  EndLine %4d\r\n",istartline,iendline);
    fprintf(fpout,"BeginPixel %4d  EndPixel %4d\r\n",istartpixel,iendpixel);
		fprintf(fp,"Pixel_No.    Pixel_Hot_Spot_NO.    Reliability       Lat.           Long.     Pixel_Size(km)     Burned_Size(hm)       FRP(mw)       Intensity    Land_Type    Administrstive_area\r\n");
		for(int ii = 1;ii <= iFires; ii++ )
		{
			
			// fprintf(fp,"%10d    %10d       %4.2f       %5.2f     %10.2f     %15.2f       %3.2f       %9d     %9d    %10d\r\n",
			     fprintf(fp,"%-2d                     %-4d                                     %-4d            %-4.2f           %-4.2f                %-5.2f                         %-3.2f               %-4.2f            %-4d              %-4d            %-4d\r\n",
                fpi_[ii].wNo,       fpi_[ii].wAreaNo,   fpi_[ii].TreL,     fpi_[ii].fLat, fpi_[ii].fLon,fpi_[ii].fFireS, AreaTemp[ii-1], fpi_[ii].fNl, fpi_[ii].intst,LandCoverNameOfNum(fpi_[ii].fLat, fpi_[ii].fLon),
               SrNameOfNum(fpXianJieDat_p, fpi_[ii].fLat, fpi_[ii].fLon));    
		}		
		fclose(fp);  
   /**********************************************fire temp FPT TXT*********************************************************/ 
    if (b1)
    {
      fclose(fpLandDat_p);
      fclose(fpLandTxt_p);
      fpLandDat_p = NULL;
      fpLandTxt_p = NULL;
    }
    if (b2)
    {
      fclose(fpXianJieDat_p);
      fclose(fpXianJieTxt_p);
      fpXianJieDat_p = NULL;
      fpXianJieTxt_p = NULL;
    }
    if(strcmp(sataName,"FY4A")==0)
    {
    	OutputFirePixInfoOfNC2_FY4A(fLatTemp,fLonTemp,fireNumbers,ConfidenceTemp,AreaTemp,FRPTemp,PowerLevelTemp,LandTypeTemp,ProvinceInfoTemp);
 
    }
    else
    {
    		OutputFirePixInfoOfNC_AHI8(fLatTemp,fLonTemp,fireNumbers,ConfidenceTemp,AreaTemp,FRPTemp,PowerLevelTemp,LandTypeTemp,ProvinceInfoTemp);
     }
		   delete []fireNumbers;
    fireNumbers = NULL;
    delete []ConfidenceTemp;
    ConfidenceTemp = NULL;
		delete []fLatTemp;
		fLatTemp = NULL;
		delete []fLonTemp;
		fLonTemp = NULL;
		delete []AreaTemp;
		AreaTemp = NULL;
		delete []FRPTemp;
		FRPTemp = NULL;
		delete []PowerLevelTemp;
		PowerLevelTemp = NULL;
		delete []LandTypeTemp;
		LandTypeTemp = NULL;
		delete []ProvinceInfoTemp;
		ProvinceInfoTemp = NULL;

    return;

}

int ProcessFire::OutputFirePixInfoOfNC2_FY4A(float *Lat,float *Lon,int *Number,char *Confidence,float *Area,float *FRP,char *PowerLevel,char *LandType,int *ProvinceInfo)
{
	printf("write NC \n");
	struct tm	structSysTm;
	time_t t;
	char strtime[15];		
	memset(strtime,0,sizeof(strtime));
	t=time(NULL);
	memcpy(&structSysTm,gmtime(&t),sizeof(struct tm));	
	sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02dZ",structSysTm.tm_year+1900,structSysTm.tm_mon+1,structSysTm.tm_mday,
																						 structSysTm.tm_hour,structSysTm.tm_min,structSysTm.tm_sec);
	GOESRNetCDFOperator myNetCDF;
	FY4_GOESR_L1B_FILE_INFO global_info;
	memset(&global_info,0,sizeof(FY4_GOESR_L1B_FILE_INFO));
	strcpy(global_info.strDatasetName,"FHS");
	strcpy(global_info.strNamingAuthority,"NSMC CMA");
	strcpy(global_info.strInstitution,"NSMC");	
	strcpy(global_info.strProject,"NOM");
	strcpy(global_info.strConventions,"CF-1.7");
	strcpy(global_info.strMetadataConventions,"Unidata Dataset Discovery v1.0");	
	strcpy(global_info.strSandardNameVocabul,"Fire/Hot Spot Characterization");	
	strcpy(global_info.strTitle,"FY4A PGS L2 FHS");
	strcpy(global_info.strSummary,"none");
	strcpy(global_info.strPlatformID,"FY4A");
	strcpy(global_info.strInstrumentType,"FY4A Advanced Geosynchronous Radiation Imager");
	strcpy(global_info.strInstrumentID,"AGRI");
	strcpy(global_info.strProcessingLevel,"L2");
	strcpy(global_info.strDateCreated,strtime);
	strcpy(global_info.strProductionSite,"NSMC");		
	strcpy(global_info.strProductionEnvironment,"UNIX");
	strcpy(global_info.strVOS,"V1.0");
	strcpy(global_info.strSRD,"2017-09-12");
	strcpy(global_info.strSceneId,"Full Disk possible values are Full Disk,Southern HEMisphere,the Northern HEMisphere,Regional,China Regional");
	strcpy(global_info.strSpatialResolution,"2km at nadir.");
	sprintf(global_info.strTimeCoverageStart,"%sT%sZ",L1FileInfo_p->strDateOfObservingBeginning,L1FileInfo_p->strTimeOfObservingBeginning);
	sprintf(global_info.strTimeCoverageEnd,"%sT%sZ",L1FileInfo_p->strDateOfObservingEnding,L1FileInfo_p->strTimeOfObservingEnding);
	//global_info.strDataQua=L1FileInfo_p->cDataQuality;
    global_info.strDataQua=DataQ;
	strcpy(global_info.nL0QualityFlag,strL0Q_p);
	strcpy(global_info.nPosQualityFlag,strPOSQ_p);
	strcpy(global_info.nCalQualityFlag,strCALQ_p);
	int status=-1;	

   int  dim[2];
   dim[0] = iFires;
   dim[1] = 1;
   float fFillValue = -999;
   float fValidRange[2];
   fValidRange[0] = -90;
   fValidRange[1] = 90;
   float ScaleFactor =  1.0,AddOffset = 0;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Lat", 2, dim, Lat, NC_FLOAT,
		"FY4A PGS L2+ Fire/Hot Spot Characterization latitude coordinate", "Latitute","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_north", 
		"2KM", "fire spot lat long","none","", 1);			
	 if(status < 0)
			exit(-1);

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Lon", 2, dim, Lon, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization longitude coordinate", "Longitude","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_east", 
		"2KM", "fire spot lat long","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   int iFillValue = 0;
   int iValidRange[2];
   iValidRange[0] = 0;
   iValidRange[1] = 1000000;		
   int iScaleFactor =  1,iAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Number", 2, dim, Number, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Number", "Number","TRUE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Number","none","", 0);			
	 if(status < 0)
			exit(-1);	

   char cFillValue = 0;
   char cValidRange[2];
   cValidRange[0] = 0;
   cValidRange[1] = 127;		
   char cScaleFactor =  1,cAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Confidence", 2, dim, Confidence, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Fire confidence level", "confidence level","FALSE",&cFillValue, cValidRange, &cScaleFactor,&cAddOffset,"none", 
		"2KM", "fire spot Number","none","", 0);			
	 if(status < 0)
			exit(-1);				

   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Area", 2, dim, Area, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire spot area", "Fire spot area","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "fire spot","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "FRP", 2, dim, FRP, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Spot Power", "Fire Spot Power","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "fire spot","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "PowerLevel", 2, dim, PowerLevel, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Spot Power Level", "Fire Spot Power Level","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire Spot Power Level","none","", 0);			
	 if(status < 0)
			exit(-1);							
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "LandType", 2, dim, LandType, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Spot Land Type", "Fire Spot Land Type","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire Spot Land Type","none","", 0);			
	 if(status < 0)
			exit(-1);				
	
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "ProvinceInfo", 2, dim, ProvinceInfo, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Information of provinces and cities", "Information of provinces and cities","TRUE",&iFillValue, iValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Information of provinces and cities","none","", 0);			
	 if(status < 0)
			exit(-1);		
			
//	int iData =0;
//	float fData = 0.0f;
//	int nTypeVal[4];
//	nTypeVal[0] = 0;
//	nTypeVal[1] = 1;
//	nTypeVal[2] = 2;
//	nTypeVal[3] = 3;	
//	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lat", &L1FileInfo_p->fNOMCenterLat,
//	 "nominal satellite subpoint latitude (platform latitude)", "Latitude", "degrees_north", 0);
//	
//	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lon", &L1FileInfo_p->fNOMCenterLon, 
//	"nominal satellite subpoint longitude (platformlongitude)", "Longitude", "degrees_east", 0);
//	
//	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_height", &L1FileInfo_p->fNOMSatHeight,
//	 "nominal satellite height above GRS 80 ellipsoid(platform altitude)",
//	  "height_above_reference_ellipsoid", "km", 0);
//
//	status =	myNetCDF.ZNetCDFWriteGeoExtentSDSData(strOutputFileName_NC,"geospatial_lat_lon_extent", &fData,
//	  "geospatial latitude and longitude references", 
//	  &(L1FileInfo_p->wBeginLineNumber), &(L1FileInfo_p->wEndLineNumber),	
//	  &(L1FileInfo_p->wBeginPixelNumber), &(L1FileInfo_p->wEndPixelNumber),
//	  &(L1FileInfo_p->fNOMCenterLat),&(L1FileInfo_p->fNOMCenterLon), 
//	  &(L1FileInfo_p->fRegLength), &(L1FileInfo_p->fRegWidth),
//	   "degrees_north", "degrees_east", 0);//
//
//  if(strstr(strAreaRange,"DISK")!=0)
//  	iData=0;
//  if(strstr(strAreaRange,"NHEM")!=0)
//  	iData=2;
//  if(strstr(strAreaRange,"SHEM")!=0)
//  	iData=1;
//  if(strstr(strAreaRange,"REG")!=0)
//  	iData=3;
//
//  status =	myNetCDF.ZNetCDFWriteOBISDSData(strOutputFileName_NC, "OBIType", &iData, 0, "Observing Type", 
//  "OBIType",nTypeVal,"0:Full_disk_observation 1:Southern_hemisphere_observation 2:Northern _hemisphere_observation 3:Regional observation");
//
//  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "processing_parm_version_container", &iData, 0,
//  "container for processing parameter package filename and product version","2016-10-10",0,"2016-10-10");
//
//  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "algorithm_product_version_container", &iData, 0,
//  "container for algorithm package filename and product version", "2016-10-10",0, "2016-10-10");	

	//写全局属性
	if(myNetCDF.ZNetCDFWriteFY4GOESRL1BFileInfo(strOutputFileName_T2NC,&global_info,0) < 0 )
	{
		printf("write global attribute error!\n");
		ZLDSWriteLog('W',"write global attribute error!");
		return ZEATTOFNCWRITE;
	}	
	
	return 0;
}
int ProcessFire::OutputFirePixInfoOfNC_AHI8(float *Lat,float *Lon,int *Number,char *Confidence,float *Area,float *FRP,char *PowerLevel,char *LandType,int *ProvinceInfo)
{
	printf("write NC \n");
	struct tm	structSysTm;
	time_t t;
	char strtime[15];		
	memset(strtime,0,sizeof(strtime));
	t=time(NULL);
	memcpy(&structSysTm,gmtime(&t),sizeof(struct tm));	
	sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02dZ",structSysTm.tm_year+1900,structSysTm.tm_mon+1,structSysTm.tm_mday,
																						 structSysTm.tm_hour,structSysTm.tm_min,structSysTm.tm_sec);
	GOESRNetCDFOperator myNetCDF;
	FY4_GOESR_L1B_FILE_INFO global_info;
	memset(&global_info,0,sizeof(FY4_GOESR_L1B_FILE_INFO));
	strcpy(global_info.strDatasetName,"FHS");
	strcpy(global_info.strNamingAuthority,"NSMC CMA");
	strcpy(global_info.strInstitution,"NSMC");	
	strcpy(global_info.strProject,"NOM");
	strcpy(global_info.strConventions,"CF-1.7");
	strcpy(global_info.strMetadataConventions,"Unidata Dataset Discovery v1.0");	
	strcpy(global_info.strSandardNameVocabul,"Fire/Hot Spot Characterization");	
	strcpy(global_info.strTitle,"FY4A PGS L2 FHS");
	strcpy(global_info.strSummary,"none");
	strcpy(global_info.strPlatformID,"H8");
	strcpy(global_info.strInstrumentType,"AHI8 Advanced Geosynchronous Radiation Imager");
	strcpy(global_info.strInstrumentID,"AHI");
	strcpy(global_info.strProcessingLevel,"L2");
	strcpy(global_info.strDateCreated,strtime);
	strcpy(global_info.strProductionSite,"NSMC");		
	strcpy(global_info.strProductionEnvironment,"UNIX");
	strcpy(global_info.strVOS,"V1.0");
	strcpy(global_info.strSRD,"2017-09-12");
	strcpy(global_info.strSceneId,"Full Disk possible values are Full Disk,Southern HEMisphere,the Northern HEMisphere,Regional,China Regional");
	strcpy(global_info.strSpatialResolution,"2km at nadir.");
	sprintf(global_info.strTimeCoverageStart,"%sT%sZ",L1FileInfo_p->strDateOfObservingBeginning,L1FileInfo_p->strTimeOfObservingBeginning);
	sprintf(global_info.strTimeCoverageEnd,"%sT%sZ",L1FileInfo_p->strDateOfObservingEnding,L1FileInfo_p->strTimeOfObservingEnding);
	//global_info.strDataQua=L1FileInfo_p->cDataQuality;
global_info.strDataQua=DataQ;
	strcpy(global_info.nL0QualityFlag,strL0Q_p);
	strcpy(global_info.nPosQualityFlag,strPOSQ_p);
	strcpy(global_info.nCalQualityFlag,strCALQ_p);
	int status=-1;	

   int  dim[2];
   dim[0] = iFires;
   dim[1] = 1;
   float fFillValue = -999;
   float fValidRange[2];
   fValidRange[0] = -90;
   fValidRange[1] = 90;
   float ScaleFactor =  1.0,AddOffset = 0;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Lat", 2, dim, Lat, NC_FLOAT,
		"FY4A PGS L2+ Fire/Hot Spot Characterization latitude coordinate", "Latitute","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_north", 
		"2KM", "fire spot lat long","none","", 1);			
	 if(status < 0)
			exit(-1);

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Lon", 2, dim, Lon, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization longitude coordinate", "Longitude","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_east", 
		"2KM", "fire spot lat long","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   int iFillValue = 0;
   int iValidRange[2];
   iValidRange[0] = 0;
   iValidRange[1] = 1000000;		
   int iScaleFactor =  1,iAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Number", 2, dim, Number, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Number", "Number","TRUE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Number","none", "",0);			
	 if(status < 0)
			exit(-1);	

   char cFillValue = 0;
   char cValidRange[2];
   cValidRange[0] = 0;
   cValidRange[1] = 127;		
   char cScaleFactor =  1,cAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Confidence", 2, dim, Confidence, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Fire confidence level", "confidence level","FALSE",&cFillValue, cValidRange, &cScaleFactor,&cAddOffset,"none", 
		"2KM", "fire spot Number","none","", 0);			
	 if(status < 0)
			exit(-1);				

   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "Area", 2, dim, Area, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire spot area", "Fire spot area","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "fire spot","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "FRP", 2, dim, FRP, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Spot Power", "Fire Spot Power","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "fire spot","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "PowerLevel", 2, dim, PowerLevel, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Spot Power Level", "Fire Spot Power Level","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire Spot Power Level","none","", 0);			
	 if(status < 0)
			exit(-1);							
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "LandType", 2, dim, LandType, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Spot Land Type", "Fire Spot Land Type","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire Spot Land Type","none","", 0);			
	 if(status < 0)
			exit(-1);				
	
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T2NC, "ProvinceInfo", 2, dim, ProvinceInfo, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Information of provinces and cities", "Information of provinces and cities","TRUE",&iFillValue, iValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Information of provinces and cities","none","", 0);			
	 if(status < 0)
			exit(-1);		
			
	int iData =0;
	float fData = 0.0f;
	int nTypeVal[4];
	nTypeVal[0] = 0;
	nTypeVal[1] = 1;
	nTypeVal[2] = 2;
	nTypeVal[3] = 3;	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lat", &L1FileInfo_p->fNOMCenterLat,
	 "nominal satellite subpoint latitude (platform latitude)", "Latitude", "degrees_north", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lon", &L1FileInfo_p->fNOMCenterLon, 
	"nominal satellite subpoint longitude (platformlongitude)", "Longitude", "degrees_east", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_height", &L1FileInfo_p->fNOMSatHeight,
	 "nominal satellite height above GRS 80 ellipsoid(platform altitude)",
	  "height_above_reference_ellipsoid", "km", 0);

	status =	myNetCDF.ZNetCDFWriteGeoExtentSDSData(strOutputFileName_NC,"geospatial_lat_lon_extent", &fData,
	  "geospatial latitude and longitude references", 
	  &(L1FileInfo_p->wBeginLineNumber), &(L1FileInfo_p->wEndLineNumber),	
	  &(L1FileInfo_p->wBeginPixelNumber), &(L1FileInfo_p->wEndPixelNumber),
	  &(L1FileInfo_p->fNOMCenterLat),&(L1FileInfo_p->fNOMCenterLon), 
	  &(L1FileInfo_p->fRegLength), &(L1FileInfo_p->fRegWidth),
	   "degrees_north", "degrees_east", 0);//

  if(strstr(strAreaRange,"DISK")!=0)
  	iData=0;
  if(strstr(strAreaRange,"NHEM")!=0)
  	iData=2;
  if(strstr(strAreaRange,"SHEM")!=0)
  	iData=1;
  if(strstr(strAreaRange,"REG")!=0)
  	iData=3;

  status =	myNetCDF.ZNetCDFWriteOBISDSData(strOutputFileName_NC, "OBIType", &iData, 0, "Observing Type", 
  "OBIType",nTypeVal,"0:Full_disk_observation 1:Southern_hemisphere_observation 2:Northern _hemisphere_observation 3:Regional observation");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "processing_parm_version_container", &iData, 0,
  "container for processing parameter package filename and product version","2016-10-10",0,"2016-10-10");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "algorithm_product_version_container", &iData, 0,
  "container for algorithm package filename and product version", "2016-10-10",0, "2016-10-10");	

	//写全局属性
	if(myNetCDF.ZNetCDFWriteFY4GOESRL1BFileInfo(strOutputFileName_T2NC,&global_info,0) < 0 )
	{
		printf("write global attribute error!\n");
		ZLDSWriteLog('W',"write global attribute error!");
		return ZEATTOFNCWRITE;
	}	
	
	return 0;
}
//-------------------------------------------------------------------------

/*: *******************  OutPutInfo_ *********************
* 函数名称：  OutPutInfo_
* 函数标题：  输出火点信息表
* 函数功能：  输出火点信息表
* 调用方法: OutPutInfo_(void)
*   参数说明：
*   无
*
*
*   作    者：闫华
*   完成日期：2013年
************************* OutPutInfo_  函数代码 ***************************  :*/
void ProcessFire::OutPutInfo_(void)
{
		//TODO: Add your source code here
		FILE *fpout;
    fpout = fopen(strOutputFileName_T1,"w");
    if (!fpout)
    {
       exit(ZEWRITETXTAOFFHS);
    }
		cout << "start wirte " << strOutputFileName_T1 <<endl;
    string s1,s2;
    s1 = fi.wYear + "-" + fi.wMonth + "-" + fi.wDay;
    s2 = fi.wHour + ":" + fi.wMinute;
    s1 = s1 + " " + s2;
    //@time:20161012 @author:ouyss    
    float *fLatTemp = new float[iFires];
    float *fLonTemp = new float[iFires];
    int *fireNumbers = new int[iFires];
    int *ZoneNumber = new int[iFires];    
    int *ZoneNumberOfProv = new int[iFires];
    char *LandType = new char[iFires];
    float *Area = new float[iFires];
    float *PercentOfArea = new float[iFires];
    float *FireTemp = new float[iFires];    
    double *DateTime = new double[iFires];
    char *ClearSkyFlag = new char[iFires];
    float *FirePower = new float[iFires];
    //fprintf(fpout,"序号	行号	列号	纬度		经度		区号 行政分区号 土地利用类型 像元面积		像元百分比	火点温度	时间		晴空标记		能量估算 \r\n");
    fprintf(fpout,"序号	行号	列号	纬度		经度		区号  土地利用类型   像元面积		像元百分比	火点温度	时间		能量估算 \r\n");
    for (int i = 1; i <= iFires; i++)
    {
    	fLatTemp[i-1]= fpi_[i].fLat;
    	fLonTemp[i-1]= fpi_[i].fLon;
    	fireNumbers[i-1] = fpi_[i].wNo - 1;
    	ZoneNumber[i-1] = fpi_[i].wAreaNo - 1;
    	ZoneNumberOfProv[i-1]= fpi_[i].iBjNo;
    	LandType[i-1]=fpi_[i].wLandId;
    	Area[i-1]=fpi_[i].fFireS;
    	PercentOfArea[i-1]=fpi_[i].fFireP;
    	FireTemp[i-1]=fpi_[i].fFireTemp;
			DateTime[i - 1]=atof(s1.c_str());
			ClearSkyFlag[i-1]=fpi_[i].iQKId;
			FirePower[i-1]=fpi_[i].fNl;
      
//      fprintf(fpout,"%d\t%d\t%d\t%f\t%f\t%d\t%d\t%d\t%f\t%f\t%f\t%s\t%d\t%f\r\n",
//               fpi_[i].wNo - 1, fpi_[i].wY,fpi_[i].wX,fpi_[i].fLat, fpi_[i].fLon, fpi_[i].wAreaNo - 1,
//               fpi_[i].iBjNo, fpi_[i].wLandId, fpi_[i].fFireS, fpi_[i].fFireP, fpi_[i].fFireTemp,
//               s1.c_str(), fpi_[i].iQKId, fpi_[i].fNl);
 fprintf(fpout,"%d\t%d\t%d\t%f\t%f\t%d\t%d\t%f\t%f\t%f\t%s\t%f\r\n",
               fpi_[i].wNo - 1, fpi_[i].wY,fpi_[i].wX,fpi_[i].fLat, fpi_[i].fLon, fpi_[i].wAreaNo - 1,
               fpi_[i].wLandId, fpi_[i].fFireS, fpi_[i].fFireP, fpi_[i].fFireTemp,
               s1.c_str(),  fpi_[i].fNl);
    }
 /****************************************fire temp FPB TXT***************************************************************/   
//  FILE* fp;
//  memset(FPBTempFileName,0,sizeof(char)*256);
//	strcpy(FPBTempFileName,"/PGSDATA/FY4A/L2/AGRIX/FHS/FPB.txt");
//	fp = fopen(FPBTempFileName,"wb+");
//	if(fp == NULL)
//		{
//			return;
//		}
//		fprintf(fp,"  NUM        Line        Column        Lat          Lon      QUNO      LanVi      PixeS      PixelPerCent      FireT  Time  EnergyEst\r\n");
//		for(int j = 0;j < iAreas; j++ )
//		{
//			 fprintf(fp,"%10d %10d %10d %10f %10f %10d %10d %10f %10f %10f %10s %10f\r\n",
//               fpi_[j].wNo - 1, fpi_[j].wY,fpi_[i].wX,fpi_[j].fLat, fpi_[j].fLon, fpi_[j].wAreaNo - 1,
//               fpi_[j].wLandId, fpi_[j].fFireS, fpi_[j].fFireP, fpi_[j].fFireTemp,
//               s1.c_str(),fpi_[j].fNl);  
//		}		
//		fclose(fp);  
   /**********************************************fire temp FPB TXT*********************************************************/
    fclose (fpout);
     if(strcmp(sataName,"FY4A")==0)
     {
     	  OutputInfoNC1_FY4A(fLatTemp,fLonTemp,fireNumbers,ZoneNumber,ZoneNumberOfProv,LandType,Area,PercentOfArea,FireTemp,DateTime,ClearSkyFlag,FirePower);

     }
     else
     {
     	 OutPutInfoNC_AHI8(fLatTemp,fLonTemp,fireNumbers,ZoneNumber,ZoneNumberOfProv,LandType,Area,PercentOfArea,FireTemp,DateTime,ClearSkyFlag,FirePower);
     }
		
    delete []fireNumbers;
    fireNumbers = NULL;
		delete []ZoneNumber;
		ZoneNumber = NULL;
		delete []ZoneNumberOfProv;
		ZoneNumberOfProv = NULL;		
		delete []fLatTemp;
		fLatTemp = NULL;
		delete []fLonTemp;
		fLonTemp = NULL;
		delete []LandType;
		LandType = NULL;
		delete []Area;
		Area = NULL;
		delete []PercentOfArea;
		PercentOfArea = NULL;
		delete []FireTemp;
		FireTemp = NULL;
		delete []DateTime;
		DateTime = NULL;
		delete []ClearSkyFlag;
		ClearSkyFlag = NULL;
		delete []FirePower;
		FirePower = NULL;
    cout << "end..." << endl;
    return;
}

int ProcessFire::OutputInfoNC1_FY4A(float *Lat,float *Lon,int *Number,int *ZoneNumber,int *ZoneNumberOfProv,char *LandType,float *Area,
																float *PercentOfArea,float *FireTemp,	double *DateTime,char *ClearSkyFlag,float *FirePower)
{
	printf("write FY4A NC \n");
	struct tm	structSysTm;
	time_t t;
	char strtime[15];		
	memset(strtime,0,sizeof(strtime));
	t=time(NULL);
	memcpy(&structSysTm,gmtime(&t),sizeof(struct tm));	
	sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02dZ",structSysTm.tm_year+1900,structSysTm.tm_mon+1,structSysTm.tm_mday,
																						 structSysTm.tm_hour,structSysTm.tm_min,structSysTm.tm_sec);
	GOESRNetCDFOperator myNetCDF;
	FY4_GOESR_L1B_FILE_INFO global_info;
	memset(&global_info,0,sizeof(FY4_GOESR_L1B_FILE_INFO));
	strcpy(global_info.strDatasetName,"FHS");
	strcpy(global_info.strNamingAuthority,"NSMC CMA");
	strcpy(global_info.strInstitution,"NSMC");	
	strcpy(global_info.strProject,"NOM");
	strcpy(global_info.strConventions,"CF-1.7");
	strcpy(global_info.strMetadataConventions,"Unidata Dataset Discovery v1.0");	
	strcpy(global_info.strSandardNameVocabul,"Fire/Hot Spot Characterization");	
	strcpy(global_info.strTitle,"FY4A PGS L2 FHS");
	strcpy(global_info.strSummary,"none");
	strcpy(global_info.strPlatformID,"FY4A");
	strcpy(global_info.strInstrumentType,"FY4A Advanced Geosynchronous Radiation Imager");
	strcpy(global_info.strInstrumentID,"AGRI");
	strcpy(global_info.strProcessingLevel,"L2");
	strcpy(global_info.strDateCreated,strtime);
	strcpy(global_info.strProductionSite,"NSMC");		
	strcpy(global_info.strProductionEnvironment,"UNIX");
	strcpy(global_info.strVOS,"V1.0");
	strcpy(global_info.strSRD,"2017-09-12");
	strcpy(global_info.strSceneId,"Full Disk possible values are Full Disk,Southern HEMisphere,the Northern HEMisphere,Regional,China Regional");
	strcpy(global_info.strSpatialResolution,"2km at nadir.");
	sprintf(global_info.strTimeCoverageStart,"%sT%sZ",L1FileInfo_p->strDateOfObservingBeginning,L1FileInfo_p->strTimeOfObservingBeginning);
	sprintf(global_info.strTimeCoverageEnd,"%sT%sZ",L1FileInfo_p->strDateOfObservingEnding,L1FileInfo_p->strTimeOfObservingEnding);
	//global_info.strDataQua=L1FileInfo_p->cDataQuality;
global_info.strDataQua=DataQ;
	strcpy(global_info.nL0QualityFlag,strL0Q_p);
	strcpy(global_info.nPosQualityFlag,strPOSQ_p);
	strcpy(global_info.nCalQualityFlag,strCALQ_p);
	int status=-1;

   int  dim[2];
   dim[0] = iFires;
   dim[1] = 1;
   float fFillValue = -999;
   float fValidRange[2];
   fValidRange[0] = -90;
   fValidRange[1] = 90;
   float ScaleFactor =  1.0,AddOffset = 0;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "Lat", 2, dim, Lat, NC_FLOAT,
		"FY4A PGS L2+ Fire/Hot Spot Characterization latitude coordinate", "Latitute","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_north", 
		"2KM", "fire spot lat long","none","", 1);			
	 if(status < 0)
			exit(-1);

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "Lon", 2, dim, Lon, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization longitude coordinate", "Longitude","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_east", 
		"2KM", "fire spot lat long","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   int iFillValue = 0;
   int iValidRange[2];
   iValidRange[0] = 0;
   iValidRange[1] = 1000000;		
   int iScaleFactor =  1,iAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "Number", 2, dim, Number, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Number", "Number","TRUE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Number","none","", 0);			
	 if(status < 0)
			exit(-1);	

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "ZoneNumber", 2, dim, ZoneNumber, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization ZoneNumber", "ZoneNumber","TRUE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot ZoneNumber","none","", 0);			
	 if(status < 0)
			exit(-1);	

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "ZoneNumberOfProv", 2, dim, ZoneNumberOfProv, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Zone number of Province", "Zone number of Province","TRUE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Zone number of Province","none","", 0);			
	 if(status < 0)
			exit(-1);				

   char cFillValue = 0;
   char cValidRange[2];
   cValidRange[0] = 0;
   cValidRange[1] = 127;		
   char cScaleFactor =  1,cAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "LandType", 2, dim, LandType, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Spot Land Type", "Fire Spot Land Type","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire Spot Land Type","none","", 0);			
	 if(status < 0)
			exit(-1);				

   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "Area", 2, dim, Area, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire spot area", "Fire spot area","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "fire spot","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "PercentOfArea", 2, dim, PercentOfArea, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Percent of Area", "Percent of Area","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Percent of Area","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "FireTemp", 2, dim, FireTemp, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire temperature", "Fire temperature","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire temperature","none","", 0);			
	 if(status < 0)
			exit(-1);				
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "DateTime", 2, dim, DateTime, NC_DOUBLE,
		"AGRI L2+ Fire/Hot Spot Characterization DateTime", "DateTime","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "DateTime","none","", 0);			
	 if(status < 0)
			exit(-1);							
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "ClearSkyFlag", 2, dim, ClearSkyFlag, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Flag of clear sky", "Flag of clear sky","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Flag of clear sky","none","", 0);			
	 if(status < 0)
			exit(-1);				
	
   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "FirePower", 2, dim, FirePower, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Power", "Fire Power","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire Power","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
	int iData =0;
	float fData = 0.0f;
	int nTypeVal[4];
	nTypeVal[0] = 0;
	nTypeVal[1] = 1;
	nTypeVal[2] = 2;
	nTypeVal[3] = 3;	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lat", &L1FileInfo_p->fNOMCenterLat,
	 "nominal satellite subpoint latitude (platform latitude)", "Latitude", "degrees_north", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lon", &L1FileInfo_p->fNOMCenterLon, 
	"nominal satellite subpoint longitude (platformlongitude)", "Longitude", "degrees_east", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_height", &L1FileInfo_p->fNOMSatHeight,
	 "nominal satellite height above GRS 80 ellipsoid(platform altitude)",
	  "height_above_reference_ellipsoid", "km", 0);

	status =	myNetCDF.ZNetCDFWriteGeoExtentSDSData(strOutputFileName_NC,"geospatial_lat_lon_extent", &fData,
	  "geospatial latitude and longitude references", 
	  &(L1FileInfo_p->wBeginLineNumber), &(L1FileInfo_p->wEndLineNumber),	
	  &(L1FileInfo_p->wBeginPixelNumber), &(L1FileInfo_p->wEndPixelNumber),
	  &(L1FileInfo_p->fNOMCenterLat),&(L1FileInfo_p->fNOMCenterLon), 
	  &(L1FileInfo_p->fRegLength), &(L1FileInfo_p->fRegWidth),
	   "degrees_north", "degrees_east", 0);//

  if(strstr(strAreaRange,"DISK")!=0)
  	iData=0;
  if(strstr(strAreaRange,"NHEM")!=0)
  	iData=2;
  if(strstr(strAreaRange,"SHEM")!=0)
  	iData=1;
  if(strstr(strAreaRange,"REG")!=0)
  	iData=3;

  status =	myNetCDF.ZNetCDFWriteOBISDSData(strOutputFileName_NC, "OBIType", &iData, 0, "Observing Type", 
  "OBIType",nTypeVal,"0:Full_disk_observation 1:Southern_hemisphere_observation 2:Northern _hemisphere_observation 3:Regional observation");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "processing_parm_version_container", &iData, 0,
  "container for processing parameter package filename and product version","2016-10-10",0,"2016-10-10");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "algorithm_product_version_container", &iData, 0,
  "container for algorithm package filename and product version", "2016-10-10",0, "2016-10-10");	

	//写全局属性
	if(myNetCDF.ZNetCDFWriteFY4GOESRL1BFileInfo(strOutputFileName_T1NC,&global_info,0) < 0 )
	{
		printf("write global attribute error!\n");
		ZLDSWriteLog('W',"write global attribute error!");
		return ZEATTOFNCWRITE;
	}		
	return 0;

}
int ProcessFire::OutPutInfoNC_AHI8(float *Lat,float *Lon,int *Number,int *ZoneNumber,int *ZoneNumberOfProv,char *LandType,float *Area,
																float *PercentOfArea,float *FireTemp,	double *DateTime,char *ClearSkyFlag,float *FirePower)
{
	printf("write AHI8 NC \n");
	struct tm	structSysTm;
	time_t t;
	char strtime[15];		
	memset(strtime,0,sizeof(strtime));
	t=time(NULL);
	memcpy(&structSysTm,gmtime(&t),sizeof(struct tm));	
	sprintf(strtime,"%04d-%02d-%02dT%02d:%02d:%02dZ",structSysTm.tm_year+1900,structSysTm.tm_mon+1,structSysTm.tm_mday,
																						 structSysTm.tm_hour,structSysTm.tm_min,structSysTm.tm_sec);
	GOESRNetCDFOperator myNetCDF;
	FY4_GOESR_L1B_FILE_INFO global_info;
	memset(&global_info,0,sizeof(FY4_GOESR_L1B_FILE_INFO));
	strcpy(global_info.strDatasetName,"FHS");
	strcpy(global_info.strNamingAuthority,"NSMC CMA");
	strcpy(global_info.strInstitution,"NSMC");	
	strcpy(global_info.strProject,"NOM");
	strcpy(global_info.strConventions,"CF-1.7");
	strcpy(global_info.strMetadataConventions,"Unidata Dataset Discovery v1.0");	
	strcpy(global_info.strSandardNameVocabul,"Fire/Hot Spot Characterization");	
	strcpy(global_info.strTitle,"FY4A PGS L2 FHS");
	strcpy(global_info.strSummary,"none");
	strcpy(global_info.strPlatformID,"H8");
	strcpy(global_info.strInstrumentType,"AHI8 Advanced Geosynchronous Radiation Imager");
	strcpy(global_info.strInstrumentID,"AHI");
	strcpy(global_info.strProcessingLevel,"L2");
	strcpy(global_info.strDateCreated,strtime);
	strcpy(global_info.strProductionSite,"NSMC");		
	strcpy(global_info.strProductionEnvironment,"UNIX");
	strcpy(global_info.strVOS,"V1.0");
	strcpy(global_info.strSRD,"2017-09-12");
	strcpy(global_info.strSceneId,"Full Disk possible values are Full Disk,Southern HEMisphere,the Northern HEMisphere,Regional,China Regional");
	strcpy(global_info.strSpatialResolution,"2km at nadir.");
	sprintf(global_info.strTimeCoverageStart,"%sT%sZ",L1FileInfo_p->strDateOfObservingBeginning,L1FileInfo_p->strTimeOfObservingBeginning);
	sprintf(global_info.strTimeCoverageEnd,"%sT%sZ",L1FileInfo_p->strDateOfObservingEnding,L1FileInfo_p->strTimeOfObservingEnding);
	//global_info.strDataQua=L1FileInfo_p->cDataQuality;
global_info.strDataQua=DataQ;
	strcpy(global_info.nL0QualityFlag,strL0Q_p);
	strcpy(global_info.nPosQualityFlag,strPOSQ_p);
	strcpy(global_info.nCalQualityFlag,strCALQ_p);
	int status=-1;

   int  dim[2];
   dim[0] = iFires;
   dim[1] = 1;
   float fFillValue = -999;
   float fValidRange[2];
   fValidRange[0] = -90;
   fValidRange[1] = 90;
   float ScaleFactor =  1.0,AddOffset = 0;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "Lat", 2, dim, Lat, NC_FLOAT,
		"FY4A PGS L2+ Fire/Hot Spot Characterization latitude coordinate", "Latitute","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_north", 
		"2KM", "fire spot lat long","none","", 1);			
	 if(status < 0)
			exit(-1);

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "Lon", 2, dim, Lon, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization longitude coordinate", "Longitude","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"degrees_east", 
		"2KM", "fire spot lat long","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   int iFillValue = 0;
   int iValidRange[2];
   iValidRange[0] = 0;
   iValidRange[1] = 1000000;		
   int iScaleFactor =  1,iAddOffset = 0;			
     
	       status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "Number", 2, dim, Number, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Number", "Number","TRUE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Number","none", "",0);			
	 if(status < 0)
			exit(-1);	

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "ZoneNumber", 2, dim, ZoneNumber, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization ZoneNumber", "ZoneNumber","TRUE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot ZoneNumber","none","", 0);			
	 if(status < 0)
			exit(-1);	

	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "ZoneNumberOfProv", 2, dim, ZoneNumberOfProv, NC_INT,
		"AGRI L2+ Fire/Hot Spot Characterization Zone number of Province", "Zone number of Province","TRUE",&iFillValue, iValidRange, &iScaleFactor,&iAddOffset,"none", 
		"2KM", "fire spot Zone number of Province","none","", 0);			
	 if(status < 0)
			exit(-1);				

   char cFillValue = 0;
   char cValidRange[2];
   cValidRange[0] = 0;
   cValidRange[1] = 127;		
   char cScaleFactor =  1,cAddOffset = 0;			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "LandType", 2, dim, LandType, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Spot Land Type", "Fire Spot Land Type","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire Spot Land Type","none","", 0);			
	 if(status < 0)
			exit(-1);				

   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "Area", 2, dim, Area, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire spot area", "Fire spot area","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "fire spot","none", "",0);			
	 if(status < 0)
			exit(-1);	
			
   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "PercentOfArea", 2, dim, PercentOfArea, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Percent of Area", "Percent of Area","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Percent of Area","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "FireTemp", 2, dim, FireTemp, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire temperature", "Fire temperature","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire temperature","none","", 0);			
	 if(status < 0)
			exit(-1);				
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "DateTime", 2, dim, DateTime, NC_DOUBLE,
		"AGRI L2+ Fire/Hot Spot Characterization DateTime", "DateTime","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "DateTime","none","", 0);			
	 if(status < 0)
			exit(-1);							
			
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "ClearSkyFlag", 2, dim, ClearSkyFlag, NC_BYTE,
		"AGRI L2+ Fire/Hot Spot Characterization Flag of clear sky", "Flag of clear sky","FALSE",&cFillValue, cValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Flag of clear sky","none","", 0);			
	 if(status < 0)
			exit(-1);				
	
   fValidRange[0] = 0;
   fValidRange[1] = 100000;
	 status = myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_T1NC, "FirePower", 2, dim, FirePower, NC_FLOAT,
		"AGRI L2+ Fire/Hot Spot Characterization Fire Power", "Fire Power","TRUE",&fFillValue, fValidRange, &ScaleFactor,&AddOffset,"none", 
		"2KM", "Fire Power","none","", 0);			
	 if(status < 0)
			exit(-1);	
			
	int iData =0;
	float fData = 0.0f;
	int nTypeVal[4];
	nTypeVal[0] = 0;
	nTypeVal[1] = 1;
	nTypeVal[2] = 2;
	nTypeVal[3] = 3;	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lat", &L1FileInfo_p->fNOMCenterLat,
	 "nominal satellite subpoint latitude (platform latitude)", "Latitude", "degrees_north", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_subpoint_lon", &L1FileInfo_p->fNOMCenterLon, 
	"nominal satellite subpoint longitude (platformlongitude)", "Longitude", "degrees_east", 0);
	
	status =	myNetCDF.ZNetCDFWriteFourSDSData(strOutputFileName_NC, "nominal_satellite_height", &L1FileInfo_p->fNOMSatHeight,
	 "nominal satellite height above GRS 80 ellipsoid(platform altitude)",
	  "height_above_reference_ellipsoid", "km", 0);

	status =	myNetCDF.ZNetCDFWriteGeoExtentSDSData(strOutputFileName_NC,"geospatial_lat_lon_extent", &fData,
	  "geospatial latitude and longitude references", 
	  &(L1FileInfo_p->wBeginLineNumber), &(L1FileInfo_p->wEndLineNumber),	
	  &(L1FileInfo_p->wBeginPixelNumber), &(L1FileInfo_p->wEndPixelNumber),
	  &(L1FileInfo_p->fNOMCenterLat),&(L1FileInfo_p->fNOMCenterLon), 
	  &(L1FileInfo_p->fRegLength), &(L1FileInfo_p->fRegWidth),
	   "degrees_north", "degrees_east", 0);//

  if(strstr(strAreaRange,"DISK")!=0)
  	iData=0;
  if(strstr(strAreaRange,"NHEM")!=0)
  	iData=2;
  if(strstr(strAreaRange,"SHEM")!=0)
  	iData=1;
  if(strstr(strAreaRange,"REG")!=0)
  	iData=3;

  status =	myNetCDF.ZNetCDFWriteOBISDSData(strOutputFileName_NC, "OBIType", &iData, 0, "Observing Type", 
  "OBIType",nTypeVal,"0:Full_disk_observation 1:Southern_hemisphere_observation 2:Northern _hemisphere_observation 3:Regional observation");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "processing_parm_version_container", &iData, 0,
  "container for processing parameter package filename and product version","2016-10-10",0,"2016-10-10");

  status =	myNetCDF.ZNetCDFWriteContainerSDSData(strOutputFileName_NC, "algorithm_product_version_container", &iData, 0,
  "container for algorithm package filename and product version", "2016-10-10",0, "2016-10-10");	

	//写全局属性
	if(myNetCDF.ZNetCDFWriteFY4GOESRL1BFileInfo(strOutputFileName_T1NC,&global_info,0) < 0 )
	{
		printf("write global attribute error!\n");
		ZLDSWriteLog('W',"write global attribute error!");
		return ZEATTOFNCWRITE;
	}		
	return 0;

}
//------------------------------------------

//------------------------------------------

/*: *******************  LandVI_ *********************
* 函数名称：  LandVI_
* 函数标题：  设置土地类型编号
* 函数功能：  设置土地类型编号
* 调用方法: LandVI_(FILE *fpLandDat_, float m_fLat_p, float fLong)
*   参数说明：
*   FILE *fpLandDat_   土地类型数据文件指针
*   float m_fLat_p    纬度
*   float fLong   经度
*
*   作    者：闫华
*   完成日期：2013年
************************* LandVI_  函数代码 ***************************  :*/
unsigned char ProcessFire::LandVI_(FILE *fpLandDat_, float m_fLat_p, float fLong)
{

   unsigned char num1,num;

   int x0,y0;
   num1 = 0;
   num = 0;
   x0 = (int)((fLong - 70.) *100);
   y0 = (int)((56. - m_fLat_p) *100);

   if (x0 >= 0 && x0 < 7100 && y0 >= 0 && y0 < 4900)
   {  //  在范围内
      long  int k;

      k = (long  int)(y0 * 7100 + x0);

      fseek(fpLandDat_,k,0);
      fread(&num1,sizeof(unsigned char),1,fpLandDat_);

      switch (num1) {
        case 1:
           num = 12;
           break;
        case 3:
           num = 7;
           break;
        case 5:
           num = 7;
           break;
        case 6:
           num = 7;
           break;
        case 7:
           num = 7;
           break;
        case 8:
           num = 7;
           break;
        case 9:
           num = 12;
           break;
        case 10:
           num = 4;
           break;
        case 11:
           num = 6;
           break;
        case 12:
           num = 12;
           break;
        case 13:
           num = 12;
           break;
        case 14:
           num = 12;
           break;
        case 15:
           num = 12;
           break;
        case 20:
           num = 8;
           break;
        case 21:
           num = 6;
           break;
        case 22:
           num = 6;
           break;
        case 41:
           num = 8;
           break;
        case 42:
           num = 8;
           break;
        default:
           num = 12;
      }
   }

   return num;
}

//-------------------------------------------------------------

//---------------------------------------------------------------------

/*: *******************  SrName_ *********************
* 函数名称：  SrName_
* 函数标题：  查找行政区划名称
* 函数功能：  返回行政区划名称
* 调用方法: string SrName_(FILE *fpXianJieDat_, FILE *fpXianJieTxt_, float fLat0, float fLong0)
*   参数说明：
*   FILE *fpXianJieDat_  县界数据文件指针
*   FILE *fpXianJieTxt_  县界名称文件指针
*   float fLat0    纬度
*   float fLong0   经度
*
*   作    者：闫华
*   完成日期：2013年
************************* SrName_ 函数代码 ***************************  :*/

/*string ProcessFire::SrName_(FILE *fpXianJieDat_, FILE *fpXianJieTxt_, float fLat0, float fLong0)
{
   char sss[256];
   char cTmpChar[256];
	 memset(sss,0,sizeof(sss));
	 memset(cTmpChar,0,sizeof(cTmpChar));
	 
   float num1,num;
   num1 = 0.0;

   int x0,y0;

   x0 = (int)((fLong0 - 70.) *100);
   y0 = (int)((56. - fLat0) *100);

   //x0 = fLong0 * 4  - 70 * 400;        // 70 is minlon
   //y0 = 56 * 400 - fLat0 * 4;          // 56 is maxlat

   if (x0 >= 0 && x0 < 7100 && y0 >= 0 && y0 < 4900)  //y===>4100???
   {  //  在范围内
     long int k;

     k = (long int)((y0 * 7100 + x0) * 4);
     fseek(fpXianJieDat_,k,0);

     fread(&num1,sizeof(float),1,fpXianJieDat_);
     if (num1 == 0)
     {
       return (string)("中国区域外.");
     }

     fseek(fpXianJieTxt_,0,0);
     while(fscanf(fpXianJieTxt_,"%s",sss)!=-1)
     {
       num = atof(sss);   //每个省市编号

       fscanf(fpXianJieTxt_,"%s",cTmpChar);

       if (num == num1)
       {
          return ((string)(cTmpChar));
       }
     }
     return (string)("中国区域外..");
   }
   else
     return (string)("中国区域外...");

}*/

string ProcessFire::SrName_(FILE *fpXianJieDat_, FILE *fpXianJieTxt_, float fLat0, float fLong0)
{
   char sss[120];
   char cTmpChar[200];
   string str1;

   float num1,num;
   num1 = 0.0;

   int x0,y0;

   x0 = (int)((fLong0 - 70.) *100);
   y0 = (int)((56. - fLat0) *100);

   //x0 = fLong0 * 4  - 70 * 400;        // 70 is minlon
   //y0 = 56 * 400 - fLat0 * 4;          // 56 is maxlat

   if (x0 >= 0 && x0 < 7100 && y0 >= 0 && y0 < 4900)
   	{  //  在范围内
     long int k;

     k = (long int)((y0 * 7100 + x0) * 4);
     fseek(fpXianJieDat_,k,0);
     
     unsigned char num1_char[4];
     fread(&num1_char,sizeof(float),1,fpXianJieDat_);
     //fread(&num1,1,sizeof(float),fpXianJieDat_);
    
     ExchangeDWord(num1_char);
     
     memcpy(&num1,num1_char,4);
   
     if (num1 == 0)
     	{
       return (string)("中国区域外");
     }

     fseek(fpXianJieTxt_,0,0);
     for (;fscanf(fpXianJieTxt_,"%s",sss)!=-1 ;)
     {
       str1 = (string)(sss);

       num = atof(sss);   // zi fu chuan zhuan huan fu dian shu
       fscanf(fpXianJieTxt_,"%s",cTmpChar);
       if (num == num1)
       {
          return ((string)(cTmpChar));
       
       }
     }
     return (string)("中国区域外");
   }
   else
     return (string)("中国区域外");
}

//int ProcessFire::SrNameOfNum(FILE *fpXianJieDat_, float fLat0, float fLong0)
//{
//   float num1;
//   num1 = 0.0;
//
//   int x0,y0;
//
//   x0 = (int)((fLong0 - 70.) *100);
//   y0 = (int)((56. - fLat0) *100);
//
//   if (x0 >= 0 && x0 < 7100 && y0 >= 0 && y0 < 4900)
//   {  //  在范围内
//     long int k;
//
//     k = (long int)((y0 * 7100 + x0) * 4);
//     fseek(fpXianJieDat_,k,0);
//
//     unsigned char num1_char[4];
//     fread(&num1_char,sizeof(float),1,fpXianJieDat_);
//     ExchangeDWord(num1_char);
//     memcpy(&num1,num1_char,4);
//    printf("****************num1 %d\n",num1); 
//     //fread(&num1,sizeof(float),1,fpXianJieDat_);
//   }
//   else
//   	{   		
//      	num1 = -9999;
//   	}
//    
//   return (int)num1;
//
//}
int ProcessFire::SrNameOfNum(FILE *fpXianJieDat_, float fLat0, float fLong0)
{
   float num1;
   num1 = 0.0;
   unsigned char num1_char[4];
   int x0,y0;

   x0 = (int)((fLong0 - 70.) *100);
   y0 = (int)((56. - fLat0) *100);

   if (x0 >= 0 && x0 < 7100 && y0 >= 0 && y0 < 4900)
   {  //  在范围内
     long int k;

     k = (long int)((y0 * 7100 + x0) * 4);
     fseek(fpXianJieDat_,k,0);

   
     fread(&num1_char,sizeof(float),1,fpXianJieDat_);
    
     ExchangeDWord(num1_char);
     memcpy(&num1,num1_char,4);
     
      if((int)num1 <= 0)
     	{
     		num1 = -9999.;
     	}
  
     //fread(&num1,sizeof(float),1,fpXianJieDat_);
   }
   else
   	{   		
      	num1 = -9999.;
   	}
   //printf("************sunr**********num1 %f  num1_char[0] %d num1_char[1] %d,num1_char[2] %d\n",num1,num1_char[0],num1_char[1],num1_char[2]); 
   return (int)num1;

}
//---------------------------------------------------------------------------------



//-------------------------------------------------------------------

/*: *******************  LandCoverName1_ *********************
* 函数名称：  LandCoverName1_
* 函数标题：  查找土地类型名称
* 函数功能：  返回土地类型名称
* 调用方法: string LandCoverName1_(float fLat0, float fLong0)
*   参数说明：
*
*   float fLat0    纬度
*   float fLong0   经度
*
*   作    者：闫华
*   完成日期：2013年
************************* LandCoverName1_ 函数代码 ***************************  :*/

string ProcessFire::LandCoverName1_(float fLat0, float fLong0)
{
   string str1;

   FILE *fp_LandDat;
   fp_LandDat = NULL;
   fp_LandDat  = fopen(strLandImg,"rb");//全球植被覆盖数据分类

   unsigned char num1;   //,num;

   int x0,y0;
   num1 = 0;
   x0 = (int)((fLong0 + 180.) * 100);
   y0 = (int)((90. - fLat0) * 100);

   if (x0 > 36000 || x0 < 0 || y0 > 18000 || y0 < 0)
   {
       fclose (fp_LandDat);
       return (string)("其他");
   }

  // x0 = fLong0 * 4  - 70 * 400;        // 70 is minlon
  // y0 = 56 * 400 - fLat0 * 4;          // 56 is maxlat

   long  int k;

   k = (long  int)(y0 * 36000 + x0);
   fseek(fp_LandDat,k,0);
   fread(&num1,sizeof(unsigned char),1,fp_LandDat);

   fclose (fp_LandDat);

   if (num1 == 1){
      return (string ("草地"));
   }
   else if (num1 == 2){
      return (string ("林地"));
   }
   else if (num1 == 3){
      return (string ("农地"));
   }
   else if (num1 == 4){
      return (string ("其他"));
   }
   else if (num1 == 5){
      return (string ("海洋"));
   }
   else {
      return (string)("其他");
   }
}

char ProcessFire::LandCoverNameOfNum(float fLat0, float fLong0)
{
   FILE *fp_LandDat = NULL;
   fp_LandDat  = fopen(strLandImg,"rb");

   unsigned char num1 = 0;   
   int x0,y0;
   num1 = 0;
   x0 = (int)((fLong0 + 180.) * 100);
   y0 = (int)((90. - fLat0) * 100);

   if (x0 > 36000 || x0 < 0 || y0 > 18000 || y0 < 0)
   {
       fclose (fp_LandDat);
       return 0;
   }
   long k;

   k = (long  int)(y0 * 36000 + x0);
   fseek(fp_LandDat,k,0);
   fread(&num1,sizeof(unsigned char),1,fp_LandDat);

   fclose (fp_LandDat);
   
   return num1;
}

//--------------------------------------------------------------

//-----------------------------------------------------------------


double ProcessFire::B1(double k,double n)       // 计算温度，利用波数和辐亮度
{
	 double c1,c2,result;
	 c1=1.1910659e-5;	 c2=1.438853;
     result=c2*k/log(1+c1*pow(k,3)/n);
	 return result;
}

double ProcessFire::B2(double k,double t)      // 计算辐射率，利用波数和温度
{
	 double c1,c2,result;
	 c1=1.1910659e-5;	 c2=1.438853;

     if ( fabs(t) < 0.00000001 || fabs(exp(c2*k/t)-1) < 0.00000001)
     {
        subpxx = 100;
        return 1;
     }

      if(fabs(t) < 0.0000000001 || fabs(exp(c2*k/t)-1) < 0.0000000001)
      {
                int bb = 0;
      }

     result = c1*pow(k,3)/(exp(c2*k/t)-1);

	 return result;
}

double ProcessFire::B3(double k,double t,double p)
{
	double c1,c2,result;
	c1=1.1910659e-5;
	c2=1.438853;       
     if ( fabs(t) < 0.00000001 || fabs(exp(c2*k/t)-1) < 0.00000001) 
     {
        subpxx = 100;
        return 1;
     }
	result=p*c1*c2*pow(k,4)*exp(c2*k/t)/(pow((exp(c2*k/t)-1),2)*t*t);
 	return result;
}

double ProcessFire::J1(double p,double t,double k3,double k4,double N3b,double N4b,double N3mix,double N4mix)
{
	double result;
	result=B3(k3,t,p)*(p*B2(k4,t)+(1-p)*B2(k4,N4b)-B2(k4,N4mix))
		  -B3(k4,t,p)*(p*B2(k3,t)+(1-p)*B2(k3,N3b)-B2(k3,N3mix));
	return result;
}

double ProcessFire::J2(double p,double t,double k3,double k4,double N3b,double N4b,double N3mix,double N4mix)
{
	double result;
	result=(p*B2(k3,t)+(1-p)*B2(k3,N3b)-B2(k3,N3mix))*(B2(k4,t)-B2(k4,N4b))
		  -(p*B2(k4,t)+(1-p)*B2(k4,N4b)-B2(k4,N4mix))*(B2(k3,t)-B2(k3,N3b));
	return result;
}

double ProcessFire::J0(double p,double t,double k3,double k4,double N3b,double N4b)
{
	double result;
	result=(B2(k3,t)-B2(k3,N3b))*B3(k4,t,p)-(B2(k4,t)-B2(k4,N4b))*B3(k3,t,p);
	return result;
}

void ProcessFire::SubPixel_1(float ch3,float ch4,float ch3b,float ch4b, double* pp, double* pt)
{
	int n1; //,N31,N32,N41,N42;
	double p0,t0,p,t,k3,k4;
	double N3b,N4b,N3mix,N4mix;
	//double testt,ntest,n3ft,n4ft,ft3,ft4,n3mix1,n4mix1,n3bg,n4bg,p3,p4;
	double n3ft,n4ft,ft3,p4;
	k3=2650.0;
	k4=921.0;


  //t0=664;    // ****  20120413 new
  //p0=0.0004;   // ****  20120413 new

	N3b=ch3b;
	N3mix=ch3;
	N4b=ch4b;
	N4mix=ch4;
	if(N4b>=N4mix) N4b=N4mix-0.4;
	/*if(N4mix>=N3b) N3b=N4mix+4;*/

	double p_lower,p_upper,p_intermy;  //ft31,ft41,

	// 计算P初始值开始 *****************************************************
	// 设P初值
	p0 = 0.5;
	p_intermy = 0.5;
	if ( p0 > 0.0001)
	{ 
		do 
		{
	
	   // 第1步，将P值带入通道3混合像元公式，先计算火点辐亮度n3ft，再计算通道3火点温度3ft   （N3mix-（ 1 – P ）N3bg）/ P
		
		p0 = p_intermy;
		n3ft = (B2(k3,N3mix)-(1-p0)*B2(k3,N3b))/p0; // 计算通道3火点辐亮度
	    ft3 = B1(k3,n3ft); // 计算 通道3火点温度
	    t0 = ft3;
	
	
	// 第2步，将上式获得的通道3温度代入通道4混合像元公式，得到通道4混合像元公式的P值
	// 通道4混合像元公式中P值计算公式：P4 = （N4mix -  N4bg）/ （N4ft -  N4bg）
	// 先计算通道4火点辐亮度n4ft,混合混合像元辐亮度n4mix1,背景辐亮度n4bg
	   n4ft = B2(k4,ft3);
	   p4 = ( B2(k4,N4mix)-B2(k4,N4b))/(B2(k4,ft3)- B2(k4,N4b) );
	
	   //printf("计算P =      %1f \n\n",p4);
	
	// 第3步，将上式得到的P值，代入二分法公式中，计算P值的中间值
	   p_lower = log(p4);
	   p_upper = log(p0);
	   p_intermy = p_lower + p_upper - p_lower/2 ;        //   =p_lower + p_upper - p_lower/2
	   p_intermy = pow(10, p_intermy) ;
	
	   //printf("计算p_intermy =      %1f \n\n",p_intermy);
	
	
	// 第4步，检查上式计算的P值中间值是否小于0.0005，若没有，将上式计算得到的P值中间值，返回第1步，带入公式中。
	
	//        若是，结束迭代，将P值中间值和通道3火点温度，作为牛顿迭代公式的初始值p0和t0。
		} 
		while (  p_intermy > 0.0005);
	}

	//printf("火点亚像元面积比例初始值: P =  %1f%%\n\n",100*p0);
	//printf("火点亚像元温度初始值（绝对）: T =  %1fK\n\n",t0);

// 计算P初始值结束 *****************************************************




	n1=0;
	if( J0(p0,t0,k3,k4,N3b,N4b) != 0 )	
	{    // printf("J0=%d",J0);
		do	
		{
			p=p0+J1(p0,t0,k3,k4,N3b,N4b,N3mix,N4mix)/J0(p0,t0,k3,k4,N3b,N4b);

			t=t0+J2(p0,t0,k3,k4,N3b,N4b,N3mix,N4mix)/J0(p0,t0,k3,k4,N3b,N4b);
			p0=p;
			t0=t;
            //printf("*高温目标面积占像元百分比: P =  %1f%%\n\n",100*p);
			//printf("*高温目标温度（绝对）: T =  %1fK\n\n",t);

			n1=n1+1;
            //printf("\n_\n\n循环次数=%d：\n\n",n1);

		} 
		while( fabs( p*B2(k3,t)+(1-p)*B2(k3,N3b)-B2(k3,N3mix) ) >0.0000001 &&

            // fabs( p*B2(k4,t)+(1-p)*B2(k4,N4b)-B2(k4,N4mix) ) >0.0000001 );

        fabs( p*B2(k4,t)+(1-p)*B2(k4,N4b)-B2(k4,N4mix) ) >0.0000001 && n1 <= 30);

	//	if (n1 >= 30) return 1; // ???? 如何退出
	}   // printf("n1=%d",n1);

	*pp = p0;
	*pt = t0;
}

//-------------------------------------------------------------------------------------------

/*: ******************* CalPlankFromRadToBT*********************
* 函数名称： CalPlankFromRadToBT
* 函数标题： 根据辐射率计算亮温
* 函数功能： 根据辐射率计算亮温
* 调用方法: CalPlankFromRadToBT(float radian,float wavelength)
*   参数说明：
* float radian  辐射率
* float wavelength  中心波长（微米）
*   作    者：鄢俊洁
*   完成日期：2013年6月18日
************** CalPlankFromRadToBT函数代码 *********************  :*/

float ProcessFire::CalPlankFromRadToBT(float radian,float wavelength)
{
    double c1=1.1910439e-16;
    double c2=1.438769e-2;
    double wl=wavelength*1.0e-6;
    double a1=log(c1/((wl*wl*wl*wl*wavelength)*radian)+1.0);
    double r2t=c2/(wl*a1);

    return r2t;
}

/*----------------------------以下函数没有用到------------------------------*/
/*: *******************  FireProSavePixelIdBmp *********************
* 函数名称：  FireProSavePixelIdBmp
* 函数标题：  保存标记数据文件为bmp图像
* 函数功能：  保存标记数据文件为bmp图像
* 调用方法: FireProSavePixelIdBmp(void)
*   参数说明：
*   无
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/
void ProcessFire::FireProSavePixelIdBmp()
{
//---------------------------
		int wid = 1000;
		int hei = 1000;
    FILE *fbmpfile;
    fbmpfile=fopen(strOutputFileName_FIMG,"wb+");
    BITMAPFILEHEADER1 bmpfile;
    BITMAPINFOHEADER1 bmpinfo;
    RGBQUAD1 *out_buf_RGB = new RGBQUAD1[wid*hei];
		memset(out_buf_RGB,0,sizeof(RGBQUAD1)*wid*hei);
		const int rgbBand = 3;
    //BMP图的头信息
    //strcpy(bmpfile.bftype,"BM");
    bmpfile.bftype[0]=0x4d;
    bmpfile.bftype[1]=0x42;
    bmpfile.bfsiZe=wid*hei*sizeof(RGBQUAD1)+sizeof(BITMAPFILEHEADER1)+sizeof(BITMAPINFOHEADER1);
    bmpfile.bfReservedl=0;
    bmpfile.bgReserved2=0;
    bmpfile.bfoffBits=sizeof(BITMAPFILEHEADER1)+sizeof(BITMAPINFOHEADER1);
    bmpinfo.biSize=sizeof(BITMAPINFOHEADER1);
    bmpinfo.biWldth=wid;
    bmpinfo.biHeight=hei;
    bmpinfo.biPlanes=1;
    bmpinfo.biBitCount=24;
    bmpinfo.biCOmpression=0;
    bmpinfo.biSiZelmage=wid*hei*sizeof(RGBQUAD1);
    bmpinfo.biXPelsPerMeter=0;//0;
    bmpinfo.biYPelsPerMeter=0;//0;
    bmpinfo.biClrUsed=0;//0;
    bmpinfo.biClrlmportant=0;//0;
		printf("111111111111111111\n");
		printf("111111111111111111%s\n",strOutputFileName_FIMG);
        //写入BMP头信息
    fwrite(&bmpfile,sizeof(BITMAPFILEHEADER1),1,fbmpfile);
    fwrite(&bmpinfo,sizeof(BITMAPINFOHEADER1),1,fbmpfile);
		for(int row = 1000 ; row < 2000; row++)
		{
			for(int idx = 1000 ; idx < 2000; idx++)
			{
					if(m_cPixelId_p[row][idx] == 100 )
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 25;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 255;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 25;
            	continue;
          }
          if(m_cPixelId_p[row][idx] == 40)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 10;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 10;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 10;		
            	continue;					
          }
          if(m_cPixelId_p[row][idx] == 50)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 50;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 50;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 50;
            	continue;							
          }  
          if(m_cPixelId_p[row][idx] == 150)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 50;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 150;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 150;		
            	continue;					
          } 
          if(m_cPixelId_p[row][idx] == 153)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 250;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 10;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 10;
            	continue;							
          } 
          if(m_cPixelId_p[row][idx] == 60)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 160;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 160;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 160;		
            	continue;					
          }   
          if(m_cPixelId_p[row][idx] == 126)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 60;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 60;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 200;				
            	continue;			
          } 
          if(m_cPixelId_p[row][idx] == 127)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 160;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 160;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 10;			
            	continue;				
          }   
          if(m_cPixelId_p[row][idx] == 200)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 200;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 200;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 200;	
            	continue;						
          } 
          if(m_cPixelId_p[row][idx] == 205)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 205;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 205;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 205;		
            	continue;					
          }  
          if(m_cPixelId_p[(row-wid)][idx] == 210)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 210;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 210;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 210;	
            	continue;						
          }           
          if(m_cPixelId_p[row][idx] == 215)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 215;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 215;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 215;		
            	continue;					
          } 
          if(m_cPixelId_p[row][idx] == 220)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 220;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 220;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 220;					
            	continue;		
          }
          if(m_cPixelId_p[row][idx] == 250)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 250;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 250;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 250;				
            	continue;			
          }                
          if(m_cPixelId_p[row][idx] == 10)
          {
              out_buf_RGB[(row-wid) * wid + idx].rgbBlue = 0;
              out_buf_RGB[(row-wid) * wid + idx].rgbGreen = 0;
            	out_buf_RGB[(row-wid) * wid + idx].rgbRed		 = 255;			
            	continue;				
          } 				
			}
		}
		printf("222222222222222222222\n");
    fwrite(out_buf_RGB,sizeof(RGBQUAD1)*wid*hei,1,fbmpfile);
    fclose(fbmpfile);
    
    delete[] out_buf_RGB;
    out_buf_RGB = NULL;
    return ;

}
/*: *******************  FireProSavePixelIdRaw *********************
* 函数名称：  FireProSavePixelIdRaw
* 函数标题：  保存标记数据文件
* 函数功能：  保存标记数据文件
* 调用方法: FireProSavePixelIdRaw(void)
*   参数说明：
*   无
*
*   作    者：闫华
*   完成日期：2013年
****************************************************  :*/

void ProcessFire::FireProSavePixelIdRaw(void)
{
    FILE *ofp;
    ofp = fopen(strOutputFileName_PIMG,"wb");
    if (!ofp) 
    {
        //ShowMessage( "文件打开失败");
        //return;
    }
	 else 
	 {
	   for (int i = 0 ; i < iDatHeight ; i++)
	       fwrite(m_cPixelId_p[i] , sizeof(unsigned char), iDatWidth, ofp);
	   fclose(ofp);
	 }

     return;

}

/*: ******************* getCFG *********************
* 函数名称： getCFG
* 函数标题： 根据辐射率计算亮温
* 函数功能： 根据辐射率计算亮温
* 调用方法: getCFG(char *cfgfname)
*   参数说明：
* char *cfgfname  文件名
*
*   作    者：
*   完成日期：2013年10月28日
************** getCFG函数代码 *********************  :*/

bool ProcessFire::getCFG(char *cfgfname,int *iFactr_Mid, int *Factr_Far, int *iDt3Th, int *iDt34Th)
{
/*
//阈值
factr_mid=4;
factr_far=4;
dt3th=150;
dt34th=150;

*/

  FILE *fp;
  char cc[100];
  char *ptr=NULL;
  char *tmp;
  if((fp=fopen(cfgfname,"r"))==NULL) return false;
 // while(!feof(fp))
  fgets(cc,100,fp);
  tmp=strtok(cc, "=");
  ptr=strtok(NULL, "="); //Linux推荐使用函数strtok_r函数。
  sscanf(ptr,"%f= ",iFactr_Mid);
  fgets(cc,100,fp);
  tmp=strtok(cc, "=");
  ptr=strtok(NULL, "="); //Linux推荐使用函数strtok_r函数。
  sscanf(ptr,"%f ",Factr_Far);
  fgets(cc,100,fp);
  tmp=strtok(cc, "=");
  ptr=strtok(NULL, "="); //Linux推荐使用函数strtok_r函数。
  sscanf(ptr,"%f ",iDt3Th);
  fgets(cc,100,fp);
  tmp=strtok(cc, "=");
  ptr=strtok(NULL, "="); //Linux推荐使用函数strtok_r函数。
  sscanf(ptr,"%f ",iDt34Th);

  fclose(fp);
  return true;
}
int ProcessFire::readHdf5Data_float(char *pfileName, char *pSDSName, float *Data)
{
	hid_t       file_id, dataset_id;  /* identifiers */
	herr_t      status;

   /* Open an existing file. */
   file_id = H5Fopen(pfileName, H5F_ACC_RDWR, H5P_DEFAULT);
   printf("read %s \n",pfileName);
   /* Open an existing dataset. */
   dataset_id = H5Dopen(file_id, pSDSName,H5P_DEFAULT);
   printf("read SDS %s \n",pSDSName);
   status = H5Dread(dataset_id, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,Data);
   if(status < 0)
   {
	   printf("**********failed**************\n");
	   return ZEGET2KMDATAOFFHS;
   }
   /* Close the dataset. */
   status = H5Dclose(dataset_id);

   /* Close the file. */
   status = H5Fclose(file_id);

   return status;

}
int ProcessFire::readHdf5Data_UShort(char *pfileName, char *pSDSName, unsigned short *Data)
{
	hid_t       file_id, dataset_id;  /* identifiers */
	herr_t      status;

   /* Open an existing file. */
   file_id = H5Fopen(pfileName, H5F_ACC_RDWR, H5P_DEFAULT);
   /* Open an existing dataset. */
   dataset_id = H5Dopen(file_id, pSDSName,H5P_DEFAULT);  
   status = H5Dread(dataset_id, H5T_NATIVE_USHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT,Data);
   if(status < 0)
   {
	   return ZEGET2KMDATAOFFHS;
   }
   /* Close the dataset. */
   status = H5Dclose(dataset_id);

   /* Close the file. */
   status = H5Fclose(file_id);

   return status;

}
int ProcessFire::readHdf5Data_UChar(char *pfileName, char *pSDSName, unsigned char *Data)
{
	hid_t       file_id, dataset_id;  /* identifiers */
	herr_t      status;

   /* Open an existing file. */
   file_id = H5Fopen(pfileName, H5F_ACC_RDWR, H5P_DEFAULT);
   /* Open an existing dataset. */
   dataset_id = H5Dopen(file_id, pSDSName,H5P_DEFAULT);  
   status = H5Dread(dataset_id, H5T_NATIVE_UCHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT,Data);
   if(status < 0)
   {
	   return ZEGET2KMDATAOFFHS;
   }
   /* Close the dataset. */
   status = H5Dclose(dataset_id);

   /* Close the file. */
   status = H5Fclose(file_id);

   return status;

}

int ProcessFire::ReadIJToLatLonHDF(const char* filename,float* lat,float* lon)
{
	hid_t       file_id;  /* identifiers */
	herr_t      status;

    /* Open an existing file. */
    file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);
    if(file_id < 0)
    {
	   printf("open %s error!\n",filename);
	   return ZEOPEN;
    }
	status = H5LTread_dataset(file_id,"Lat",H5T_NATIVE_FLOAT,lat);
	if(status < 0)
	{
		printf("get Lat error.\n");
		return ZEREAD;
	}
	status = H5LTread_dataset(file_id,"Lon",H5T_NATIVE_FLOAT,lon);
	if(status < 0)
	{
		printf("get Lon error.\n");
		return ZEREAD;
	}
	H5Fclose(file_id);
	return 0;
}

void ProcessFire::ExchangeDWord(unsigned char* pData)
{
 unsigned char bSave;
 bSave    = pData[0];
 pData[0] = pData[3];
 pData[3] = bSave;
 bSave    = pData[1];
 pData[1] = pData[2];
 pData[2] = bSave; 
 return;
}

//WriteStringToFHS(strOutputEnFPAFile,"FPA" ,strOutputFileName_NC,myNetCDF,char* FPALongName,char*FPADescription,char* strResolution,char*FPAStandardName);
int ProcessFire::WriteStringToFHS(char* tempfile, char* SDSName,char* FHSFilename,GOESRNetCDFOperator myNetCDF,char* LongName,char* Description,char* Resoluton,char* StandardName )
{
		//火点信息FPA_txt
	vector<string> strTmpFileName;
	FILE *fp1;
	char buf[256];	
	int index = 0;
	int nDimCount = 0;
	float fScale = 1, fOffset = 1;
	memset(buf,0,sizeof(buf));
	fp1 = fopen(tempfile,"r");
	printf("tempfile: %s\n",tempfile);
	if(fp1 == NULL)
		{
			printf("%s file is not open!\n",tempfile);
		}
		int mynum = 0;
		while(!feof(fp1))
		{
			memset(buf,0,256);
			fgets(buf,sizeof(buf),fp1);
			if(strlen(buf) > 20 )
				{
					mynum++;
					//printf("buf : %d    %s\n",mynum, buf);
					strTmpFileName.push_back(buf);
				}
		}	
	  index = strTmpFileName.size() ;
		
    //printf("----20180426---index %d\n",index);
    
    char *sTest[index];		
     nDimCount =index ;
    char *sDimName[1];
    for (int nIndex = 0; nIndex < 1; ++nIndex)
    {
    	sDimName[nIndex] = new char[256];
			memset(sDimName[nIndex],0,256);
      
      sprintf(sDimName[nIndex], "%d%s",index,"_dims");
    }
		for(int i = 0; i<index ; i++)
		{
			sTest[i] = new char[256];
			memset(sTest[i],0,256);
			//printf("***%d*****strTmpFileName[%d].c_str():%s\n",index,i,strTmpFileName[i].c_str());
      strcpy(sTest[i], strTmpFileName[i].c_str());
     // printf("-----sTest[%d] %s\n",i,sTest[i]);
		} 
  printf("-----------start write NC string sds------nDimCount %d\n",nDimCount);
  
  //myNetCDF.ZNetCDFWriteSDSCommonData(strOutputFileName_NC, "FHS", 2, dims, PixedTemp, NC_SHORT,
//				"FY4A PGS L2+ Fire/Hot Spot Characterization", "Fire/Hot Spot Characterization","True", &FillValue, iValidRange,&ScaleFactor,&AddOffset, "none", 
//				"2000M", "x y","DQF",
//				"150:desert,153:water,200:cloud01,205:cloud02,210:cloud03,215:cloud04,220:cloud05,65535:space,40:fillvalue,100:land,10:fire point,50:satallite zenithangle>80,60:flare angle<30,126:BT3.9um<200K,127:BT10.8um<200K",
//				0) < 0

//char* LongName,char* Description,char* Resoluton,char* StandardName
	myNetCDF.ZNetCDFWriteSDSCommonData(FHSFilename, SDSName, 1, &nDimCount, sTest, NC_STRING,
	       LongName, StandardName, "NULL", "NULL", "NULL", &fScale, &fOffset, "NULL",
	        Resoluton, "y x", "DQF", Description, 0,sDimName);

	printf("-----------END write NC string sds------\n");	
	fclose(fp1);
	for(int ii = 0; ii <index; ii++)
	{
		delete[] sTest[ii];
	}	
	strTmpFileName.clear();
	return 0;
}
//---------------------------------------------------------------------------




