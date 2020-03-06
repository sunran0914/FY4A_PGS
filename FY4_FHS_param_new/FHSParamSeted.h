#ifndef FHSParamSetted_H_
#define FHSParamSetted_H_
/********************************生成标记图**************************************/
float SZAngleUp;                  // 太阳天顶角订正角度参数
float Nodatavalue;              //非数据区值参数
float SatZAngLimit;            //卫星天顶角上限参数
float SZAngleNt;           //太阳天顶角晨昏线天顶角度参数
float VisAngleNt;          //晨昏线可见光通道反射率值参数
float RNir_water;         //近红外水体判识阈值参数
float RNir_Vis_water;      //近红外可见光差异水体判识参数
float SunGtAngLimit;      //耀斑区角度参数
float T4LowTemp;     //中红外通道低温区阈值参数
float T11LowTemp;             //远红外通道低温区阈值参数
float VisDCloudBJ;            //可见光通道云判识阈值参数1               
float SZAngDCloudBJ;         //白天太阳天顶角云判识阈值参数
float T11DCloud;               //远红外亮温云判识阈值参数
float RVisCloud;               //可见光通道反射率云判识阈值参数2
float SZAngNCloudBJ;	 	     //夜间太阳天顶角云判识阈值参数
float T11NCloud;		          //夜间远红外亮温云判识阈值参数
float VisDCloudth;	       //白天云区判识仅用可见光通道阈值参数
float T11NCloudth;	      //夜间云区判识仅用红外通道阈值参数

/***************************晴空植被像元背景值计算*******************************************************/
float  T4BJLimit;		            //背景中红外计算邻近像元取舍温度参数
float  RegnWEBJ;		              //背景像元中心区域列位置参数
float  Coe1;  	                  //背景像元选取中东部区域权重系数参数1
float  Coe2	;	                  //背景像元选取中东部区域权重系数参数2
float  DtBJ1;		               //远红外亮温增量系数参数
float  DtBJ2;		                //中红外亮温增量系数参数
float  Count_Limit;		          //背景邻域像元个数比例下限参数
float  Count_No	;	             //背景邻域像元个数下限参数
float  BJlimit;                //背景温度计算邻域范围上限
//20180315新增
float  BJDNIR;        // //背景像元选取的近红外差异参数阈值（用于去除水体）
float  BJDT11;        ////背景像元选取的远红外差异参数阈值 （用于去除水体）


 /***************************云区像元背景值计算*********************************************/
float  T4CLDBJ;		              //云区中红外温度界限参数
float  VisCLDBJLimit;		         //云区像元可见光反射率界限参数
float  T11CLD;	               //远红外温度界限参数
float  T4CLDBJLimit;		          //云区像元背景选择中红外阈值

float  fCLDTempLimit;         //云区背景邻域像元个数比例下限参数
float  iCLDCountLmt;        //云区背景邻域像元个数下限参数
float  CBJlimit;	                //云区像元背景计算邻域范围上限

/*****************************计算短波红外通道5、6晴空植被像元背景平均值****************************/
float  iCountSIR;	               //通道5（1.6μ），通道6（2.2μ）背景计算的像元个数阈值		                      

/*****************************火点判识************************************************************/
/*****************************1）生成镜面反射点数组************************************************/
float  SunZRFLT;		           //太阳天顶角镜面反射阈值参数
float  DR6SLimit;	           //镜面反射1.6μ通道背景差异阈值参数
float  DT4SLTUP;	           //镜面反射中红外通道与背景差异上限参数
float  DT4SLTLW;	           // 镜面反射中红外通道与背景差异下限参数
float   T4SunLT;	            //中红外通道镜面反射阈值参数

/*****************************2）晴空绝对高温火点像元确认*******************************************/
float  T4HiFire;		              // 绝对高温火点中红外阈值参数
float DT4HiFire;	              //绝对高温火点中红外与背景差异阈值参数
float  DT4_11HiFire;	           //绝对高温火点中红外与远红外差异阈值
float  CloudHiFire;
/*****************************3）缺少足够背景像元个数的像元*******************************/
float  LKT4RL;	          //图像右下方中红外阈值参数
float  DLKT4_11RL;		       // 图像右下方中红外与远红外差异阈值参数                                    
float LKT11RL;	              //图像右下方远红外通道下限参数
float  LKLineRL;            //图像右下方行号界限参数
float  LKRawRL;	            //图像右下方列号界限参数
float  LKT4CW;	              //图像左下方中红外阈值参数
float  DLKT4_11CW;	           //图像左下方中红外与远红外差异阈值参数
float  LKT11CW;		                 //图像左下方远红外通道下限参数
float  LKT4NH;	               //图像北部中红外阈值参数
float  DLKT4_11NH;		           //图像北部中红外与远红外差异阈值参数
float  LKT11NH;		               //图像北部远红外通道下限参数
float  LKLineNH	;	              //图像北部行号界限参数
float LKRawNH	;	                 //图像北部列号界限参数
float  T4NR_Df;	             //中红外相近亮温差异阈值参数
float  NRT4Limit;	              //不足像元个数中红外亮温阈值参数
float  NRPixNo1;		                  //中红外相近像元个数阈值参数
float  NRPixNo2;	                  //中红外相近像元个数阈值参数

