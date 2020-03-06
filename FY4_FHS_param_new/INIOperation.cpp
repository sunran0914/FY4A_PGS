/** @addtogroup INIOperation
*  @{
*/
/** @defgroup FHS INIOperation主程序操作包首部详细设计
*\li
* 操作包名称: INIOperation\n                        
*\li             
* 操作包标题: 输入输出操作包\n
*\li
* 操作包作用: 输入输出操作包\n    
*\li        
* 包含的操作:    无  \n                                  
*\li
* 引用的操作包:      无  \n                                        
*\li
* 源程序文件名:\n                                     
*     INIOperation.cpp \n
*
* 版权:         国家卫星气象中心星地通公司
*  @author      欧阳飒飒
* @date    2016.8.18
* @version 1.0.0
*  @{
*/
#include "INIOperation.h"
INIOperation::INIOperation()
{
  m_wChannelsData = NULL;
  m_fSunZenithAnale = NULL;
  m_fSunGlitAngle = NULL;
  m_fLat = NULL;
  m_fLong = NULL;
  pSFTMask = NULL;
  m_ucCloud = NULL;
  m_cPixelSurfaceType = NULL;
  m_fPixelSatelliteZenithAnale = NULL;
  HDFFileAtt = NULL;
  
   m_startLine=0;
   m_endLine=0;
   m_startPixel=0;
   m_endPixel=0;
   cloud_ID =1;
  memset(sataName,0,sizeof(sataName));
  initData();
}

INIOperation::~INIOperation()
{
  freeMemory();
}

int INIOperation::newline(char c)
{
  return ('\n' == c ||  '\r' == c )? 1 : 0;
}

int INIOperation::load_ini_file(const char *file, char *buf,int *file_size)
{
  FILE *in = NULL;
  int i=0;
  *file_size =0;

  assert(file !=NULL);
  assert(buf !=NULL);

  in = fopen(file,"r");
  if( NULL == in) 
  {
    return 0;
  }

  buf[i]=fgetc(in);
  
  //load initialization file
  while( buf[i]!= (char)EOF) 
  {
    i++;
    assert( i < MAX_FILE_SIZE ); //file too big, you can redefine MAX_FILE_SIZE to fit the big file 
    buf[i]=fgetc(in);
  }
  
  buf[i]='\0';
  *file_size = i;

  fclose(in);
  return 1;
}

int INIOperation::end_of_string(char c)
{
  return '\0'==c? 1 : 0;
}

int INIOperation::left_barce(char c)
{
  return LEFT_BRACE == c? 1 : 0;
}

int INIOperation::isright_brace(char c)
{
  return RIGHT_BRACE == c? 1 : 0;
}

int INIOperation::parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
            int *key_s,int *key_e, int *value_s, int *value_e)
{
//  const char *p = buf;
//  int i=0;
//
//  assert(buf!=NULL);
//  assert(section != NULL && strlen(section));
//  assert(key != NULL && strlen(key));
//
//  *sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;
//
//  while( !end_of_string(p[i]) ) {
//    //find the section
//    if( ( 0==i ||  newline(p[i-1]) ) && left_barce(p[i]) )
//    {
//      int section_start=i+1;
//
//      //find the ']'
//      do {
//        i++;
//      } while( !isright_brace(p[i]) && !end_of_string(p[i]));
//
//      if( 0 == strncmp(p+section_start,section, i-section_start)) {
//        int newline_start=0;
//
//        i++;
//
//        //Skip over space char after ']'
//        while(isspace(p[i])) {
//          i++;
//        }
//
//        //find the section
//        *sec_s = section_start;
//        *sec_e = i;
//
//        while( ! (newline(p[i-1]) && left_barce(p[i])) 
//        && !end_of_string(p[i]) ) {
//          int j=0;
//          //get a new line
//          newline_start = i;
//
//          while( !newline(p[i]) &&  !end_of_string(p[i]) ) {
//            i++;
//          }
//          
//          //now i  is equal to end of the line
//          j = newline_start;
//
//          if(';' != p[j]) //skip over comment
//          {
//            while(j < i && p[j]!='=') {
//              j++;
//              if('=' == p[j]) {
//                if(strncmp(key,p+newline_start,j-newline_start)==0)
//                {
//                  //find the key ok
//                  *key_s = newline_start;
//                  *key_e = j-1;
//
//                  *value_s = j+1;
//                  *value_e = i;
//
//                  return 1;
//                }
//              }
//            }
//          }
//
//          i++;
//        }
//      }
//    }
//    else
//    {
//      i++;
//    }
//  }
//  return 0;

   const char *p = buf;  
    int i = 0;  
  
    assert(buf != NULL);  
    assert(section != NULL && strlen(section));  
    assert(key != NULL && strlen(key));  
  
    *sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = - 1;  
  
    while (!end_of_string(p[i]))  
    {  
        // find the section  
        if ((0 == i ||  newline(p[i - 1])) && left_barce(p[i]))  
        {  
            int section_start = i + 1;  
  
            // find the ']'  
  
            do  
            {  
                ++i;  
            }  
            while (!isright_brace(p[i]) && !end_of_string(p[i]));  
            //////////////////////////////////////////////////////////////////////////  
            if (i - section_start == strlen(section) && 0 == strncmp(p + section_start, section, strlen(section)))   
            {  
                int newline_start = 0;  
  
                ++i;  
  
                // Skip over space char after ']'  
  
                while (isspace(p[i]))  
                {  
                    ++i;  
                }  
  
                // find the section  
                *sec_s = section_start;  
  
                *sec_e = i;  
  
                while (!(newline(p[i - 1]) && left_barce(p[i]))  
                    && !end_of_string(p[i]))  
                {  
                    int j = 0;  
                    // get a new line  
                    newline_start = i;  
  
                    while (!newline(p[i]) &&  !end_of_string(p[i]))  
                    {  
                        ++i;  
                    }  
  
                    // now i  is equal to end of the line  
                    j = newline_start;  
                    int valid = j;  
  
                    if ('#' != p[j]) // skip over comment  
                    {  
                        while (j < i && p[j] != '=')  
                        {  
                            ++j;  
                            if (' ' != p[j] && '/t' != p[j] && '=' != p[j])  
                                valid = j;  
  
                            if ('=' == p[j])  
                            {  
                                if (j - newline_start == strlen(key) && strncmp(key, p + newline_start, strlen(key)) == 0)   
                                {  
                                    // find the key ok  
                                    *key_s = newline_start;  
                                    *key_e = j - 1;  
  
                                    valid = j + 1;  
                                    while (' ' == p[valid] || '/t' == p[valid])  
                                        valid++;  
                                    *value_s = valid;  
                                    *value_e = i;  
  
                                    return 1;  
                                }  
                            }  
                        }  
                    }  
  
                    ++i;  
                }  
            }  
        }  
        else  
        {  
            ++i;  
        }  
    }  
  
    return 0;  


}

int INIOperation::read_profile_string(const char *section, const char *key,char *value, int size, const char *default_value, const char *file)
{
  char buf[MAX_FILE_SIZE];memset(buf,0,sizeof(char)*MAX_FILE_SIZE);
  int file_size;
  int sec_s,sec_e,key_s,key_e, value_s, value_e;

  //check parameters
  assert(section != NULL && strlen(section));
  assert(key != NULL && strlen(key));
  assert(value != NULL);
  assert(size > 0);
  assert(file !=NULL &&strlen(key));
  //printf("read_profile_string...\n");
  if(!load_ini_file(file,buf,&file_size))
  {
    if(default_value!=NULL)
    {
      strncpy(value,default_value, size);
    }
    return 0;
  }
 // printf("read_profile_string...o\n");
  if(!parse_file(section,key,buf,&sec_s,&sec_e,&key_s,&key_e,&value_s,&value_e))
  {
    if(default_value!=NULL)
    {
      strncpy(value,default_value, size);
    }
    return 0; //not find the key
  }
  else
  {
    int cpcount = value_e -value_s;
   // printf("***key***%s****%d = %d - %d****************\n",key,cpcount , value_e ,value_s);
    if( size-1 < cpcount)
    {
      cpcount =  size-1;
    }
  
    memset(value, 0, size);
    memcpy(value,buf+value_s, cpcount );
    value[cpcount] = '\0';

    return 1;
  }
}

int INIOperation::read_profile_int(const char *section, const char *key,int default_value,const char *file)
{
  char value[32] = {0};
  if(!read_profile_string(section,key,value, sizeof(value),NULL,file))
  {
    return default_value;
  }
  else
  {
    return atoi(value);
  }
}

float INIOperation::read_profile_float( const char *section, const char *key,float default_value,const char *file)
{
  char value[32] = {0};
  if(!read_profile_string(section,key,value, 32,NULL,file))
  {
     printf("=000====key %s %f======\n",key,value);
    //printf("********section  **%s**key**%s**default_value*%f***********\n",section,key,default_value);
    return default_value;
  }
  else
  {
    //printf("********section  **%s**key**%s**value*%s**********\n",section,key,value);
    printf("===111==key %s %s======\n",key,value);
    return atof(value);
  }
  
}

void INIOperation::initData()
{
  memset(strIJToLonLatFileName,0,sizeof(strIJToLonLatFileName));
  memset(strSurfaceTypeFileName,0,sizeof(strSurfaceTypeFileName));
  memset(strSatZenFileName,0,sizeof(strSatZenFileName));
  memset(strnLandingFileName,0,sizeof(strnLandingFileName));
  memset(strxjrasterFileName,0,sizeof(strxjrasterFileName));
  memset(strxjdh_newFileName,0,sizeof(strxjdh_newFileName));
  memset(strLandCoverFileName,0,sizeof(strLandCoverFileName));
  memset(strLandImgFileName,0,sizeof(strLandImgFileName));
  memset(strSFTFileName,0,sizeof(strSFTFileName));
  memset(strInputFileName4000,0,sizeof(strInputFileName4000));
  memset(strInputFileNameGEO,0,sizeof(strInputFileNameGEO));
  memset(strInputFileName,0,sizeof(strInputFileName));
  memset(strInputCLMFileName,0,sizeof(strInputCLMFileName));
  memset(strOutputHDFFileName,0,sizeof(strOutputHDFFileName));
  memset(strOutputNCFileName,0,sizeof(strOutputNCFileName));
  memset(strOutputFIMGFileName,0,sizeof(strOutputFIMGFileName));
  memset(strOutputPIMGFileName,0,sizeof(strOutputPIMGFileName));
  memset(strOutputFDJPGFileName,0,sizeof(strOutputFDJPGFileName));
   memset(strOutputFNJPGFileName,0,sizeof(strOutputFNJPGFileName));
  memset(strOutputTable1FileName,0,sizeof(strOutputTable1FileName));
  memset(strOutputTable2FileName,0,sizeof(strOutputTable2FileName));
  memset(strOutputTable3FileName,0,sizeof(strOutputTable3FileName)); 
  memset(strOutputEnFPAFileName,0,sizeof(strOutputEnFPAFileName));
  memset(strOutputEnFPTFileName,0,sizeof(strOutputEnFPTFileName));
  memset(strOutputTable1FileNameNC,0,sizeof(strOutputTable1FileNameNC));
  memset(strOutputTable2FileNameNC,0,sizeof(strOutputTable2FileNameNC));
  memset(strOutputTable3FileNameNC,0,sizeof(strOutputTable3FileNameNC));
  memset(strOutputFileNameOK,0,sizeof(strOutputFileNameOK));
  memset(strOutputIMGPath,0,sizeof(strOutputIMGPath));
  memset(strTXTConfigPath,0,sizeof(strTXTConfigPath));
  memset(diskarr,0,sizeof(diskarr));
  memset(m_strpython,0,sizeof(m_strpython));
  memset(m_commandexe,0,sizeof(m_commandexe));
  memset(m_strpgs,0,sizeof(m_strpgs));
  
  memset(m_strL0Q,0,sizeof(m_strL0Q));
  memset(m_strPOSQ,0,sizeof(m_strPOSQ));
  memset(m_strCALQ,0,sizeof(m_strCALQ));
  
  factor_mid = 0;
  factor_far = 0;
  dt3th = 0;
  dt34th = 0;  
}

void INIOperation::initDataMemory()
{
  cout << "start MemInit" << endl;

  ///内存分配   m_iWidth * m_iHeight 分配2Km的内存空间
  m_wChannelsData = new unsigned short*[8];
  for (int i = 0; i <= 7 ; i++)
  {
    m_wChannelsData[i] = new unsigned short[m_iWidth * m_iHeight];
  }
  
  m_fSunZenithAnale  = new float* [m_iHeight];              //存放每个象元的太阳天顶角，数据类型为4字节浮点型
  m_fSunGlitAngle = new float* [m_iHeight];                //存放每个象元的太阳耀斑角，数据类型为4字节浮点型
  m_ucCloud = new unsigned char *[m_iHeight];               //存放每个象元的经度值，数据类型为4字节浮点型
  m_fLat  = new float* [m_iHeight];                //存放每个象元的纬度值，数据类型为4字节浮点型
  m_fLong = new float* [m_iHeight];                //存放每个象元的经度值，数据类型为4字节浮点型
  pSFTMask = new unsigned short* [m_iHeight];
  m_cPixelSurfaceType = new char *[m_iHeight];     //存放每个象元地表类型值，数据类型为1字节整型  
  m_fPixelSatelliteZenithAnale = new float* [m_iHeight];//   卫星天顶角原始数据是4Km 需要插值成2KM

  for (int i = 0;i < m_iHeight; i++)
  {
    m_ucCloud[i] = new unsigned char[m_iWidth];
    m_fSunZenithAnale[i]  = new float[m_iWidth];
    m_fSunGlitAngle[i] = new float[m_iWidth];
    
    memset(m_ucCloud[i],255,sizeof(unsigned char)*m_iWidth);//对云检测进行赋初始值
    memset(m_fSunZenithAnale[i],0,sizeof(float)*m_iWidth);
    memset(m_fSunGlitAngle[i],0,sizeof(float)*m_iWidth);
    
    m_fLat[i]  = new float[m_iWidth];
    m_fLong[i] = new float[m_iWidth];
    pSFTMask[i] = new unsigned short[m_iWidth];
    m_cPixelSurfaceType[i] = new char[m_iWidth];    
    m_fPixelSatelliteZenithAnale[i] = new float[m_iWidth];    
    memset(m_fLat[i],0,sizeof(float)*m_iWidth);
    memset(m_fLong[i],0,sizeof(float)*m_iWidth);
    memset(pSFTMask[i],0,sizeof(unsigned short)*m_iWidth);
    memset(m_cPixelSurfaceType[i],0,sizeof(char)*m_iWidth);
    memset(m_fPixelSatelliteZenithAnale[i],0,sizeof(float)*m_iWidth);
  } 
  //for(int i = 0 ; i < m_iheightOfstatic ; i++)
  //{
    
  //}
  cout << "MemInit end" << endl;  
}

