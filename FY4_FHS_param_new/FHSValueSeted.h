#ifndef FHSValueSetted_H_
#define FHSValueSetted_H_
/********************************生成标记图**************************************/
#define  SZAngleUp         70              // 太阳天顶角订正角度参数
#define  Nodatavalue       -900            //非数据区值参数
#define  SatZAngLimit      82             //卫星天顶角上限参数
#define  SZAngleNt         85             //太阳天顶角晨昏线天顶角度参数
#define  VisAngleNt        0.6            //晨昏线可见光通道反射率值参数
#define  RNir_wate         0.1            //近红外水体判识阈值参数
#define  RNir-Vis_water    0             //近红外可见光差异水体判识参数
#define  SunGtAngLimit    10             //耀斑区角度参数
#define  T4LowTemp        200            //中红外通道低温区阈值参数
#define  T11LowTemp       200            //远红外通道低温区阈值参数
#define  VisDCloudBJ      0.15           //可见光通道云判识阈值参数1               
#define  SZAngDCloudBJ    87            //白天太阳天顶角云判识阈值参数
#define  T11DCloud        270           //远红外亮温云判识阈值参数
#define  RVisCloud        0.15          //可见光通道反射率云判识阈值参数2
#define  SZAngNCloudBJ	 	87          //夜间太阳天顶角云判识阈值参数
#define  T11NCloud		    270          //夜间远红外亮温云判识阈值参数
#define  VisDCloudth		 0.35           //白天云区判识仅用可见光通道阈值参数
#define  T11NCloudth		 270           //夜间云区判识仅用红外通道阈值参数

/***************************晴空植被像元背景值计算*******************************************************/
#define  T4BJLimit		   315           //背景中红外计算邻近像元取舍温度参数
#define  RegnWEBJ		    1001           //背景像元中心区域列位置参数
#define  Coe1  	          1            //背景像元选取中东部区域权重系数参数1
#define  Coe2		         0.6           //背景像元选取中东部区域权重系数参数2
#define  DtBJ1		       35           //远红外亮温增量系数参数
#define  Count_Limit		0.15           //背景邻域像元个数比例下限参数
#define  Count_No		     8.            //背景邻域像元个数下限参数
#define  BJlimit		     51            //背景温度计算邻域范围上限

 /***************************云区像元背景值计算*********************************************/
#define  T4CLDBJ		     310           //云区中红外温度界限参数
#define  VisCLDBJLimit		0.5          //云区像元可见光反射率界限参数
#define  T11CLD		       250          //远红外温度界限参数
#define  T4CLDBJLimit		 290           //云区像元背景选择中红外阈值
#define  VisCLDBJLimit		0.5           //云区像元背景选择可见光阈值
#define  CBJlimit		      21             //云区像元背景计算邻域范围上限

/*****************************计算短波红外通道5、6晴空植被像元背景平均值****************************/
#define  iCountSIR	      10         //通道5（1.6μ），通道6（2.2μ）背景计算的像元个数阈值		                      

/*****************************火点判识************************************************************/
/*****************************1）生成镜面反射点数组************************************************/
#define  SunZRFLT		      85         //太阳天顶角镜面反射阈值参数
#define  DR6SLimit	      0.1        //镜面反射1.6μ通道背景差异阈值参数
#define  DT4SLTUP		      25         //镜面反射中红外通道与背景差异上限参数
#define  DT4SLTLW		      12         // 镜面反射中红外通道与背景差异下限参数
#define   T4SunLT		      310         //中红外通道镜面反射阈值参数

/*****************************2）晴空绝对高温火点像元确认*******************************************/
#define  T4HiFire		      340         // 绝对高温火点中红外阈值参数
#define  DT4HiFire		      20          //绝对高温火点中红外与背景差异阈值参数
#define  DT4_11HiFire		  15           //绝对高温火点中红外与远红外差异阈值