/*****************************4)云区像元火点判识********************************************/
float T4CLDFIRELW	;	       //云区低端火点中红外阈值参数
float DT4CLDFIRELWLW;		        //云区低端火点中红外与平均值差异下限阈值参数
float DT4CLDFIRELWUP	;	         //云区低端火点中红外与平均值差异上限阈值参数
float DT11CLDFIREBJCLW ;              //云区低端火点远红外标准差下限阈值参数
float VISCLDFIREBJCUP;               //云区低端火点可见光标准差上限阈值参数
float DT11CLDFIRELW;                 //云区低端火点远红外亮温下限阈值参数
float T4CLDFIREUP;		       //云区高端火点中红外阈值参数
float DT4CLDFIREUPLW	;	        //云区高端火点中红外与平均值差异下限阈值参数
float DT411CLDFIRELW	;        //云区高端火点中红外与远红外差异下限阈值参数
float DT11CLDFIREBJCUP;              //云区高端端火点远红外标准差上限阈值参数
float VISCLDFIREBJCUPUP ;             //云区高端火点可见光标准差上限阈值参数
//--------20180315新增-----up-------------

float  DT11CLDFIRELAND;                //云区边晴空火点判识远红外差异阈值参数
float  VISCLDFIRELAND ;           //云区边晴空火点判识可见光差异阈值参数

float  T4CLDFIRENH;
float  DT4CLDFIRENH;


float  T4CLDFIREWN;		       //云区西北部中红外阈值参数
float  VisCLDFIRECE;	           //中东部云区可见光阈值参数
float  DT4CLDFIREWN;		         //云区西北部中红外与平均值差异阈值参数
float  DT4CLDFIRESH	;	         //南部云区中红外与平均值差异阈值参数
float  DT411CLDFIRECE;		        //中东部云区中红外与远红外差异阈值参数

//--------20180315新增-----end-------------


float  T4CLDFIRECT	  ;	         //云区东部中红外阈值参数
float  VisCLDFIRECW	  ;	    //云区可见光阈值参数
float  DT4CLDFIRECT	  ;	   //云区东部中红外与平均值差异阈值参数
float  DT411CLDFIRECW	;     // 中西部云区中红外与远红外差异阈值参数
float  DT11CLDETUP    ;              //云区东部火点远红外标准差上限阈值参数
float  VISCLDBJCETUP  ;            //云区东部可见光标准差上限阈值参数
float  T4CLDFIREWS	  ;	      //云区西部中红外阈值参数
float  VisCLDFIREWS	  ;	      //云区可见光阈值参数
float  DT4CLDFIREWS	  ;	    //云区西部中红外与平均值差异阈值参数
float  DT411CLDFIREWS	;  //西部云区中红外与远红外差异阈值参数
float  DT11CLDWSUP    ;               //云区西部部火点远红外标准差上限阈值参数
float  VISCLDBJCWSUP  ;            //云区西部部可见光标准差上限阈值参数
//20180315********************up*******************
float  CLDLineNHUP;		            // 东北部圆盘图云区上限行号参数
float  CLDLineNHLW;		            // 东北部圆盘云区下限行号参数
float  CLDRawNHUP	;	               // 东北部和东南部圆盘图云区列号上限（左端）参数
float  CLDRawNHLW	;	              // 东北部和东南部圆盘图云区列号下限（右端）参数

float  CLDRGLineNHUP;		              // 东北部区域图云区上限行号参数
float  CLDRGLineNHLW;		            // 东北部区域云区下限行号参数
float  CLDRGRawNHUP		;           // 东北部和东南部区域图云区列号上限（左端）参数
float  CLDRGRawNHLW	;	           // 东北部和东南部区域图云区列号下限（右端）参数  
//20180315********************end*******************

float  CLDLineETUP	  ;	      //东部和西部云区行号上限参数CLDLineETUP
float  CLDLineETLW    ;		      //东部和云区行号下限参数