void INIOperation::freeMemory()
{
///内存释放      
  for (int i = 0; i <= 7; i++)
    delete [] m_wChannelsData[i];
     delete [] m_wChannelsData;
  
  for (int i = 0; i < m_iHeight; i++)
  {
    delete [] m_ucCloud[i];
    delete [] m_fLat[i];
    delete [] m_fLong[i];
    delete [] pSFTMask[i];
    delete [] m_fSunZenithAnale[i];
    delete [] m_fSunGlitAngle[i];
    delete [] m_cPixelSurfaceType[i];             
    delete [] m_fPixelSatelliteZenithAnale[i];
  }
  
  delete[] m_ucCloud;
  delete[] m_fLat;
  delete[] m_fLong;
  delete[] pSFTMask;
  delete[] m_fSunZenithAnale;
  delete[] m_fSunGlitAngle;
  delete[] m_cPixelSurfaceType;        
  delete[] m_fPixelSatelliteZenithAnale;
  
  if(HDFFileAtt != NULL)
  {
    delete HDFFileAtt;
    HDFFileAtt = NULL;
  }
  
  m_ucCloud = NULL;
  m_wChannelsData = NULL;
  m_fLat = NULL;
  m_fLong = NULL;
  pSFTMask = NULL;
  m_fSunZenithAnale = NULL;
  m_fSunGlitAngle = NULL;
  m_cPixelSurfaceType   = NULL;         
  m_fPixelSatelliteZenithAnale = NULL;  
}