/*****************************3）缺少足够背景像元个数的像元*******************************/
#define  LKT4RL		       325         //图像右下方中红外阈值参数
#define  DLKT4_11RL		   30           // 图像右下方中红外与远红外差异阈值参数                                    
#define  LKT11RL		         278           //图像右下方远红外通道下限参数
#define  LKLineRL	         	600        //图像右下方行号界限参数
#define  LKRawRL	         	2300       //图像右下方列号界限参数
#define  LKT4CW		         338           //图像左下方中红外阈值参数
#define  DLKT4_11CW		     30             //图像左下方中红外与远红外差异阈值参数
#define  LKT11CW		        278               //图像左下方远红外通道下限参数
#define  LKLineRL		       600             //图像右下方行号界限参数
#define  LKRawRL		      2300            //图像右下方列号界限参数
#define  LKT4NH		         315            //图像北部中红外阈值参数
#define  DLKT4_11NH		      30            //图像北部中红外与远红外差异阈值参数
#define  LKT11NH		       250              //图像北部远红外通道下限参数
#define  LKLineNH		       600             //图像北部行号界限参数
#define  LKRawNH		         600              //图像北部列号界限参数
#define  T4NR_Df		       4              //中红外相近亮温差异阈值参数
#define  NRT4Limit	        315            //不足像元个数中红外亮温阈值参数
#define  NRPixNo1		        4                  //中红外相近像元个数阈值参数
#define  NRPixNo2		        5                  //中红外相近像元个数阈值参数

/*****************************4)云区像元火点判识********************************************/
/#define  T4CLDFIRECE		  310           //中东部云区中红外阈值参数
#define   VisCLDFIRECE	  	0.5          //中东部云区可见光阈值参数
#define  DT4CLDFIRECE		  20           //中东部云区中红外与平均值差异阈值参数
#define  DT411CLDFIRECE		25          //中东部云区中红外与远红外差异阈值参数
#define  CLDLineCE		    100               //中东部云区行号参数
#define  CLDRawCE		      1100               //中东部云区列号参数
#define  CLDRawET		      2300               //东部云区列号参数
#define  T4CLDFIRECW		 338           //中西部云区中红外阈值参数
#define  VisCLDFIRECW		 0.5            //中西部云区可见光阈值参数
#define  DT4CLDFIRECW		 20            //中西部云区中红外与平均值差异阈值参数
#define  DT411CLDFIRECW		25          // 中西部云区中红外与远红外差异阈值参数
#define  CLDLineCW		   100              //中西部云区行号参数
#define  CLDRawCW		     2300               //中西部云区列号参数		
#define  T4CLDFIREWS		   318           //西部云区中红外阈值参数
#define  VisCLDFIREWS		   0.5            //西部云区可见光阈值参数
#define  DT4CLDFIREWS		   20           //西部云区中红外与平均值差异阈值参数
#define   DT411CLDFIREWS		25        //西部云区中红外与远红外差异阈值参数
#define  CLDLineWS		      100              //西部云区行号参数
#define  CLDRawWS		       1100                //西部云区列号参数

/*****************************5）晴空植被像元火点初步判识（1）************************************/
#define  T4BJDVLineNH	      600	              //图像北部行号
#define  T4BJDV_NHUP	      3	             //中红外标准差图像北部上限阈值参数
#define  T4BJDV_NHUPAJ      3	           //中红外标准差图像北部上限修正值参数
#define  T4BJDV_NHLW	      2	             //中红外标准差图像北部下限阈值参数
#define  T4BJDV_SHUP	      10	             //中红外标准差图像南部上限阈值参数
#define  T4BJDV_SHUPAJ      4	            //中红外标准差图像南部上限修正值参数
#define  T4BJDV_SHLW	      3	             //中红外标准差图像南部下限阈值参数
#define  T4BJDV_SHLWAJ       2	           //中红外标准差图像南部下限修正参数，
#define  T4BJDV_SHAJ	       3	             //中红外标准差图像南部一般修正值参数
#define  T4_11BJDV_NHUP	     3	           //中红外和远红外差异标准差图像北部上限阈值参数
#define  T4_11BJDV_NHUPAJ	   3	          //中红外和远红外差异标准差图像北部上限修正值参数
#define  T45BJDVLineNH	    600	           //中红外和远红外差异标准差图像北部界限行号 
#define  T4_11BJDV_NHLW	    2	           //中红外和远红外差异标准差图像北部下限阈值参数
#define  T4_11BJDV_SHUP	    10           //中红外标准差图像南部上限阈值参数
#define  T4_11BJDV_SHUPAJ	  4	         //中红外和远红外差异标准差图像南部上限修正值参数
#define  T4_11BJDV_SHLW	    3	           //中红外和远红外差异标准差图像南部下限阈值参数.
#define  T4_11BJDV_SHLWAJ	  2	         //中红外标准差图像南部下限修正参数
#define  T4_11BJDV_SHAJ	    3	           //中红外和远红外差异标准差图像南部一般修正值参数