//20180315********************up*******************
float  CLDRGLineETUP;		         //东南部区域图云区行号上限参数 
float  CLDRGLineETLW;		           //东南部区域图云区行号下限参数

float  CLDLineWNUP;		               // 西北部圆盘图云区上限行号参数
float  CLDLineWNLW;		           // 西北部圆盘云区下限行号参数
float  CLDRawWNUP	;	                 // 西北部和西南部圆盘图云区列号上限（左端）参数
float  CLDRawWNLW	;	               // 西北部和西南部圆盘图云区列号下限（右端）参数

float  CLDRGLineWNUP;		             // 西北部区域图云区上限行号参数
float  CLDRGLineWNLW;		            // 西北部区域图云区下限行号参数
float  CLDRGRawWNUP;		        // 西北部和西南部区域图云区列号上限（左端）参数
float  CLDRGRawWNLW	;	         // 西北部和西南部区域图云区列号下限（右端）参数

float  CLDLineWSUP;		            // 西南部圆盘图云区行号上限参数
float  CLDLineWSLW;		            // 西南部圆盘图云区行号上限参数
float  CLDRGLineWSUP;		            // 西南部区域图云区行号上限参数
float  CLDRGLineWSLW;		            // 西南部区域图云区行号上限参数

//20180315********************end*******************

//float  CLDRawCWUP	    ;	    //西部云区列号上限参数	
////float  CLDLineNHUP    ;		          //北部云区上限行号参数
//float  CLDLineNHLW    ;		          //北部云区下限行号参数
//float  CLDRawNHUP	    ;	           //北部和东部云区列号上限参数
//float  CLDRawNHLW	    ;	           //北部和西部云区列号下限参数
//float  CLDRawETLW	    ;	           //东部云区列号下限参数
//float  CLDLineWS      ;		    //西部云区行号参数
//float  CLDRawWS	      ;	    //西部云区列号参数

float  CLDSameNoNH    ;		    //北部云区多个相近像元个数阈值参数
//20180315*******************up******************
float  CLDSameNoWN	;	         //西北部云区多个相近像元个数阈值参数
//20180315*******************end******************

float  CLDSameNoET    ;		    //东部部云区多个相近像元个数阈值参数
float  CLDSameNoWS    ;		    //西部部云区多个相近像元个数阈值参数


/*****************************5）晴空植被像元火点初步判识（1）************************************/
float  T4BJDVLineNH	;                //图像北部行号
float  T4BJDV_NHUP;	             //中红外标准差图像北部上限阈值参数
float  T4BJDV_NHUPAJ;           //中红外标准差图像北部上限修正值参数
float  T4BJDV_NHLW;	             //中红外标准差图像北部下限阈值参数
float  T4BJDV_SHUP;	               //中红外标准差图像南部上限阈值参数
float  T4BJDV_SHUPAJ;            //中红外标准差图像南部上限修正值参数
float  T4BJDV_SHLW;	             //中红外标准差图像南部下限阈值参数
float  T4BJDV_SHLWAJ ;            //中红外标准差图像南部下限修正参数，
float  T4BJDV_SHAJ;	               //中红外标准差图像南部一般修正值参数
float  T4_11BJDV_NHUP	;           //中红外和远红外差异标准差图像北部上限阈值参数
float  T4_11BJDV_NHUPAJ	;        //中红外和远红外差异标准差图像北部上限修正值参数
float  T45BJDVLineNH;	           //中红外和远红外差异标准差图像北部界限行号 
float  T4_11BJDV_NHLW	;         //中红外和远红外差异标准差图像北部下限阈值参数
float  T4_11BJDV_SHUP	;         //中红外标准差图像南部上限阈值参数
float  T4_11BJDV_SHUPAJ	;     //中红外和远红外差异标准差图像南部上限修正值参数
float  T4_11BJDV_SHLW	;         //中红外和远红外差异标准差图像南部下限阈值参数.
float  T4_11BJDV_SHLWAJ	;     //中红外标准差图像南部下限修正参数
float  T4_11BJDV_SHAJ;	         //中红外和远红外差异标准差图像南部一般修正值参数

