/** @defgroup FHS进程设计说明
 *\n
 *	- 进程名称：FHS\n
 *	- 进程标题：热点检测产品进程\n
 *	- 进程作用：
 *	- 进程类型：实时、临时进程。\n
 *	- 进程运行环境要求：\n
 *		-# 操作系统：UNIX; \n
 *		-# 进程特权： 一般;\n
 *		-# 进程优先级：缺省;\n
 *		-# 启动方式：由调度进程启动 
 * - 
 * - 包含的操作包：
 *        -# INIOperation:        输入输出操作类 \n
 *        -# ProcessFire：    		火点处理操作类 \n
 * - 
 * - 引用的软件外部接口：\n
 *        -# ZHDF:        HDF读写公共服务 \n
 *        -# ZNetCDF：    NC读写公共服务  \n
 *        -# ZZLS:        写日志公共服务  \n
 *        -# 执行参数：\n
 *						argv[0] 			  调度控制参数\n
 *						argv[1] 				全圆盘+星下点坐标\n
 *            argv[2]         开始时间
 *            argv[3]         结束时间
 * 版权:    星地通公司
 * @author  欧阳飒飒
 * @date    2016.8.18
 * @version 1.0.0
 * @{
*/
#include "ProcessFire.h"
#include "INIOperation.h"
/**  @brief FHS进程主函数 \n
*
*    @remark 输入0.65、0.83、3.79、10.8、12um等通道数据\n
* 
*    @param  - char *argv[0],//调度控制参数      		
*    @param  - char *argv[1],//全圆盘产品+星下点坐标
*    @param  - char *argv[2],//开始时间      		
*    @param  - char *argv[3],//结束时间       	
*    @return   0-成功，3810-3900-失败
/FHS.e 
//F4AHotDetecR_FY4A-_AGRI--_AFN20180314054500_DISK_001_R 
//N_DISK_1047E 
//20180314054500 
//20180314055959
*/
int main(int argc,char* argv[])
{	
///@code
///调度参数判断
  if(argc != 5)
  {
  	cout << "parameter is small !" <<endl;
  	return ZEFIREPARA;
  }
///日志初始化
	if(ZLDSInit(argv[1]) != 0)
	{
		printf("Init Param Error!\n");
	}
	ZLDSWriteLog('N',"Start Process Of FHS!");	      
		
	ProcessFire pFire;
	INIOperation iniOperation;
	
	char sataName[32];
	char Regional[32];
	char starttime[128];
	int iyear = 0;
	int imonth = 0;
	int iday = 0;
	int ihour = 0;
	int iminute = 0;
	int isecond = 0;
	memset(sataName,0,sizeof(sataName));
	memset(Regional,0,sizeof(Regional));
	memset(starttime,0,sizeof(starttime));
	strncpy(sataName, argv[1] + 13, 4);
	strncpy(Regional, argv[1] + 44, 4);
	strcpy(starttime,argv[3]);
	printf("starttime %s\n",starttime);
	sscanf(starttime,"%04d%02d%02d%02d%02d%02d",&iyear,&imonth,&iday,&ihour,&iminute,&isecond);
	strncpy(pFire.sataName,sataName, sizeof(sataName));
	strncpy(pFire.regional,Regional, sizeof(sataName));
	strncpy(iniOperation.sataName,sataName, sizeof(sataName));
	pFire.iHour = ihour;
	printf("sataName:%s\n",pFire.sataName);
	printf("regional:%s\n",pFire.regional);
  printf("pFire.iHour:%d\n",pFire.iHour);
	printf("iyear %04d imonth %02d iday %02d ihour %02d iminute %02d isecond %02d\n",iyear,imonth,iday,ihour,iminute,isecond);
	
	///调度参数解析
	iniOperation.analysisSHM(argv[1],argv[2],argv[3],argv[4]);
	///获取数据
	if(iniOperation.GetData() != 0)
	{
		ZLDSWriteLog('W',"Get Data error!");
		printf("Get Data error!\n");
		return ZEREADOFFIRE;
	}
	pFire.DataQ=iniOperation.DataQ;
	///时间解析
	pFire.DateTimeParser(argv[3]);
	strcpy(pFire.strnLanding,iniOperation.strnLandingFileName);	
	strcpy(pFire.strxjraster,iniOperation.strxjrasterFileName);	
	strcpy(pFire.strxjdh_new,iniOperation.strxjdh_newFileName);	
	strcpy(pFire.strLandCover,iniOperation.strLandCoverFileName);	
	strcpy(pFire.strLandImg,iniOperation.strLandImgFileName);	
	strcpy(pFire.strSFTFile,iniOperation.strSFTFileName);
	strcpy(pFire.strOutputFileName_NC,iniOperation.strOutputNCFileName);	
	strcpy(pFire.strOutputFileName_HDF,iniOperation.strOutputHDFFileName);	
	strcpy(pFire.strOutputFileName_FIMG,iniOperation.strOutputFIMGFileName);	
	strcpy(pFire.strOutputFileName_PIMG,iniOperation.strOutputPIMGFileName);
	strcpy(pFire.strOutputFileName_FDJPG,iniOperation.strOutputFDJPGFileName);	
	strcpy(pFire.strOutputFileName_FNJPG,iniOperation.strOutputFNJPGFileName);	
	strcpy(pFire.strOutputFileName_T1,iniOperation.strOutputTable1FileName);	//FPB
	strcpy(pFire.strOutputFileName_T2,iniOperation.strOutputTable2FileName);	//FPT
	strcpy(pFire.strOutputFileName_T3,iniOperation.strOutputTable3FileName);	//FPA

	strcpy(pFire.strOutputEnFPAFile,iniOperation.strOutputEnFPAFileName);	//FPT
	strcpy(pFire.strOutputEnFPTFile,iniOperation.strOutputEnFPTFileName);	//FPA

	strcpy(pFire.strOutputFileName_T1NC,iniOperation.strOutputTable1FileNameNC);	//FPB_NC
	strcpy(pFire.strOutputFileName_T2NC,iniOperation.strOutputTable2FileNameNC);	//FPT_NC
	strcpy(pFire.strOutputFileName_T3NC,iniOperation.strOutputTable3FileNameNC);		//FPA_NC
	pFire.m_factr_mid = iniOperation.factor_mid;
	pFire.m_factr_far = iniOperation.factor_far;
	pFire.m_dt3th = iniOperation.dt3th;
	pFire.m_dt34th = iniOperation.dt34th;	
	pFire.istartline=iniOperation.m_startLine;
	pFire.iendline=iniOperation.m_endLine;
	pFire.istartpixel=iniOperation.m_startPixel;
	pFire.iendpixel=iniOperation.m_endPixel;
	
	printf("mid:%f  far:%f  3th:%f  34th: %f \n",pFire.m_factr_mid,pFire.m_factr_far,pFire.m_dt3th,pFire.m_dt34th);	
	printf("lonLat : %s\n",iniOperation.strIJToLonLatFileName);
	printf("sur : %s\n",iniOperation.strSurfaceTypeFileName);
	printf("satZA : %s\n",iniOperation.strSatZenFileName);
	printf("nland : %s\n",pFire.strnLanding);
	printf("xjraster : %s\n",pFire.strxjraster);
	printf("strxjdh_new : %s\n",pFire.strxjdh_new);
	printf("strLandCover : %s\n",pFire.strLandCover);
	printf("strLandImg : %s\n",pFire.strLandImg);
	printf("strSFTFile : %s\n",pFire.strSFTFile);

	printf("HDF : %s\n",pFire.strOutputFileName_HDF);
	printf("FIMG : %s\n",pFire.strOutputFileName_FIMG);
	printf("PIMG : %s\n",pFire.strOutputFileName_PIMG);
	printf("FDJPG : %s\n",pFire.strOutputFileName_FDJPG);
	printf("FNJPG : %s\n",pFire.strOutputFileName_FNJPG);
	printf("T1 : %s\n",pFire.strOutputFileName_T1);
	printf("T2 : %s\n",pFire.strOutputFileName_T2);
	printf("T3 : %s\n",pFire.strOutputFileName_T3);
	printf("T1NC : %s\n",pFire.strOutputFileName_T1NC);
	printf("T2NC : %s\n",pFire.strOutputFileName_T2NC);
	printf("T3NC : %s\n",pFire.strOutputFileName_T3NC);	
	
	ZLDSWriteLog('N',"Start Process Fire");
	cout << "fire pro start..." << endl;

	pFire.m_wSatDataBuff_p = iniOperation.m_wChannelsData;
	pFire.m_fLat_p = iniOperation.m_fLat;
	pFire.m_fLong_p = iniOperation.m_fLong;
	pFire.pSFTMask_p = iniOperation.pSFTMask;
	pFire.m_ucCloud_p = iniOperation.m_ucCloud;
	pFire.m_fSunZenithAnale = iniOperation.m_fSunZenithAnale;
	pFire.m_fSunGlitAngle = iniOperation.m_fSunGlitAngle;
	pFire.m_fPixelSatelliteZenithAnale = iniOperation.m_fPixelSatelliteZenithAnale;	
	pFire.m_cPixelSurfaceType = iniOperation.m_cPixelSurfaceType;	
	
	pFire.iDatWidth = iniOperation.m_iWidth;		
	pFire.iDatHeight = iniOperation.m_iHeight;	
	pFire.L1FileInfo_p = iniOperation.HDFFileAtt;	
	
	strcpy(pFire.strL0Q_p,iniOperation.m_strL0Q);
	strcpy(pFire.strPOSQ_p,iniOperation.m_strPOSQ);
	strcpy(pFire.strCALQ_p,iniOperation.m_strCALQ);
	strcpy(pFire.strAreaRange,iniOperation.diskarr);
	
///进行火点检测处理 
  if(pFire.FireProMain() != 0)
  {
		ZLDSWriteLog('W',"process fire error!");
		exit(ZEPROCESSOFFIRE);
  }
  
//	int hei , wid;
//	hei = pFire.iDatHeight;
//	wid = pFire.iDatWidth;
//	
//	for(int i=0; i<hei;i++)
//	{
//		for (int j = 0; j < wid; j++)
//		{
//			 if(i== 1250 && j == 3710  )
//        	{
//        		printf("-2222---i %d j %d 2channel: %d  3channel: %d  7channel %d  5channel: %d  6channel: %d \n", i,j,pFire.m_wSatDataBuff_p[5][i * wid + j],
//        		pFire.m_wSatDataBuff_p[3][i * wid + j],pFire.m_wSatDataBuff_p[4][i * wid + j],
//        		pFire.m_wSatDataBuff_p[6][i * wid + j],pFire.m_wSatDataBuff_p[7][i * wid + j]);
//        	}
//         
//          if(i== 761 && j == 2862  )
//        	{
//        		printf("--2222--i %d j %d 2channel: %d  3channel: %d  7channel %d  5channel: %d  6channel: %d \n",i,j, pFire.m_wSatDataBuff_p[5][i * wid + j],
//        		pFire.m_wSatDataBuff_p[3][i * wid + j],pFire.m_wSatDataBuff_p[4][i * wid + j],
//        		pFire.m_wSatDataBuff_p[6][i * wid + j],pFire.m_wSatDataBuff_p[7][i * wid + j]);
//        	}
//
//		}
//	}
  
///写OK文件	    
	if(iniOperation.WriteOK() != 0 )
	{
		ZLDSWriteLog('W',"Write OKFile error!");
		exit(ZEOKWRITE);
	}
///python绘图
	if(iniOperation.JPGFromPython(argv[1])!=0)
	{
		printf("Python Plot Error!\n");
	 	ZLDSWriteLog('W',"Python Plot error");
	}
	else
	{
	 	printf("Python Plot success!\n");
		ZLDSWriteLog('N',"Python Plot success!");
	}	  
	     
 	cout << "end..." << endl;
	return 0;
///@endcode
}

//void* thread_func(void *threadparam)
//{    
//    struct thread_param *pThreadParam = (struct thread_param *)threadparam;
//    int iThreadNo = pThreadParam->iThreadNo;
//    int iThreadSum = pThreadParam->iThreadSum;
//    int iWorkID = pThreadParam->iWorkID; 
//    ProcessFire *pfirepro =  (ProcessFire *)(pThreadParam->pFireProcess);
//    
//
//    switch(iWorkID)
//    {
//        case WORKID_GEOLOCATION:
//        	pfirepro->FireProcess(iThreadNo, iThreadSum);
//            break;
//        
//        default:
//            break;
//    }
//    return NULL;
//}