/*****************************5）晴空植被像元火点初步判识（2）设置火点阈值**********************/
#define  T4_Factr	          3	             //中红外火点确认系数参数
#define  T4_11Factr	       3	           //中红外与远红外亮温差异火点确认系数参数 
/*****************************5）晴空植被像元火点初步判识（3）初步确认火点***/
#define   Vis_Fireth	   0.45	            //可见光火点确认阈值参数
#define   DFR6_Fire	     0.1	                //通道6与背景差异火点确认阈值参数
#define  SatZ_fire	     81             //初判卫星天顶角范围阈值参数
/*****************************5）晴空植被像元火点初步判识（4)判断镜面反射影响**************/
#define  SunRfT4	         15	                  //中红外与背景差异镜面反射阈值
#define   SunRfCH6LW	    0.06	             //通道6与背景差异下限阈值参数
#define   SunRfCH6UP	    0.1	             //通道6与背景差异上限阈值参数
/*****************************5）晴空植被像元火点初步判识（5)判断云边缘影响***************************/
#define   CldT4_Edg	     300             //中红外通道云区边缘下限阈值参数
#define   CldT4BjDf_Edg	  15             //云区边缘中红外与背景差异阈值参数

/*****************************5）晴空植被像元火点初步判识（6)判断荒漠边缘影响********/
#define  DstT4_Edg	      330	             //荒漠区边缘中红外通道阈值参数

/****************************5）晴空植被像元火点初步判识（7）判断水体边缘影响****************************/
#define   WatT4_Edg	      330	             //水体边缘中红外通道阈值参数

/****************************6）判断各类晴空影响下垫面差异（1）*************************************/
#define  DBjT4_dfLw	      320	             //下垫面差异中红外下限阈值参数
#define  DBjT4bj_dfLw	    12	             //下垫面中红外与背景差异下限阈值参数
#define  DBjT4bjc_dfLw	  12	           //下垫面中红外标准差阈值参数

/****************************6）（2）云区污染影响***********************/
#define  CldDisT11Dbj	    5	             //云污染远红外通道与背景差异阈值参数
#define  CldDisVisDbj	    0.15	           //云污染可见光与背景差异阈值参数
#define  CldDisVisUp	    0.35	           //云污染可见光上限阈值参数

/****************************6）（3)多个温度相近判识（计算个数）****************/
#define  SameT4Up	       300               //多个相近像元中红外阈值参数
#define  SameNDVI	       0.25	               //多个相近像元NDVI修正值参数
#define  SameT4BJDf       15	               //多个相近像元中红外与背景差异阈值参数
#define  SameT4DfUP       4	               //多个相近像元中红外差异上限阈值参数
#define  SameT4DfLW       -4               //多个相近像元中红外差异下限阈值参数
 
 /****************************6）（3)多个温度相近判识（判断个数过多）***************/
#define  SamePixNo1       5	                 //相近像元个数阈值1参数
#define  SameT4Lw1	      320	             //相近像元中红外温度下限1参数
#define  SameT4Up1	      330	             //相近像元中红外温度上限1参数
#define  SamePixNo2       5	                 //相近像元个数阈值2参数
#define  SameT4Lw2	      300	             //相近像元中红外温度下限2参数
#define  SameT4Up2	      330	             //相近像元中红外温度上限2参数
#define  SamePixNo3       3	                 //相近像元个数阈值3参数
#define  SameT4Up3	     300             //相近像元中红外温度上限3参数
#define  SameRawNo	     2300	           //相近像元列号阈值

/***********************6)(4)噪声判识和消除****************************************************/
#define  NoisePixNo	     30	             //噪声线像元个数阈值参数
#define  NoisePixT4	     470             //噪声像元亮温阈值参数





  
 
#endif /* FHSValueSetted_H_ */