/*****************************5）晴空植被像元火点初步判识（2）设置火点阈值**********************/
float  T4_Factr	;                      //中红外火点确认系数参数
float  T4_11Factr	;                  //中红外与远红外亮温差异火点确认系数参数 
/*****************************5）晴空植被像元火点初步判识（3）初步确认火点***/
float   Vis_Fireth;	              //可见光火点确认阈值参数
float   DFR6_Fire	;                    //通道6与背景差异火点确认阈值参数
float  SatZ_fire	;              //初判卫星天顶角范围阈值参数
float  SunZ_fire;	                 //初判太阳天顶角阈值参数
float  IR45BjAver_fire;                 //初判中红外与远红外差异平均值上限阈值参数
float  IR45BjAver_fireAJ;                 //初判中红外与远红外差异平均值上限修正阈值参数
/*****************************5）晴空植被像元火点初步判识（4)判断镜面反射影响**************/
float  SunRfT4	;                    //中红外与背景差异镜面反射阈值
float   SunRfCH6LW;	             //通道6与背景差异下限阈值参数
float   SunRfCH6UP;	           //通道6与背景差异上限阈值参数
/*****************************5）晴空植被像元火点初步判识（5)判断云边缘影响***************************/
float   CldT4_Edg	;                 //中红外通道云区边缘下限阈值参数
float   CldT4BjDf_Edg	;             //云区边缘中红外与背景差异阈值参数

float   CldSunZenith_Edg;        //云区边缘太阳天顶角阈值参数                
float   CldT4_EdgLW	    ;            //云区边缘低温火点中红外通道下限阈值参数
float   CldT4BjDf_EdgLW	;           //云区边缘低温火点中红外与背景差异阈值参数 /*****************************5）晴空植被像元火点初步判识（6)判断荒漠边缘影响********/

/*****************************5）晴空植被像元火点初步判识（6)判断荒漠边缘影响********/

float  DstT4_Edg;	                  //荒漠区边缘中红外通道阈值参数

/****************************5）晴空植被像元火点初步判识（7）判断水体边缘影响****************************/
float   WatT4_Edg	;                  //水体边缘中红外通道阈值参数

/****************************6）判断各类晴空影响下垫面差异（1）*************************************/
float  DBjT4_dfLw	;                   //下垫面差异中红外下限阈值参数
float  DBjT4bj_dfLw	;                 //下垫面中红外与背景差异下限阈值参数
float  DBjT4bjc_dfLw	;             //下垫面中红外标准差阈值参数
float DT4b_dfLw;  	           //下垫面中红外差异阈值参数
float  DT4b_dfCoe;  	           //下垫面中红外差异计算系数阈值参数
float  DBjT4_dfSH;     	   //下垫面南半球差异中红外下限阈值参数
float  DB_dfLineNS;               //下垫面图像南北部界限行号 
float   DT11bjc_dfUP;             //下垫面远红外标准差上限阈值参数
float   DT11bjc_dfLW;  	           //下垫面远红外标准差下限阈值参数
//20180227增加*********上***********
float  T11land_W;
float  DT4land_W; 	            //下垫面水体影响中红外陆地差异阈值参数
float  DT11land_W; 	           //下垫面水体影响远红外陆地差异阈值参数
float  DVISland_W ;	            //下垫面水体影响可见光陆地差异阈值参数
//20180227增加*********下***********
/****************************6）（2）云区污染影响***********************/
float  CldDisT11Dbj	;            //云污染远红外通道与背景差异阈值参数
//20180315**************up*********************
float  CldDisVisDbj;               //云污染可见光与背景差异阈值参数
float  CldDisVisUp1	;               //云污染可见光上限阈值参数1
float  CldDisVisUp2	;  	           //云污染可见光上限阈值参数2
float  CldT4_UP	;         //云污染中红外通道上限阈值参数
float  CldT4bj_UP	;                //云污染中红外通道与背景差异上限阈值参数
float  CldT4_UP1;	   	           //云反射干扰中红外通道上限阈值参数1
float  CldT4_UP2	;            //云反射干扰中红外通道上限阈值参数2
float  CldT11_UP;	             //云反射干扰远红外通道上限阈值参数
float  CldlandDVis;	           //云或雪区边缘陆地误判可见光与背景差异阈值参数
float  CldlandDT11;	              //云或雪区边缘陆地误判标准差阈值参数
float  CldT11_bjc	;                 //云污染远红外通道标准差阈值参数
//20180315**************end********************
//float  Same4BJDT;
//float  CldDisVisDbj	;              //云污染可见光与背景差异阈值参数
//float  CldDisVisUp	;              //云污染可见光上限阈值参数
//float  CldT4_UP;	  	   //云污染中红外通道上限阈值参数
//float  CldT4bj_UP;   	           //云污染中红外通道与背景差异上限阈值参数
//float  CldT4_UP2;   	           //云污染中红外通道上限阈值参数2
//float  CldT11_bjc;    	           //云污染远红外通道标准差阈值参数