void INIOperation::analysisSHM(char* param1,char* param2,char* startDate,char* endDate)
{  
  
  if(strcmp(sataName,"FY4A")==0)
  {
     strncpy(FIREPAROFINPUT, FIREPAROFINPUT_FY4, sizeof(FIREPAROFINPUT_FY4));
     strncpy(FIREPAR, FIREPAR_FY4, sizeof(FIREPAR_FY4));
     printf("----FY4A process----\n");
  }
  else
  {
     strncpy(FIREPAROFINPUT, FIREPAROFINPUT_AHI8, sizeof(FIREPAROFINPUT_AHI8));
     strncpy(FIREPAR, FIREPAR_AHI8, sizeof(FIREPAR_AHI8));
      printf("----AHI8 process----\n");
  }
  
  ///@code
  ///获取输入输出配置文件
    read_profile_string("ConfigFileName","IJToLonLatFileName",strIJToLonLatFileName,sizeof(strIJToLonLatFileName),"",FIREPAROFINPUT);
    read_profile_string("ConfigFileName","SurfaceTypeName",strSurfaceTypeFileName,sizeof(strSurfaceTypeFileName),"",FIREPAROFINPUT);
    read_profile_string("ConfigFileName","nLandingName",strnLandingFileName,sizeof(strnLandingFileName),"",FIREPAROFINPUT);
    read_profile_string("ConfigFileName","xjrasterName",strxjrasterFileName,sizeof(strxjrasterFileName),"",FIREPAROFINPUT);
    read_profile_string("ConfigFileName","xjdh_newName",strxjdh_newFileName,sizeof(strxjdh_newFileName),"",FIREPAROFINPUT);
    read_profile_string("ConfigFileName","LandCoverName",strLandCoverFileName,sizeof(strLandCoverFileName),"",FIREPAROFINPUT);
    read_profile_string("ConfigFileName","LandImgName",strLandImgFileName,sizeof(strLandImgFileName),"",FIREPAROFINPUT);
    read_profile_string("ConfigFileName","SFTFile",strSFTFileName,sizeof(strSFTFileName),"",FIREPAROFINPUT);
    
    read_profile_string("InOutPutFileName","InputFileName",strInputFileName,sizeof(strInputFileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","InputCLMFileName",strInputCLMFileName,sizeof(strInputCLMFileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","Input4KMFileName",strInputFileName4000,sizeof(strInputFileName4000),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","InputGEOFileName",strInputFileNameGEO,sizeof(strInputFileNameGEO),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_HDF",strOutputHDFFileName,sizeof(strOutputHDFFileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_NC",strOutputNCFileName,sizeof(strOutputNCFileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_FIMG",strOutputFIMGFileName,sizeof(strOutputFIMGFileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_PIMG",strOutputPIMGFileName,sizeof(strOutputPIMGFileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_FDJPG",strOutputFDJPGFileName,sizeof(strOutputFDJPGFileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_FNJPG",strOutputFNJPGFileName,sizeof(strOutputFNJPGFileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_Table1",strOutputTable1FileName,sizeof(strOutputTable1FileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_Table2",strOutputTable2FileName,sizeof(strOutputTable2FileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_Table3",strOutputTable3FileName,sizeof(strOutputTable3FileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileNameNC_Table1",strOutputTable1FileNameNC,sizeof(strOutputTable1FileNameNC),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileNameNC_Table2",strOutputTable2FileNameNC,sizeof(strOutputTable2FileNameNC),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileNameNC_Table3",strOutputTable3FileNameNC,sizeof(strOutputTable3FileNameNC),"",FIREPAROFINPUT); 
     
     read_profile_string("InOutPutFileName","OutputFileName_FPA_TXT",strOutputEnFPAFileName,sizeof(strOutputEnFPAFileName),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","OutputFileName_FPT_TXT",strOutputEnFPTFileName,sizeof(strOutputEnFPTFileName),"",FIREPAROFINPUT); 
       
    read_profile_string("InOutPutFileName","OKOutputFileName",strOutputFileNameOK,sizeof(strOutputFileNameOK),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","TXTFileDir",strTXTConfigPath,sizeof(strTXTConfigPath),"",FIREPAROFINPUT);
    read_profile_string("InOutPutFileName","IMGFileDir",strOutputIMGPath,sizeof(strOutputIMGPath),"",FIREPAROFINPUT);
    
  ///获取python绘图信息
    read_profile_string("Python","CommandPython",m_strpython,sizeof(m_strpython),"",FIREPAROFINPUT); 
    read_profile_string("Python","CommandEXE",m_commandexe,sizeof(m_commandexe),"",FIREPAROFINPUT); 
    read_profile_string("Python","CommandPGS",m_strpgs,sizeof(m_strpgs),"",FIREPAROFINPUT);
    
//读参数文件  
if(access(FIREPAR,0)!= 0)
    {
      ZLDSWriteLog('W',"Thresh File not exist!");
      exit(ZEPARAMOFFIRE);
    }
    
 SZAngleUp   =  read_profile_float("ThreadValue","SZAngleUp",0,FIREPAR);
 Nodatavalue =  read_profile_float("ThreadValue","Nodatavalue",0,FIREPAR);
 SatZAngLimit=  read_profile_float("ThreadValue","SatZAngLimit",0,FIREPAR);
 SZAngleNt   =  read_profile_float("ThreadValue","SZAngleNt",0,FIREPAR);
 VisAngleNt  =  read_profile_float("ThreadValue","VisAngleNt",0,FIREPAR);
 RNir_water  =  read_profile_float("ThreadValue","RNir_water",0,FIREPAR);
 RNir_Vis_water=  read_profile_float("ThreadValue","RNir_Vis_water",0,FIREPAR);
 SunGtAngLimit =  read_profile_float("ThreadValue","SunGtAngLimit",0,FIREPAR);
 T4LowTemp     =  read_profile_float("ThreadValue","T4LowTemp",0,FIREPAR);
 T11LowTemp    =  read_profile_float("ThreadValue","T11LowTemp",0,FIREPAR);
 VisDCloudBJ   =  read_profile_float("ThreadValue","VisDCloudBJ",0,FIREPAR);
 SZAngDCloudBJ =  read_profile_float("ThreadValue","SZAngDCloudBJ",0,FIREPAR);
 T11DCloud     =  read_profile_float("ThreadValue","T11DCloud",0,FIREPAR);
 RVisCloud     =  read_profile_float("ThreadValue","RVisCloud",0,FIREPAR);
 SZAngNCloudBJ =  read_profile_float("ThreadValue","SZAngNCloudBJ",0,FIREPAR);
 T11NCloud     =  read_profile_float("ThreadValue","T11NCloud",0,FIREPAR);
 VisDCloudth   =  read_profile_float("ThreadValue","VisDCloudth",0,FIREPAR);
 T11NCloudth   =  read_profile_float("ThreadValue","T11NCloudth",0,FIREPAR);  

                                         
  T4BJLimit     =  read_profile_float("ThreadValue","T4BJLimit",0,FIREPAR);  
  RegnWEBJ      =  read_profile_float("ThreadValue","RegnWEBJ",0,FIREPAR);  
  Coe1          =  read_profile_float("ThreadValue","Coe1",0,FIREPAR);  
  Coe2          =  read_profile_float("ThreadValue","Coe2",0,FIREPAR);  
  DtBJ1         =  read_profile_float("ThreadValue","DtBJ1",0,FIREPAR); 
   DtBJ2         =  read_profile_float("ThreadValue","DtBJ2",0,FIREPAR);  
  Count_Limit   =  read_profile_float("ThreadValue","Count_Limit",0,FIREPAR);  
  Count_No      =  read_profile_float("ThreadValue","Count_No",0,FIREPAR);
  BJlimit      =  read_profile_float("ThreadValue","BJlimit",0,FIREPAR); 
                                         
 //20180315-----------up-----------------------------
  BJDNIR      =  read_profile_float("ThreadValue","BJDNIR",0,FIREPAR);
  BJDT11      =  read_profile_float("ThreadValue","BJDT11",0,FIREPAR);                                         
  //20180315-----------down-----------------------------
                                          
  T4CLDBJ       =  read_profile_float("ThreadValue","T4CLDBJ",0,FIREPAR);  
  VisCLDBJLimit =  read_profile_float("ThreadValue","VisCLDBJLimit",0,FIREPAR);  
  T11CLD        =  read_profile_float("ThreadValue","T11CLD",0,FIREPAR);    
  T4CLDBJLimit =  read_profile_float("ThreadValue","T4CLDBJLimit",0,FIREPAR); 
  fCLDTempLimit = read_profile_float("ThreadValue","fCLDTempLimit",0,FIREPAR);
  iCLDCountLmt = read_profile_float("ThreadValue","iCLDCountLmt",0,FIREPAR);
  CBJlimit      =  read_profile_float("ThreadValue","CBJlimit",0,FIREPAR);  
    
  iCountSIR  =  read_profile_float("ThreadValue","iCountSIR",0,FIREPAR);  
   
  SunZRFLT=  read_profile_float("ThreadValue","SunZRFLT",0,FIREPAR);  
  DR6SLimit=  read_profile_float("ThreadValue","DR6SLimit",0,FIREPAR);  
  DT4SLTUP=  read_profile_float("ThreadValue","DT4SLTUP",0,FIREPAR);
  DT4SLTLW=  read_profile_float("ThreadValue","DT4SLTLW",0,FIREPAR);  
  T4SunLT=  read_profile_float("ThreadValue","T4SunLT",0,FIREPAR); 
  
  T4HiFire =read_profile_float("ThreadValue","T4HiFire",0,FIREPAR);
  DT4HiFire =read_profile_float("ThreadValue","DT4HiFire",0,FIREPAR);   
  DT4_11HiFire =read_profile_float("ThreadValue","DT4_11HiFire",0,FIREPAR);
  CloudHiFire =read_profile_float("ThreadValue","CloudHiFire",0,FIREPAR); 
  
 LKT4RL=  read_profile_float("ThreadValue","LKT4RL",0,FIREPAR);  
 DLKT4_11RL=  read_profile_float("ThreadValue","DLKT4_11RL",0,FIREPAR);  
 LKT11RL=  read_profile_float("ThreadValue","LKT11RL",0,FIREPAR);       
 LKLineRL=  read_profile_float("ThreadValue","LKLineRL",0,FIREPAR); 
 LKRawRL  =  read_profile_float("ThreadValue","LKRawRL",0,FIREPAR);    
 LKT4CW    =  read_profile_float("ThreadValue","LKT4CW",0,FIREPAR); 
 DLKT4_11CW  =  read_profile_float("ThreadValue","DLKT4_11CW",0,FIREPAR); 
 LKT11CW  =  read_profile_float("ThreadValue","LKT11CW",0,FIREPAR);  


 LKT4NH    =  read_profile_float("ThreadValue","LKT4NH",0,FIREPAR); 
 DLKT4_11NH=  read_profile_float("ThreadValue","DLKT4_11NH",0,FIREPAR); 
 LKT11NH  =  read_profile_float("ThreadValue","LKT11NH",0,FIREPAR); 
 LKLineNH  =  read_profile_float("ThreadValue","LKLineNH",0,FIREPAR); 
 LKRawNH  =  read_profile_float("ThreadValue","LKRawNH",0,FIREPAR);
 T4NR_Df  =  read_profile_float("ThreadValue","T4NR_Df",0,FIREPAR);
 NRT4Limit=  read_profile_float("ThreadValue","NRT4Limit",0,FIREPAR);
 NRPixNo1  =  read_profile_float("ThreadValue","NRPixNo1",0,FIREPAR);   
 NRPixNo2  =  read_profile_float("ThreadValue","NRPixNo2",0,FIREPAR);
  
 T4CLDFIRELW	      =  read_profile_float("ThreadValue", "T4CLDFIRELW"	     , 0,FIREPAR);  
 DT4CLDFIRELWLW		  =  read_profile_float("ThreadValue", "DT4CLDFIRELWLW"		 ,0,FIREPAR); 
 DT4CLDFIRELWUP		     =  read_profile_float("ThreadValue","DT4CLDFIRELWUP"	 ,0,FIREPAR);
 DT11CLDFIREBJCLW    = read_profile_float("ThreadValue",    "DT11CLDFIREBJCLW" ,0,FIREPAR);
 VISCLDFIREBJCUP    =  read_profile_float("ThreadValue",  "VISCLDFIREBJCUP",0,FIREPAR); 
 DT11CLDFIRELW       =  read_profile_float("ThreadValue",   "DT11CLDFIRELW"    ,0,FIREPAR);  
 T4CLDFIREUP	      =  read_profile_float("ThreadValue", "T4CLDFIREUP"	   ,0,FIREPAR); 
 DT4CLDFIREUPLW		   =  read_profile_float("ThreadValue",   "DT4CLDFIREUPLW"	 ,0,FIREPAR); 
 DT411CLDFIRELW	   =  read_profile_float("ThreadValue",     "DT411CLDFIRELW" ,0,FIREPAR);  
 DT11CLDFIREBJCUP   =  read_profile_float("ThreadValue",   "DT11CLDFIREBJCUP" ,0,FIREPAR); 
 VISCLDFIREBJCUPUP   =  read_profile_float("ThreadValue",   "VISCLDFIREBJCUPUP"  ,0,FIREPAR); 
 
 //20180315----------------------up--------------------------------------
DT11CLDFIRELAND   =  read_profile_float("ThreadValue",   "DT11CLDFIRELAND" ,0,FIREPAR); 
VISCLDFIRELAND   =  read_profile_float("ThreadValue",   "VISCLDFIRELAND"  ,0,FIREPAR); 
 //20180315----------------------down-------------------------------------- 
 
 
 T4CLDFIRENH    =  read_profile_float("ThreadValue","T4CLDFIRENH"  ,0,FIREPAR); 
 VisCLDFIRECE   =  read_profile_float("ThreadValue","VisCLDFIRECE"  ,0,FIREPAR); 
 DT4CLDFIRENH =  read_profile_float("ThreadValue", "DT4CLDFIRENH" ,0,FIREPAR); 
 DT4CLDFIRESH=  read_profile_float("ThreadValue",   "DT4CLDFIRESH" ,0,FIREPAR); 
 DT411CLDFIRECE =  read_profile_float("ThreadValue","DT411CLDFIRECE" ,0,FIREPAR);
 
 //20180315----------------------up--------------------------------------
T4CLDFIREWN   =  read_profile_float("ThreadValue",   "T4CLDFIREWN" ,0,FIREPAR); 
VisCLDFIRECE   =  read_profile_float("ThreadValue",   "VisCLDFIRECE"  ,0,FIREPAR); 
DT4CLDFIREWN   =  read_profile_float("ThreadValue",   "DT4CLDFIREWN" ,0,FIREPAR); 
DT4CLDFIRESH   =  read_profile_float("ThreadValue",   "DT4CLDFIRESH"  ,0,FIREPAR); 
DT411CLDFIRECE   =  read_profile_float("ThreadValue",   "DT411CLDFIRECE"  ,0,FIREPAR); 

 //20180315----------------------down--------------------------------------  
 
  
T4CLDFIRECT	    =  read_profile_float("ThreadValue",  "T4CLDFIRECT"     ,0,FIREPAR); 
VisCLDFIRECW	  =  read_profile_float("ThreadValue", "VisCLDFIRECW"   ,0,FIREPAR); 
DT4CLDFIRECT	  =  read_profile_float("ThreadValue", "DT4CLDFIRECT"   ,0,FIREPAR); 
DT411CLDFIRECW  =  read_profile_float("ThreadValue", "DT411CLDFIRECW" ,0,FIREPAR); 
DT11CLDETUP     =  read_profile_float("ThreadValue",    "DT11CLDETUP"      ,0,FIREPAR); 
VISCLDBJCETUP   =  read_profile_float("ThreadValue",   "VISCLDBJCETUP"       ,0,FIREPAR); 
T4CLDFIREWS	    =  read_profile_float("ThreadValue",   "T4CLDFIREWS"     ,0,FIREPAR);                                                                                            
VisCLDFIREWS	  =  read_profile_float("ThreadValue",  "VisCLDFIREWS"   ,0,FIREPAR);                                                                                               
DT4CLDFIREWS	  =  read_profile_float("ThreadValue",  "DT4CLDFIREWS"   ,0,FIREPAR);       
DT411CLDFIREWS  =  read_profile_float("ThreadValue",  "DT411CLDFIREWS" ,0,FIREPAR);        
DT11CLDWSUP     =  read_profile_float("ThreadValue",     "DT11CLDWSUP"      ,0,FIREPAR);
VISCLDBJCWSUP   =  read_profile_float("ThreadValue",   "VISCLDBJCWSUP"     ,0,FIREPAR);  


 //20180315----------------------up--------------------------------------  
CLDLineNHUP     =  read_profile_float("ThreadValue",    "CLDLineNHUP"      ,0,FIREPAR); 
CLDLineNHLW   =  read_profile_float("ThreadValue",   "CLDLineNHLW"       ,0,FIREPAR); 
CLDRawNHUP	    =  read_profile_float("ThreadValue",   "CLDRawNHUP"     ,0,FIREPAR);                                                                                            
CLDRawNHLW	  =  read_profile_float("ThreadValue",  "CLDRawNHLW"   ,0,FIREPAR);                                                                                               
CLDRGLineNHUP	  =  read_profile_float("ThreadValue",  "CLDRGLineNHUP"   ,0,FIREPAR);       
CLDRGLineNHLW  =  read_profile_float("ThreadValue",  "CLDRGLineNHLW" ,0,FIREPAR);        
CLDRGRawNHUP     =  read_profile_float("ThreadValue",     "CLDRGRawNHUP"      ,0,FIREPAR);
CLDRGRawNHLW   =  read_profile_float("ThreadValue",   "CLDRGRawNHLW"     ,0,FIREPAR);  
//20180315----------------------down--------------------------------------  

    
CLDLineETUP	    =  read_profile_float("ThreadValue",  "CLDLineETUP"   ,0,FIREPAR);       
CLDLineETLW     =  read_profile_float("ThreadValue",  "CLDLineETLW"   ,0,FIREPAR); 

//20180315----------------------up--------------------------------------  
CLDRGLineETUP     =  read_profile_float("ThreadValue",    "CLDRGLineETUP"      ,0,FIREPAR); 
CLDRGLineETLW   =  read_profile_float("ThreadValue",   "CLDRGLineETLW"       ,0,FIREPAR);
 
CLDLineWNUP	  =  read_profile_float("ThreadValue",  "CLDLineWNUP"   ,0,FIREPAR);       
CLDLineWNLW  =  read_profile_float("ThreadValue",  "CLDLineWNLW" ,0,FIREPAR);        
CLDRawWNUP     =  read_profile_float("ThreadValue",     "CLDRawWNUP"      ,0,FIREPAR);
CLDRawWNLW   =  read_profile_float("ThreadValue",   "CLDRawWNLW"     ,0,FIREPAR);  

 
CLDRGLineWNUP	  =  read_profile_float("ThreadValue",  "CLDRGLineWNUP"   ,0,FIREPAR);       
CLDRGLineWNLW  =  read_profile_float("ThreadValue",  "CLDRGLineWNLW" ,0,FIREPAR);        
CLDRGRawWNUP     =  read_profile_float("ThreadValue", "CLDRGRawWNUP" ,0,FIREPAR);
CLDRGRawWNLW   =  read_profile_float("ThreadValue",   "CLDRGRawWNLW"     ,0,FIREPAR);  


CLDLineWSUP	  =  read_profile_float("ThreadValue",  "CLDLineWSUP"   ,0,FIREPAR);       
CLDLineWSLW  =  read_profile_float("ThreadValue",  "CLDLineWSLW" ,0,FIREPAR);        
CLDRGLineWSUP     =  read_profile_float("ThreadValue",     "CLDRGLineWSUP"      ,0,FIREPAR);
CLDRGLineWSLW   =  read_profile_float("ThreadValue",   "CLDRGLineWSLW"     ,0,FIREPAR);  
//20180315----------------------down--------------------------------------  
      
CLDSameNoNH     =  read_profile_float("ThreadValue",  "CLDSameNoNH"   ,0,FIREPAR);
//20180315----------------------up--------------------------------------  
CLDSameNoWN     =  read_profile_float("ThreadValue",  "CLDSameNoWN"   ,0,FIREPAR);
// 20180315----------------------down--------------------------------------      
CLDSameNoET     =  read_profile_float("ThreadValue",  "CLDSameNoET"   ,0,FIREPAR);      
CLDSameNoWS     =  read_profile_float("ThreadValue",  "CLDSameNoWS" ,0,FIREPAR);         



 
  T4BJDVLineNH  =  read_profile_float("ThreadValue", "T4BJDVLineNH"    ,0,FIREPAR);    
  T4BJDV_NHUP    =  read_profile_float("ThreadValue","T4BJDV_NHUP"    ,0,FIREPAR);   
  T4BJDV_NHUPAJ =  read_profile_float("ThreadValue", "T4BJDV_NHUPAJ"  ,0,FIREPAR); 
  T4BJDV_NHLW    =  read_profile_float("ThreadValue","T4BJDV_NHLW"    ,0,FIREPAR);  
  T4BJDV_SHUP    =  read_profile_float("ThreadValue","T4BJDV_SHUP"    ,0,FIREPAR);  
  T4BJDV_SHUPAJ =  read_profile_float("ThreadValue", "T4BJDV_SHUPAJ"  ,0,FIREPAR);  
  T4BJDV_SHLW    =  read_profile_float("ThreadValue","T4BJDV_SHLW"    ,0,FIREPAR); 
  T4BJDV_SHLWAJ =  read_profile_float("ThreadValue", "T4BJDV_SHLWAJ"  ,0,FIREPAR);   
  T4BJDV_SHAJ    =  read_profile_float("ThreadValue","T4BJDV_SHAJ"    ,0,FIREPAR);   
  T4_11BJDV_NHUP=  read_profile_float("ThreadValue", "T4_11BJDV_NHUP" ,0,FIREPAR); 
  T4_11BJDV_NHUPAJ= read_profile_float("ThreadValue","T4_11BJDV_NHUPAJ" ,0,FIREPAR);  
  T45BJDVLineNH   = read_profile_float("ThreadValue","T45BJDVLineNH"    ,0,FIREPAR);   
  T4_11BJDV_NHLW  = read_profile_float("ThreadValue","T4_11BJDV_NHLW"   ,0,FIREPAR); 
  T4_11BJDV_SHUP  =  read_profile_float("ThreadValue","T4_11BJDV_SHUP"  ,0,FIREPAR);     
  T4_11BJDV_SHUPAJ=  read_profile_float("ThreadValue","T4_11BJDV_SHUPAJ",0,FIREPAR); 
  T4_11BJDV_SHLW  =  read_profile_float("ThreadValue","T4_11BJDV_SHLW"  ,0,FIREPAR); 
  T4_11BJDV_SHLWAJ=  read_profile_float("ThreadValue","T4_11BJDV_SHLWAJ",0,FIREPAR);  
  T4_11BJDV_SHAJ  =  read_profile_float("ThreadValue","T4_11BJDV_SHAJ"  ,0,FIREPAR);   
      
  T4_Factr   =  read_profile_float("ThreadValue",     "T4_Factr"        ,0,FIREPAR);    
  T4_11Factr =  read_profile_float("ThreadValue",     "T4_11Factr"      ,0,FIREPAR);  
                                                                          
  Vis_Fireth=  read_profile_float("ThreadValue",     "Vis_Fireth"       ,0,FIREPAR); 
  DFR6_Fire  =  read_profile_float("ThreadValue",    "DFR6_Fire"        ,0,FIREPAR);  
 SatZ_fire  =  read_profile_float("ThreadValue",     "SatZ_fire"        ,0,FIREPAR); 
  SunZ_fire=  read_profile_float("ThreadValue",      "SunZ_fire"        ,0,FIREPAR); 
  IR45BjAver_fire =read_profile_float("ThreadValue", "IR45BjAver_fire"  ,0,FIREPAR);  
 IR45BjAver_fireAJ =read_profile_float("ThreadValue","IR45BjAver_fireAJ",0,FIREPAR); 
  
 
   SunRfT4     =  read_profile_float("ThreadValue",  "SunRfT4"          ,0,FIREPAR);    
  SunRfCH6LW  =  read_profile_float("ThreadValue",   "SunRfCH6LW"       ,0,FIREPAR); 
  SunRfCH6UP  =  read_profile_float("ThreadValue",   "SunRfCH6UP"       ,0,FIREPAR);    
  
   CldT4_Edg     =  read_profile_float("ThreadValue","CldT4_Edg"        ,0,FIREPAR);  
   CldT4BjDf_Edg  = read_profile_float("ThreadValue","CldT4BjDf_Edg"   ,0,FIREPAR);        
  CldSunZenith_Edg     =  read_profile_float("ThreadValue",  "CldSunZenith_Edg"   ,0,FIREPAR);  
  CldT4_EdgLW	         =  read_profile_float("ThreadValue",  "CldT4_EdgLW"   ,0,FIREPAR);  
  CldT4BjDf_EdgLW	     =  read_profile_float("ThreadValue",  "CldT4BjDf_EdgLW" ,0,FIREPAR);     
   
   DstT4_Edg=  read_profile_float("ThreadValue",     "DstT4_Edg"        ,0,FIREPAR);  
   
   WatT4_Edg=  read_profile_float("ThreadValue",     "WatT4_Edg"        ,0,FIREPAR); 
   
   DBjT4_dfLw    =  read_profile_float("ThreadValue","DBjT4_dfLw"       ,0,FIREPAR);   
   DBjT4bj_dfLw  =  read_profile_float("ThreadValue","DBjT4bj_dfLw"     ,0,FIREPAR);   
   DBjT4bjc_dfLw=  read_profile_float("ThreadValue", "DBjT4bjc_dfLw"    ,0,FIREPAR); 
   DT4b_dfLw    =  read_profile_float("ThreadValue", "DT4b_dfLw"        ,0,FIREPAR);   
   DT4b_dfCoe  =  read_profile_float("ThreadValue",  "DT4b_dfCoe"       ,0,FIREPAR);   
   DBjT4_dfSH=  read_profile_float("ThreadValue",    "DBjT4_dfSH"       ,0,FIREPAR); 
    DB_dfLineNS = read_profile_float("ThreadValue",  "DB_dfLineNS"      ,0,FIREPAR);   
   DT11bjc_dfUP  =  read_profile_float("ThreadValue","DT11bjc_dfUP"     ,0,FIREPAR);   
   DT11bjc_dfLW=  read_profile_float("ThreadValue",  "DT11bjc_dfLW"     ,0,FIREPAR);    
   //20180227*********up****
   T11land_W=  read_profile_float("ThreadValue",    "T11land_W"       ,0,FIREPAR); 
    DT4land_W = read_profile_float("ThreadValue",  "DT4land_W"      ,0,FIREPAR);   
   DT11land_W  =  read_profile_float("ThreadValue","DT11land_W"     ,0,FIREPAR);   
   DVISland_W=  read_profile_float("ThreadValue",  "DVISland_W"     ,0,FIREPAR);   
   //20180227******down******* 
   CldDisT11Dbj  =  read_profile_float("ThreadValue","CldDisT11Dbj"     ,0,FIREPAR); 
   CldDisVisDbj =  read_profile_float("ThreadValue", "CldDisVisDbj"      ,0,FIREPAR); 
   //20180315----------------------up--------------------------------------  
  CldDisVisUp1  =  read_profile_float("ThreadValue", "CldDisVisUp1"       ,0,FIREPAR); 
  CldDisVisUp2     =  read_profile_float("ThreadValue",  "CldDisVisUp2"   ,0,FIREPAR);
// 20180315----------------------down--------------------------------------     
 
   
   
   CldT4_UP =  read_profile_float("ThreadValue",     "CldT4_UP"          ,0,FIREPAR);    
   CldT4bj_UP =  read_profile_float("ThreadValue",   "CldT4bj_UP"        ,0,FIREPAR);
   
   //20180315----------------------up--------------------------------------  
  CldT4_UP1  =  read_profile_float("ThreadValue", "CldT4_UP1"       ,0,FIREPAR); 
   CldT4_UP2  =  read_profile_float("ThreadValue",   "CldT4_UP2"         ,0,FIREPAR); 
   CldT11_UP  =  read_profile_float("ThreadValue", "CldT11_UP"       ,0,FIREPAR); 
  CldlandDVis     =  read_profile_float("ThreadValue",  "CldlandDVis"   ,0,FIREPAR);
   CldlandDT11     =  read_profile_float("ThreadValue",  "CldlandDT11"   ,0,FIREPAR);
// 20180315----------------------down--------------------------------------     
      
    CldT11_bjc  =  read_profile_float("ThreadValue",  "CldT11_bjc"       ,0,FIREPAR);   
  // printf("------CldDisVisUp  %f--\n",CldDisVisUp);
    
    
    SameT4Up  =  read_profile_float("ThreadValue",    "SameT4Up"         ,0,FIREPAR);    
    SameNDVI  =  read_profile_float("ThreadValue",    "SameNDVI"         ,0,FIREPAR);
    SameT4BJDf=  read_profile_float("ThreadValue",    "SameT4BJDf"       ,0,FIREPAR); 
    SameT4DfUP=  read_profile_float("ThreadValue",    "SameT4DfUP"       ,0,FIREPAR);   
    SameT4DfLW=  read_profile_float("ThreadValue",    "SameT4DfLW"       ,0,FIREPAR);
     SameT11_bjc=  read_profile_float("ThreadValue",  "SameT11_bjc"      ,0,FIREPAR);
 
 SamePixUP =  read_profile_float("ThreadValue",  "SamePixUP"     ,0,FIREPAR);
 SamePixNHLW  =  read_profile_float("ThreadValue",  "SamePixNHLW"     ,0,FIREPAR);    
 SamePixNHUP=  read_profile_float("ThreadValue",    "SamePixNHUP"   ,0,FIREPAR);           
 SameT4NH   =  read_profile_float("ThreadValue",    "SameT4NH"   ,0,FIREPAR);           
 SameT4DFNH  =  read_profile_float("ThreadValue",   "SameT4DFNH" ,0,FIREPAR);           
 SameT4NHUP  =  read_profile_float("ThreadValue",   "SameT4NHUP"      ,0,FIREPAR);   
 SameT4DFUP  =  read_profile_float("ThreadValue",   "SameT4DFUP"       ,0,FIREPAR);
 //20180227*********up*************************************************************
 SameT4NHLW  =  read_profile_float("ThreadValue",   "SameT4NHLW"      ,0,FIREPAR);   
 SameT4DFNHLW  =  read_profile_float("ThreadValue",   "SameT4DFNHLW"       ,0,FIREPAR);  
 
  //20180315----------------------up--------------------------------------  
  SameT4DFWNLW1  =  read_profile_float("ThreadValue", "SameT4DFWNLW1"       ,0,FIREPAR); 
   SameT4DFWNLW2  =  read_profile_float("ThreadValue",   "SameT4DFWNLW2"         ,0,FIREPAR); 
   SameT11DFWN  =  read_profile_float("ThreadValue", "SameT11DFWN"       ,0,FIREPAR); 
  SameVISDFWN     =  read_profile_float("ThreadValue",  "SameVISDFWN"   ,0,FIREPAR);
// 20180315----------------------down--------------------------------------    
 
   
 //20180227******down************************************************************** 
 SameT4ET     =  read_profile_float("ThreadValue",  "SameT4ET"     ,0,FIREPAR);    
 SameT4DFETUP =  read_profile_float("ThreadValue",    "SameT4DFETUP",0,FIREPAR);           
 SameT4ETUP   =  read_profile_float("ThreadValue",    "SameT4ETUP",0,FIREPAR);           
 SamePixETLW   =  read_profile_float("ThreadValue",   "SamePixETLW",0,FIREPAR);           
 //20180227*********up*************************************************************
SamePixETUP   =  read_profile_float("ThreadValue",   "SamePixETUP"      ,0,FIREPAR); 
 //20180227******down************************************************************** 
   //20180315----------------------up--------------------------------------              
  SameT4DFETLW     =  read_profile_float("ThreadValue",  "SameT4DFETLW"   ,0,FIREPAR); 
  //20180315----------------------down--------------------------------------           

 SameT4WS      =  read_profile_float("ThreadValue",   "SameT4WS",0,FIREPAR);     
 SameT4DFWS     =  read_profile_float("ThreadValue",  "SameT4DFWS"   ,0,FIREPAR); 
  //20180315----------------------up--------------------------------------              
   SameT4DFWSUP     =  read_profile_float("ThreadValue",  "SameT4DFWSUP"   ,0,FIREPAR);  
   //20180315----------------------down--------------------------------------           
       
 SameT4WSUP    =  read_profile_float("ThreadValue",   "SameT4WSUP"   ,0,FIREPAR);         
 SamePixWSLW    =  read_profile_float("ThreadValue",  "SamePixWSLW" ,0,FIREPAR);            
 SamePixWSUP     =  read_profile_float("ThreadValue", "SamePixWSUP"      ,0,FIREPAR);  
 SameT4Up3     = read_profile_float("ThreadValue", "SameT4Up3"      ,0,FIREPAR);


   SameRawNo  =  read_profile_float("ThreadValue","SameRawNo",0,FIREPAR); 
   SameDT4LW  =  read_profile_float("ThreadValue","SameDT4LW",0,FIREPAR);
    //20180227*********up************************************************************* 
 SameT4bjc     =  read_profile_float("ThreadValue",  "SameT4bjc"   ,0,FIREPAR);       
 SameT11bjc    =  read_profile_float("ThreadValue",   "SameT11bjc"   ,0,FIREPAR);         
 SameQZGYDT4    =  read_profile_float("ThreadValue",  "SameQZGYDT4" ,0,FIREPAR);            
 QZGYLINE     =  read_profile_float("ThreadValue", "QZGYLINE"      ,0,FIREPAR);  
 QZGYLINELW = read_profile_float("ThreadValue", "QZGYLINELW"      ,0,FIREPAR);  
 QZGYRAW     = read_profile_float("ThreadValue", "QZGYRAW"      ,0,FIREPAR);
 QZGYRGLINEUP =  read_profile_float("ThreadValue", "QZGYRGLINEUP"      ,0,FIREPAR);  
 QZGYRGLINELW = read_profile_float("ThreadValue", "QZGYRGLINELW"      ,0,FIREPAR);  
   //20180227******down**************************************************************  
   NoisePixNo=  read_profile_float("ThreadValue","NoisePixNo",0,FIREPAR);     
   NoisePixT4=  read_profile_float("ThreadValue","NoisePixT4",0,FIREPAR); 
   
    Try_DT4BJ=  read_profile_float("ThreadValue","Try_DT4BJ",0,FIREPAR);  
    //20180315----------------------up-------------------------------------- 
   Try_DT4BJ2     =  read_profile_float("ThreadValue",  "Try_DT4BJ2"   ,0,FIREPAR);  
   //20180315----------------------down--------------------------------------      
   Try_NoiseT4=  read_profile_float("ThreadValue","Try_NoiseT4",0,FIREPAR); 
  printf("------NoisePixT4  %f--\n",NoisePixT4);
  printf("------NoisePixNo  %f--\n",NoisePixNo);
  printf("------CldDisT11Dbj  %f--\n",CldDisT11Dbj);
  



//    if(access(FIREPAR,0)!= 0)
//    {
//      ZLDSWriteLog('W',"Thresh File not exist!");
//      exit(ZEPARAMOFFIRE);
//    }
//  ///获取火点阈值参数信息
//    factor_mid = read_profile_float("ThreadValue","factr_mid",0,FIREPAR);
//    factor_far = read_profile_float("ThreadValue","factr_far", 0 ,FIREPAR);
//    dt3th = read_profile_float("ThreadValue","dt3th",0 ,FIREPAR);
//    dt34th = read_profile_float("ThreadValue","dt34th", 0 ,FIREPAR);
//  ///火点阈值参数信息进行判断
//    if(factor_mid == 0 || factor_far == 0 || dt3th == 0 || dt34th == 0)
//    {
//      ZLDSWriteLog('W',"Get Thresh File Error!");
//      exit(ZEPARAMOFFIRE);
//    }    
  ///对输入输出文件进行文件名称拼接
    parseFileName(param1,param2,startDate,endDate);

  ///对输入文件判断是否存在
    if(strcmp(strIJToLonLatFileName,"") == 0 || access(strIJToLonLatFileName,0)!= 0)
    {
      ZLDSWriteLog('W',"Get IJToLonLat error! or IJToLonLat not exist!");
      exit(ZEIJTOLATLONOFFIRE);
    }  
    if(strcmp(strSurfaceTypeFileName,"") == 0 || access(strSurfaceTypeFileName,0)!= 0)
    {
      ZLDSWriteLog('W',"Get SurfaceTypeFileName error! or SurfaceTypeFileName not exist!");
      exit(ZESURFTYPEOFFIRE);
    }    

    if(strcmp(strnLandingFileName,"") == 0 || access(strnLandingFileName,0)!= 0)
    {
      ZLDSWriteLog('W',"Get strnLandingFileName error! or strnLandingFileName not exist!");
      exit(ZENLANDIMGOFFIRE);
    }  
    if(strcmp(strxjrasterFileName,"") == 0 || access(strxjrasterFileName,0)!= 0)
    {
      ZLDSWriteLog('W',"Get strxjrasterFileName error! or strxjrasterFileName not exist!");
      exit(ZEXJRASTEROFFIRE);
    }
    if(strcmp(strxjdh_newFileName,"") == 0 || access(strxjdh_newFileName,0)!= 0)
    {
      ZLDSWriteLog('W',"Get strxjdh_newFileName error! or strxjdh_newFileName not exist!");
      exit(ZEXJTXTOFFIRE);
    }
    if(strcmp(strLandCoverFileName,"") == 0 || access(strLandCoverFileName,0)!= 0)
    {
      ZLDSWriteLog('W',"Get strLandCoverFileName error! or strLandCoverFileName not exist!");
      exit(ZELANDCOVEROFFIRE);
    }
    if(strcmp(strLandImgFileName,"") == 0 || access(strLandImgFileName,0)!= 0)
    {
      ZLDSWriteLog('W',"Get strLandImgFileName error! or strLandImgFileName not exist!");
      exit(ZELANDIMGOFFIRE);
    }
    if(strcmp(strSFTFileName,"") == 0 || access(strSFTFileName,0)!= 0)
    {
      ZLDSWriteLog('W',"Get strSFTFileName error! or strSFTFileName not exist!");
      exit(ZESFTFIRE);
    }
    printf("2KM:%s\n",strInputFileName);
    if(strcmp(strInputFileName,"") == 0 || access(strInputFileName,0)!= 0)
    {
      ZLDSWriteLog('W',"Get 2000M HDF error! or  2000M HDF not exist!");
      exit(ZE2KMHDFOFFIRE);
    }
    printf("4KM:%s\n",strInputFileName4000);  
    if(strcmp(strInputFileName4000,"") == 0 || access(strInputFileName4000,0)!= 0)
    {
      ZLDSWriteLog('W',"Get 4000M HDF error! or  4000M HDF not exist!");
      exit(ZE4KMHDFOFFIRE);
    }  
    printf("GEO:%s\n",strInputFileNameGEO);  
    if(strcmp(strInputFileNameGEO,"") == 0 || access(strInputFileNameGEO,0)!= 0)
    {
      ZLDSWriteLog('W',"Get GEO HDF error! or  GEO HDF not exist!");
      exit(ZEGEOOFFIRE);
    }      
    if(strcmp(strInputCLMFileName,"") == 0 || access(strInputCLMFileName,0)!= 0)
    {
      cloud_ID = 0;
      ZLDSWriteLog('N',"Get strInputCLMFileName error! or  strInputCLMFileName not exist!");
    }                
  ///写当前时次的输入输出信息文件
    if(writeConfigFile() != 0)
    {
      ZLDSWriteLog('W',"Write Config error!");
      exit(ZECONFIGWRITE);
    }
  ///获取通道数据宽高
    if(GetWidHei(strInputFileName,"NOMChannel07",&m_iWidth ,&m_iHeight)!=0)
    {
      ZLDSWriteLog('W',"Get wid and hei error!");
      exit(ZEGETWIDHEIOFFHS);
    }
  ///获取经纬度等静态数据宽高
    if(GetWidHei(strIJToLonLatFileName,"Lat",&m_iwidthOfstatic ,&m_iheightOfstatic)!=0)
    {
      ZLDSWriteLog('W',"Get wid and hei error!");
      exit(ZEGETWIDHEIOFFHS);
    }  
  ///获取4km分辨率数据宽高
    if(GetWidHei(strInputFileName4000,"NOMChannel12",&m_widlength ,&m_heilength)!=0)
    {
      ZLDSWriteLog('W',"Get wid and hei error!");
      exit(ZEGETWIDHEIOFFHS);
    }  
  ///数据分配内存
  initDataMemory();    
  ///@endcode    
}

/**  @brief   获取数据操作 \n
*    
*    @return   0-成功，其他失败
*/
int INIOperation::GetData()
{
   printf("-------------------------------startL endL startP %s\n",strInputFileName);
   printf("-------------------------------startL endL startP %s\n",strInputFileName4000);
  
   //读取区域数据在圆盘中的起始行列
   if(GetAttributeOf2kmFile() != 0)
  {
    ZLDSWriteLog('W',"Get Attribute Data error!");
    printf("Get Attribute Data error!\n");
    return ZEREADOFFIRE;
  }  
   
  // FY4A_AGRI_4K_FILE_INFO* InputL1FileInfo = new FY4A_AGRI_4K_FILE_INFO;
   //if(m_SensorHdfOperator4km.ZHDFReadFY4AAGRI4KMFileInfo(strInputFileName,InputL1FileInfo) < 0 ) 
   //{
    //  ZLDSWriteLog('W',"Read InputL1FileInfo error!");
    //  printf("Read InputL1FileInfo error !\n");
    //  return ZEREADOFFIRE;
   //}
   m_startLine = HDFFileAtt->wBeginLineNumber;  //起始行号
   m_endLine = HDFFileAtt->wEndLineNumber;      //结束行号
   m_startPixel = HDFFileAtt->wBeginPixelNumber;//起始象元号
   m_endPixel = HDFFileAtt->wEndPixelNumber;    //结束象元号
   
   printf("------startL endL startP endP %d %d %d %d--------\n",m_startLine,m_endLine,m_startPixel,m_endPixel);  
   printf("------2km input hei %d and wid %d  ** static_IJtolatlonfile hei  %d  wid  %d--------\n",m_iHeight,m_iWidth,m_iheightOfstatic,m_iwidthOfstatic);
    
    if((m_endLine-m_startLine+1 != m_iHeight)  || (m_endPixel-m_startPixel+1 !=m_iWidth))
    {  
        ZLDSWriteLog('W',"startLine endline endPixel startPixel  error!");
        printf("startLine endline endPixel startPixel  error !\n");
        return ZEREADOFFIRE;
    }
  
  if(GetLonLatData() != 0)
  {
    ZLDSWriteLog('W',"Get Lon and lat Data error!");
    printf("Get Lon and lat Data error!\n");
    return ZEREADOFFIRE;
  }
  if(GetAngleData() != 0)
  {
    ZLDSWriteLog('W',"Get Angle Data error!");
    printf("Get Angle Data error!\n");
    return ZEREADOFFIRE;
  }  
  if(GetSurfaceTypeData() != 0)
  {
    ZLDSWriteLog('W',"Get Surface Type Data error!");
    printf("Get Surface Type Data error!\n");
    return ZEREADOFFIRE;
  }    
  if(GetQualityFlagData() != 0)
  {
    ZLDSWriteLog('W',"Get Quality Flag Data error!");
    printf("Get Quality Flag Data error!\n");
    return ZEREADOFFIRE;
  }  
  
  if(GetChannelData() != 0)
  {
    ZLDSWriteLog('W',"Get Channel Data error!");
    printf("Get Channel Data error!\n");
    return ZEREADOFFIRE;
  }  
  GetCLMData();
  return 0;    
}

/**  @brief   python绘图操作 \n
*
*    @remark   python绘图操作\n
* 
*    @param  - char* Param,//调度控制参数       
*    @return   0-成功，其他失败
*/
int INIOperation::JPGFromPython(char* Param)
{
  char command_FHS[256];
   char command_FHSD[1024];
    char command_FHSN[1024];
      int status = 0;
  memset(command_FHS,0,sizeof(command_FHS));
   memset(command_FHSD,0,sizeof(command_FHSD));
   memset(command_FHSN,0,sizeof(command_FHSN));
    //sprintf(command,"%s %s %s %s %s %s",m_strpython,m_commandexe,m_strpgs,Param,strOutputNCFileName,strOutputIMGPath);
  printf("strOutputFDJPGFileName %s\n",strOutputFDJPGFileName);
  printf("strOutputFNJPGFileName %s\n",strOutputFNJPGFileName); 
  if(strcmp(sataName,"FY4A")==0)
  {
     sprintf(command_FHS,"%s %s %s %s","python /PGSWORK/FY4A/EXE/IMAGE/FY4ProductDrawer.py PGS",Param,strOutputNCFileName,strOutputIMGPath);
     status = system(command_FHS);
      printf("command_FHS:%s\n",command_FHS);
    if(access(strOutputFDJPGFileName,0) == 0)
    	{
    		sprintf(command_FHSD,"%s %s %s","python /PGSWORK/FY4A/EXE/IMAGE/PGS_FHS_text.py",strOutputFDJPGFileName,strOutputFDJPGFileName);
    	   status = system(command_FHSD);
    	   printf("command_FHSD:%s\n",command_FHSD);
    	   
    	}
      if(access(strOutputFNJPGFileName,0) == 0)
      	{
      		sprintf(command_FHSN,"%s %s %s","python /PGSWORK/FY4A/EXE/IMAGE/PGS_FHS_text.py",strOutputFNJPGFileName,strOutputFNJPGFileName);
      	 status = system(command_FHSN);
      	  printf("command_FHSN:%s\n",command_FHSN);
      	}     
  }
  else
  {
     sprintf(command_FHS,"%s %s %s %s","python /PGSWORK/AHI8/EXE/IMAGE/FY4ProductDrawer.py PGS",Param,strOutputNCFileName,strOutputIMGPath);
     status = system(command_FHS);
  }
    printf("------strOutputFDJPGFileName %s\n",strOutputFDJPGFileName); 
  return status;  
}
/**  @brief 获取文件名称 \n
* 
*    @param  - char *param1,//调度控制参数          
*    @param  - char *param1,//全圆盘产品+星下点坐标
*    @param  - char *startDate,//开始时间          
*    @param  - char *endDate,//结束时间         
*    @return   0-成功，1~255 -失败
*/
void INIOperation::parseFileName(char* param1,char* param2,char* startDate,char* endDate)
{
  //in out parse
  FileStructlist fileStructList;
  char okFileName[256];
  memset(&fileStructList,0,sizeof(FileStructlist));
  memset(okFileName,0,sizeof(okFileName));
  parseSHM(param1,param2,&fileStructList);
  //in
  fileconnect_name(strInputFileName,startDate,endDate,&fileStructList);
  fileconnect_name(strInputFileName4000,startDate,endDate,&fileStructList);
  fileconnect_name(strInputFileNameGEO,startDate,endDate,&fileStructList);
  fileconnect_name(strInputCLMFileName,startDate,endDate,&fileStructList);
  //out
  fileconnect_name(strOutputNCFileName,startDate,endDate,&fileStructList);  
  fileconnect_name(strOutputHDFFileName,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputFIMGFileName,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputPIMGFileName,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputFDJPGFileName,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputFNJPGFileName,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputTable1FileName,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputTable2FileName,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputTable3FileName,startDate,endDate,&fileStructList);  
  fileconnect_name(strOutputTable1FileNameNC,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputTable2FileNameNC,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputTable3FileNameNC,startDate,endDate,&fileStructList); 
   fileconnect_name(strOutputEnFPAFileName,startDate,endDate,&fileStructList);
  fileconnect_name(strOutputEnFPTFileName,startDate,endDate,&fileStructList);       
  fileconnect_name(strTXTConfigPath,startDate,endDate,&fileStructList);  

    

                                                                                                                                                                                                                                                                                                                                                                                                
  sprintf(okFileName,"%s_%s_%s_%s_%s_%s.OK",fileStructList.filHedarr,fileStructList.satIDarr,
                                                                             fileStructList.insIDarr,fileStructList.taskIDarr,
                                                                              fileStructList.subtaskTYPEarr,fileStructList.subtaskNUMarr);  
                                                                            
  strcat(strOutputFileNameOK,okFileName);  

//  printf("2km:",strInputFileName);   
//  printf("4km:",strInputFileName4000);       
//  printf("clm:",strInputCLMFileName);   
//  printf("output:%s\n",strOutputHDFFileName);
  printf("OK:%s\n",strOutputFileNameOK);   

}
/**  @brief 写输入输出文件 \n     
*    @return   0-成功，1~255 -失败
*/
int INIOperation::writeConfigFile()
{
  printf("config:%s\n",strTXTConfigPath);
  FILE* fp = NULL;
  fp = fopen(strTXTConfigPath,"w");
  if(fp == NULL)
  {
    ZLDSWriteLog('W',"Open Config error!");
    exit(ZECONFIGOPEN);
  }
  
  fprintf(fp,"[InOutPutFileName]\n");
  fprintf(fp,"InputFileName=%s\n",strInputFileName);
  fprintf(fp,"InputCLMFileName=%s\n",strInputCLMFileName);
  fprintf(fp,"Input4KMFileName=%s\n",strInputFileName4000);
  fprintf(fp,"OutputFileName_HDF=%s\n",strOutputHDFFileName);
  fprintf(fp,"OutputFileName_FIMG=%s\n",strOutputFIMGFileName);
  fprintf(fp,"OutputFileName_PIMG=%s\n",strOutputPIMGFileName);
   fprintf(fp,"OutputFileName_FDJPG=%s\n",strOutputFDJPGFileName);
    fprintf(fp,"OutputFileName_FNJPG=%s\n",strOutputFNJPGFileName);
  fprintf(fp,"OutputFileName_Table1=%s\n",strOutputTable1FileName);
  fprintf(fp,"OutputFileName_Table2=%s\n",strOutputTable2FileName);
  fprintf(fp,"OutputFileName_Table3=%s\n",strOutputTable3FileName);
  fprintf(fp,"OKOutputFileName=%s\n",strOutputFileNameOK);
  fprintf(fp,"[ConfigFileName]\n");
  fprintf(fp,"IJToLonLatFileName=%s\n",strIJToLonLatFileName);
  fprintf(fp,"SurfaceTypeName=%s\n",strSurfaceTypeFileName);
  //fprintf(fp,"SatZenAngleName=%s\n",strSatZenFileName);
  fprintf(fp,"nLandingName=%s\n",strnLandingFileName);
  fprintf(fp,"xjrasterName=%s\n",strxjrasterFileName);
  fprintf(fp,"xjdh_newName=%s\n",strxjdh_newFileName);
  fprintf(fp,"LandCoverName=%s\n",strLandCoverFileName);
  fprintf(fp,"LandImgName=%s\n",strLandImgFileName);
  fprintf(fp,"SFTName=%s\n",strSFTFileName);
  fclose(fp);
  return 0;
}

/**  @brief 拆分调度令参数 \n     
*/
void INIOperation::parseSHM(char* strFilename,char* rangename,FileStructlist *FilAna)
{
  strncpy(FilAna->filHedarr, strFilename, 12);
  printf("%s\n",FilAna->filHedarr);
  
  strncpy(FilAna->satIDarr, strFilename + 13, 5);
  printf("%s\n",FilAna->satIDarr);

  strncpy(FilAna->insIDarr, strFilename + 19, 6);
  printf("%s\n",FilAna->insIDarr);
  
  strncpy(FilAna->taskIDarr, strFilename + 26, 17);
  printf("%s\n",FilAna->taskIDarr);
  
  strncpy(FilAna->subtaskTYPEarr, strFilename + 44, 4);
  printf("%s\n",FilAna->subtaskTYPEarr);
  
  strncpy(FilAna->subtaskNUMarr, strFilename + 49, 3);
  printf("%s\n",FilAna->subtaskNUMarr);
  
//  strncpy(FilAna->beginYMDarr, strFilename + 53, 8);
//  printf("%s\n",FilAna->beginYMDarr);
//  
//  strncpy(FilAna->beginHMSarr, strFilename + 62, 6);
//  printf("%s\n",FilAna->beginHMSarr);
  
  strncpy(FilAna->RDarr, rangename, 1);
  printf("%s\n",FilAna->RDarr);
  
  strncpy(FilAna->diskarr, rangename + 2, 4);
  printf("%s\n",FilAna->diskarr);
  
  strncpy(FilAna->rangearr, rangename + 7, 5);
  printf("%s\n",FilAna->rangearr);
}

/**  @brief 写OK文件 \n     
*    @return   0-成功，1~255 -失败
*/
int INIOperation::WriteOK()
{
  struct stat stBuf;
  struct tm  structSysTm;
  char strtime[15];  
  
  FILE* fp = NULL;    
  fp = fopen(strOutputFileNameOK,"w");
  if(fp == NULL )
  {
    ZLDSWriteLog('W',"Open OKFile error!");
    exit(ZEOKOPEN);
  }
  stat(strOutputNCFileName,&stBuf);

  memset(strtime,0,sizeof(strtime));
  memcpy(&structSysTm,gmtime(&stBuf.st_mtime),sizeof(struct tm));
  sprintf(strtime,"%04d%02d%02d%02d%02d%02d",structSysTm.tm_year+1900,structSysTm.tm_mon+1,structSysTm.tm_mday,
                                             structSysTm.tm_hour,structSysTm.tm_min,structSysTm.tm_sec);
  
  fprintf(fp,"%s\n","DataType     Completefilename     QC     Createfiletime     filesize");
  fprintf(fp,"     %s","FHS-");
  fprintf(fp,"     %s",strOutputNCFileName);
  fprintf(fp,"     %d",DataQ);
  fprintf(fp,"     %s",strtime);
  fprintf(fp,"     %ld\n",stBuf.st_size);
  fclose(fp);  
  return 0;
}

/**  @brief 拼接文件名称 \n     
*/
void INIOperation::fileconnect_name(char*filename,char*starttime,char*endtime,FileStructlist* FilAna)
{
  char ObserType[4];
  sprintf(ObserType,"_%s_",FilAna->RDarr);
  memcpy(strstr(filename,"SATID"),FilAna->satIDarr,5);
  memcpy(strstr(filename,"INSTRU"),FilAna->insIDarr,6);  
  memcpy(strstr(filename,"_T_"),ObserType,3);  
  memcpy(strstr(filename,"OBST"),FilAna->diskarr,4);  
  memcpy(strstr(filename,"SUBPO"),FilAna->rangearr,5);
  memcpy(strstr(filename,"YYYYMMDDHHMMSS"),starttime,14);
  memcpy(strstr(filename,"yyyymmddhhmmss"),endtime,14);
  
  memset(diskarr,0,sizeof(diskarr));
  memcpy(diskarr,FilAna->diskarr,4);
}

///获取数据集的大小
int INIOperation::GetWidHei(char* filename,char* datasetname,int* wid ,int* hei)
{
  hid_t       file_id,space, dset; 
  hsize_t     dims[2] = {0, 0};
  int         ndims = 0;
  file_id = H5Fopen(filename, H5F_ACC_RDWR, H5P_DEFAULT);
  if(file_id < 0)
  {
    printf("open %s error!\n",filename);
    return ZEGETWIDHEIOFFHS;
  }  

  dset = H5Dopen(file_id, datasetname, H5P_DEFAULT);
  if(dset < 0)
  {
    printf("open %s error!\n",datasetname);
    return ZEGETWIDHEIOFFHS;
  }

  space = H5Dget_space (dset);
  ndims = H5Sget_simple_extent_dims(space, dims, NULL);
  if(ndims < 0 || dims[0] <= 0 || dims[1] <= 0)
  {
    printf("Get DataSet width and height Error!\n");
    return ZEGETWIDHEIOFFHS;
  }
  H5Fclose(file_id);
  
  *wid  = dims[1];
  *hei  = dims[0];
  
  printf("width:%d height:%d\n",*wid,*hei);    
  return 0;
}
/**  @brief 获取云检测数据 \n     
*    @return   0-成功，1~255 -失败
*/
int INIOperation::GetCLMData()
{
  ///获取云检测数据（如果存在4km）
  if( access(strInputCLMFileName,0)== 0)
  {
    printf("%s is exit.\n read CLM Data!\n",strInputCLMFileName);    
    unsigned char *clmTemp = new unsigned char[m_widlength * m_heilength];
    char iValidRange[2];
    char longname[256];
    char standname[256];
    char type[16];
    char unit[16];
    char resolution[16];    
    char Coordinates[256];
    char ancillary[256];
    char sDescription[100];  
    char FillValue=0;
    char fScaleFactor=0;
    char fAddOffset=0;
    memset(iValidRange,0,sizeof(iValidRange));
    memset(longname,0,sizeof(longname));
    memset(standname,0,sizeof(standname));
    memset(type,0,sizeof(type));
    memset(unit,0,sizeof(unit));
    memset(resolution,0,sizeof(resolution));
    memset(Coordinates,0,sizeof(Coordinates));
    memset(ancillary,0,sizeof(ancillary));
    GOESRNetCDFOperator myNetCDF;
    if(myNetCDF.ZNetCDFReadSDSComData(strInputCLMFileName, "CLM",clmTemp,  longname, standname,type,&FillValue, iValidRange,
                                       &fScaleFactor, &fAddOffset,unit, resolution,Coordinates,ancillary,sDescription) < 0 )  
    {
      printf("get CLM error!\n");
      ZLDSWriteLog('W',"Get CLM error!");
      return ZEREADCLMOFFIRE;
    }  
    for (int i = 0; i < m_heilength; i++)
    {
      for (int j = 0; j < m_widlength; j++)
      {
        m_ucCloud[i*2][j*2] = clmTemp[i * m_widlength+ j] ;
        m_ucCloud[i*2][j*2 +1] = clmTemp[i * m_widlength + j] ;
        m_ucCloud[i*2 + 1][j*2] = clmTemp[i * m_widlength + j] ;
        m_ucCloud[i*2 + 1][j*2 + 1] = clmTemp[i * m_widlength+ j] ;
      }
    }
    delete[] clmTemp;
    clmTemp = NULL;
  }
  else
  {
    printf("%s is not exit.\n ",strInputCLMFileName);
  }
  return 0;
}
/**  @brief 获取经纬度数据 \n     
*    @return   0-成功，1~255 -失败
*/
int INIOperation::GetLonLatData()
{
  ///获取经纬度数据
  char strDataUnit[256];
  char strLongName[256];
  char strBandName[256];
  float fValidRange[2];  
  float fFillValue = 0;
  float fIntercept=0;
  float fslope = 0;
  memset(strDataUnit,0,sizeof(strDataUnit));
  memset(strLongName,0,sizeof(strLongName));
  memset(strBandName,0,sizeof(strBandName));
  memset(fValidRange,0,sizeof(fValidRange));

  float *flatTemp = new float [m_iwidthOfstatic * m_iheightOfstatic];
  float *flonTemp = new float [m_iwidthOfstatic * m_iheightOfstatic];  
  unsigned short *tmpSFTMask = new unsigned short[m_iwidthOfstatic*m_iheightOfstatic];
  if(m_SensorHdfOperator.ZHDFReadSDSData(strIJToLonLatFileName,HDFROOT,"Lat",flatTemp,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    ZLDSWriteLog('W',"Get Lat error!");
    printf("get Lat error!\n");
    return ZEREADOFFIRE;
  }      
  if(m_SensorHdfOperator.ZHDFReadSDSData(strIJToLonLatFileName,HDFROOT,"Lon",flonTemp,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    ZLDSWriteLog('W',"Get Lon error!");
    printf("get Lon error!\n");
    return ZEREADOFFIRE;
  }  
  //读取sft
  if(m_SensorHdfOperator.ZHDFReadSDSData(strSFTFileName,HDFROOT,"FillValue",tmpSFTMask,HDFUSHORT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    ZLDSWriteLog('W',"Read ZESFTMASK error!");
    printf("Read SFTMASK error !\n");
    return ZESFTFIRE;
  }
  //m_iWidth
  /*for (int i = 0; i < m_iheightOfstatic; i++)
  {
    for (int j = 0; j < m_iwidthOfstatic; j++)
    {
      m_fLong[i][j] = flonTemp[i * m_iwidthOfstatic + j];
      m_fLat[i][j] = flatTemp[i * m_iwidthOfstatic + j];
    }
  }*/
  //从圆盘中取数据
  for(int il=m_startLine;il<= m_endLine;il++)
  {
    for(int ip=m_startPixel;ip<= m_endPixel;ip++)
    {
      m_fLong[il-m_startLine][ip-m_startPixel] = flonTemp[(il)*m_iwidthOfstatic + ip];
      pSFTMask[il-m_startLine][ip-m_startPixel] = tmpSFTMask[(il)*m_iwidthOfstatic + ip];
      
      m_fLat[il-m_startLine][ip-m_startPixel] = flatTemp[(il)*m_iwidthOfstatic + ip];
  //  printf("m_fLong %f m_fLat %f \n",m_fLong[il-m_startLine][ip-m_startPixel],m_fLat[il-m_startLine][ip-m_startPixel]);
    }
    
  }
  
  
  delete[] flatTemp;
  flatTemp = NULL;
  delete[] flonTemp;
  flonTemp = NULL;  
  
  delete[] tmpSFTMask;
  tmpSFTMask = NULL;
  return 0;
}
/**  @brief 获取角度数据 \n     
*    @return   0-成功，1~255 -失败
*/
int INIOperation::GetAngleData()
{
  float *fSatZenTemp = new float [m_widlength * m_heilength];
  memset(fSatZenTemp,0,sizeof(float)*m_widlength * m_heilength);
  float *fSunZenTemp = new float [m_widlength * m_heilength];
  memset(fSunZenTemp,0,sizeof(float)*m_widlength * m_heilength);
  float *fSunGlintTemp = new float [m_widlength * m_heilength];
  memset(fSunGlintTemp,0,sizeof(float)*m_widlength * m_heilength);  
  char strDataUnit[256];
  char strLongName[256];
  char strBandName[256];
  float fValidRange[2];  
  float fFillValue = 0;
  float fIntercept=0;
  float fslope = 0;
  memset(strDataUnit,0,sizeof(strDataUnit));
  memset(strLongName,0,sizeof(strLongName));
  memset(strBandName,0,sizeof(strBandName));
  memset(fValidRange,0,sizeof(fValidRange));  
  if(m_SensorHdfOperator4km.ZHDFReadSDSData(strInputFileNameGEO,HDFROOT,"NOMSatelliteZenith",fSatZenTemp,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    ZLDSWriteLog('W',"Read SatZA error!");
    printf("Read sat ZA error !\n");
    return ZEREADOFFIRE;
  }  
  if(m_SensorHdfOperator4km.ZHDFReadSDSData(strInputFileNameGEO,HDFROOT,"NOMSunZenith",fSunZenTemp,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get NOMSunZenith error!\n");
    ZLDSWriteLog('W',"Get NOMSunZenith error!");
    return ZEREADOFFIRE;
  }  
  if(m_SensorHdfOperator4km.ZHDFReadSDSData(strInputFileNameGEO,HDFROOT,"NOMSunGlintAngle",fSunGlintTemp,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get NOMSunGlintAngle error!\n");
    ZLDSWriteLog('W',"Get NOMSunGlintAngle error!");
    return ZEREADOFFIRE;
  }        
  for (int i = 0; i < m_heilength; i++) 
  {
    for (int j = 0; j < m_widlength; j++)
    {
      m_fPixelSatelliteZenithAnale[i*2][j*2] = (float)fSatZenTemp[i * m_widlength+ j] ;
      m_fPixelSatelliteZenithAnale[i*2][j*2 +1] = (float)fSatZenTemp[i * m_widlength + j] ;
      m_fPixelSatelliteZenithAnale[i*2 + 1][j*2] = (float)fSatZenTemp[i * m_widlength + j] ;
      m_fPixelSatelliteZenithAnale[i*2 + 1][j*2 + 1] = (float)fSatZenTemp[i * m_widlength+ j] ;
      m_fSunZenithAnale[i*2][j*2] = (float)fSunZenTemp[i * m_widlength+ j] ;
      m_fSunZenithAnale[i*2][j*2 +1] = (float)fSunZenTemp[i * m_widlength + j] ;
      m_fSunZenithAnale[i*2 + 1][j*2] = (float)fSunZenTemp[i * m_widlength + j] ;
      m_fSunZenithAnale[i*2 + 1][j*2 + 1] = (float)fSunZenTemp[i * m_widlength+ j] ;
      m_fSunGlitAngle[i*2][j*2] = (float)fSunGlintTemp[i * m_widlength + j] ;
      m_fSunGlitAngle[i*2][j*2 + 1] = (float)fSunGlintTemp[i * m_widlength + j] ;
      m_fSunGlitAngle[i*2 + 1][j*2] = (float)fSunGlintTemp[i * m_widlength + j] ;
      m_fSunGlitAngle[i*2 + 1][j*2 + 1] = (float)fSunGlintTemp[i * m_widlength + j] ;      
    }
  }  
  delete []fSatZenTemp;
  fSatZenTemp = NULL;
  delete []fSunZenTemp;
  fSunZenTemp = NULL;
  delete []fSunGlintTemp;
  fSunGlintTemp = NULL;    
  return 0;
}
/**  @brief 获取地表类型数据 \n     
*    @return   0-成功，1~255 -失败
*/
int INIOperation::GetSurfaceTypeData()
{
  char *cbuff = new char [m_iwidthOfstatic * m_iheightOfstatic];    
  memset(cbuff,0,sizeof(char)*m_iwidthOfstatic * m_iheightOfstatic);
//  int recode = 0;
//  FILE *fpSurType=NULL;
//  fpSurType = fopen(strSurfaceTypeFileName,"rb");
//  if(fpSurType == NULL)
//  {
//    ZLDSWriteLog('W',"Open surface type error!");
//    printf("open surface type error!\n");
//    delete [] cbuff;
//    cbuff = NULL;  
//    return ZEOPEN;
//  }
//  recode = fread(cbuff,sizeof(char),m_iwidthOfstatic * m_iheightOfstatic,fpSurType);
//  if(recode != m_iwidthOfstatic * m_iheightOfstatic)
//  {
//    ZLDSWriteLog('W',"Read surface type error!");
//    printf("Read surface type error!\n");
//    delete [] cbuff;
//    cbuff = NULL;  
//    return ZEREADOFFIRE;
//  }
//  fclose(fpSurType);
//  if(recode > 0)
//  {
//    cout << "read surface type success!" << endl;
//    for (int i = 0; i < m_iheightOfstatic; i++)
//      for (int j = 0; j < m_iwidthOfstatic; j++)
//      {        
//        m_cPixelSurfaceType[i][j] = cbuff[i * m_iwidthOfstatic + j];
//      }    
//  }
//  else
//  {
//    printf("read surface type error!\n");
//    ZLDSWriteLog('W',"Read surface type error!");
//    delete [] cbuff;
//    cbuff = NULL;  
//    return ZEREADOFFIRE;
//  }
    hid_t       file_id, dataset_id;  
    herr_t      status;

    file_id = H5Fopen(strSurfaceTypeFileName, H5F_ACC_RDWR, H5P_DEFAULT);
    dataset_id = H5Dopen(file_id, "LandCover",H5P_DEFAULT);  
    status = H5Dread(dataset_id, H5T_NATIVE_UCHAR, H5S_ALL, H5S_ALL, H5P_DEFAULT,cbuff);
    if(status < 0)
    {
      printf("read surface type error!\n");
      ZLDSWriteLog('W',"Read surface type error!");
      delete [] cbuff;
      cbuff = NULL;  
      return ZEREADOFFIRE;
    }
    else
    {
      cout << "read surface type success!\n" << endl;
      /*for (int i = 0; i < m_iheightOfstatic; i++)
        for (int j = 0; j < m_iwidthOfstatic; j++)
        {        
          m_cPixelSurfaceType[i][j] = cbuff[i * m_iwidthOfstatic + j];
        }  */  
        
        
        //从圆盘中取数据
          for(int il=m_startLine;il<m_endLine;il++)
          {
            for(int ip=m_startPixel;ip<m_endPixel;ip++)
            {
              m_cPixelSurfaceType[il-m_startLine][ip-m_startPixel] = cbuff[(il)*m_iwidthOfstatic + ip];
            }
          }    
}
    status = H5Dclose(dataset_id);    
    status = H5Fclose(file_id);
  delete [] cbuff;
  cbuff = NULL;    
  return 0;      
}
/**  @brief 获取2km、4km通道数据 \n     
*    @return   0-成功，1~255 -失败
*/
int INIOperation::GetChannelData()
{
  //read channel data
  const int CalLength = 65536;
  unsigned short* usChannel2 = new unsigned short[m_iWidth * m_iHeight];
  unsigned short* usChannel3 = new unsigned short[m_iWidth * m_iHeight];
  /*sunr_NEW 20170914********************************************/
  unsigned short* usChannel5 = new unsigned short[m_iWidth * m_iHeight];
  unsigned short* usChannel6 = new unsigned short[m_iWidth * m_iHeight];
  /***************************************************************/
  unsigned short* usChannel7 = new unsigned short[m_iWidth * m_iHeight];  
  float* pCal2 = new float[CalLength];
  float* pCal3 = new float[CalLength];
  /*sunr_NEW 20170914********************************************/
  float* pCal5 = new float[CalLength];
  float* pCal6 = new float[CalLength];
  /***************************************************************/
  float* pCal7 = new float[CalLength] ;  
  
  char strDataUnit[256];
  char strLongName[256];
  char strBandName[256];
  float fValidRange[2];  
  float fFillValue = 0;
  float fIntercept=0;
  float fslope = 0;
  memset(strDataUnit,0,sizeof(strDataUnit));
  memset(strLongName,0,sizeof(strLongName));
  memset(strBandName,0,sizeof(strBandName));
  memset(fValidRange,0,sizeof(fValidRange));  

  memset(usChannel2,0,sizeof(usChannel2));
  memset(usChannel3,0,sizeof(usChannel3));
  /*sunr_NEW 20170914********************************************/
  memset(usChannel5,0,sizeof(usChannel5));
  memset(usChannel6,0,sizeof(usChannel6));
  /***************************************************************/
  memset(usChannel7,0,sizeof(usChannel7));
  memset(pCal2,0,sizeof(pCal2));
  memset(pCal3,0,sizeof(pCal3));
  /*sunr_NEW 20170914********************************************/
  memset(pCal5,0,sizeof(pCal5));
  memset(pCal6,0,sizeof(pCal6));
  /***************************************************************/
  memset(pCal7,0,sizeof(pCal7));  
///获取2km分辨率的0.65um数据
  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"CALChannel02",pCal2,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get CALChannel02 error!\n");
    ZLDSWriteLog('W',"Get CALChannel03 error!");
    return ZEREADOFFIRE;
  }  
///获取2km分辨率的0.83um数据  
  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"CALChannel03",pCal3,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    ZLDSWriteLog('W',"Get CALChannel03 error!");
    printf("get CALChannel05 error!\n");
    return ZEREADOFFIRE;
  }  

/*sunr_NEW 20170914***********获取1.6um 数据*********************************/
if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"CALChannel05",pCal5,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    ZLDSWriteLog('W',"Get CALChannel05 error!");
    printf("get CALChannel05 error!\n");
    return ZEREADOFFIRE;
  }  
/*sunr_NEW 20170914***********获取2.3um 数据*********************************/  
if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"CALChannel06",pCal6,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    ZLDSWriteLog('W',"Get CALChannel06 error!");
    printf("get CALChannel06 error!\n");
    return ZEREADOFFIRE;
  }  
///获取2km分辨率的3.9um数据  
  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"CALChannel07",pCal7,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get CALChannel07 error!\n");
    ZLDSWriteLog('W',"Get CALChannel07 error!");
    return ZEREADOFFIRE;
  }    
  
  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"NOMChannel02",usChannel2,HDFUSHORT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get NOMChannel03 error!\n");
    ZLDSWriteLog('W',"Get NOMChannel03 error!");
    return ZEREADOFFIRE;
  }  
  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"NOMChannel03",usChannel3,HDFUSHORT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get NOMChannel05 error!\n");
    ZLDSWriteLog('W',"Get NOMChannel05 error!");
    return ZEREADOFFIRE;
  }  
  
  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"NOMChannel05",usChannel5,HDFUSHORT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get NOMChannel05 error!\n");
    ZLDSWriteLog('W',"Get NOMChannel05 error!");
    return ZEREADOFFIRE;
  }  
  
  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"NOMChannel06",usChannel6,HDFUSHORT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get NOMChannel06 error!\n");
    ZLDSWriteLog('W',"Get NOMChannel06 error!");
    return ZEREADOFFIRE;
  }  
  

  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName,HDFROOT,"NOMChannel07",usChannel7,HDFUSHORT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get NOMChannel07 error!\n");
    ZLDSWriteLog('W',"Get NOMChannel07 error!");
    return ZEREADOFFIRE;
  }        
  for (int i = 0; i < m_iHeight; i++)
  {  // 转区域 可见光放入通道5
    for (int j = 0; j < m_iWidth; j++)
    {
      if ( usChannel7[i * m_iWidth  + j] < 65534 )
      {
          m_wChannelsData[4][i * m_iWidth + j] = (unsigned short)((pCal7[usChannel7[i * m_iWidth  + j]] ) * 10);
        if(usChannel7[i * m_iWidth  + j] >= 63750 && usChannel7[i * m_iWidth  + j] <= 64040 )//20171031
   	   {
   		   m_wChannelsData[4][i * m_iWidth + j] =  (unsigned short)(((290 -(((usChannel7[i * m_iWidth  + j] - 63750)/(64040.0-63750.0))*(290.0-260.0)))+0.05)*10.0);//20171123   	  
   	    }  
      }
      else 
       {
           m_wChannelsData[4][i * m_iWidth + j] = 65534;
       }
       //对7通道亮温值修正
     
     if(usChannel2[i * m_iWidth  + j] < 4096)
     	{
     		m_wChannelsData[5][i * m_iWidth + j] = (unsigned short)((pCal2[usChannel2[i * m_iWidth  + j]] + 0.0005) * 1000);
     	}
     	else
     	{
     			 m_wChannelsData[5][i * m_iWidth + j] = 65534;
     	}
     if(usChannel3[i * m_iWidth  + j] < 4096)
     		{
     			 m_wChannelsData[3][i * m_iWidth + j] = (unsigned short)((pCal3[usChannel3[i * m_iWidth  + j]] + 0.0005) * 1000);  
     		}
     		else
     		{
     				m_wChannelsData[3][i * m_iWidth + j] = 65534;
     		}
     	if(usChannel5[i * m_iWidth  + j] < 4096)
     		{
     		m_wChannelsData[6][i * m_iWidth + j] = (unsigned short)((pCal5[usChannel5[i * m_iWidth  + j]] + 0.0005) * 1000);	
     		}
     		else
     			{
     				m_wChannelsData[6][i * m_iWidth + j] = 65534;
     			}	
     		if(usChannel6[i * m_iWidth  + j] < 4096)
     			{
     				m_wChannelsData[7][i * m_iWidth + j] = (unsigned short)((pCal6[usChannel6[i * m_iWidth  + j]] + 0.0005) * 1000);
     			}
     else
     	{
     	 m_wChannelsData[7][i * m_iWidth + j] = 65534	;
     	}
//      if(usChannel2[i * m_iWidth  + j] < 4096 || usChannel3[i * m_iWidth  + j] < 4096 || 
//           usChannel5[i * m_iWidth  + j] < 4096 ||
//        usChannel6[i * m_iWidth  + j] < 4096 )
//      {
//           m_wChannelsData[5][i * m_iWidth + j] = (unsigned short)((pCal2[usChannel2[i * m_iWidth  + j]] + 0.0005) * 1000);
//          m_wChannelsData[3][i * m_iWidth + j] = (unsigned short)((pCal3[usChannel3[i * m_iWidth  + j]] + 0.0005) * 1000);  
//          m_wChannelsData[6][i * m_iWidth + j] = (unsigned short)((pCal5[usChannel5[i * m_iWidth  + j]] + 0.0005) * 1000);
//          m_wChannelsData[7][i * m_iWidth + j] = (unsigned short)((pCal6[usChannel6[i * m_iWidth  + j]] + 0.0005) * 1000);                  
//       }     
      
//      else
//      {
//           //by liup 增加对圆盘数据容错
//          m_wChannelsData[5][i * m_iWidth + j] = 65534;
//          m_wChannelsData[3][i * m_iWidth + j] = 65534; 
//          m_wChannelsData[6][i * m_iWidth + j] = 65534;
//          m_wChannelsData[7][i * m_iWidth + j] = 65534;
//      }
//      if(m_wChannelsData[3][i * m_iWidth + j] == 65535)
//      {
//      	printf("m_wChannelsData[3] %d i %d j %d\n",m_wChannelsData[3][i * m_iWidth + j],i,j);
//      }
//      if(i == 711 && j == 664)
//      	{
//      		printf("m_wChannelsData[3] %d i %d j %d\n",m_wChannelsData[3][i * m_iWidth + j],i,j);
//      		getchar();
//      	}
  
    }
  }
  delete[] usChannel2;
  usChannel2 = NULL;
  delete[] usChannel3;
  usChannel3 = NULL;
  delete[] usChannel7;
  usChannel7 = NULL;
  delete[] pCal2;
  pCal2 = NULL;
  delete[] pCal3;
  pCal3 = NULL;
  delete[] pCal7;
  pCal7 = NULL;  
  delete[] usChannel5;
  usChannel5 = NULL;
  delete[] usChannel6;
  usChannel6 = NULL;
  delete[] pCal5;
  pCal5 = NULL;
  delete[] pCal6;
  pCal6 = NULL;
  
  unsigned short* usChannel12 = new unsigned short[m_widlength * m_heilength];
  unsigned short* usChannel13 = new unsigned short[m_widlength * m_heilength];  
  float* pCal12 = new float[CalLength];
  float* pCal13 = new float[CalLength];
///获取4km分辨率的10.8um数据  
  if(m_SensorHdfOperator4km.ZHDFReadSDSData(strInputFileName4000,HDFROOT,"CALChannel12",pCal12,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get CALChannel12 error!\n");
    ZLDSWriteLog('W',"Get CALChannel12 error!");
    return ZEREADOFFIRE;
  }  
///获取4km分辨率的12um数据  
  if(m_SensorHdfOperator4km.ZHDFReadSDSData(strInputFileName4000,HDFROOT,"CALChannel13",pCal13,HDFFLOAT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get CALChannel13 error!\n");
    ZLDSWriteLog('W',"Get CALChannel13 error!");
    return ZEREADOFFIRE;
  }    
  if(m_SensorHdfOperator4km.ZHDFReadSDSData(strInputFileName4000,HDFROOT,"NOMChannel12",usChannel12,HDFUSHORT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get NOMChannel12 error!\n");
    ZLDSWriteLog('W',"Get NOMChannel12 error!");
    return ZEREADOFFIRE;
  }  
  if(m_SensorHdfOperator4km.ZHDFReadSDSData(strInputFileName4000,HDFROOT,"NOMChannel13",usChannel13,HDFUSHORT,strDataUnit, strLongName,strBandName,fValidRange, &fFillValue, &fIntercept,&fslope) < 0 ) 
  {
    printf("get NOMChannel13 error!\n");
    ZLDSWriteLog('W',"Get NOMChannel13 error!");
    return ZEREADOFFIRE;
  }  
///进行邻近点插值计算    
  for (int i = 0; i < m_heilength ; i++)  // 转区域   远红外2放入通道2
    for (int j = 0; j < m_widlength ; j++)
    {
      if(usChannel13[i * m_widlength  + j] < 60000 || usChannel12[i * m_widlength + j] < 60000)
      {
        m_wChannelsData[2][i * 2 * m_iWidth + j * 2] = (unsigned short)((pCal13[usChannel13[i * m_widlength + j]] + 0.05) * 10);
        m_wChannelsData[2][i * 2 * m_iWidth + j * 2 + 1] = (unsigned short)((pCal13[usChannel13[i * m_widlength  + j]] + 0.05) * 10);
        m_wChannelsData[2][(i * 2 + 1) * m_iWidth + j * 2 ] = (unsigned short)((pCal13[usChannel13[i * m_widlength + j]] + 0.05) * 10);
        m_wChannelsData[2][(i * 2 + 1) * m_iWidth + j * 2 + 1] = (unsigned short)((pCal13[usChannel13[i * m_widlength + j]] + 0.05) * 10);
        m_wChannelsData[1][i * 2 * m_iWidth + j * 2] = (unsigned short)((pCal12[usChannel12[i * m_widlength + j]] + 0.05) * 10);
        m_wChannelsData[1][i * 2 * m_iWidth + j * 2 + 1] = (unsigned short)((pCal12[usChannel12[i * m_widlength  + j]] + 0.05) * 10);
        m_wChannelsData[1][(i * 2 + 1) * m_iWidth + j * 2 ] = (unsigned short)((pCal12[usChannel12[i * m_widlength + j]] + 0.05) * 10);
        m_wChannelsData[1][(i * 2 + 1) * m_iWidth + j * 2 + 1] = (unsigned short)((pCal12[usChannel12[i * m_widlength + j]] + 0.05) * 10);
      }
      else
     {//by liup 增加对圆盘数据容错
        m_wChannelsData[2][i * 2 * m_iWidth + j * 2] = 65534;
        m_wChannelsData[2][i * 2 * m_iWidth + j * 2 + 1] = 65534;
        m_wChannelsData[2][(i * 2 + 1) * m_iWidth + j * 2 ] = 65534;
        m_wChannelsData[2][(i * 2 + 1) * m_iWidth + j * 2 + 1] = 65534;
        m_wChannelsData[1][i * 2 * m_iWidth + j * 2] = 65534;
        m_wChannelsData[1][i * 2 * m_iWidth + j * 2 + 1] = 65534;
        m_wChannelsData[1][(i * 2 + 1) * m_iWidth + j * 2 ] = 65534;
        m_wChannelsData[1][(i * 2 + 1) * m_iWidth + j * 2 + 1] = 65534;
      
     }
    }
  delete[] usChannel12;
  usChannel12 = NULL;
  delete[] usChannel13;
  usChannel13 = NULL;
  delete[] pCal12;
  pCal12 = NULL;
  delete[] pCal13;
  pCal13 = NULL;
  return 0;
}

int INIOperation::GetAttributeOf2kmFile()
{
  HDFFileAtt = new FY4A_AGRI_2K_FILE_INFO;
  memset(HDFFileAtt,0,sizeof(FY4A_AGRI_2K_FILE_INFO));
  hid_t  m_FileID;
  m_FileID = H5Fopen(strInputFileName, H5F_ACC_RDWR, H5P_DEFAULT);  
  if(m_FileID < 0)
  {
    ZLDSWriteLog('W',"Open 2km file error!");
    printf("Open 2km file error!\n");
    return ZEREAD2KMHDFATT;    
  }
  H5LTget_attribute_string(m_FileID, "/", "AdditionalAnnotation",  HDFFileAtt->strAdditionalAnotation);
  H5LTget_attribute_ushort(m_FileID, "/", "Begin Line Number",  &HDFFileAtt->wBeginLineNumber);
  H5LTget_attribute_ushort(m_FileID, "/", "Begin Pixel Number",  &HDFFileAtt->wBeginPixelNumber);
  H5LTget_attribute_string(m_FileID, "/", "Data Creating Date",  HDFFileAtt->strDateOfDataCreating);
  H5LTget_attribute_string(m_FileID, "/", "Data Creating Time",  HDFFileAtt->strTimeOfDataCreating);
  H5LTget_attribute_uchar(m_FileID, "/", "Data Quality",  &HDFFileAtt->cDataQuality);
  H5LTget_attribute_string(m_FileID, "/", "Dataset Name",  HDFFileAtt->strDatasetName);
  H5LTget_attribute_ushort(m_FileID, "/", "End Line Number",  &HDFFileAtt->wEndLineNumber);
  H5LTget_attribute_ushort(m_FileID, "/", "End Pixel Number",  &HDFFileAtt->wEndPixelNumber);
  H5LTget_attribute_string(m_FileID, "/", "File Alias Name",  HDFFileAtt->strFileAliasName);
  H5LTget_attribute_string(m_FileID, "/", "File Name",  HDFFileAtt->strFileName);
  H5LTget_attribute_int(m_FileID, "/", "Incomplete Scans",  &HDFFileAtt->iIncompleteScans);
  H5LTget_attribute_float(m_FileID, "/", "NOMCenterLat",  &HDFFileAtt->fNOMCenterLat);
  H5LTget_attribute_float(m_FileID, "/", "NOMCenterLon",  &HDFFileAtt->fNOMCenterLon);
  H5LTget_attribute_float(m_FileID, "/", "NOMSatHeight",  &HDFFileAtt->fNOMSatHeight);
  H5LTget_attribute_int(m_FileID, "/", "Number Of Scans",  &HDFFileAtt->iNumberOfScans);
  H5LTget_attribute_string(m_FileID, "/", "OBIType",  HDFFileAtt->strOBIType);
  H5LTget_attribute_string(m_FileID, "/", "Observing Beginning Date",  HDFFileAtt->strDateOfObservingBeginning);
  H5LTget_attribute_string(m_FileID, "/", "Observing Beginning Time",  HDFFileAtt->strTimeOfObservingBeginning);
  H5LTget_attribute_string(m_FileID, "/", "Observing Ending Date",  HDFFileAtt->strDateOfObservingEnding);
  H5LTget_attribute_string(m_FileID, "/", "Observing Ending Time",  HDFFileAtt->strTimeOfObservingEnding);
  H5LTget_attribute_string(m_FileID, "/", "ProducetName",  HDFFileAtt->sProductName);
  H5LTget_attribute_string(m_FileID, "/", "ProductID",  HDFFileAtt->sProductID);
  H5LTget_attribute_ushort(m_FileID, "/", "QA_Pixel_Flag",  &HDFFileAtt->wQAPixelFlag);
  H5LTget_attribute_uchar(m_FileID, "/", "QA_Scan_Flag",  &HDFFileAtt->cQAScanFlag);
  H5LTget_attribute_float(m_FileID, "/", "RegCenterLat",  &HDFFileAtt->fRegCenterLat);
  //H5LTget_attribute_float(m_FileID, "/", "RegCentralLon",  &HDFFileAtt->fRegCenterLon);
  H5LTget_attribute_float(m_FileID, "/", "RegCenterLon",  &HDFFileAtt->fRegCenterLon);
  H5LTget_attribute_float(m_FileID, "/", "RegLength",  &HDFFileAtt->fRegLength);
  H5LTget_attribute_float(m_FileID, "/", "RegWidth",  &HDFFileAtt->fRegWidth);
  H5LTget_attribute_string(m_FileID, "/", "Responser",  HDFFileAtt->strResponser);
  H5LTget_attribute_string(m_FileID, "/", "Satellite Name",  HDFFileAtt->strSatelliteName);
  H5LTget_attribute_string(m_FileID, "/", "Sensor Identification Code",  HDFFileAtt->strSensorIdentificationCode);
  H5LTget_attribute_string(m_FileID, "/", "Sensor Name",  HDFFileAtt->strSensorName);
  H5LTget_attribute_string(m_FileID, "/", "Software Revision Date",  HDFFileAtt->strDateOfSoftwareRevision);
  H5LTget_attribute_string(m_FileID, "/", "Version Of Software",  HDFFileAtt->strVersionOfSoftware);
  H5LTget_attribute_double(m_FileID, "/", "dEA",  &HDFFileAtt->dEA);
  H5LTget_attribute_double(m_FileID, "/", "dObRecFlat",  &HDFFileAtt->dObRecFlat);
  H5LTget_attribute_double(m_FileID, "/", "dSamplingAngle",  &HDFFileAtt->dSamplingAngle);
  H5LTget_attribute_double(m_FileID, "/", "dSteppingAngle",  &HDFFileAtt->dSteppingAngle);
  H5Fclose(m_FileID);  
  return 0;
}

int INIOperation::GetQualityFlagData()
{
  float L0QData[14];
  unsigned short POSQData[14];
  unsigned short CALQData[14];
  memset(L0QData,0,sizeof(L0QData));
  memset(POSQData,0,sizeof(POSQData));
  memset(CALQData,0,sizeof(CALQData));
  
  char strDataUnit[256];
  char strLongName[256];
  char strBandName[256];
  float fValidRange[2];  
  float fFillValue = 0;
  float fIntercept=0;
  float fslope = 0;
  memset(strDataUnit,0,sizeof(strDataUnit));
  memset(strLongName,0,sizeof(strLongName));
  memset(strBandName,0,sizeof(strBandName));
  memset(fValidRange,0,sizeof(fValidRange));  
  hid_t       file_id; 
	file_id = H5Fopen(strInputFileName, H5F_ACC_RDWR, H5P_DEFAULT);  
  if(file_id < 0)
  {
    ZLDSWriteLog('W',"Open 2km file error!");
    printf("Open 2km file error!\n");
    return ZEREAD2KMHDFATT;    
  }	
//  H5LTread_dataset_float(file_id,"L0QualityFlag",L0QData);
//  GetSDSInfo(file_id,"L0QualityFlag",&fIntercept,&fslope,strBandName,
//	                             strLongName,strDataUnit,fValidRange,&fFillValue);
//	  
//	  H5LTread_dataset(file_id,"PosQualityFlag",POSQData);
//	  GetSDSInfo(file_id,"L0QualityFlag",&fIntercept,&fslope,strBandName,
//	                             strLongName,strDataUnit,fValidRange,&fFillValue); 
//	  
//	  H5LTread_dataset(file_id,"CalQualityFlag",CALQData);  
//	  GetSDSInfo(file_id,"CalQualityFlag",&fIntercept,&fslope,strBandName,
//	                             strLongName,strDataUnit,fValidRange,&fFillValue);                          
//  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName, HDFROOT, "L0QualityFlag", L0QData,HDFFLOAT, strDataUnit, strLongName, 
//                                              strBandName, fValidRange, &fFillValue, &fIntercept, &fslope) < 0)
//  {
//    printf("read 2km HDF L0QualityFlag error!");
//    ZLDSWriteLog('W',"Get 2km HDF L0QualityFlag error!");
//    return ZEGET2KMDATAOFFHS;
//  }                                              
//  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName, HDFROOT, "PosQualityFlag", POSQData,HDFFLOAT, strDataUnit, strLongName, 
//                                          strBandName, fValidRange, &fFillValue, &fIntercept, &fslope) < 0)
//  {
//    printf("read 2km HDF PosQualityFlag error!");
//    ZLDSWriteLog('W',"Get 2km HDF PosQualityFlag error!");
//    return ZEGET2KMDATAOFFHS;
//  }                                                
//  if(m_SensorHdfOperator.ZHDFReadSDSData(strInputFileName, HDFROOT, "CalQualityFlag", CALQData,HDFFLOAT, strDataUnit, strLongName, 
//                                              strBandName, fValidRange, &fFillValue, &fIntercept, &fslope)< 0)
//  {
//    printf("read 2km HDF CalQualityFlag error!");
//    ZLDSWriteLog('W',"Get 2km HDF CalQualityFlag error!");
//    return ZEGET2KMDATAOFFHS;
//  }                                            
  sprintf(m_strL0Q,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f",L0QData[0],L0QData[1],L0QData[2],L0QData[3],L0QData[4],L0QData[5],
                                      L0QData[6],L0QData[7],L0QData[8],L0QData[9],L0QData[10],L0QData[11],L0QData[12],L0QData[13]);
  sprintf(m_strPOSQ,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f",POSQData[0],POSQData[1],POSQData[2],POSQData[3],POSQData[4],POSQData[5],
                                      POSQData[6],POSQData[7],POSQData[8],POSQData[9],POSQData[10],POSQData[11],POSQData[12],POSQData[13]);
  sprintf(m_strCALQ,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f",CALQData[0],CALQData[1],CALQData[2],CALQData[3],CALQData[4],CALQData[5],
                                      CALQData[6],CALQData[7],CALQData[8],CALQData[9],CALQData[10],CALQData[11],CALQData[12],CALQData[13]);  
  DataQ=(unsigned short)POSQData[0];
  for(int i=0;i<14;i++)
  {
      if(DataQ <= (unsigned short)POSQData[i])
          DataQ = (unsigned short)POSQData[i];
      if(DataQ <=(unsigned short)CALQData[i] )
          DataQ = (unsigned short)CALQData[i];  
  }
  printf("--------------DataQ:%d\n",DataQ);
  return 0;                                      
}

int  INIOperation::GetSDSInfo(hid_t file_id,char* sdsname,float*intercept,float* slope,char* bandname,
	                             char*longname,char*units,unsigned int* valid_range,unsigned int* fillvalue)
{
	
//  
//    H5LTget_attribute_float(file_id,sdsname,"Intercept",intercept);
//    H5LTget_attribute_float(file_id,sdsname,"Slope",slope);
//    
//    H5LTget_attribute_string(file_id,sdsname,"band_names",bandname); 
//    H5LTget_attribute_string(file_id,sdsname,"long_name",longname);
//    H5LTget_attribute_string(file_id,sdsname,"units",units);
//       
//    H5LTget_attribute_uint(file_id,sdsname,"valid_range",valid_range,2);  
//    H5LTget_attribute_uint(file_id,sdsname,"FillValue",fillvalue,1);  
   
 return 0;       
}
