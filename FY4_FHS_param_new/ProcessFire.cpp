/** @addtogroup ProcessFire
*  @{
*/
/** @defgroup FHS ProcessFire������������ײ���ϸ���
*\li
* ����������: ProcessFire\n                        
*\li             
* ����������: ��㴦�������\n
*\li
* ����������: ��㴦�������\n    
*\li        
* �����Ĳ���:\n                                  
*		FireProMain   			ProcessFire����Ҫ����\n
*		FireProSetPixelId   ProcessFire����Ҫ����\n
*		FireInfoPro				  ProcessFire����Ҫ����\n
*\li
* ���õĲ�����:			��	\n                                        
*\li
* Դ�����ļ���:\n                                     
*     ProcessFire.cpp \n
*
* ��Ȩ:    		 �����������������ǵ�ͨ��˾
*	@author 		 ŷ����
* @algorithm   �ƻ�
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
	///ʱ�����
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

/**  @brief FHS���������� \n
*    @remark ����0.65��0.83��3.79��10.8��12um��ͨ������\n
*    @return   0-�ɹ�������-ʧ��
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
    FireProMemInit();    //������������
    cout << "success Memory " <<endl;
		ZLDSWriteLog('N',"success Fire Init!");   
    FireProSetPixelId();
    cout << "set id end" << endl;
 		ZLDSWriteLog('N',"success Set ID Of Pixel!"); 		
 		  //���㱳����Ԫƽ��ֵ
     FireProBJ1();
     cout << "process bj1 end" << endl;
 		ZLDSWriteLog('N',"success of bj1 process!");
     //���㱳���¶ȱ�׼��
     FireProBJ2();
     cout << "process bj2 end" << endl;
 		ZLDSWriteLog('N',"success of bj2 process!");

     //�����ʶ
 	 FirePro();   //20170707   ����̫���춥��
   cout << "process fire end" << endl;
 		ZLDSWriteLog('N',"success of fire process!");


   ZLDSWriteLog('N',"Start Write Data!");
	 //�������ļ�
	if(FireProSavePixelIdHDF() != 0)
	{
		 ZLDSWriteLog('W',"Write HDF ERROR!");
		 return ZEHDFWRITE;
	}
	///��������ͼ��
	if(FireProSaveFRTOfPixe() != 0)
	{
		 ZLDSWriteLog('W',"Write fire flag JPEG ERROR!");
		 return ZEWRITEFLAGOFJPEG;		
	}
	
	///������ǿ��ͼ��	
	if(FireProSaveFRPOfPixe() != 0)
	{
		 ZLDSWriteLog('W',"Write fire Power JPEG ERROR!");
		 return ZEWRITEPOWEROFJPEG;			
	}

///�������Ǻϳ�ͼ��	
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
	//�����Ϣ����
	FireInfoPro();//���TXT��FPT��FPB�ļ�
 if(strcmp(sataName,"FY4A")==0)
	{
		if(FireProSavePixelIdNC_FY4A()!= 0) // //�������ļ�NC
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
	//�ͷ�ȫ�ֱ����ڴ�ռ�
	FireProMemDelete();

  return 0;
}



//-----------  �����ʶ���������Ӻ���  ----------------------------------------------

/*: ******************* FireProVarInit *********************
* �������ƣ� FireProVarInit
* �������⣺ �����ʶ��Ϣ�������õ�ȫ�ֱ�����ʼ��
* �������ܣ� ��ʼ��ȫ�ֱ���
* ���÷���: FireProVarInit(void)
*   ����˵����
*        ��
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::FireProVarInit(void)
{
     m_ResultBuff_p = NULL;
     m_cPixelId_p   = NULL;
     m_TempBuff2_p = NULL;
     //���ӷ���
     fIR1Bj_p = NULL;
      fIR4Bj_p = NULL;
     fIR45Bj_p = NULL;
     fRefl_p =   NULL;
    fIR5Bj_p  = NULL;
      //���Ӿ��淴������***********************************************************20170828
     SolarRef_p = NULL ;   //���淴�������
    //��������ƽ��ֵ**********************************************************20170716
     CloudAver_p = NULL;      //20170716
   //���Ӳ��㱳����Ԫ��������ļ�*****************************************20170823
     Noefft_p =  NULL;   // ���㱳����Ԫ��������ļ�
     NDVI=NULL;
    
     fpi_ = NULL;
     ffi_ = NULL;

     iFires = 0;           // �������
     iAreas = 0;
}
//---------------------------------------------

/*: ******************* FireProMemInit*********************
* �������ƣ� FireProMemInit
* �������⣺ �����ʶ��Ϣ�������õ�ȫ�ֱ�����̬�����ڴ漰��ʼ��
* �������ܣ� ��̬�����ڴ�ռ估��ʼ��
* ���÷���: bool FireProMemInit(void)
*   ����˵����
*        ��
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::FireProMemInit(void)
{
     m_ResultBuff_p = new unsigned char *[iDatHeight];            //�������ʶ�������������Ϊ�޷���1�ֽ�����
     m_cPixelId_p  = new unsigned char *[iDatHeight];             //���ÿ����Ԫ�ı�ʶ�룬��������Ϊ�޷���1�ֽ�����
     m_TempBuff2_p = new unsigned char *[iDatHeight];
     fRefl_p = new float *[iDatHeight];
 //���ӿɼ����Զ����*******************20170914****************************************
    wIR1BjAver_p = new unsigned short int *[iDatHeight];           //���Զ�����11um����ƽ��ֵ����������Ϊ�޷���2�ֽ�����  20170706
    wIR5BjAver_p = new unsigned short int *[iDatHeight];           //��ſɼ��ⱳ��ƽ��ֵ����������Ϊ�޷���2�ֽ�����      20170706 
     //����ͨ��6��1.6����ͨ��7��2.3����NDVIƽ��ֵ����******************************
     
    wIR4BjAver_p= new unsigned short int *[iDatHeight];  
    wIR45BjAver_p= new unsigned short int *[iDatHeight];  
    wIR6BjAver_p = new unsigned short int *[iDatHeight];        //���ͨ��6��1.6��ƽ��ֵ���飬��������Ϊ�޷���2�ֽ�����  20170825      
    wIR7BjAver_p =  new unsigned short int *[iDatHeight];       //���ͨ��7��2.3��ƽ��ֵ���飬��������Ϊ�޷���2�ֽ�����  20170825  
    NDVIAver_p =  new unsigned short int *[iDatHeight];             //���NDVIƽ��ֵ���飬��������Ϊ�޷���2�ֽ�����  20170825 
    fIR4Bj_p = new float *[iDatHeight];                         //���ÿ����Ԫ���к��ⱳ��������ֵ����������Ϊ4�ֽڸ�����
    fIR45Bj_p = new float *[iDatHeight];                        //���ÿ����Ԫ���к���-Զ����1����ֵ�������������Ϊ4�ֽڸ�����
    fIR5Bj_p  =  new float *[iDatHeight];
   //����Զ�����׼��*******************************************************20171107
	  fIR1Bj_p = new float *[iDatHeight];   //Զ�����׼�� 20171107
    //���Ӿ��淴������***********************************************************
     SolarRef_p =  new unsigned short int *[iDatHeight] ;     //���淴�������
    //��������ƽ��ֵ***********************************************************
     CloudAver_p = new unsigned short int *[iDatHeight];           //�������ƽ��ֵ����������Ϊ�޷���2�ֽ�����      20170716 
     Noefft_p = new unsigned short int *[iDatHeight];   // ���㱳����Ԫ��������ļ�
     NDVI = new float*[iDatHeight];
     for (int i = 0; i < iDatHeight; i++)
     {
          m_ResultBuff_p[i] = new unsigned char [iDatWidth];
          m_cPixelId_p[i] = new unsigned char [iDatWidth];
          m_TempBuff2_p[i] = new unsigned char [iDatWidth];
          fRefl_p[i] = new float [iDatWidth];
           //���ӿɼ����Զ����***********************************************************20170706
         wIR1BjAver_p[i] = new unsigned short int [iDatWidth];         // 12ͨ�� 11um
          wIR5BjAver_p[i] = new unsigned short int [iDatWidth];        //2ͨ�� O.65um
          //����ͨ��6��1.6����ͨ��7��2.3����NDVIƽ��ֵ����******************************20170825
         wIR6BjAver_p[i] = new unsigned short int [iDatWidth];        //���ͨ��5��1.6��ƽ��ֵ���飬��������Ϊ�޷���2�ֽ�����  20170825      
         wIR7BjAver_p[i] =  new unsigned short int [iDatWidth];       //���ͨ��6��2.3��ƽ��ֵ���飬��������Ϊ�޷���2�ֽ�����  20170825  
         NDVIAver_p[i] =  new unsigned short int [iDatWidth];             //���NDVIƽ��ֵ���飬��������Ϊ�޷���2�ֽ�����  20170825
          
         wIR4BjAver_p[i] = new unsigned short int [iDatWidth];
         wIR45BjAver_p[i] = new unsigned short int [iDatWidth];
          
          fIR4Bj_p[i] = new float [iDatWidth];
          fIR45Bj_p[i] = new float [iDatWidth];
           fIR1Bj_p[i] = new float [iDatWidth];
           fIR5Bj_p[i] = new float [iDatWidth];
         //���Ӿ��淴������***********************************************************20170828
          SolarRef_p[i] =  new unsigned short int [iDatWidth] ;   //���淴�������

         //��������ƽ��ֵ***********************************************************20170716
        CloudAver_p[i] = new unsigned short int [iDatWidth];       //20170716
          
         //���Ӳ��㱳����Ԫ��������ļ�*****************************************20170823
        Noefft_p[i] = new unsigned short int [iDatWidth];   // ���㱳����Ԫ��������ļ�
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
        //���ӿɼ����Զ����***********************************************************20170706
         wIR1BjAver_p[i][j] = 0;  //20170627
         wIR5BjAver_p[i][j] = 0;  //20170627
         wIR6BjAver_p[i][j] = 0;  //20170825
         wIR7BjAver_p[i][j] = 0;  //20170825
         NDVIAver_p[i][j] = 0;    //20170825 
         
       //����Զ�����׼��*******************************************************20171107
			   fIR1Bj_p[i][j] = 0.;  // Զ�����׼�� 20171107
        fIR5Bj_p[i][j] = 0;
       //���Ӿ��淴������***********************************************************20170828
         SolarRef_p[i][j] =  0;   //���淴�������
         CloudAver_p[i][j] = 0;  //2017804
         NDVI[i][j]=0.;
          
          fRefl_p[i][j] = 0.;
          fIR4Bj_p[i][j] = 0.;
          fIR45Bj_p[i][j] = 0.;
          
        
       
      }  
     
}

//-------------------------------------------------------
/*: ******************* FireProMemDelete *********************
* �������ƣ� FireProMemDelete
* �������⣺ �����ʶ��Ϣ�������õ�ȫ�ֱ�����̬�����ڴ���ͷ�
* �������ܣ� �ͷŷ�����ڴ�ռ�
* ���÷���: bool FireProMemDelete(void)
*   ����˵����
*        ��
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::FireProMemDelete(void)
{

     for (int i = 0; i < iDatHeight; i++)
     {
          delete [] m_ResultBuff_p[i];
          delete [] m_cPixelId_p[i];
          delete [] m_TempBuff2_p[i];
           //���ӿɼ����Զ����***********************************************************20170706
      		delete [] wIR1BjAver_p[i];         //20170706
      		delete [] wIR5BjAver_p[i];         //20170706
      			
      		//����ͨ��6��1.6����ͨ��7��2.3����NDVIƽ��ֵ����******************************20170825
      		 delete [] wIR6BjAver_p[i];        // 20170825      
      			delete [] wIR7BjAver_p[i];       //  20170825  
      			delete [] NDVIAver_p[i];             //  20170825 
      			
      			//���Ӿ��淴������***********************************************************20170828
      			  delete [] SolarRef_p[i];   //���淴�������
      			
      			// ��������ƽ��ֵ***********************************************************20170716
      			 delete [] CloudAver_p[i];       //20170716
      			  
      		  //���Ӳ��㱳����Ԫ��������ļ�*****************************************20170823
      		 delete [] Noefft_p[i];   // ���㱳����Ԫ��������ļ�
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
       //����Զ�����׼��*******************************************************20171107
	   delete [] fIR1Bj_p;  //Զ�����׼�� 20171107 

     m_ResultBuff_p = NULL;
     m_cPixelId_p   = NULL;
     m_TempBuff2_p = NULL;
     NDVI = NULL;
   
}

/*: ******************* FireProSetRefl *********************
* �������ƣ� FireProSetRefl
* �������⣺ �����к����Զ�����¶�ֵת�����ʵĲ�
* �������ܣ� �����к����Զ�����¶�ֵת�����ʵĲ�
* ���÷���: FireProSetRefl(unsigned short int *m_wSatDataBuff_p[])
*   ����˵����
*   unsigned short int *m_wSatDataBuff_p[] ��������
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/

void ProcessFire::FireProSetRefl()
{
   // �����к��ⷴ����
	double c1,c2,WAV3,WAV4,LTB,LTF;     //WAV,LT,
	c1 = 1.1910427/100000;//c1,c2ϵ����VC���Ĳ���,ABT���Ĳ������桢б��
	c2 = 1.4387752;
	WAV3 = 2640.0;          // VΪ���Ĳ������к�����Ϊ2640    2671.2150;
	WAV4= 921.0;            //              Զ������Ϊ921      924.8922;
	//short TF=750;
    float p,T;     //TB,

        for (int i = 0; i < iDatHeight; i++)
            for (int j = 0; j < iDatWidth; j++){
                T = (float)(m_wSatDataBuff_p[4][i * iDatWidth + j] / 10.); //�к�������
                p = (float)(m_wSatDataBuff_p[1][i * iDatWidth + j] / 10.); //Զ��������
	        //LT = c1 * pow(WAV3,3)/(exp(c2*WAV3/T)-1);
	        LTF = (c1 * pow(WAV3,3)) / (exp(c2 * WAV3 / T) - 1);
	        LTB = (c1 * pow(WAV4,3)) / (exp(c2 * WAV4 / p) - 1);
	        fRefl_p[i][j] = LTF - LTB;  //fRefl_pΪȫ�ֱ�������ż�����
            }

	//return;

}
//------------------------------------------------------------------------
/*: *******************  FireProSetPixelId *********************
* �������ƣ�  FireProSetPixelId
* �������⣺  �ӱ��
* �������ܣ�  ��ȫԲ�����ݽ��з�����������ֵ
* ���÷���: FireProSetPixelId()
*   ����˵����
*   unsigned short int *m_wSatDataBuff_p[] ��������
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
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
             if ((i - 1) < 0)  // ������
                c1 = m_cPixelSurfaceType[i][j];
             else
                c1 = m_cPixelSurfaceType[i - 1][j];

             if ((i + 1) > (iDatHeight - 1))  // ������
                c2 = m_cPixelSurfaceType[i][j];
             else
                c2 = m_cPixelSurfaceType[i + 1][j];

             if ((j - 1) < 0)  // ������
                c3 = m_cPixelSurfaceType[i][j];
             else
                c3 = m_cPixelSurfaceType[i][j - 1];

             if ((j + 1) > (iDatWidth - 1))  // ������
                c4 = m_cPixelSurfaceType[i][j];
             else
                c4 = m_cPixelSurfaceType[i][j + 1];

             float fRefl_p1,fRefl_p2;
             if ((j - 3) < 0)  // ������
                fRefl_p1 = fRefl_p[i][j];
             else
                fRefl_p1 = fRefl_p[i][j - 3];

             fRefl_p1 = fRefl_p[i][j] -  fRefl_p1;

             //--------------------------------------------------------------------
             if ((j + 3) > (iDatWidth - 1))  // ������
                fRefl_p2 = fRefl_p[i][j];
             else
                fRefl_p2 = fRefl_p[i][j + 3];

             fRefl_p2 = fRefl_p[i][j] -  fRefl_p2;
            // �����������Ҽ���м�ֵ����û���õ�
             //--------------------------------------------------------------------
		
             m_cPixelId_p[i][j] = 100;    // ���ֵ Ϊ 100

			 
             // �ɼ���ͽ�����ͨ��̫���춥�Ƕ��� 20170711 ��   ***********
       
        if(pSFTMask_p[i][j]>=65534)//by liup 20170606  ��̫��
        {
             	   m_cPixelId_p[i][j] = 255;
             	  
                 continue;
        }
        if(m_wSatDataBuff_p[3][i * iDatWidth + j] < 60000 )
        	{
        	   	 if  ( m_fSunZenithAnale[i][j] > SZAngleUp )
                    //#### ̫���춥�Ƕ����ǶȲ���  SatZAngLimit ����ֵ70��
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
                 //#### ̫���춥�Ƕ����ǶȲ���  SatZAngLimit ����ֵ70��
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
                 //#### ̫���춥�Ƕ����ǶȲ���  SatZAngLimit ����ֵ70��
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
                 //#### ̫���춥�Ƕ����ǶȲ���  SatZAngLimit ����ֵ70��
				  {
					solarznh =70*rad; 
					//continue;
				  }	 
				   else 
			    solarznh = m_fSunZenithAnale[i][j]*rad;
        		m_wSatDataBuff_p[7][i * iDatWidth + j] = m_wSatDataBuff_p[7][i * iDatWidth + j] / cos (solarznh );
        	}
        	

			 // �ɼ���ͽ�����ͨ��̫���춥�Ƕ��� 20170711 ��   ***********
			   if ( ( (m_wSatDataBuff_p[3][i * iDatWidth + j]== 0 ||m_wSatDataBuff_p[5][i * iDatWidth + j]==0)) )  //20180128
			   NDVI[i][j] = 0.;                                                                                   //20180128
			 else             			  
			  NDVI[i][j] = (float)((float)m_wSatDataBuff_p[3][i * iDatWidth + j]-(float)m_wSatDataBuff_p[5][i * iDatWidth + j])/
				  ((float)m_wSatDataBuff_p[3][i * iDatWidth + j]+(float)m_wSatDataBuff_p[5][i * iDatWidth + j]);
      
       if (m_wSatDataBuff_p[4][i * iDatWidth + j] > 60000 || m_wSatDataBuff_p[1][i * iDatWidth + j] > 60000
             	||m_wSatDataBuff_p[2][i * iDatWidth + j] > 60000 || m_wSatDataBuff_p[3][i * iDatWidth + j] > 60000
             	||m_wSatDataBuff_p[5][i * iDatWidth + j] > 60000 ) // ��������   (m_fLat_p[i][j] == -32768 )
			 {
                 //#### ��������ֵ����  Nodatavalue  ����ֵ-900��
				 m_cPixelId_p[i][j] = 40 ;   
			 }
      
      else if (m_fPixelSatelliteZenithAnale[i][j] >  SatZAngLimit )  // �����춥�� ���� 82������ͼ����Ч�����  20160914
			 {
                  //#### �����춥�����޲��� SatZAngLimit ����ֵ80��
			
				 m_cPixelId_p[i][j] = 50;  
				
			 }

			 // ����Ƿ��г����߸����ĸ߷�����ţ���̫���춥�Ǵ���85�ȣ��ɼ��ⷴ���ʴ���0.6����ʱ�к�������½���̫���������  20170712 

			 else if (m_fSunZenithAnale[i][j] >  SZAngleNt  && m_wSatDataBuff_p[5][i * iDatWidth + j] > VisAngleNt   )  // ̫���춥�Ǵ���85�ȣ��ɼ��ⷴ���ʴ���0.6  20170712 
            //#### ������̫���춥�ǽǶȲ��� SZAngleNt�� ����ֵ85��  �����߿ɼ���ͨ���ǶȲ��� VisAngleNt������ֵ0.6�� 
				
				{
					m_cPixelId_p[i][j] = 180;    //������̫��������� ���Ϊ180   20170712  
					
				} 

          // ˮ����
         else if (m_cPixelSurfaceType[i][j] == 0  ) 
         	{
         		if(i ==1867 && j == 2406)
         			{
         				printf("***********water***********\n");
         			}
         		m_cPixelId_p[i][j] = 153;
         			
         	} 
         //#### ҫ�߽ǶȲ���   SunGtAngLimit,��ֵΪ10�� 
         else if (m_fSunGlitAngle[i][j] < SunGtAngLimit)    
         	{
         		m_cPixelId_p[i][j] = 60; 
         		
         	}  
          //#### ҫ�߽ǶȲ���   SunGtAngLimit  
				 //ҫ�������
				  //#### �к���ͨ����������ֵ����  T4LowTemp,����ֵ2000������ͨ��3���к��⣩����С��200K�����Ϊ126�������ȵ��ǣ�
          else if (m_wSatDataBuff_p[4][i * iDatWidth + j] < T4LowTemp )  
				
          {
                  	m_cPixelId_p[i][j] = 126;
                  
          }
          //#### Զ����ͨ����������ֵ����  T11LowTemp, ����ֵ2000���� ��ͨ��4��Զ����1������С��200K�����Ϊ127�������ȵ��ǣ�
         else if (m_wSatDataBuff_p[1][i * iDatWidth + j] < T11LowTemp)  
          {
                 	m_cPixelId_p[i][j] = 127;
                
          } 
			 //#### ����������ʶ:�Ƽ���ļ����ڣ��ɼ���ͨ������ VisDCloudBJ,����ֵ150��������ʶ̫���춥�ǲ��� SZAngDCloudBJ,����ֵ87�� 
			 else if (cloud_ID == 1 && m_ucCloud_p[i][j] == 0 && m_wSatDataBuff_p[5][i * iDatWidth + j] >VisDCloudBJ &&
			 	          m_fSunZenithAnale[i][j]< SZAngDCloudBJ )     // �����Ƽ�⣬���Ƽ���Ʒ��ֵΪ��0�����ɼ������15%��̫���춥��С��87��Ϊ�ơ�20170804
			 {
				 
				 m_cPixelId_p[i][j] = 220; 
				
			 }
			  //#### ����������ʶ���Ƽ���ļ������ڣ����ÿɼ���ͨ����ֵ������VisDCloudth����ֵΪ350.
			 else if (cloud_ID == 0 && m_wSatDataBuff_p[5][i * iDatWidth + j] >VisDCloudth   &&
			 	 m_fSunZenithAnale[i][j]< SZAngDCloudBJ )     // �����Ƽ�⣬��û���Ƽ���Ʒ���ɼ������25%��̫���춥��С��87��Ϊ�ơ�20171014
			 {
				
				 m_cPixelId_p[i][j] = 220;
				
			 }
			 
			   //#### ҹ��������ʶ��ҹ��̫���춥������ʶ��ֵ���� SZAngNCloudBJ,��ֵ87�ȡ�
			   //��û���Ƽ���Ʒ��ʹ��Զ����ͨ����ʶ,#### Զ������������ʶ��ֵ������T11NCloud����ֵΪ2700,��270K��
			 else if ( cloud_ID == 1 && m_ucCloud_p[i][j] == 0  && m_fSunZenithAnale[i][j]> SZAngNCloudBJ )     // ҹ���Ƽ�⣬���Ƽ���Ʒ��ֵΪ��0��̫���춥�Ǵ���86��Ϊ�ƣ���ҹ�䲻���ǿɼ��⡣20170804��
			 {
			   m_cPixelId_p[i][j] = 220;
			 }
			 //#### ҹ��������ʶ���ú���ͨ����ֵ������T11NCloudth����ֵ,2700.
			 else if ( cloud_ID == 0 && m_fSunZenithAnale[i][j]> SZAngNCloudBJ && m_wSatDataBuff_p[5][i * iDatWidth + j] < T11NCloudth   )     // ҹ���Ƽ�⣬���Ƽ���Ʒ��ֵΪ��0��̫���춥�Ǵ���86��Ϊ�ƣ���ҹ�䲻���ǿɼ��⡣20170804��
			 {
			   m_cPixelId_p[i][j] = 220;
			 }  

     // else if (m_cPixelSurfaceType[i][j] == 16 && c1 == 16 && c2 == 16 && c3 == 16 && c4 == 16)  // ��Į����ǣ��ο����ظ������ݣ���12��Ϊ16���������������ݽ����ı���˵��Ϊ15 20170429��
        else if (m_cPixelSurfaceType[i][j] == 16)  //   ���Ϊ150����Į������
         {
            m_cPixelId_p[i][j] = 150;
           
         }    //  20170429  �޸�  �������������ʹ��󣬽���Į��Ǹ�Ϊֲ�������������ֵ150��Ϊ100
         

       }
    }
     // �ӱ�ǽ���


     return;
}

//----------------------------------------------------------------------------

/*: *******************  FireProBJ1 *********************
* �������ƣ�  FireProBJ1
* �������⣺  ���㱳���¶�
* �������ܣ�  �����к���ı����¶ȼ��к����Զ����Ĳ�ı����¶�
* ���÷���: FireProBJ1(unsigned short int *m_wSatDataBuff_p[])
*   ����˵����
*   unsigned short int *m_wSatDataBuff_p[] ��������
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::FireProBJ1()
{
         //------------- ���㱳���¶�  -----------------
     int ij, iCount;
	 int i,j,ia;  //20170708
    // ij = 2;
	   ij = 3;   // 20161005********************************
	  i = 0;
	  j = 0;
	  float coe1 = 0.; 
     iCount = 0;
     bool bDoId = true;
    

    
	 
	 //����������㱳������Ԫ������Ϣ�ļ�  20170710 ��  *****

	
     for (int i = 0; i < iDatHeight - 10; i++)
     {     //20180314
		   for (int j = 0; j < iDatWidth; j++)
		   {	      
			   if (m_cPixelId_p[i][j] == 100)
			   { // �Ա��ֵΪ100�ĵ���㱳��ƽ��ֵ
              bDoId = true;
         	    ij = 3;   // 20161005********************************
            
              do
              {    // ��ͬ�ı�������
                 iCount = 0;
                 int iIr3BjAv, iIr34BjAv,iIr1BjAv, iIr5BjAv,iIr6BjAv, iIr7BjAv;
				         float NDVIAv;
				         int iaround;
                 //iIr3BjAv �к��ⱳ��ƽ��ֵ�� iIr34BjAv �к����Զ�����ı���ƽ��ֵ
                 iIr3BjAv = 0;
                 iIr34BjAv = 0;

				          iIr1BjAv = 0;  // Զ���ⱳ��ƽ��ֵ20170706
				          iIr5BjAv = 0;  //�ɼ��ⱳ��ƽ��ֵ20170706
                  
				          iIr6BjAv = 0;  // ͨ��6��1.6΢�ף�����ƽ��ֵ20170825
				          iIr7BjAv = 0;  // ͨ��7��2.2΢�ף�����ƽ��ֵ20170825
				          NDVIAv = 0.;  // NDVI����ƽ��ֵ20170825
                  

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
                  
                          
					          // ���ǿɼ��ⷴ���ʵ��к���ͨ�����Ƹ�����Ԫ�ж�  20170809
					          //#### �����к�������ڽ���Ԫȡ���¶Ȳ����� T4BJLimit����ֵ3150����315K��
				
				            if (  m_wSatDataBuff_p[4][i * iDatWidth + j]  > T4BJLimit )    //20180314
				             {
                                   
				            		iaround = 0; //20170828
				               // ���������̽����Ԫi,j����Ԫ����������i-1,j-1;i-1,j;i-1,j+1;i,j-1;i,j;i,j+1;i+1,j-1,i+1,j;i+1,j+1����Ԫ 20170716  ��				                         
				            	if (m_cPixelId_p[i0][j0] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] <= m_wSatDataBuff_p[4][i * iDatWidth + j] - DtBJ2
				            		&& ( m_wSatDataBuff_p[3][i0 * iDatWidth + j0] >= m_wSatDataBuff_p[3][i * iDatWidth + j] - BJDNIR &&
				            		m_wSatDataBuff_p[1][i0 * iDatWidth + j0] >= m_wSatDataBuff_p[1][i * iDatWidth + j]- BJDT11)  &&(i0 != i && j0 != j))  //20180314  С��̽����Ԫ�к�������-4K,��ȥ�����ܵ�ˮ��					
				            		{
				            	if(( i0 == i-1 && j0 == j-1) ||( i0 == i-1 && j0 == j) ||( i0 == i-1 && j0 == j+1) ||( i0 == i && j0 == j-1) ||( i0 == i && j0 == j+1) ||( i0 == i+1 && j0 == j-1) ||( i0 == i+1 && j0 == j) ||( i0 == i+1 && j0 == j+1))//  20170828
                          iaround = 1;
			              
                      if ( iaround == 0 &&  m_cPixelId_p[i][j] != 153   ) //20180226   ����ˮ��Ӱ��
				            	{
                    
                                     
				            		iCount++; //��Чֵ�ĸ���
                                
				            		  iIr3BjAv =  iIr3BjAv + m_wSatDataBuff_p[4][i0 * iDatWidth + j0]; //20170627
                          iIr34BjAv =  iIr34BjAv + (abs(m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0])); //20170627
                    
				            		  //���ӿɼ����Զ����***********************************************************20170706
				            		  iIr1BjAv =  iIr1BjAv + m_wSatDataBuff_p[1][i0 * iDatWidth + j0];   //    ����Զ����ͨ���ۼ�ֵ20170627
				            		  iIr5BjAv =  iIr5BjAv + m_wSatDataBuff_p[5][i0 * iDatWidth + j0];   //    ����ɼ���ͨ���ۼ�ֵ20170627
                    
				            		  //NDVIƽ��ֵ����******************************20170825
	                           		 NDVIAv = NDVIAv + NDVI[i0][j0];          //    ����NDVI�ۼ�ֵ20170825
				            	}  
                       }
				             } 
                              
				            //  ����к���ͨ��С��315K��ʹ���ڽ���Ԫ���㱳���¶� 20170722
                          
				            	 if (  m_wSatDataBuff_p[4][i * iDatWidth + j]  < 3150 )      //20180314
				            	 {
                        
				            	  if (m_cPixelId_p[i0][j0] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] <= m_wSatDataBuff_p[4][i * iDatWidth + j] - DtBJ2
				            	  &&  ( m_wSatDataBuff_p[3][i0 * iDatWidth + j0] >= m_wSatDataBuff_p[3][i * iDatWidth + j] - BJDNIR &&
				            	   m_wSatDataBuff_p[1][i0 * iDatWidth + j0] >= m_wSatDataBuff_p[1][i * iDatWidth + j]- BJDT11) && (i0 != i && j0 != j))
				            	  {  //20180314  С��̽����Ԫ�к�������-4K,��ȥ�����ܵ�ˮ��
				            	  
				            	   iCount++; //��Чֵ�ĸ���
                               
				            	   iIr3BjAv =  iIr3BjAv + m_wSatDataBuff_p[4][i0 * iDatWidth + j0]; //�����к���ƽ��ֵ 20170627
                         iIr34BjAv =  iIr34BjAv + (abs(m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0])); //������Զ�������ƽ��ֵ 20170627
				            	   
				            	   //���ӿɼ����Զ����***********************************************************20170706
				            	   iIr1BjAv =  iIr1BjAv + m_wSatDataBuff_p[1][i0 * iDatWidth + j0];   //    ����Զ����ͨ��ƽ��ֵ20170627
				            	   iIr5BjAv =  iIr5BjAv + m_wSatDataBuff_p[5][i0 * iDatWidth + j0];   //    ����ɼ���ͨ��ƽ��ֵ20170627
                    
				            	   //����NDVIƽ��ֵ����******************************20170825
	                               	NDVIAv = NDVIAv + NDVI[i0][j0];          //    ����NDVIƽ��ֵ20170825 
				            
                    
                        }
				               } 
                    }
                 }
			                
				        float fTemp; //��������Чֵ��ռ��
                fTemp = (float)((ij * 2 + 1) * (ij * 2 + 1));
				        fTemp =  (float)iCount / fTemp * 100.;
                //#### ����������Ԫ�����������޲�����Count_Limit����ֵ15��������0.15��#### ����������Ԫ�������޲����� Count_No����ֵ8��  

			 	       if (fTemp < Count_Limit ||  iCount < Count_No )
			 	   	    {  //��Чֵռ�� �� 15%   �����С��6 ������������ //20170716
                   ij++;            //������
                 }
                 else
                 	{    // ���㱳����Ԫ�������Ԫ���������㱳��ֵ
                    bDoId = false;
                    wIR4BjAver_p[i][j] = (unsigned short int)(iIr3BjAv / iCount);
                    wIR45BjAver_p[i][j] = (unsigned short int)(iIr34BjAv / iCount);

					           //���ӿɼ����Զ����***********************************************************20170706
					          wIR1BjAver_p[i][j] = (unsigned short int)(iIr1BjAv / iCount);
                    wIR5BjAver_p[i][j] = (unsigned short int)(iIr5BjAv / iCount);

					          //����NDVIƽ��ֵ����******************************20170825
	         	       NDVIAver_p[i][j] =  ( NDVIAv / iCount)*100.;          //    ����NDVIƽ��ֵ20170825				  
                 }
				               //#### �����¶ȼ�������Χ���� BJlimit (����Ϊ51 )
      
                   if ((ij * 2 + 1) > BJlimit)
                 { // �������Ϊ21*21 20171107
					          bDoId = false;   //������ѭ����־
                
                    Noefft_p[i][j] = 250;   //20170823
                    wIR4BjAver_p[i][j] = 0;
                    wIR45BjAver_p[i][j] = 0;

                    //���ӿɼ����Զ����***********************************************************20170706
					          wIR1BjAver_p[i][j] = 0;             //20170706
                    wIR5BjAver_p[i][j] = 0;             //20170706

					          //���� NDVI***********************************************************20170706
					           NDVIAver_p[i][j] = 0.;             //20170825
					
				
				         }
              }
              while (bDoId);
			  }

          


		  //  ������Ԫƽ��ֵ���� 20170716 �� *********************************************************

			 // ���������Ԫ�Ƿ��������ƽ��ֵ������#### �����к����¶Ƚ��޲�����T4CLD����ֵ3100����310K��#### �����ɼ��ⷴ���ʽ���ֵ�� VisCLD����ֵ500����0.5��#### ����Զ�����¶Ƚ���ֵ�� T11CLD����ֵ2500����250K��
               
			 if (m_cPixelId_p[i][j] == 220 && m_wSatDataBuff_p[4][i * iDatWidth + j] > T4CLDBJ &&  
			 	m_wSatDataBuff_p[5][i * iDatWidth + j] <VisCLDBJLimit && 
			 	  m_wSatDataBuff_p[1][i * iDatWidth + j] > T11CLD)
			 	{ //������ֵΪ����220 ���� 20170716
              
			    bDoId = true;   
		      ij = 3;
	
              do{    // ��ͬ�ı�������
                 iCount = 0;
                 int iIr3BjAv, iIr34BjAv,iIr1BjAv, iIr5BjAv;
				         int iaround = 0;  //20170828
                 //iIr3BjAv �к��ⱳ��ƽ��ֵ�� iIr34BjAv �к����Զ�����ı���ƽ��ֵ
                 iIr3BjAv = 0;
                 iIr34BjAv = 0;

				          iIr1BjAv = 0;  //20170706
				          iIr5BjAv = 0;  //20170706
                // ����������Ԫ����ƽ��ֵ�����������СΪ(ij*2+1)*(ij*2+1),����ֵ5*5.
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
                   
					              //#### ������Ԫ�����к������ֵ T4CLDBJ ����ֵ3150����315K�� ����к���ͨ������315K���ų��ڽ���Ԫ���㱳���¶� 20170722  ��
					  
					
                       if (  m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  > T4BJLimit )    //20180124
					            {

				               // ���������̽����Ԫi,j����Ԫ����������i-1,j-1;i-1,j;i-1,j+1;i,j-1;i,j;i,j+1;i+1,j-1,i+1,j;i+1,j+1����Ԫ 20170716  ��
				
                        if (m_cPixelId_p[i0][j0] == 220  && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > T4CLDBJLimit &&  
              	        m_wSatDataBuff_p[5][i0 * iDatWidth + j0] < VisCLDBJLimit &&  ii != i && jj != j  )
              	        {        //20180316
							         if((i0 == i && j0 == j) ||( i0 == i-1 && j0 == j-1) ||( i0 == i-1 && j0 == j) ||( i0 == i-1 && j0 == j+1) ||( i0 == i && j0 == j-1) ||( i0 == i && j0 == j+1) ||( i0 == i+1 && j0 == j-1) ||( i0 == i+1 && j0 == j) ||( i0 == i+1 && j0 == j+1))//  20170828
						           {
						            iaround = 1;
						           }
					             if ( iaround == 0)
					             {			

                         iCount++; //��Чֵ�ĸ���
                  
						            iIr3BjAv =  iIr3BjAv + m_wSatDataBuff_p[4][i0 * iDatWidth + j0]; //20170627
                         iIr34BjAv =  iIr34BjAv + (abs(m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0])); //20170627

						            iIr1BjAv =  iIr1BjAv + m_wSatDataBuff_p[1][i0 * iDatWidth + j0];   //    ����Զ����ͨ��ƽ��ֵ20170627
						            iIr5BjAv =  iIr5BjAv + m_wSatDataBuff_p[5][i0 * iDatWidth + j0];   //    ����ɼ���ͨ��ƽ��ֵ20170627
					             }
                        }
					            }  // �к���ͨ������315K  20170722  ��

					            // #### ������Ԫ�����к������ֵ T4CLDBJ ����ֵ3150����315K���� ����к���ͨ������315K���ų��ڽ���Ԫ���㱳���¶� 20170722  ������к���ͨ��С��315K��ʹ���ڽ���Ԫ���㱳���¶� 20170722  ��				
                      if (  m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  <= T4BJLimit )    //20180124
					           {
                       //#### ͬ�ϣ���������Ԫ����ѡ���к�����ֵ T4CLDBJLimit ����ֵ 2900�� ��290K���� �ɼ�����ֵ VisCLDBJLimit ����ֵ500����0.5����
				  	  
                      if (m_cPixelId_p[i0][j0] == 220 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > T4CLDBJLimit && 
                          m_wSatDataBuff_p[5][i0 * iDatWidth + j0] < VisCLDBJLimit &&  ii != i && jj != j )
                          { //������ֵΪ100�����к��⡶ ���ǿɼ��ⷴ����������к�������20180316
                        
						 
						                   iCount++; //��Чֵ�ĸ���
                                  //   printf(" VIS AVER CH3>295K    %d,  %d,CH3 = %d, CH4 = %d, VIS = %d, VISAver = %d, CloudAver = %d,\n", i,j, m_wSatDataBuff_p[4][i * iDatWidth + j],m_wSatDataBuff_p[1][i * iDatWidth + j],m_wSatDataBuff_p[5][i * iDatWidth + j],wIR5BjAver_p[i][j], CloudAver_p[i][j]); //20171126
						                  iIr3BjAv =  iIr3BjAv + m_wSatDataBuff_p[4][i0 * iDatWidth + j0]; //20170627
                              iIr34BjAv =  iIr34BjAv + (abs(m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0])); //20170627
                             
						                  iIr1BjAv =  iIr1BjAv + m_wSatDataBuff_p[1][i0 * iDatWidth + j0];   //    ����Զ����ͨ��ƽ��ֵ20170627
						                  iIr5BjAv =  iIr5BjAv + m_wSatDataBuff_p[5][i0 * iDatWidth + j0];   //    ����ɼ���ͨ��ƽ��ֵ20170627
			
                          }
					           }  // �к���ͨ��С��325K  20170722  ��
                    }  
                 }

                 float fTemp; //��������Чֵ��ռ��
                 fTemp = (float)((ij * 2 + 1) * (ij * 2 + 1));
				         fTemp =  (float)iCount / fTemp * 100.;
                			                
                 //#### ������Ԫ�����¶ȼ�����Ԫ����������ֵ���� fCLDTempLimit����ֵ15��������0.15���������¶ȼ�����Ԫ������ֵ���� iCLDCountLmt ����ֵ8�� 
			 	         if (fTemp <  fCLDTempLimit || iCount < iCLDCountLmt )
			 	         {  //��Чֵռ�� �� 15%   �����С��8 ������������ //20170716
                   ij++;            //������
                 }
                 else
                 {    // ������㱳��ֵ
                    bDoId = false;

                    wIR4BjAver_p[i][j] = (unsigned short int)(iIr3BjAv / iCount);
                    wIR45BjAver_p[i][j] = (unsigned short int)(iIr34BjAv / iCount);

					          //���ӿɼ����Զ����***********************************************************20170706
					          wIR1BjAver_p[i][j] = (unsigned short int)(iIr1BjAv / iCount);
                    wIR5BjAver_p[i][j] = (unsigned short int)(iIr5BjAv / iCount);

					          //��������ƽ��ֵ***********************************************************20170716
					         CloudAver_p[i][j] = (unsigned short int)(iIr3BjAv / iCount);
		
                 }
				          // ####������Ԫ�����¶ȼ�������Χ���� CBJlimit (����Ϊ21   )
                  //   if ((ij * 2 + 1) > CBJlimit){ // �����������Ϊ11*11�������Ϊ����220����������ƽ��ֵ����Ϊ400K
                   if ((ij * 2 + 1) > BJlimit)
                { // �����������Ϊ11*11�������Ϊ����220����������ƽ��ֵ����Ϊ400K  //20180209  
					         bDoId = false;   //������ѭ����־
                
                    wIR4BjAver_p[i][j] = 0;
                    wIR45BjAver_p[i][j] = 0;

                    //���ӿɼ����Զ����***********************************************************20170706
					          wIR1BjAver_p[i][j] = 0;             //20170706
                    wIR5BjAver_p[i][j] = 0;             //20170706
             
                    //��������ƽ��ֵ*******����û���㹻�ı�����Ԫ��ʱ��������ƽ��ֵ��Ϊ40000******************20170716
				           CloudAver_p[i][j] = 4000;  //20171101					
					        // ��ʾ��������������Ԫ������Ϣ���������кš��кš���Ԫ�����������������С   ��  //2017010
				        }
              }
              while (bDoId);

          //  ����ƽ��ֵ���� 20170716 ��  *********************************************************

        } // ���㱳��ƽ��ֵ����
       
       
       }  //   iDatWidth  ѭ������
     }     //   iDatWidth  ѭ������


	 //����ͨ��6��1.6΢�ף���ͨ��7��2.2΢�ף�ƽ��ֵ 20170831 ��	
	  for (int i = 0; i < iDatHeight; i++)
	  {
      for (int j = 0; j < iDatWidth; j++)
      {
  
        if (m_cPixelId_p[i][j] == 100)
        { // �Ա��ֵΪ100�ĵ���㱳��ƽ��ֵ
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
					              iIr6BjAv =  iIr6BjAv + m_wSatDataBuff_p[6][i0 * iDatWidth + j0];    //    ����ͨ��6��1.6��ƽ��ֵ20170825      
                       iIr7BjAv =  iIr7BjAv + m_wSatDataBuff_p[7][i0 * iDatWidth + j0];   //    ����ͨ��7��2.3��ƽ��ֵ20170825  
                       iCount++; //
					             }
					          }
					       }
                   //#### ͨ��6��1.6��,ͨ��7��2.3�������¶ȼ�����Ԫ������ֵ iCountSIR (��ֵΪ10)
					if ( iCount > iCountSIR ) 
					 {
					  wIR6BjAver_p[i][j] = (unsigned short int)( iIr6BjAv / iCount);   //    ����ͨ��6��1.6��ƽ��ֵ20170825      
            wIR7BjAver_p[i][j] = (unsigned short int)( iIr7BjAv / iCount);   //    ����ͨ��7��2.3��ƽ��ֵ20170825  
					 }              
			  }           
		  }
	  }
	 // ����1.6��2.3ƽ��ֵ 20170831 ��
     return;
}

/*: *******************  FireProBJ2 *********************
* �������ƣ�  FireProBJ2
* �������⣺  ���㱳���¶�
* �������ܣ�  �����к���ı�׼ƫ����¶ȼ��к����Զ�����׼ƫ��Ĳ�ı����¶�
* ���÷���: FireProBJ2(unsigned short int *m_wSatDataBuff_p[])
*   ����˵����
*   unsigned short int *m_wSatDataBuff_p[] ��������
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::FireProBJ2()
{
          //------------- ���㱳���¶�  -----------------
     int ij, iCount;
	 float coe1 = 1.;
	 float fTemp = 0.; //��������Чֵ��ռ��
      ij = 3;       //     20161005********************************

     iCount = 0;
     bool bDoId = true;     //     20161005********************************



	  for (int i = 0; i < iDatHeight - 10; i++) {
	 	 for (int j = 0; j < iDatWidth; j++) {
		
			ij = 3;  //     20161005********************************
        
			bDoId = true;
      
       if (wIR4BjAver_p[i][j] > 0 && ( m_cPixelId_p[i][j] == 100 || m_cPixelId_p[i][j] == 220 ) )    //20180102
		   {  // ����Чƽ��ֵ����� ���������׼��
                 //ShowMessage("bejing" + IntToStr(wIR4BjAver[i][j]));
			   do
			  {
				 //---------  �����׼��   -----------
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
				
                      //#### ���������ڽ���Ԫȡ���¶Ȳ��� T4BJLimit����ֵ3150����315K��
				
					                 if ( m_cPixelId_p[i][j] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  > T4BJLimit )  //20180102
						             {
                          //#### ������Ԫѡȡ����������� RegnWEBJ (��ֵ1000)
						                   if ( jj < 1001) coe1 = 1.;
                          //#### ������ԪѡȡȨ�ز���1 Coe1,(��ֵ1)
						               //#### ������ԪѡȡȨ�ز���1 Coe2,(��ֵ0.6)
						                else  coe1 = 0.6;
					             
                           iaround = 0;                    
                           //#### ������Ԫѡȡ��Զ�����¶�����ϵ�� DtBJ1   ����ֵ350����35K��
				
                            if (m_cPixelId_p[i0][j0] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] <= m_wSatDataBuff_p[4][i * iDatWidth + j] - DtBJ2 &&
                         	 (i0 != i && j0 != j))  //20180124  С��̽����Ԫ�к�������4K
						              {						 					 
						           
                           if(( i0 == i-1 && j0 == j-1) ||( i0 == i-1 && j0 == j) ||( i0 == i-1 && j0 == j+1) ||( i0 == i && j0 == j-1) ||( i0 == i && j0 == j+1) ||( i0 == i+1 && j0 == j-1) ||( i0 == i+1 && j0 == j) ||( i0 == i+1 && j0 == j+1))//  20170828
						               {
						                 iaround = 1;
						               }
					                  if ( iaround == 0)
					                {					   
						               iCount++;
                             // �����׼��
					  		       				  
						                fIR4Bj_p[i][j] = fIR4Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv));   //   20170627 �к���ͨ�������ƽ����
						                fIR1Bj_p[i][j] = fIR1Bj_p[i][j] + (float)((m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv) * (m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv));   //   20171107 Զ����ͨ�������ƽ���� *******
                            fIR45Bj_p[i][j] = fIR45Bj_p[i][j] +  (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv));   // 20170627   �к�����Զ��������ƽ����					
					                  fIR5Bj_p[i][j] = fIR5Bj_p[i][j] + (float)((m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv) * (m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv));   //   20180217 �ɼ���ͨ�������ƽ���� *******
					                }
					                }
					               }      
                      // ����к���ͨ��С����315K��ʹ���ڽ���Ԫ���㱳���¶�
					         //#### ͬ�ϣ����������ڽ���Ԫȡ���¶Ȳ��� T4BJLimit����ֵ3150����315K�� 
			            
					         if ( m_cPixelId_p[i][j] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  <= T4BJLimit )  //20180102
					          {
                      			 //#### ͬ�ϣ�������Ԫѡȡ��Զ�����¶�����ϵ�� DtBJ1   ����ֵ350����35K��							
                         
						          if (m_cPixelId_p[i0][j0] == 100 && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] <= m_wSatDataBuff_p[4][i * iDatWidth + j] - DtBJ2 &&
						          	 (i0 != i && j0 != j)) 
						          { //20180124
                                 iCount++;

                      // �����׼��
					          
						          fIR4Bj_p[i][j] = fIR4Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv));   //   20170627 �к���ͨ�������ƽ����
                      fIR1Bj_p[i][j] = fIR1Bj_p[i][j] + (float)((m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv) * (m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv));   //   20171107 Զ����ͨ�������ƽ���� *******
                      fIR45Bj_p[i][j] = fIR45Bj_p[i][j] +  (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv));   // 20170627   �к�����Զ��������ƽ����				
					            fIR5Bj_p[i][j] = fIR5Bj_p[i][j] + (float)((m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv) * (m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv));   //   20180217 �ɼ���ͨ�������ƽ���� *******
					           }
					          }  
					              //������Ԫ��׼�����     20180102  ��ʼ
                   if (m_cPixelId_p[i][j] == 220 && m_wSatDataBuff_p[4][i * iDatWidth + j] > T4CLDBJ &&  
                     	m_wSatDataBuff_p[5][i * iDatWidth + j] <VisCLDBJLimit &&   m_wSatDataBuff_p[1][i * iDatWidth + j] > T11CLD)
                    { //������ֵΪ����220���й�����  20180102
                      
                    
                       if (  m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  > T4BJLimit )   // 20180316
					            {

				                  // ���������̽����Ԫi,j����Ԫ����������i-1,j-1;i-1,j;i-1,j+1;i,j-1;i,j;i,j+1;i+1,j-1,i+1,j;i+1,j+1����Ԫ 20170716  ��
					              //#### ������Ԫ����ѡ���к�����ֵ T4CLDBJLimit ����ֵ 2650�� ��265K����####  ������Ԫ����ѡ��ɼ�����ֵ VisCLDBJLimit ����ֵ500����0.5����
                
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
                                 iCount++; //��Чֵ�ĸ���
                                 fIR4Bj_p[i][j] = fIR4Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv));   //   20180102 �к���ͨ�������ƽ����
						                     fIR1Bj_p[i][j] = fIR1Bj_p[i][j] + (float)((m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv) * (m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv));   //   20180102 Զ����ͨ�������ƽ���� *******
                                 fIR5Bj_p[i][j] = fIR5Bj_p[i][j] + (float)((m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv) * (m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv));   //   20180102 �ɼ���ͨ�������ƽ���� *******
                                 fIR45Bj_p[i][j] = fIR45Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv));   // 20180102   �к�����Զ��������ƽ����			
					                    }
                       }
					            }  // �к���ͨ������295K  20180102  ��

				
                       if (  m_wSatDataBuff_p[4][i0 * iDatWidth + j0]  <= T4BJLimit )        //20180113
					          {
                     // �ɼ�����ֵ VisCLDBJLimit ����ֵ500����0.5����
				  	  
                  
                       if (m_cPixelId_p[i0][j0] == 220  && m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > T4CLDBJLimit &&
                       	  m_wSatDataBuff_p[5][i0 * iDatWidth + j0] < VisCLDBJLimit  &&  ii != i && jj != j )
                       { //������ֵΪ100�����к��⡶ ���ǿɼ��ⷴ����������к�������20180316
                         
						                iCount++; //��Чֵ�ĸ���                          
						               fIR4Bj_p[i][j] = fIR4Bj_p[i][j] + (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - wIR4BjAv));   //   20180102 �к���ͨ�������ƽ����
						               fIR1Bj_p[i][j] = fIR1Bj_p[i][j] + (float)((m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv) * (m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR1BjAv));   //   20180102 Զ����ͨ�������ƽ���� *******
                          fIR5Bj_p[i][j] = fIR5Bj_p[i][j] + (float)((m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv) * (m_wSatDataBuff_p[5][i0 * iDatWidth + j0] - wIR5BjAv));   //   20180102 �ɼ���ͨ�������ƽ���� *******
                          fIR45Bj_p[i][j] = fIR45Bj_p[i][j] +  (float)((m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv) * (m_wSatDataBuff_p[4][i0 * iDatWidth + j0] - m_wSatDataBuff_p[1][i0 * iDatWidth + j0] - wIR45BjAv));   // 20180102   �к�����Զ��������ƽ����			
			                  }
					          }  // �к���ͨ��С��295K  20170722  ��

					          }
                       //������Ԫ��׼�����     20180102  ����


                 }         
                 }
                 //float fTemp; //��������Чֵ��ռ��
                 fTemp = (float)((ij * 2 + 1) * (ij * 2 + 1));
				         fTemp =  (float)iCount / fTemp * 100.;
                				 
                  //#### �����¶ȼ�����Ԫ����������ֵ���� fTempLimit����ֵ15��������0.15���������¶ȼ�����Ԫ������ֵ���� iCountLimit ����ֵ8�� 
                  if (fTemp < 15. || iCount < 8 )
                  {  //��Чֵռ�� �� 15%   �����С��6 ������������ //20170716
                   ij++;            //������
                  }
                 else
                 	{    // ������㱳����׼��
                    bDoId = false;
				
				          	fIR4Bj_p[i][j] = sqrt( (fIR4Bj_p[i][j]) / iCount);       // ******************20180217*******************
                    fIR1Bj_p[i][j] = sqrt( (fIR1Bj_p[i][j]) / iCount);       //                  Զ�����׼�� 20180217
                    fIR45Bj_p[i][j] = sqrt( (fIR45Bj_p[i][j]) / iCount);     // ******************20180217*******************
                    fIR5Bj_p[i][j] = sqrt( (fIR5Bj_p[i][j]) / iCount);        //                 �ɼ����׼��20180217

                 }
                  //#### �����¶ȼ�������Χ���� BJlimit (����Ϊ51 )
          
				     if ((ij * 2 + 1) >BJlimit)
			       { //    20181024
					     bDoId = false;   //������ѭ����־
                //    m_cPixelId_p[i][j] = 250;
					    Noefft_p[i][j] = 250;   //20170823
				
              fIR4Bj_p[i][j] = 0;
					    fIR1Bj_p[i][j] = 0;    //20171107 
              fIR45Bj_p[i][j] = 0;
				      fIR5Bj_p[i][j] = 0;    //20180102          
             }
        }  //���󴰿�ѭ��
			while (bDoId); 
		   }   // ������������		          
        }      //end === iDatWidth
     }         //end === iDatHeight

	


	  return;
}

/*: *******************  FirePro *********************
* �������ƣ�  FirePro
* �������⣺  �����ʶ
* �������ܣ�  ȫԲ�����������ʶ��
* ���÷���: FirePro(unsigned short int *m_wSatDataBuff_p[], unsigned char * m_TempBuff2_p[])
*   ����˵����
*   unsigned short int *m_wSatDataBuff_p[] ��������
*   unsigned char * m_TempBuff2_p[]       ��ʶ���
     m_fSunZenithAnale
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::FirePro() //20170707	
{
          // ��ʶ���
     float fTb3th, fTb34th;
     //fTb3th �к�����ֵ��fTb34th �к��� - Զ�������ֵ
     fTb3th = 0.; fTb34th = 0.;

//  ------- 20131028 ����ֵ�ļ� ------------------------

  int factr_mid, factr_far,dt3th,dt34th; // �ֱ�Ϊ�к������ӣ�Զ�������ӣ��к�����ֵ���к����Զ��������ֵ

           
		//#### �к�����ȷ��ϵ��������T4_Factr����ֵΪ3K, #### �к�����Զ�������²�����ȷ��ϵ������:T4_11Factr,��ֵΪ3K ��
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

	  int LineNHUP,LineNHLW,RawNHUP,RawNHLW;    // �������к����ޡ����ޣ��������Ͷ��ϲ��к����ޡ����ޡ�20180311
	  int LineETUP,LineETLW;                    // ���ϲ��к����ޡ����ޡ�20180311
	  int LineWNUP,LineWNLW,RawWNUP,RawWNLW;    // �������к����ޡ����ޣ������������ϲ��к����ޡ����ޡ�20180311
	  int LineWSUP,LineWSLW;                    // ���ϲ��к����ޡ����ޡ�20180311              
      int QZGYLINENH,QZGYLINESH;     // ��ظ�ԭ���к�  20180403
	  int iregn;                //20180227
    //  /*  
	
	  iregn = 1;    // for test  20180312 
    if (  iregion == 1 )       //  ����ͼ���������к� ��ʼ  20180313 regional
	  {      // ��������ͼ���к�
	   LineNHUP = CLDRGLineNHUP ;LineNHLW = CLDRGLineNHLW; RawNHUP=CLDRGRawNHUP ;RawNHLW = CLDRGRawNHLW;    // ���趫�����к����ޡ����ޣ��������Ͷ��ϲ��к����ޡ����ޡ�20180311
	   LineETUP = CLDRGLineETUP ;LineETLW = CLDRGLineETLW;                                                    // ���趫�ϲ��к����ޡ����ޡ�20180311
	   LineWNUP = CLDRGLineWNUP;LineWNLW =CLDRGLineWNLW; RawWNUP=CLDRGRawWNUP;RawWNLW=CLDRGRawWNLW;       // �����������к����ޡ����ޣ������������ϲ��к����ޡ����ޡ�20180311  
	   LineWSUP = CLDRGLineWSUP; LineWSLW = CLDRGLineWSLW;                                                   // �������ϲ��к����ޡ����ޡ�20180311
	   QZGYLINENH = QZGYRGLINEUP;       // 20180403 ������ͼ��ظ�ԭ�����к�
       QZGYLINESH = QZGYRGLINELW;       // 20180403 ������ͼ��ظ�ԭ�϶��к�  	  
	  }
	  else    // Բ��ͼ���Բ��ͼ   //20180311
	  {
	  LineNHUP=CLDLineNHUP; LineNHLW=CLDLineNHLW; RawNHUP=CLDRawNHUP; RawNHLW=CLDRawNHLW;    // ���趫�����к����ޡ����ޣ��������Ͷ��ϲ��к����ޡ����ޡ�20180311
	  LineETUP=CLDLineETUP; LineETLW=CLDLineETLW;                                            // ���趫�ϲ��к����ޡ����ޡ�20180311
	  LineWNUP=CLDLineWNUP; LineWNLW=CLDLineWNLW; RawWNUP=CLDRawWNUP;RawWNLW=CLDRawWNLW;     // �����������к����ޡ����ޣ������������ϲ��к����ޡ����ޡ�20180311  
	  LineWSUP=CLDLineWSUP; LineWSLW=CLDLineWSLW;                                            // �������ϲ��к����ޡ����ޡ�20180311
	  QZGYLINENH = QZGYLINELW;       // 20180403 ��Բ��ͼ��ظ�ԭ�����к�
      QZGYLINESH = QZGYLINE;         // 20180403 ��Բ��ͼ��ظ�ԭ�϶��к� 
	  	  
	  }
    //  */                      //  ����ͼ���������к� ����  20180311
	 printf("iregn=%d, LineNHUP=%d, LineNHLW=%d,RawNHUP=%d,RawNHLW=%d,LineETUP=%d,LineETLW=%d,LineWNUP=%d,LineWNLW =%d,RawWNUP=%d, RawWNLW=%d,LineWSUP=%d,LineWSLW =%d, \n ",
		 iregn,LineNHUP,LineNHLW,RawNHUP,RawNHLW,LineETUP,LineETLW,LineWNUP,LineWNLW,RawWNUP, RawWNLW,LineWSUP,LineWSLW ); //20180312	 
	 
	int inm = 0;
	int inm1 = 0;  // 20170810 ����ܱ���Ԫ��̽����Ԫ�к�������ĸ�������	

	 // ****�м����ļ����ɺʹ洢  20170627   �� *********************************     
	 string sFN; 

  

	 // ****���淴����ļ����ɺʹ洢  20180514   �� *********************************


	//���ɾ��淴�������  20170828 �� *********************************
	 int inm3 = 0;	
	 for (int i = 0; i < iDatHeight; i++)
	 {
          for (int j = 0; j < iDatWidth; j++)
     {
               // ���淴��������̫���춥��С��85�ȣ�ͨ��6��1.6�����ڱ���0.1���к������310K�����뱳���������12K��С��25K��ͨ��6�����������0
	               //#### ̫���춥����ֵ SunZRFLT (��ֵ85  )��####ͨ��6��1.6���뱳��������ֵ DR6SLimit ����ֵ100����0.1����####�к���ͨ���뱳��������ֵ���� DT4SLTLW ����ֵ120����12K����####�к���ͨ���뱳��������ֵ���� DT4SLTUP ����ֵ250����25K��. ####�к���ͨ�����淴���¶���ֵ T4SunLT ����ֵ3100����310K��. 
       if ( m_fSunZenithAnale[i][j] < SunZRFLT && m_wSatDataBuff_p[6][i * iDatWidth + j] - wIR6BjAver_p[i][j] > DR6SLimit && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4SLTLW  && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < DT4SLTUP  && m_wSatDataBuff_p[4][i * iDatWidth + j] >  T4SunLT &&  wIR6BjAver_p[i][j] != 0 )  //20170829
	     {
         SolarRef_p[i][j] = 1;
	     }
	   }
	 }
   	//���ɾ��淴�������  20170828 ��  *******************************

   