/****************************6）（3)多个温度相近判识（计算个数）****************/
float  SameT4Up	;                   //多个相近像元中红外阈值参数
float  SameNDVI	;                     //多个相近像元NDVI修正值参数
float  SameT4BJDf ;                   //多个相近像元中红外与背景差异阈值参数
float  SameT4DfUP ;                 //多个相近像元中红外差异上限阈值参数
float  SameT4DfLW  ;                //多个相近像元中红外差异下限阈值参数
float  SameT11_bjc;               //多个相近像元远红外标准差阈值参数
 
 /****************************6）（3)多个温度相近判识（判断个数过多）***************/
float  SamePixUP;    //20180207
float   SamePixNHLW;               //晴空北部相近像元个数下限阈值参数
float   SamePixNHUP;               //晴空北部相近像元个数上限阈值参数
float   SameT4NH   ;	          //晴空北部相近像元中红外温度阈值参数
float   SameT4DFNH ;	      //晴空北部相近像元中红外温度与背景差异上限阈值参数
float   SameT4NHUP ;	          //晴空北部相近像元中红外温度上限阈值参数
float   SameT4DFUP ;	      //晴空高端火点相近像元中红外温度与背景差异上限阈值参数
//20180227**********up*************
float  SameT4NHLW;	    	         //晴空北部低温火点相近像元中红外温度上限阈值参数
float  SameT4DFNHLW;   	         //晴空北部低温火点相近像元中红外温度与背景差异上限阈值参数
//20180227**********down*************

//20180315*********up********************************
float  SameT4DFWNLW1;	    //晴空西北部低温火点相近像元中红外温度与背景差异下限阈值参数1
float  SameT4DFWNLW2;	     //晴空西北部低温火点相近像元中红外温度与背景差异下限阈值参数2
float  SameT11DFWN	;      //晴空西北部水体边缘判断远红外亮温与背景差异下限阈值参数（4K）
float  SameVISDFWN	; 	    //晴空西北部水体边缘判断可见光反射率与背景差异下限阈值参数（-10%）
//20180315*********end********************************

float   SameT4ET   ;	         //晴空东部相近像元中红外温度阈值参数
float   SameT4DFETUP;	      //晴空东部部相近像元中红外温度与背景差异上限阈值参数
float   SameT4ETUP  ;	          //晴空东部部相近像元中红外温度上限阈值参数
float   SamePixETLW ;               //晴空东部相近像元个数下限阈值参数
float   SamePixETUP ;               //晴空东部相近像元个数上限阈值参数
//20180227**********up*************
float SameT4DFETLW;
//20180227**********down*************
float   SameT4WS    ;	          //晴空西部相近像元中红外温度阈值参数
float   SameT4DFWS  ;	      //晴空西部相近像元中红外温度与背景差异上限阈值参数
//20180315************up****************************
float  SameT4DFWSUP	;    //晴空西南部相近像元中红外温度与背景差异上限阈值参数
//20180315*************end*************************
float   SameT4WSUP  ;	          //晴空西部部相近像元中红外温度上限阈值参数
float   SamePixWSLW ;                  //晴空西部相近像元个数下限阈值参数
float   SamePixWSUP ;                 //晴空西部相近像元个数上限阈值参数
float   SameT4Up3;                 // 相近像元中红外
float  SameRawNo	;              //相近像元列号阈值
float   SameDT4LW;                 //相近像元中红外与背景差异阈值参数
//20180227**********up*************
float SameT4bjc;           //相近像元中红外标准差阈值参数
float SameT11bjc;	           //相近像元远红外标准差阈值参数
float SameQZGYDT4;	           //相近像元青藏高原中红外与背景差异阈值参数
float  QZGYLINE;         //青藏高原南端边界行号（圆盘图），      //20180403  修改值
float  QZGYLINELW;	     	    //青藏高原北端边界行号（圆盘图），      //20180403  新增
float  QZGYRAW;	          //青藏高原南端边界列号（圆盘图和区域图）float
float  QZGYRGLINEUP; 	    //青藏高原北端端边界行号（区域图）      // 20180403   新增
float  QZGYRGLINELW;     //青藏高原南端端边界行号（区域图）      // 20180403   新增
//20180227**********down*************
/***********************6)(4)噪声判识和消除****************************************************/
float  NoisePixNo	;                 //噪声线像元个数阈值参数
float  NoisePixT4	;                 //噪声像元亮温阈值参数
/*********************** 可信度参数 ****************************************************/
float  Try_DT4BJ;                //可信度中红外与背景差异参数
//20180315************up****************************
float  Try_DT4BJ2	;    //晴空西南部相近像元中红外温度与背景差异上限阈值参数
//20180315*************end*************************
float  Try_NoiseT4;             //可信度中红外与背景差异噪声参数

#endif /* FHSParamSetted_H_ */