//  �����Ԫȷ�� *****************************************************************************************************************

      for (int i = 10; i < iDatHeight - 10; i++)
      {    //20180316   ���Ƕ�����ͼ�п�������������磬���Ԥ�ȼ�ȥ10�С� ������ʼ�г�ֵ��λ10����������ͼ��һ����ȱֵ��
        for (int j = 0; j < iDatWidth; j++)
        {
   
	        fTb3th = 0.; fTb34th = 0.;
          idcloud = 0;   //20170627
	        idesert = 0;   //20170627
	         iwater = 0;    //20170810
		       iSolarRef = 0;  //20170828
		       iSolarRef2 = 0;
 

	      // ��վ��Ը��»����ʶ 20170707 ��
	      //#### ���Ը��»����ʶ�к��� ��ֵ T4HiFire  ( ��ֵ3400����340K), #### ���Ը����к����뱳��������ֵ  DT4HiFire ( ��ֵ120����12K), #### ���Ը����к�����Զ���������ֵ�� DT4_11HiFire ( ��ֵ150����15K)��#### ���Ը��»��ɼ���ͨ����ֵ CloudHiFire�� ��ֵ500����0.5��           
	      	if ( ( m_cPixelId_p[i][j] == 100 || m_cPixelId_p[i][j] == 220) && m_wSatDataBuff_p[4][i * iDatWidth + j] > T4HiFire
	      		&& m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4HiFire
	      		&& m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] > DT4_11HiFire
	      		&& m_wSatDataBuff_p[5][i * iDatWidth + j] < CloudHiFire    // ���Ը��»����ʶ 20180124
	      	    && ( j> 1000 && m_wSatDataBuff_p[5][i * iDatWidth + j] < 250 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > wIR1BjAver_p[i][j] && m_wSatDataBuff_p[5][i * iDatWidth + j] < wIR5BjAver_p[i][j]))   // 20180326 ͼ������ˮ�徵�淴���ж�
	      	 {
	           inm += 1;   // 20170707
	           m_cPixelId_p[i][j] = 10;  // �����Ϊ10
             m_TempBuff2_p[i][j] = 1;   // ��ʶ������Ϊ1	                                                                                                                                                                                                                                                                                                
           }                // ��վ��Ը��»����ʶ 20170707 ��
        
	      // ȱ�ٱ�����Ԫ������ԭ���Ļ��ȷ�ϣ����250����
	      // ȱ�ٱ�����Ԫ����ͼ�񶫲��ĸ��»����ʶ 20170711 ��   �����к������325K�����к������Զ����30K ���� Զ������� 278K���� ��Ԫλ�ڶ��������кŴ���600���кŴ���2300.
          //#### ȱ�ٱ�����Ԫ������Ԫ�����ʶ,ͼ�����·� �к����¶���ֵ LKT4RL (��ֵ3250����35K),�к���Զ���������ֵ DLKT4_11RL (��ֵ300����30K)��Զ����������ֵ  LKT11RL (��ֵ2780����278K),���·��кŽ���ֵ LKLineRL,����ֵ600�����к���ֵ LKRawRL ����ֵ2300��.
        if (  Noefft_p[i][j] == 250  && m_wSatDataBuff_p[4][i * iDatWidth + j] > LKT4RL && m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] > DLKT4_11RL   &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > LKT11RL && i >= LKLineRL &&  j >= LKRawRL )    // ȷʵ������Ԫ���»����ʶ 20171119  
        	
	      {
	          inm += 1;   // 20170711
	          m_cPixelId_p[i][j] = 10;  //�����Ϊ10
             m_TempBuff2_p[i][j] = 1;   // ��ʶ������Ϊ1
	      }  
      
	         // ȱ�ٱ�����Ԫ���������250��������ͼ���������ĸ��»����ʶ ��  �����к������338K�����к������Զ����30K ���� Զ������� 278K���� ��Ԫλ�������������кŴ���600���к�С��2300.
	          //#### ȱ�ٱ�����Ԫ������Ԫ�����ʶ,ͼ�����·� �к����¶���ֵ LKT4CW (��ֵ3380����338K),#### �к���Զ���������ֵ DLKT4_11CW (��ֵ300����30K)��#### Զ����������ֵ  LKT11CW (��ֵ2780����278K),#### ���·��кŽ���ֵ LKLineRL,����ֵ600�����к���ֵ LKRawRL ����ֵ2300��.
	          else if ( Noefft_p[i][j] == 250  && m_wSatDataBuff_p[4][i * iDatWidth + j] > LKT4CW && m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] > DLKT4_11CW  &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > LKT11CW   && i >= LKLineRL  &&  j < LKRawRL )    //  ȷʵ������Ԫ���»����ʶ 20170711 
	       {
	          inm += 1;   // 20170711
	          m_cPixelId_p[i][j] = 10;  //�����Ϊ10
              m_TempBuff2_p[i][j] = 1;   // ��ʶ������Ϊ1
	       
	       }  
    
            // ȱ�ٱ�����Ԫ���������250��������ͼ�񱱲��ĸ��»����ʶ ��  20170810       
             //#### ȱ�ٱ�����Ԫ������Ԫ�����ʶ,ͼ�񱱲� �к����¶���ֵ LKT4NH (��ֵ3150����315K),####�к���Զ���������ֵ DLKT4_11NH (��ֵ300����30K)��####Զ����������ֵ LKT11NH (��ֵ2500����250K), ####ͼ�񱱲��кŽ��޲��� LKLineNH,����ֵ600���� ####ͼ�񱱲��кŽ��޲��� LKRawNH ����ֵ600��.
            else if (  Noefft_p[i][j] == 250  && m_wSatDataBuff_p[4][i * iDatWidth + j] > LKT4NH && m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] > DLKT4_11NH  &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > LKT11NH   && i > 100 && i < LKLineNH &&  j > LKRawNH )    //  ȷʵ������Ԫ���»����ʶ 20170711 
          	
	        {
	           inm += 1;   // 20170711
	           m_cPixelId_p[i][j] = 10;  //�����Ϊ10
               m_TempBuff2_p[i][j] = 1;   // ��ʶ������Ϊ1
              
	         }  
     
	         // ****�������ʶΪ���Ĳ��㱳����Ԫ������Ԫ�Ƿ���ڶ���ڽ���Ԫ�����Ԫ�¶���������¶Ȳ����ߵ���������ǣ���Ϊ������  20170824
	          inm1 = 0;
	         if (m_cPixelId_p[i][j] == 10 && Noefft_p[i][j] == 250  ) // �����ȷ��Ϊ���㱳�������Ļ����Ԫ
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
                               // ####  �к���������²�����ֵ������T4NR_Df��f��ֵΪ4K��  20171010                      
		      			              if (  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] < T4NR_Df  && ( i0 != i ||  j0 != j ) ) // 20180514
		      			               inm1 = inm1 + 1;
		      			          }
		      		         }         
	            
	            //#### �������Ԫ������������  �����Ԫ������ֵ����NRPixNo1 ����ֵ4�����к�����ֵ���� NRT4Limit ����ֵ3150����315K���� 
	            if (inm1 > NRPixNo1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= NRT4Limit   )   //  �����̽����Ԫ�к������²���С��3K����Ԫ����5������Ϊ����Ԫ�ǻ��  // 20170811  ��
	            {
	            m_cPixelId_p[i][j] = 100;
	            m_TempBuff2_p[i][j] = 0;
	            }
	             // �к���С����315K 
	            //#### �������Ԫ�����������У���С���к�����ֵ��������£�  �����Ԫ������ֵ����NRPixNo2 ����ֵ5�� 
	            else if (inm1 > NRPixNo2  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < NRT4Limit   )   //  �����̽����Ԫ�к������²���С��3K����Ԫ����5������Ϊ����Ԫ�ǻ��  // 20170811  ��
	            {
	            m_cPixelId_p[i][j] = 100;
	            m_TempBuff2_p[i][j] = 0;
	            }
	         }
	        //  �����̽����Ԫ�к������²���С��4K����Ԫ����3������Ϊ����Ԫ�ǻ��  // 20170824  ��   	 
	          else if (   Noefft_p[i][j] == 250 &&  m_cPixelId_p[i][j] == 100  )
	          m_cPixelId_p[i][j] = 250;
         
            // ȱ�ٱ�����Ԫ������ԭ���Ļ��ȷ�ϣ����250����
	       // ������Ԫ�����ʶ�������Ϊ��������Ԫ�����ʶ  20170713 ��  ************ 	     
	       if ( m_cPixelId_p[i][j] != 10)  //20170810   
	       {
	       
		          ii = i;
	           jj = j;
	           if ( ii - 1 < 0 ) ii = 2;
	          if ( ii+1 > iDatHeight-1 ) ii =  iDatHeight - 2;
	          if ( jj -1 < 0) jj = 2;
	          if (  jj+1 > iDatWidth-1) jj = iDatWidth - 2;
	          // 20170801 �����±��޽���
            
	          // �ų������ܱ��л�Į����Ԫ��� 20170801 ��
	          if ( CloudAver_p[i][j] != NULL  && m_cPixelId_p[ii-1][jj-1]  != 150  && m_cPixelId_p[ii-1][jj]  != 150  && m_cPixelId_p[ii-1][jj+1]  != 150  && m_cPixelId_p[ii][jj-1]  != 150 && m_cPixelId_p[ii][jj+1]  != 150  && m_cPixelId_p[ii+1][jj-1]  != 150  && m_cPixelId_p[ii+1][jj]  != 150  && m_cPixelId_p[ii+1][jj+1]  != 150 ) // ���Ӽ��������Ԫ�ܱ��Ƿ��л�Į��    20170801 
	          {     // �ж������������Ԫ��ʶ  �к������310K 20170801
	          
	       
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
		           	&& m_wSatDataBuff_p[1][i * iDatWidth + j] > DT11CLDFIRELW ) // ���Ӽ��������׼��  20180223
                       || m_wSatDataBuff_p[4][i * iDatWidth + j] > T4CLDFIREUP && m_wSatDataBuff_p[4][i * iDatWidth + j] - CloudAver_p[i][j] > DT411CLDFIRELW  // 20180303 ���Ӷ���������ջ����ʶ
		           	&& m_wSatDataBuff_p [1][i * iDatWidth + j] > T4CLDFIRELW && m_wSatDataBuff_p [1][i * iDatWidth + j] - wIR1BjAver_p[i][j] > DT11CLDFIRELAND
		           	&&  m_wSatDataBuff_p [5][i * iDatWidth + j] < 250 )  //20180304
		          {
	       
		      
		      	  //#### ������Ԫ�����ʶ��ͼ�񶫲��кŲ�����CLDRawET ( ��ֵ2300 )
         
	                   if ( j < CLDRawNHLW)     // 20180104
		                 {
	                          //  ������������Ԫ�жϿ�ʼ   20180124
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
                               
		              			              //����¶���?Ԫaѡ?��?
		              			              // ####  ��������Ԫ�к������������ֵ����.
		              	
                              if (  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] < SameT4DfUP &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > SameT4DfLW   && ( i0 != i  ||  j0 != j  )) // 20171012
		              			        {
		              			           inm1 = inm1 + 1; 
		              
		              			        }
		              			       }
		              		        }       
	                 
		              			   
		      // ���������������Ԫ��������  20180104 ����    
		      //�������������ȷ�� 20180311
         
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
	       
             m_TempBuff2_p[i][j] = 1;   // ��Dʶo?�ᡧ���?�ࡧ��㨬?Ϊa1
             //   ����¼д���ļ�t
		      inm += 1;
	         }
	       
         // �������������ȷ�� 20180308 ��ʼ
          
         
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
	       
		      m_cPixelId_p[i][j] = 10;  //��e�㨬?�ꨤ��o��?Ϊa10
          m_TempBuff2_p[i][j] = 1;   // ��Dʶo?�ᡧ���?�ࡧ��㨬?Ϊa1     
		      inm += 1;
	       }		
         //  �������������ȷ�� 20180308 ����
         
		       //���ϲ��������ȷ�� 20180308  ��ʼ  
          if ( ( (j >= RawNHUP &&  j <= RawNHLW ) && (i >=LineETUP &&  i <= LineETLW )) //20180312
	          && ( (inm1 < CLDSameNoET-1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIRECT   //  20180224
		      && ( m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4CLDFIRECT        //  20180224
             &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] <= DT4CLDFIREUPLW ))              //  20180313
	         &&  m_wSatDataBuff_p[1][i * iDatWidth + j] > wIR1BjAver_p[i][j]                      //20180313
             &&  m_wSatDataBuff_p[5][i * iDatWidth + j] < wIR5BjAver_p[i][j]  )                  //  20180305 
		      ||  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4CLDFIREUPLW                  //  20180224
		      && (inm1 < CLDSameNoET  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIRENH ))  //  20180224
         
	        { 
	          m_cPixelId_p[i][j] = 10;  //��e�㨬?�ꨤ��o��?Ϊa10
             m_TempBuff2_p[i][j] = 1;   // ��Dʶo?�ᡧ���?�ࡧ��㨬?Ϊa1
	         inm += 1;
	         }
         
	        //���ϲ��������ȷ�� 20180308
	       
          if ( ( (j >= RawWNUP &&  j <= RawWNLW ) && (i >= LineWSUP &&  i <= LineWSLW ))    //20180312
	       
                &&( ( inm1 < CLDSameNoWS -1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIREWS 
		       &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] > DT4CLDFIREWS  
		      &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] <= DT4CLDFIREUPLW )        //20180313	
            ||  inm1 < CLDSameNoWS +1  && (  m_wSatDataBuff_p[4][i * iDatWidth + j] >= T4CLDFIREWS       //20180217		  
		        && (m_wSatDataBuff_p[4][i * iDatWidth + j]-CloudAver_p[i][j] >DT4CLDFIREUPLW ))))    //20180211
	        { 
	       
		        m_cPixelId_p[i][j] = 10;  //�����Ϊ10
             m_TempBuff2_p[i][j] = 1;   // ��ʶ������Ϊ1
		      inm += 1;
	         }	 
         
	         // ��Ӱ���ˮ��Ӱ���ж� ��ʼ 20180224  ********************
         
	       if (m_cPixelId_p[i][j] == 10 &&
	       	    m_wSatDataBuff_p[4][i * iDatWidth + j] < 3200 && m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] && m_wSatDataBuff_p[5][i * iDatWidth + j] >= wIR5BjAver_p[i][j] 
	         || m_wSatDataBuff_p[4][i * iDatWidth + j] < 3200 && m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] - 30 && m_wSatDataBuff_p[5][i * iDatWidth + j] >= wIR5BjAver_p[i][j] - 50 // 20180316	  
	         || m_wSatDataBuff_p[4][i * iDatWidth + j] < 3250 && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 200 && m_wSatDataBuff_p[1][i * iDatWidth + j]- wIR1BjAver_p[i][j] > 20  && 
	         m_wSatDataBuff_p[5][i * iDatWidth + j]- wIR5BjAver_p[i][j] > 100   //20180226	ˮ���ԵӰ��   
	         || m_wSatDataBuff_p[5][i * iDatWidth + j] > 400 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < 2650         //20180226    // �Ʒ���Ӱ��
	                 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3100               //20180226
	         || ( j< 1000 && m_wSatDataBuff_p[5][i * iDatWidth + j] > 200 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] <= wIR1BjAver_p[i][j] +10 && m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j]-10 )   // 20180326 ͼ������ˮ�徵�淴���ж�
	          || (  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3250 && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 300 && m_wSatDataBuff_p[1][i * iDatWidth + j]- wIR1BjAver_p[i][j] > 50  && m_wSatDataBuff_p[5][i * iDatWidth + j]- wIR5BjAver_p[i][j] < 30)  )   // 20180513 ½�������ཻ����Ӱ���ж�
	         {
              m_cPixelId_p[i][j] = 100;
	           m_TempBuff2_p[i][j] = 0;
	       
	       }
             // ��Ӱ���ˮ��Ӱ���ж� ���� 20180224   *********************
         
                    }    //  ������������Ԫ�жϽ���   20180124   
		          }    		
         
	       }   //���������ʶ���� 
	       } //  ���������ʶ ��
         
			
	       if (wIR4BjAver_p[i][j] > 0 && m_cPixelId_p[i][j] == 100)
	        { //�б���ֵ�ͱ��Ϊ100�ĵ�Ϊ��Ч��ʶ��
                	if (b123)
                	{
                	    cout << "id = 100..." << endl;
                	    b123 = false;
                	}
	       
			    	
			    	// ����׼�������е����ݴ��뻺��������ȷ�ϻ�㹫ʽ��ʹ�á�����׼����������ݻ����ں���������ȷ����ʹ�ã��������Ԫ�¶Ƚϵͣ���׼��ϴ���������	20171010  
			    	 f4biaoji  = fIR4Bj_p[i][j];
			    	 f45biaoji = fIR45Bj_p[i][j];
	          // ��׼�����������ڻ��ȷ��  20171010
                 //#### �к����׼��ͼ�񱱲�������ֵ���� T4BJDV_NHUP ( ��ֵ3.)
                     if (fIR4Bj_p[i][j] > T4BJDV_NHUP)    // 20171027 *****************
                	 { 
			         //#### �к����׼��ͼ�񱱲������к� T4BJDVLineNH ( ��ֵ600   )
			    	 if ( i < T4BJDVLineNH )      // ����ͼ���׼���к�����
                 //#### �к����׼��ͼ�񱱲���������ֵ������ T4BJDV_NHUPAJ,��ֵΪ3K��   20101010
			    	  TfIR4Bjc =  T4BJDV_NHUPAJ; //20171027
			    	  else
			     //#### �к����׼��ͼ�����ϲ�������ֵ���� T4BJDV_SHUP ( ��ֵ10K) 20171010       
			      if ( fIR4Bj_p[i][j] >= T4BJDV_SHUP  )  // 20171008
			     // ####  �к����׼��ͼ���ϲ���������ֵ����: T4BJDV_SHUPAJ,����ֵ4K��     20171010   
			    		  TfIR4Bjc =  T4BJDV_SHUPAJ ;
			    	  else
			      // #### �к����׼��ͼ���ϲ�һ������ֵ����: T4BJDV_SHAJ,����ֵ2K��   ���� ��׼����3K��10K֮�䣬20171010          
			    	  TfIR4Bjc =  T4BJDV_SHLWAJ;   //20171027				 
			    	 }
			    	 //#### �к����׼��ͼ�����ϲ�����ֵ��ֵ���� T4BJDV_SHLW ( ��ֵ4K)
                         else if (fIR4Bj_p[i][j] <= T4BJDV_SHLW)   //    20171027
			    	//#### �к����׼��ͼ���ϲ������������� T4BJDV_SHLWAJ ( ��ֵ2K)��#### �к�����ȷ��ϵ������ T4_Factr ����ֵΪ3��
			    	   TfIR4Bjc =  T4BJDV_SHLWAJ ;     			         
			    	   fTb3th =  TfIR4Bjc  * T4_Factr;    		  
			    	    fTb3th = fTb3th;      //20180217
			       if (  fTb3th < 0.  )  fTb3th = 150.;   
               
			      // �к����Զ��������׼������
			       //####  �к����Զ�����׼��ͼ�񱱲�������ֵ������ T4_11BJDV_NHUP (��ֵ4.)
             if (fIR45Bj_p[i][j] > T4_11BJDV_NHUP)  //20171027
			    	 {
			         //####  �к����Զ��������׼��ͼ�񱱲������к� T45BJDVLineNH (��ֵ600) ��####  �к����Զ�����׼��ͼ�񱱲���������ֵ������T4_11BJDV_NHUPAJ��(��ֵ2.5)  
			    		 if(i < T45BJDVLineNH)
			    		 TfIR45Bjc = T4_11BJDV_NHUPAJ;   //20171027
			    	  //#### �к����Զ��������׼��ͼ���ϲ�������ֵ������ T4_11BJDV_SHUP��(��ֵ10K)         
			    	   else if ( fIR45Bj_p[i][j] >=  T4_11BJDV_SHUP )   //20171008
			         // ####  �к����Զ��������׼��ͼ���ϲ���������ֵ������T4_11BJDV_SHUPAJ ����ֵ4K��   20171010		  				           
			    		  TfIR4Bjc =  T4_11BJDV_SHUPAJ;//20171008
			    	    else
			    	   // #### �к����Զ�����׼��ͼ�����ϲ�һ������ֵ������ T4_11BJDV_SHAJ������ֵ2K��   ���� ��׼����3K��10K֮�䣬20171010
			    		  TfIR45Bjc =  T4_11BJDV_SHAJ;          // 20171027
			    	 }
			        //#### �к����Զ��������׼��ͼ��������ֵ���� T4_11BJDV_SHLW ( ��ֵ3K)
              if (fIR45Bj_p[i][j] < T4_11BJDV_SHLW)     //20171027
               //#### �к����Զ�����׼��ͼ����������ֵ���� T4_11BJDV_SHLWAJ ( ��ֵ2K)�� #### �к�����ȷ��ϵ������ T4_11Factr ����ֵΪ3��   
                   TfIR45Bjc = T4_11BJDV_SHLWAJ;        // 20170804 *****************
			             
			             fTb34th =TfIR45Bjc *  T4_11Factr;   //20170804         
		              fTb34th = fTb34th;  //20180217
			       
		     // �����жϻ�㣺�к�����к���ı�����к��������ֵ�� ���к����Զ����Ĳ����к����Զ���ⱳ���Ĳ��� �� ��ֵ���ҿɼ��� �� 0.45����ͨ��6�뱳������С��0.1 Ϊ���.
		     //####   �ɼ�����ȷ����ֵ������Vis_Fireth ����ֵ450����0.45����####ͨ��6�뱳��������ȷ�Ͼ��淴����ֵ������DFR1.6_Fire, (��ֵ100����0.1)�����������춥����ֵ������SatZ_fire ,��ֵ81.#### ����̫���춥����ֵ������SunZ_fire (  ��ֵΪ95)
	
            if (  m_fSunZenithAnale[i][j] > SunZ_fire ) //20171031
			    	{	
		               //#### �����к�����Զ�������ƽ��ֵ������ֵ����  IR45BjAver_fire ����ֵΪ100����#### #### �����к�����Զ�������ƽ��ֵ����������ֵ����  IR45BjAver_fireAJ		
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
			    	   m_cPixelId_p[i][j] = 10;  //�����Ϊ10
                m_TempBuff2_p[i][j] = 1;   // ��ʶ������Ϊ1  
                // �����ж��Ƿ��ھ��淴����ڽ�
			    
			    	   if (( SolarRef_p[i-1][j-1]== 1 ||  SolarRef_p[i-1][j]== 1 || SolarRef_p[i-1][j+1]== 1 || SolarRef_p[i][j-1]== 1 ||  SolarRef_p[i][j]== 1 || SolarRef_p[i][j+1]== 1 ||  SolarRef_p[i+1][j-1]== 1 || SolarRef_p[i+1][j]== 1  || SolarRef_p[i+1][j+1]== 1
			    		   ||  SolarRef_p[i-2][j-2]== 1 || SolarRef_p[i-2][j-1]== 1 || SolarRef_p[i-2][j]== 1 || SolarRef_p[i-2][j+1]== 1 ||  SolarRef_p[i-2][j+2]== 1 
			    		   ||  SolarRef_p[i-1][j-2]== 1 || SolarRef_p[i][j-2]== 1 || SolarRef_p[i+1][j-2]== 1 || SolarRef_p[i-1][j+2]== 1 ||  SolarRef_p[i][j+2]== 1 ||  SolarRef_p[i+1][j+2]== 1
			    		   ||  SolarRef_p[i+2][j-2]== 1 || SolarRef_p[i+2][j-1]== 1 || SolarRef_p[i+2][j]== 1 || SolarRef_p[i+2][j+1]== 1 ||  SolarRef_p[i+2][j+2]== 1    )     //20180514
			    		   && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j]  < 200 )    //20180514
			    	    {
			    	      iSolarRef = 1;
			    	    }
         
                    // �����ж��Ƿ��о��淴��Ӱ��    //20170828
			    	//#### �к���ͨ���뱳��������ֵ���� SunRfT4 (��ֵ150����15K)��####ͨ��6�뱳������������ֵ������SunRfCH6LW (��ֵ60����0.06)��#### ͨ��6�뱳������������ֵ������ SunRfCH6Up (��ֵ100����0.1)=  %d,
            if ((m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j]) < SunRfT4 && m_wSatDataBuff_p[6][i * iDatWidth + j] - wIR6BjAver_p[i][j] > SunRfCH6LW && m_wSatDataBuff_p[6][i * iDatWidth + j] - wIR6BjAver_p[i][j] < SunRfCH6UP )
			    	 {
			    	   iSolarRef2 = 1;
			    	 }
                     
			    	// �����ж��Ƿ���������Ե			
			     //       ���������ܱ߷�Χ��   20180207
			    	   if ( m_cPixelId_p[i-1][j-1] == 220 ||  m_cPixelId_p[i][j-1] == 220 ||  m_cPixelId_p[i+1][j-1] == 220 ||  m_cPixelId_p[i-1][j] == 220 ||  m_cPixelId_p[i+1][j] == 220 ||  m_cPixelId_p[i-1][j+1] == 220||  m_cPixelId_p[i][j+1]== 220  ||  m_cPixelId_p[i+1][j+1] == 220
			    		  || m_cPixelId_p[i-2][j-2] == 220 ||  m_cPixelId_p[i-2][j-1] == 220 ||  m_cPixelId_p[i-2][j] == 220 ||  m_cPixelId_p[i-2][j+1] == 220 ||  m_cPixelId_p[i-2][j+2] == 220 
			    		  || m_cPixelId_p[i-1][j-2] == 220 ||  m_cPixelId_p[i-1][j+2] == 220 
			    		  || m_cPixelId_p[i][j-2] == 220   ||  m_cPixelId_p[i][j+2] == 220 
			    		  || m_cPixelId_p[i+1][j-2] == 220 ||  m_cPixelId_p[i+1][j+2] == 220 
			    		  || m_cPixelId_p[i+2][j-2] == 220 ||  m_cPixelId_p[i+2][j-1] == 220 ||  m_cPixelId_p[i+2][j] == 220 ||  m_cPixelId_p[i+2][j+1] == 220 ||  m_cPixelId_p[i+2][j+2] == 220  )
			    //       ���������ܱ߷�Χ��   20180207	  
			    	  {
			    		  idcloud = 1;    //  ��������Ե���
               }
         
			    	 //�����ж��Ƿ��ڻ�Į����Ե 20170627
         
			    	  if ( m_cPixelId_p[i-1][j-1] == 150 ||  m_cPixelId_p[i][j-1] == 150 ||  m_cPixelId_p[i+1][j-1] == 150 ||  m_cPixelId_p[i-1][j] == 150 ||  m_cPixelId_p[i+1][j] == 150 ||  m_cPixelId_p[i-1][j+1] == 150||  m_cPixelId_p[i][j+1]== 150  ||  m_cPixelId_p[i+1][j+1] == 150 )
			    	  {
			    	  idesert = 1;     //  �û�Į����Ե���
			    	  }
			    	   //�����ж��Ƿ���ˮ���Ե 201706810 ��  
			    	  if ( m_cPixelId_p[i-1][j-1] == 153 ||  m_cPixelId_p[i][j-1] == 153 ||  m_cPixelId_p[i+1][j-1] == 153 ||  m_cPixelId_p[i-1][j] == 153 ||  m_cPixelId_p[i+1][j] == 153 ||  m_cPixelId_p[i-1][j+1] == 153||  m_cPixelId_p[i][j+1]== 153  ||  m_cPixelId_p[i+1][j+1] == 153 )
			    	  {
			    	  iwater = 1;     //  ��ˮ���Ե���
			    	  }
			    	  
			    	   //�ж��Ƿ��ھ��淴����ڽ���Ԫ ,���ǣ�ȡ�������
			    
              if (  iSolarRef == 1 || iSolarRef2 == 1 )      //20170828
               {
			    	      m_cPixelId_p[i][j] = 100;  // ȡ������ǣ��ظ�Ϊ100�������
                  m_TempBuff2_p[i][j] = 0;   // ��ʶ������Ϊ1
			    	      iSolarRef = 0;
			    	      iSolarRef2 = 0;
			    	
			         }   //20170828			  
         
			    	  //�ж�������Ե�����Ԫ�����Ƿ�С��300K�����к����뱳�����죬����Զ�������С��15K ,���ǣ�ȡ�������
			          //#### �к���ͨ��������Ե������ֵ������ CldT4_Edg (��ֵ3000����300K)��#### �к����뱳������������Ե��ֵ������ CldT4BjDf_Edg (��ֵ100����10K)��                 
			    	  if ( idcloud == 1 && m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_Edg  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < CldT4BjDf_Edg && m_fSunZenithAnale[i][j] < CldSunZenith_Edg )      //20180112
			    	  {
			    	    m_cPixelId_p[i][j] = 100;  // ȡ������ǣ��ظ�Ϊ100�������
                m_TempBuff2_p[i][j] = 0;   // ��ʶ������Ϊ1
			    	    idcloud = 0;
			        }   //20170627
			    	  //�ж�С��290K����ջ�� //20171102				
			    	  if ( idcloud == 1 && m_wSatDataBuff_p[4][i * iDatWidth + j] <  CldT4_EdgLW  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < CldT4BjDf_EdgLW )      //20180113
			    	   {
			    	      m_cPixelId_p[i][j] = 100;  // ȡ������ǣ��ظ�Ϊ100�������
                  m_TempBuff2_p[i][j] = 0;   // ��ʶ������Ϊ1
			    	      idcloud = 0;
			         }   //20171102
         
         
			    	  // �жϻ�Į����Ե�����Ԫ�����Ƿ����330K���粻�ǣ�ȡ�������  20170810 ��
			    	  //####  ��Į����Ե�к���ͨ����ֵ������DstT4_Edg�� (��ֵ3300����330K)
			    	   if ( m_wSatDataBuff_p[4][i * iDatWidth + j] < DstT4_Edg && idesert == 1)
			    	   { 
                      m_cPixelId_p[i][j] = 100;  // ȡ������ǣ��ظ�Ϊ100�������
                      m_TempBuff2_p[i][j] = 0;   // ��ʶ������Ϊ1
			    	           idesert = 0;
			    	
			    	   }   //20170627
                   
                      // �ж�ˮ���Ե�����Ԫ�����Ƿ����330K���粻�ǣ�ȡ�������  20170810 ��
			    	   //####  ˮ���Ե�к���ͨ����ֵ������WatT4_Edg�� (��ֵ3300����330K)
			    	   if ( m_wSatDataBuff_p[4][i * iDatWidth + j] < WatT4_Edg && iwater == 1
                      || ( j< 1000 && m_wSatDataBuff_p[5][i * iDatWidth + j] > 150 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] <= wIR1BjAver_p[i][j]+10 && m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j]-30 ))   // 20180326 ͼ������ˮ�徵�淴���ж�
			    	   { 
                      m_cPixelId_p[i][j] = 100;  // ȡ������ǣ��ظ�Ϊ100�������
                      m_TempBuff2_p[i][j] = 0;   // ��ʶ������Ϊ1
			    	          iwater = 0;
			    	 
			    	   }   //20170810
                     // �ж�ˮ���Ե�����Ԫ�����Ƿ����330K���粻�ǣ�ȡ�������  20170810 ��
		     
         
	  //�жϳ��л���Ƿ�Ϊ���µ������ϴ󣬼���׼��ϴ�����,���к�������С��320K���к����뱳������С��20K    ��   // 20170804
          //#### �µ�������к���������ֵ������DBjT4_dfLw������ֵ3200����320K����#### �µ����к����뱳������������ֵ������DBjT4bj_dfLw����ֵ80����8K����#### �µ����к����׼����ֵ������DBjT4bjc_dfLw����ֵ12K��    
		if ( m_cPixelId_p[i][j]==10  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < DBjT4_dfLw &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < DT4b_dfLw +  fIR4Bj_p[i][j] && i < DB_dfLineNS       //20180217
		    || m_cPixelId_p[i][j]==10  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < DBjT4_dfSH &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < DT4b_dfLw +  fIR4Bj_p[i][j] * DT4b_dfCoe && i >= DB_dfLineNS    ) //20180112     
	       {
	          m_cPixelId_p[i][j] = 100;  // ȡ������ǣ��ظ�Ϊ100�������
            m_TempBuff2_p[i][j] = 0;   // ��ʶ������Ϊ1
	       
	       }    
	 //    �жϳ��л���Ƿ�Ϊ���µ������ϴ󣬼���׼��ϴ�����,���к�������С��320K���к����뱳������С��20K    ��   // 20170804
         
	 //     ����Զ�����ƽ�ж��µ������ϴ� 20171120 ��  
         //#### �µ���Զ�����׼��������ֵ����  DT11bjc_dfLw ,
	       
           if ( m_cPixelId_p[i][j]==10  && fIR1Bj_p[i][j] > DT11bjc_dfUP  && m_wSatDataBuff_p[4][i * iDatWidth + j] < DBjT4_dfLw && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 250    //20180206
	        || (  m_wSatDataBuff_p[4][i * iDatWidth + j] <DBjT4_dfLw && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 150  && ( fIR1Bj_p[i][j] > 50. ||  fIR4Bj_p[i][j] > 55. ) ) //20180217		 
	        ||m_cPixelId_p[i][j]==10  &&( m_wSatDataBuff_p[4][i * iDatWidth + j] >= DBjT4_dfLw && m_wSatDataBuff_p[4][i * iDatWidth + j] < DBjT4_dfSH) &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 250  &&  fIR1Bj_p[i][j] > DT11bjc_dfUP  &&  fIR1Bj_p[i][j] < DT11bjc_dfLW //20180208                                                                             	 
	        || m_wSatDataBuff_p[4][i * iDatWidth + j] < T11land_W && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < DT4land_W
		        && m_wSatDataBuff_p[1][i * iDatWidth + j]- wIR1BjAver_p[i][j] > DT11land_W  && m_wSatDataBuff_p[5][i * iDatWidth + j]- wIR5BjAver_p[i][j] > DVISland_W )  //20180226	 ˮ��Ӱ���ж�   	   	   	   
	       {
	        m_cPixelId_p[i][j] = 100;  // ȡ������ǣ��ظ�Ϊ100�������
          m_TempBuff2_p[i][j] = 0;   // ��ʶ������Ϊ1
	       
	        }    
	       
         // ����Զ�����ƽ�ж��µ������ϴ� 20171120 �� 
         
       //     printf(" &&&&& before cloud influence1  %d,  %d, m_cPixelId_p= %d ,CH3 = %d, CH4 = %d, CH4_AV =  %d, VIS = %d, VIS_AV = %d\n ",i,j, m_cPixelId_p[i][j] ,m_wSatDataBuff_p[4][i * iDatWidth + j], m_wSatDataBuff_p[1][i * iDatWidth + j],wIR1BjAver_p[i][j] ,m_wSatDataBuff_p[5][i * iDatWidth + j], wIR5BjAver_p[i][j]); //20180224
         
	       if ( m_cPixelId_p[i][j] == 10) 
	       {
         
	         //    �ж�����ʶ����Ƿ�Ϊ������Ⱦ�����ɼ��ⷴ����ƫ�ߣ�Զ��������ƫ��  20170711 �� �ɼ���ƫ�ͣ�Զ�����ƽƫ���к���ƫ�͵ȣ��ƻ�ѩ����Ե�����½�أ�       ��
         //#### ����ȾԶ����ͨ���뱳��������ֵ������ CldDisT11Dbj ����ֵ12����1.2K����####����Ⱦ�ɼ����뱳��������ֵ������ CldDisVisDbj ����ֵ150����0.15����####	����Ⱦ�ɼ���������ֵ������ CldDisVisUp ����ֵ350����0.35���� 	 	
		     if (  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] - CldDisT11Dbj && m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP &&  m_wSatDataBuff_p[4][i * iDatWidth + j]- wIR4BjAver_p[i][j] < CldT4bj_UP   && m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j]     // ���������ж� 20180316		  					
		       ||  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] - CldDisT11Dbj && m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP2             // ���������ж�
		       ||  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j] - CldT11_bjc                                                                 // ���������ж�
		       ||  m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j] + CldDisVisDbj &&  m_wSatDataBuff_p[5][i * iDatWidth + j] > CldDisVisUp1    //20180224     // ���������ж�
		       ||  m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j] &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j]         //20180224  ���������ж�
	               &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < CldT4bj_UP  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP1              //20180224  ���������ж�
	           || m_wSatDataBuff_p[5][i * iDatWidth + j] > CldDisVisUp1 &&  wIR5BjAver_p[i][j] > CldDisVisUp1  && fIR5Bj_p[i][j] > CldT4bj_UP  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP1  //20180224  ���������ж�
		       ||  m_wSatDataBuff_p[5][i * iDatWidth + j] > wIR5BjAver_p[i][j] &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < wIR1BjAver_p[i][j]         //20180224  
	               &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP2  &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < CldT11_UP              //20180224	���������ж�		 
	           || m_wSatDataBuff_p[5][i * iDatWidth + j] > CldDisVisUp2 &&  m_wSatDataBuff_p[1][i * iDatWidth + j] < CldT11_UP         //20180226  ���������ж�
	               &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < CldT4_UP1               //20180226	 ���������ж�
		       || m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] <CldlandDVis  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] <CldT4_UP1 && fIR1Bj_p[i][j] > CldlandDT11  // 21080314 �ƻ�ѩ��Ե½�������ж�
		        || m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] < -70  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] <CldT4_UP1 &&  m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] < 250 )   // �ܱ�δ��ʶ�����Ӱ���ж� 20180516
		   {                                                      
		     m_cPixelId_p[i][j] = 100;
	        m_TempBuff2_p[i][j] = 0;
	       
         
	     }
 	       
	   else 
	 	  {	
	       
         
   //     ������¶������Ԫ�����У����к���С��325K����NDVI����ֵ�����뱳������С��15Kʱ������Ƿ��ܱ�5*5��Ԫ�У���̽����Ԫ���к������С��4K����Ԫ����5��  20170810  *** ��	  
	 	     
    //####  ��������Ԫ�к�����ֵ����: SameT4Up (��ֵ3200����320K)��#### ��������ԪNDVI����ֵ���� SameNDVI ����ֵ250����0.25��,#### ��������Ԫ�к����뱳��������ֵ����:SameT4BJDf,��ֵΪ120,��15K. #### ��������ԪԶ�����׼����ֵ���� SameT11_bjc ����ֵΪ4.��
		     inm1 = 0;   // 20180128 			 
		     if (m_cPixelId_p[i][j] == 10 && m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4Up + (SameNDVI - NDVI[i][j]*100.) && (m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j]) < SameT4BJDf &&  fIR1Bj_p[i][j] > SameT11_bjc && 
			    ( (m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] < 150 &&m_wSatDataBuff_p[5][i * iDatWidth + j] == 0  || m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[1][i * iDatWidth + j] < 450  && m_wSatDataBuff_p[5][i * iDatWidth + j] > 5 ))   //20180128 ���Ӷ��к�����Զ�������Ϳɼ������0���ж�����	    
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
                    
			    		 // ����¶���Ԫѡ��
			    		 // ####  ��������Ԫ�к������������ֵ����: SameT4DfUP ����ֵ40����4K����####��������Ԫ�к������������ֵ����: SameT4DfLW ����ֵ-40����-4K��.				     
			    		   if (  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] < SameT4DfUP &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - m_wSatDataBuff_p[4][i0 * iDatWidth + j0] > SameT4DfLW   && ( i0 != i  ||  j0 != j  )) // 20171012
			    		   {
			    		  inm1 = inm1 + 1; 
			    
			    		   }          
			    		}
			    	 }      
         
	             }	  
	       
          
		      // ����¶���ԪӰ���ж�
		// ��������Ⱦ���  20180204 ��
		      int iCLOUDid;
		       iCLOUDid = 0;
		       if (   m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < 120 && m_wSatDataBuff_p[1][i * iDatWidth + j] - wIR1BjAver_p[i][j] <= 0 &&  m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] > 0 )
		       {
		        iCLOUDid = 1; 
		       }
           // ��������Ⱦ���  20180204 ��
         
            //  �����������Ԫ�жϡ�20180308		   
           if ( ((j >= RawNHUP &&  j <= RawNHLW ) && (i >= LineNHUP &&  i <= LineNHLW ))     //20180312
		       && ( ( ( inm1 >= SamePixNHLW  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHUP &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFNH  ))   	
	           || ( inm1>= SamePixETLW  && iCLOUDid == 1  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHUP)   //20180313		 
	           || ( inm1>= SamePixETLW  && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFNH && ( fIR4Bj_p[i][j] > SameT4bjc ||  fIR1Bj_p[i][j] > SameT11bjc))  //20180217		  
		       ||  inm1 >= SamePixUP ||  inm1 >= SamePixUP-2 && m_wSatDataBuff_p[4][i * iDatWidth + j]  < SameT4NHUP )    //20180515 
		       ||  inm1 >= SamePixETLW-2 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHLW &&   m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFNHLW   //20180226
	           || inm1 >= SamePixETLW-2 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3150 && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFNHLW  // 20180304
		          && m_wSatDataBuff_p[1][i * iDatWidth + j] - wIR1BjAver_p[i][j] > 40 && m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] < -100)  // 20180304 ��������Ե���������
	       {
	        m_cPixelId_p[i][j] = 100;
	        m_TempBuff2_p[i][j] = 0;
	       
	        }    
          
	       //  �����������Ԫ�жϡ�20180308		   
	           if ( ((j >= RawWNUP &&  j <= RawWNLW ) && (i >= LineWNUP &&  i <= LineWNLW ))    //20180312
		       && ( ( ( inm1 >= SamePixNHLW  &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHUP &&  m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFWNLW1 ))   	
	           || ( inm1 >= SamePixETLW  && iCLOUDid == 1)   //20180204		 
	           || ( inm1 >= SamePixETLW  && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] <SameT4DFWNLW2 && ( fIR4Bj_p[i][j] > SameT4bjc ||  fIR1Bj_p[i][j] > SameT11bjc))  //20180217		  
		       ||  inm1 >= SamePixUP ||  inm1 >= SamePixUP-2 && m_wSatDataBuff_p[4][i * iDatWidth + j]  < SameT4NHUP )    //20180515
		       ||  inm1 >= SamePixETLW-2 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < SameT4NHLW &&   m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFWNLW2   //20180226
	           || inm1 >= SamePixETLW-2 &&  m_wSatDataBuff_p[4][i * iDatWidth + j] < 3150 && m_wSatDataBuff_p[4][i * iDatWidth + j] - wIR4BjAver_p[i][j] < SameT4DFWNLW1  // 20180304
		          && m_wSatDataBuff_p[1][i * iDatWidth + j] - wIR1BjAver_p[i][j] > SameT11DFWN && m_wSatDataBuff_p[5][i * iDatWidth + j] - wIR5BjAver_p[i][j] < SameVISDFWN)  // 20180304 ��������Ե���������
	       {
	  m_cPixelId_p[i][j] = 100;
	  m_TempBuff2_p[i][j] = 0;
	      
	  }    
         
	       //���ϲ������Ԫ�ж�?��?20180308                                                 
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
	       
	        //���ϲ������Ԫ�жϡ�20180308 
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
         
  //     �ж��ڽ���Ԫ�Ƿ���ǿ���淢��� 20180515  ��
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
               
			    		 
			    		   // ����¶���Ԫѡ��
			    		 // ####  ����Ƿ���ǿ���淴��㸽����searching strong solar reflec pixel 	20180515			     
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
         
           // �ж��ڽ���Ԫ�Ƿ���ǿ���淢��� 20180515  ��
         
         
         
	       if( m_cPixelId_p[i][j] == 10 && ireflc == 0 )    //20180515      
	       {
           	  inm += 1;         
	        }
	    }               
	        }  // 20170711	
            } 
          }
         
    
        }  // ��ѭ������
     }    // ��ѭ������
	
	 // ��������ʶ  20170830   ��
	  int inscount = 0,inoline = 0;
	
	  for (int i = 0; i < iDatHeight; i++)
	  {
		  inscount = 0;
          for (int j = 0; j < iDatWidth; j++)
		  {
          if (m_cPixelId_p[i][j] == 10  )
          inscount  += 1;
		  }
      //#### �������ж� ��������Ԫ������ֵ���� NoisePixNo ����ֵ30��
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
		//#### ������Ԫ�ж�  �к����������� NoisePixT4 (��ֵ4700����470K)
		  if (  m_cPixelId_p[i][j] == 10  && m_wSatDataBuff_p[4][i * iDatWidth + j] >= NoisePixT4 )
		  {
	   	 m_cPixelId_p[i][j] = 100;
		   m_TempBuff2_p[i][j] = 0;
		  }

	   }	   
	  }

  	 // ��������ʶ  20170830   ��	


     // ��ʶ������
	
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
		ZLDSWriteLog('W',"Write DQF Of NC Error��\n");
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
	
		//�����ϢFPA_txt
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
	
	
	//дȫ������
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
		ZLDSWriteLog('W',"Write DQF Of NC Error��\n");
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



	//дȫ������
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
///�����ͼ
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
    jcs.image_width =  iDatWidth; // ͼƬ�Ŀ�
    jcs.image_height = iDatHeight; // ͼƬ�ĸ�
    jcs.input_components=3; //1Ϊ�Ҷ�ͼ��3ΪRGBͼ
    jcs.in_color_space = JCS_RGB;//JCS_GRAYSCALE��ʾ�Ҷ�ͼ��JCS_RGB��ʾ��ɫͼ
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
///���ǿ��ͼ
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
    jcs.image_width =  iDatWidth; // ͼƬ�Ŀ�
    jcs.image_height = iDatHeight; // ͼƬ�ĸ�
    jcs.input_components=3; //1Ϊ�Ҷ�ͼ��3ΪRGBͼ
    jcs.in_color_space = JCS_RGB;//JCS_GRAYSCALE��ʾ�Ҷ�ͼ��JCS_RGB��ʾ��ɫͼ
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
										//������Ԫ����ֻ��ȼ�									
							       if (fFireS < 5000)
							       {
							          //��ɫ  �ֱ�Ϊ���̺��ֵ									
							          out_buf_RGB[idx * rgbBand] = 0;            //��
							          out_buf_RGB[idx * rgbBand + 1] = 255;      //��
							          out_buf_RGB[idx *rgbBand + 2] = 0;   	//��
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

//20180105 ���� RGB�ϳ�ͼ
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
    
    jcs_day.image_width =  iDatWidth; // ͼƬ�Ŀ�
    jcs_day.image_height = iDatHeight; // ͼƬ�ĸ�
    jcs_day.input_components=3; //1Ϊ�Ҷ�ͼ��3ΪRGBͼ
    jcs_day.in_color_space = JCS_RGB;//JCS_GRAYSCALE��ʾ�Ҷ�ͼ��JCS_RGB��ʾ��ɫͼ
    
    
    
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
            out_buf_RGB_day[ix * rgbBand +2 ] = ich; //��ɫ 
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
       
    jcs_night.image_width =  iDatWidth; // ͼƬ�Ŀ�
    jcs_night.image_height = iDatHeight; // ͼƬ�ĸ�
    jcs_night.input_components=3; //1Ϊ�Ҷ�ͼ��3ΪRGBͼ
    jcs_night.in_color_space = JCS_RGB;//JCS_GRAYSCALE��ʾ�Ҷ�ͼ��JCS_RGB��ʾ��ɫͼ
    
     
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

//--------    �����Ϣ������ ----------------------------------------------

/*: *******************  FireInfoPro *********************
* �������ƣ�  FireInfoPro
* �������⣺  �����Ϣ����
* �������ܣ�  �����Ϣ����
* ���÷���: FireInfoPro(void)
*   ����˵����
*   ��
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::FireInfoPro() //���ֻ���
{
  //���ֻ��� ���������������������Ϣ�� ��� 2��  txt�����Ϣ��
   int n = 0;
//       FILE *fpout;
//       char table1[256];
//       memset(table1, 0, sizeof(char)*256);
//       strcpy(table1,"/PGSDATA/FY4A/L2/firetest.txt");  
//        fpout = fopen(table1,"w"); 
   //������ͼ�Ļ�����
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

   //�����ʶ�����е�nΪ�����ڵĻ��������˴���nΪȫͼ��Χ�Ļ�����
   //05��4��28�� ͳ�ƻ��ͷ�������Ϣ�ռ���ڴ˺�����
   iFires = n;
   printf("******fire count is %d************\n",iFires);
   //���·�������Ϣ�ռ�
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

   //���û���������
	for (int iy = 0; iy < iDatHeight; iy++)
	{
		for (int ix = 0; ix < iDatWidth; ix++)
		{
	     if (m_TempBuff2_p[iy][ix] == 1 )
	     { 
          iFqNo = iFqNo + 1;
          fire_fq_(iy,ix);								//ȷ��������������¼������ص���¶���Ϣ
          iFqNum ++;
	     }
		}
	}

  iAreas =  iFqNum-2;     // ��������

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
	//��µ㼰��Ե��ͨ��3��4��5�ı����¶�
	BJtemp();
	//�����ڵ�id=2�ĵ㱳���¶�
	BJtemp2();
	//������Ԫ���
	SaveFireInfo_();
  cout << "save fire info end" << endl;

  //����������ĵ�ȵ�
  //�������P
  float xx;
  for (int i = 1; i <= iFires; i++)
  {
    int iX,iY;
    iX = fpi_[i].wX;
    iY = fpi_[i].wY;
    float FireT;							//����¶�
    unsigned char  bF1;				//��������ȼ�
		//��������Ԫ����������¶Ⱥͷ�������
    fpi_[i].fFireP = SubPixel_(fpi_[i].fMixTemp[0], fpi_[i].fMixTemp[1], fpi_[i].fBackTemp[0], fpi_[i].fBackTemp[1], &FireT , &bF1);  

    fpi_[i].fFireTemp = FireT; //�����Ԫ�¶�
    fpi_[i].TreL = bF1;

     xx = fpi_[i].fFireS * fpi_[i].fFireP;  //����Ԫ���

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
     //�Ի�����Ϣ���д�����ÿһ����������ffi�ṹ������ֵ
     FireFQP_(i);
  }
  OutputInfo1_(); //���������Ϣ��

  OutPutInfo_();  //��������Ϣ�м��ļ�

  OutputFirePixInfo_();  //��������Ϣ�ļ�
 
  
  
  cout << "fire txt end..." << endl;
}
//---------------------------------------------------------------------------

//---- ������ -----------

/*: *******************  fire_fq_*********************
* �������ƣ�  fire_fq_
* �������⣺  ������
* �������ܣ�  ȷ������
* ���÷���: fire_fq_(int y, int x)
*   ����˵����
*   int y   �к�
*   int x   �к�
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/

void ProcessFire::fire_fq_(int y, int x)
{
	//TODO: Add your source code here   �����Ľ�
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


   m_TempBuff2_p[y][x] = 2; //�Ƚ����ֵ���1��Ϊ2
   iFireNo = iFireNo + 1; //�������1

   // ȷ�������ӵ��id �������е�λ�ã�id=0 Ϊ�µ㣬id=1Ϊ��Ե�㣬id=2Ϊ�ڵ㡣
   for (int i = y1; i <= y2; i++)
   {
     for (int j = x1; j <= x2; j++)
     {
       s1 = s1 + m_TempBuff2_p[i][j];
       s2 = s2 * m_TempBuff2_p[i][j];
     }
   }
	 //ȷ��������ʽ���
   if (s1 == 2)
      fpi_[iFireNo].bId = 0;
   else if (s2 == 0)
      fpi_[iFireNo].bId = 1;
   else
      fpi_[iFireNo].bId = 2;
   

		//�������Ϣ��ֵ
   fpi_[iFireNo].wNo = iFireNo;    //�����
   fpi_[iFireNo].wAreaNo = iFqNo;  //�������
   fpi_[iFireNo].wY = y;           //������ڵ��к�
   fpi_[iFireNo].wX = x;           //������ڵ��к�
   fpi_[iFireNo].fLat = m_fLat_p[y][x];   //����γ��ֵ                     //buff.fMaxLat - y * 0.01;
   fpi_[iFireNo].fLon = m_fLong_p[y][x];  //��㾭��ֵ                      //buff.fMinLon + x * 0.01;
   fpi_[iFireNo].fMixTemp[0] = (float)m_wSatDataBuff_p[4][iDatWidth * y + x] / 10.0;  //�����к���ֵ
   fpi_[iFireNo].fMixTemp[1] = (float)m_wSatDataBuff_p[1][iDatWidth * y + x] / 10.0;  //����Զ����1��ֵ
   fpi_[iFireNo].fMixTemp[2] = (float)m_wSatDataBuff_p[2][iDatWidth * y + x] / 10.0;  //����Զ����2��ֵ


   // �Ѵ����ӵ㿪ʼ�����������
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
* �������ƣ�  BJtemp
* �������⣺  ���㱳���¶�
* �������ܣ�  ����µ㼰��Ե���ͨ��3��4��5�ı����¶�
* ���÷���: BJtemp(void)
*   ����˵����
*   ��
*
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/

void ProcessFire::BJtemp()
{
	//TODO: Add your source code here
	//��µ㼰��Ե��ͨ��3��4��5�ı����¶�

   unsigned int   num;
   float s1,s2,s3,s5;
   short x1,x2,y1,y2;
   bool fpid = false;
   FILE* fpLandDat_p = NULL;

   if (!fpLandDat_p)
   {
       fpLandDat_p = fopen(strnLanding,"rb");	//�������������������ļ�
       fpid = true;
   }

   if (!fpLandDat_p)
   {
      cout << "���������������ļ�����" << endl;
      for (int i = 1; i <= iFires ; i++)
      {
         fpi_[i].fBackTemp[1] = 0;   //���ͨ��4�ı����¶ȣ�����¼
         fpi_[i].fBackTemp[0] = 0;   // ���ͨ��3�ı����¶ȣ�����¼
         fpi_[i].fBackTemp[2] = 0;  // ���ͨ��5�ı����¶ȣ�����¼
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
            fpi_[i].iQKId = 1;      // �������Ϊ1
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
            s3 = s2 + LandVI_(fpLandDat_p, fpi_[i].fLat , fpi_[i].fLon) ;			//��LandVI���������������ͱ��
            s5 = s2;
            fpi_[i].iQKId = 0;
        }

        fpi_[i].fBackTemp[1] = s2;   //���ͨ��4�ı����¶ȣ�����¼
        fpi_[i].fBackTemp[0] = s3;   // ���ͨ��3�ı����¶ȣ�����¼
        fpi_[i].fBackTemp[2] = s5;  // ���ͨ��5�ı����¶ȣ�����¼
     }
   }
		fclose(fpLandDat_p);
		fpLandDat_p = NULL;
}
//-------------------------------------------------------

/*: *******************  BJtemp2 *********************
* �������ƣ�  BJtemp2
* �������⣺  ���㱳���¶�
* �������ܣ�  ��������ڵ��ͨ��3��4��5�ı����¶�
* ���÷���: BJtemp2(void)
*   ����˵����
*   ��
*
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::BJtemp2()
{
	//TODO: Add your source code here
	// ���� �ڵ�id=2�ĵ㱳���¶�
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

        fpi_[i].iQKId = fpi_[s2].iQKId;   								//�����ڻ����ձ�־ȡ�������ֵ
        fpi_[i].fBackTemp[1] =  fpi_[s2].fBackTemp[1];    // �����ڻ�㱳��ȡ������ı���ֵ
        fpi_[i].fBackTemp[0] = fpi_[s2].fBackTemp[0];  //
        fpi_[i].fBackTemp[2] = fpi_[s2].fBackTemp[2];  //
     }
   }
}

//------------------------------------------

/*: *******************  SaveFireInfo_ *********************
* �������ƣ�  SaveFireInfo_
* �������⣺  �����Ϣ����
* �������ܣ�  �����Ϣ���浽�����Ϣ������
* ���÷���: SaveFireInfo_(void)
*   ����˵����
*   ��
*
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::SaveFireInfo_(void)
{
	//TODO: Add your source code here
	// ������Ԫ��� ���������ͱ�ţ� �����߽����
	float num1;
	unsigned char  num;
	int x0,y0;
	long int k;
	
	FILE *fpLandDat_p;																		//�����������ļ�
	fpLandDat_p = NULL;																				
	FILE *fpXianJieDat_p;																	//��ʡ��
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
     cout << "�������߽��ļ�����" << endl;
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
	
	//�����Ի����Ϣ��ֵ
	for (int i = 1; i <= iFires; i++)
	{
	   fpi_[i].fFireS = PixS_(fpi_[i].wX, fpi_[i].wY); //�����Ԫ���
	
	   num1 = 0.0; num = 0; x0 = 0; y0 = 0;
	   x0 = (int)((fpi_[i].fLon - 70.) * 100);
	   y0 = (int)((56. - fpi_[i].fLat) * 100);
	
	   k = (long int)((y0 * 7100 + x0) * 4);
	   fseek(fpXianJieDat_p, k, 0);
	   fread(&num1,sizeof(float),1,fpXianJieDat_p);
	   fpi_[i].iBjNo = (int) num1;  //�ؽ���
	
	   k = (long  int)(y0 * 7100 + x0);
	   fseek(fpLandDat_p,k,0);
	   fread(&num,sizeof(unsigned char),1,fpLandDat_p);
	   fpi_[i].wLandId = num;   //������������
	}
	
	
	fclose(fpLandDat_p);
	fpLandDat_p = NULL;
	
	fclose(fpXianJieDat_p);
	fpXianJieDat_p = NULL;


}
//------------------------------------------

/*: *******************  PixS_ *********************
* �������ƣ�  PixS_
* �������⣺  ������Ԫ���
* �������ܣ�  ����ĳ�����Ԫ���
* ���÷���: PixS_(int PixX,int PixY)
*   ����˵����
*   int PixY   �к�
*   int PixX   �к�
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
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
* �������ƣ�  FireFQP_
* �������⣺  ������Ϣ����
* �������ܣ�  ������Ϣ�����浽������Ϣ������
* ���÷���: FireFQP_(int FQNo)
*   ����˵����
*   int FQNo ������
*
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
void ProcessFire::FireFQP_(int FQNo)
{
	//TODO: Add your source code here
	//����ÿ�������Ļ����Ϣ ������ ������ٷֱȣ�����������
  int n1,n2,lindinum,caochangnum,qitanum,maxY,maxX,mixY,mixX;
  bool b1,b2;
  b1 = true;
  b2 = true;

//�����������ֵ
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
  ffi_[FQNo].sCloudsId = "��";
  ffi_[FQNo].cCloudsId = 0;
  //������Ԫ��� 20171208
  ffi_[FQNo].fNPixelS = 0;
  FILE *fpLandDat_p, *fpLandTxt_p;
  fpLandDat_p = NULL;
  fpLandTxt_p = NULL;

  if ( fpLandDat_p == NULL)
  {
      fpLandDat_p = fopen(strnLanding,"rb");											//���������������ļ�
      fpLandTxt_p = fopen(strLandCover,"rt");											//�������������Ͷ�Ӧ������
      if (!fpLandDat_p)
         b1 =!b1;
  }

  FILE *fpXianJieDat_p, *fpXianJieTxt_p;
  fpXianJieTxt_p = NULL;
  fpXianJieDat_p = NULL;

  if (!fpXianJieDat_p)
  {
      fpXianJieDat_p = fopen(strxjraster,"rb");									//���ؽ������ļ�
      fpXianJieTxt_p = fopen(strxjdh_new,"rt");									//���ؽ����ݶ�Ӧ������
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
        ffi_[FQNo].wFireAreaFires += 1;  //�����Ļ�����
        ffi_[FQNo].fFireAreaS += (fpi_[i].fFireS * fpi_[i].fFireP);   // ����������Ԫ���
        ffi_[FQNo].fNPixelS += fpi_[i].fFireS ; ////20171208 ÿ������������Ԫ��� 
        num1 = 0;

        x0 = (int)((fpi_[i].fLon + 180.) * 100);
        y0 = (int)((90. - fpi_[i].fLat) * 100);

        if (x0 < 36000 && x0 >= 0 && y0 < 18000 && y0 >= 0)
        {
            k = (long  int)(y0 * 36000 + x0);
            fseek(fp_LandDat,k,0);
            fread(&num1,sizeof(unsigned char),1,fp_LandDat);
        }

       //�����ͳ���ֵء��ݵء�����������
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
                 ffi_[FQNo].sCloudsId = "��";
                 ffi_[FQNo].cCloudsId = 1;
                }
           }
       }
     }
  } //�ҳ������Сֵ�ͻ���������

  //cout<<"*****fNPixelS:******"<<fNPixelS<<endl;
  
 
  fclose (fp_LandDat);

	ffi_[FQNo].fFireLinDi =  ffi_[FQNo].fFireLinDi / ffi_[FQNo].wFireAreaFires;
	ffi_[FQNo].fFireCaoChang =  ffi_[FQNo].fFireCaoChang / ffi_[FQNo].wFireAreaFires;
	ffi_[FQNo].fFireQiTa =  ffi_[FQNo].fFireQiTa / ffi_[FQNo].wFireAreaFires;

  int   centerX1,centerY1;

  //������������ĵ����к�
  centerX1 = maxX - (maxX - mixX) / 2;
  centerY1 = maxY - (maxY - mixY) / 2;

  //�ҳ���ӽ��������ĵ�Ļ��
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
    ffi_[FQNo].sShengMing =  SrName_(fpXianJieDat_p, fpXianJieTxt_p, ffi_[FQNo].fCenterLat ,ffi_[FQNo].fCenterLon);//��ȡʡ�߽���Ϣ  
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
* �������ƣ�  SubPixel_
* �������⣺  ��������Ԫ���������¶Ⱥͻ������
* �������ܣ�  ��������Ԫ���������¶Ⱥͻ������
* ���÷���: SubPixel_(float ch3,float ch4,float ch3b,float ch4b, float *Ft,  unsigned char *TL)
*   ����˵����
*   float ch3 �к���ֵ
*   float ch4 Զ����ֵ
*   float ch3b �к��ⱳ��ֵ
*   float ch4b Զ���ⱳ��ֵ
*   float *Ft ������¶�ֵ
*   unsigned char *TL ����Ļ�������ȼ�
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/
float ProcessFire::SubPixel_(float ch3,float ch4,float ch3b,float ch4b, float *Ft,  unsigned char *TL)
{
 	double c1,c2,WAV3,WAV4,WAV,LT,LTB,LTF;
	c1=1.1910427/100000;//c1,c2ϵ����VC���Ĳ���,ABT���Ĳ������桢б��
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

		SubPixel_1(ch3,ch4,ch3b,ch4b, &pp, &pt);//������������Ԫ���������¶�
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
* �������ƣ�  OutputInfo1_
* �������⣺  ���������Ϣ��
* �������ܣ�  ���������Ϣ��
* ���÷���: OutputInfo1_(void)
*   ����˵����
*   ��
*
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
************************** OutputInfo1_ �������� **************************  :*/
void ProcessFire::OutputInfo1_()
{
		//TODO: Add your source code here
		// ���������Ϣ����
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

    fprintf(fpout,"\r\n                                      �������ǻ�������Ϣ��                              \r\n\r\n");
    s0 = "�۲�ʱ�䣨����ʱ����" + s1 +  "                                       �й�����ֹ�������������������\r\n";



    fprintf(fpout,"%s\r\n",s0.c_str());
    fprintf(fpout,"BeginLine %4d  EndLine %4d\r\n",istartline,iendline);
    fprintf(fpout,"BeginPixel %4d  EndPixel %4d\r\n",istartpixel,iendpixel);
    fprintf(fpout,"------------------------------------------------------------------------------------------------------\r\n");

    //fprintf(fpout,"�������    γ��       ����    ������    ������(����)    �ݵ�     �ֵ�     ����     �Ʊ�Ե    ʡ����                       \r\n");
    fprintf(fpout,"�������    γ��       ����    ������    ���������ƽ�����  ���������(����)    �ݵ�     �ֵ�   ����     �Ʊ�Ե    ʡ����\r\n");
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
////********20180412*******дstring���͵����ݼ�
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
	
	
	//дȫ������
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

	//дȫ������
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
* �������ƣ�  OutputFirePixInfo_
* �������⣺  ��������Ϣ��
* �������ܣ�  ��������Ϣ��
* ���÷���: OutputFirePixInfo_(void)
*   ����˵����
*   ��
*
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
************************* OutputFirePixInfo_ �������� ***************************  :*/

void ProcessFire::OutputFirePixInfo_()
{
	//TODO: Add your source code here
	// ��������Ϣ����

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
// ---------- 2017/11/24 ���Ŷ� --------------------------//

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

    fprintf(fpout,"\r\n                                      �������ǻ�������Ϣ��                              \r\n\r\n");
    s0 = "�۲�ʱ�䣨����ʱ����" + s1 +  "                                       �й�����ֹ�������������������\r\n";
    fprintf(fpout,"%s\r\n",s0.c_str());
    fprintf(fpout,"BeginLine %4d  EndLine %4d\r\n",istartline,iendline);
    fprintf(fpout,"BeginPixel %4d  EndPixel %4d\r\n",istartpixel,iendpixel);
    fprintf(fpout,"------------------------------------------------------------------------------------------------------\r\n");
    fprintf(fpout,"�����  ��Ԫ������� ���Ŷ�    γ��       ����    ���������ƽ����� ���������(����)   FRP         ǿ�ȵȼ�        ��������      ʡ����\r\n");

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
               SrName_(fpXianJieDat_p, fpXianJieTxt_p, fpi_[i].fLat, fpi_[i].fLon).c_str());                                                                //�½�ά���������
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

	//дȫ������
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

	//дȫ������
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
* �������ƣ�  OutPutInfo_
* �������⣺  ��������Ϣ��
* �������ܣ�  ��������Ϣ��
* ���÷���: OutPutInfo_(void)
*   ����˵����
*   ��
*
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
************************* OutPutInfo_  �������� ***************************  :*/
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
    //fprintf(fpout,"���	�к�	�к�	γ��		����		���� ���������� ������������ ��Ԫ���		��Ԫ�ٷֱ�	����¶�	ʱ��		��ձ��		�������� \r\n");
    fprintf(fpout,"���	�к�	�к�	γ��		����		����  ������������   ��Ԫ���		��Ԫ�ٷֱ�	����¶�	ʱ��		�������� \r\n");
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

	//дȫ������
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

	//дȫ������
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
* �������ƣ�  LandVI_
* �������⣺  �����������ͱ��
* �������ܣ�  �����������ͱ��
* ���÷���: LandVI_(FILE *fpLandDat_, float m_fLat_p, float fLong)
*   ����˵����
*   FILE *fpLandDat_   �������������ļ�ָ��
*   float m_fLat_p    γ��
*   float fLong   ����
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
************************* LandVI_  �������� ***************************  :*/
unsigned char ProcessFire::LandVI_(FILE *fpLandDat_, float m_fLat_p, float fLong)
{

   unsigned char num1,num;

   int x0,y0;
   num1 = 0;
   num = 0;
   x0 = (int)((fLong - 70.) *100);
   y0 = (int)((56. - m_fLat_p) *100);

   if (x0 >= 0 && x0 < 7100 && y0 >= 0 && y0 < 4900)
   {  //  �ڷ�Χ��
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
* �������ƣ�  SrName_
* �������⣺  ����������������
* �������ܣ�  ����������������
* ���÷���: string SrName_(FILE *fpXianJieDat_, FILE *fpXianJieTxt_, float fLat0, float fLong0)
*   ����˵����
*   FILE *fpXianJieDat_  �ؽ������ļ�ָ��
*   FILE *fpXianJieTxt_  �ؽ������ļ�ָ��
*   float fLat0    γ��
*   float fLong0   ����
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
************************* SrName_ �������� ***************************  :*/

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
   {  //  �ڷ�Χ��
     long int k;

     k = (long int)((y0 * 7100 + x0) * 4);
     fseek(fpXianJieDat_,k,0);

     fread(&num1,sizeof(float),1,fpXianJieDat_);
     if (num1 == 0)
     {
       return (string)("�й�������.");
     }

     fseek(fpXianJieTxt_,0,0);
     while(fscanf(fpXianJieTxt_,"%s",sss)!=-1)
     {
       num = atof(sss);   //ÿ��ʡ�б��

       fscanf(fpXianJieTxt_,"%s",cTmpChar);

       if (num == num1)
       {
          return ((string)(cTmpChar));
       }
     }
     return (string)("�й�������..");
   }
   else
     return (string)("�й�������...");

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
   	{  //  �ڷ�Χ��
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
       return (string)("�й�������");
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
     return (string)("�й�������");
   }
   else
     return (string)("�й�������");
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
//   {  //  �ڷ�Χ��
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
   {  //  �ڷ�Χ��
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
* �������ƣ�  LandCoverName1_
* �������⣺  ����������������
* �������ܣ�  ����������������
* ���÷���: string LandCoverName1_(float fLat0, float fLong0)
*   ����˵����
*
*   float fLat0    γ��
*   float fLong0   ����
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
************************* LandCoverName1_ �������� ***************************  :*/

string ProcessFire::LandCoverName1_(float fLat0, float fLong0)
{
   string str1;

   FILE *fp_LandDat;
   fp_LandDat = NULL;
   fp_LandDat  = fopen(strLandImg,"rb");//ȫ��ֲ���������ݷ���

   unsigned char num1;   //,num;

   int x0,y0;
   num1 = 0;
   x0 = (int)((fLong0 + 180.) * 100);
   y0 = (int)((90. - fLat0) * 100);

   if (x0 > 36000 || x0 < 0 || y0 > 18000 || y0 < 0)
   {
       fclose (fp_LandDat);
       return (string)("����");
   }

  // x0 = fLong0 * 4  - 70 * 400;        // 70 is minlon
  // y0 = 56 * 400 - fLat0 * 4;          // 56 is maxlat

   long  int k;

   k = (long  int)(y0 * 36000 + x0);
   fseek(fp_LandDat,k,0);
   fread(&num1,sizeof(unsigned char),1,fp_LandDat);

   fclose (fp_LandDat);

   if (num1 == 1){
      return (string ("�ݵ�"));
   }
   else if (num1 == 2){
      return (string ("�ֵ�"));
   }
   else if (num1 == 3){
      return (string ("ũ��"));
   }
   else if (num1 == 4){
      return (string ("����"));
   }
   else if (num1 == 5){
      return (string ("����"));
   }
   else {
      return (string)("����");
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


double ProcessFire::B1(double k,double n)       // �����¶ȣ����ò����ͷ�����
{
	 double c1,c2,result;
	 c1=1.1910659e-5;	 c2=1.438853;
     result=c2*k/log(1+c1*pow(k,3)/n);
	 return result;
}

double ProcessFire::B2(double k,double t)      // ��������ʣ����ò������¶�
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

	// ����P��ʼֵ��ʼ *****************************************************
	// ��P��ֵ
	p0 = 0.5;
	p_intermy = 0.5;
	if ( p0 > 0.0001)
	{ 
		do 
		{
	
	   // ��1������Pֵ����ͨ��3�����Ԫ��ʽ���ȼ����������n3ft���ټ���ͨ��3����¶�3ft   ��N3mix-�� 1 �C P ��N3bg��/ P
		
		p0 = p_intermy;
		n3ft = (B2(k3,N3mix)-(1-p0)*B2(k3,N3b))/p0; // ����ͨ��3��������
	    ft3 = B1(k3,n3ft); // ���� ͨ��3����¶�
	    t0 = ft3;
	
	
	// ��2��������ʽ��õ�ͨ��3�¶ȴ���ͨ��4�����Ԫ��ʽ���õ�ͨ��4�����Ԫ��ʽ��Pֵ
	// ͨ��4�����Ԫ��ʽ��Pֵ���㹫ʽ��P4 = ��N4mix -  N4bg��/ ��N4ft -  N4bg��
	// �ȼ���ͨ��4��������n4ft,��ϻ����Ԫ������n4mix1,����������n4bg
	   n4ft = B2(k4,ft3);
	   p4 = ( B2(k4,N4mix)-B2(k4,N4b))/(B2(k4,ft3)- B2(k4,N4b) );
	
	   //printf("����P =      %1f \n\n",p4);
	
	// ��3��������ʽ�õ���Pֵ��������ַ���ʽ�У�����Pֵ���м�ֵ
	   p_lower = log(p4);
	   p_upper = log(p0);
	   p_intermy = p_lower + p_upper - p_lower/2 ;        //   =p_lower + p_upper - p_lower/2
	   p_intermy = pow(10, p_intermy) ;
	
	   //printf("����p_intermy =      %1f \n\n",p_intermy);
	
	
	// ��4���������ʽ�����Pֵ�м�ֵ�Ƿ�С��0.0005����û�У�����ʽ����õ���Pֵ�м�ֵ�����ص�1�������빫ʽ�С�
	
	//        ���ǣ�������������Pֵ�м�ֵ��ͨ��3����¶ȣ���Ϊţ�ٵ�����ʽ�ĳ�ʼֵp0��t0��
		} 
		while (  p_intermy > 0.0005);
	}

	//printf("�������Ԫ���������ʼֵ: P =  %1f%%\n\n",100*p0);
	//printf("�������Ԫ�¶ȳ�ʼֵ�����ԣ�: T =  %1fK\n\n",t0);

// ����P��ʼֵ���� *****************************************************




	n1=0;
	if( J0(p0,t0,k3,k4,N3b,N4b) != 0 )	
	{    // printf("J0=%d",J0);
		do	
		{
			p=p0+J1(p0,t0,k3,k4,N3b,N4b,N3mix,N4mix)/J0(p0,t0,k3,k4,N3b,N4b);

			t=t0+J2(p0,t0,k3,k4,N3b,N4b,N3mix,N4mix)/J0(p0,t0,k3,k4,N3b,N4b);
			p0=p;
			t0=t;
            //printf("*����Ŀ�����ռ��Ԫ�ٷֱ�: P =  %1f%%\n\n",100*p);
			//printf("*����Ŀ���¶ȣ����ԣ�: T =  %1fK\n\n",t);

			n1=n1+1;
            //printf("\n_\n\nѭ������=%d��\n\n",n1);

		} 
		while( fabs( p*B2(k3,t)+(1-p)*B2(k3,N3b)-B2(k3,N3mix) ) >0.0000001 &&

            // fabs( p*B2(k4,t)+(1-p)*B2(k4,N4b)-B2(k4,N4mix) ) >0.0000001 );

        fabs( p*B2(k4,t)+(1-p)*B2(k4,N4b)-B2(k4,N4mix) ) >0.0000001 && n1 <= 30);

	//	if (n1 >= 30) return 1; // ???? ����˳�
	}   // printf("n1=%d",n1);

	*pp = p0;
	*pt = t0;
}

//-------------------------------------------------------------------------------------------

/*: ******************* CalPlankFromRadToBT*********************
* �������ƣ� CalPlankFromRadToBT
* �������⣺ ���ݷ����ʼ�������
* �������ܣ� ���ݷ����ʼ�������
* ���÷���: CalPlankFromRadToBT(float radian,float wavelength)
*   ����˵����
* float radian  ������
* float wavelength  ���Ĳ�����΢�ף�
*   ��    �ߣ�۳����
*   ������ڣ�2013��6��18��
************** CalPlankFromRadToBT�������� *********************  :*/

float ProcessFire::CalPlankFromRadToBT(float radian,float wavelength)
{
    double c1=1.1910439e-16;
    double c2=1.438769e-2;
    double wl=wavelength*1.0e-6;
    double a1=log(c1/((wl*wl*wl*wl*wavelength)*radian)+1.0);
    double r2t=c2/(wl*a1);

    return r2t;
}

/*----------------------------���º���û���õ�------------------------------*/
/*: *******************  FireProSavePixelIdBmp *********************
* �������ƣ�  FireProSavePixelIdBmp
* �������⣺  �����������ļ�Ϊbmpͼ��
* �������ܣ�  �����������ļ�Ϊbmpͼ��
* ���÷���: FireProSavePixelIdBmp(void)
*   ����˵����
*   ��
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
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
    //BMPͼ��ͷ��Ϣ
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
        //д��BMPͷ��Ϣ
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
* �������ƣ�  FireProSavePixelIdRaw
* �������⣺  �����������ļ�
* �������ܣ�  �����������ļ�
* ���÷���: FireProSavePixelIdRaw(void)
*   ����˵����
*   ��
*
*   ��    �ߣ��ƻ�
*   ������ڣ�2013��
****************************************************  :*/

void ProcessFire::FireProSavePixelIdRaw(void)
{
    FILE *ofp;
    ofp = fopen(strOutputFileName_PIMG,"wb");
    if (!ofp) 
    {
        //ShowMessage( "�ļ���ʧ��");
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
* �������ƣ� getCFG
* �������⣺ ���ݷ����ʼ�������
* �������ܣ� ���ݷ����ʼ�������
* ���÷���: getCFG(char *cfgfname)
*   ����˵����
* char *cfgfname  �ļ���
*
*   ��    �ߣ�
*   ������ڣ�2013��10��28��
************** getCFG�������� *********************  :*/

bool ProcessFire::getCFG(char *cfgfname,int *iFactr_Mid, int *Factr_Far, int *iDt3Th, int *iDt34Th)
{
/*
//��ֵ
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
  ptr=strtok(NULL, "="); //Linux�Ƽ�ʹ�ú���strtok_r������
  sscanf(ptr,"%f= ",iFactr_Mid);
  fgets(cc,100,fp);
  tmp=strtok(cc, "=");
  ptr=strtok(NULL, "="); //Linux�Ƽ�ʹ�ú���strtok_r������
  sscanf(ptr,"%f ",Factr_Far);
  fgets(cc,100,fp);
  tmp=strtok(cc, "=");
  ptr=strtok(NULL, "="); //Linux�Ƽ�ʹ�ú���strtok_r������
  sscanf(ptr,"%f ",iDt3Th);
  fgets(cc,100,fp);
  tmp=strtok(cc, "=");
  ptr=strtok(NULL, "="); //Linux�Ƽ�ʹ�ú���strtok_r������
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
		//�����ϢFPA_txt
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




