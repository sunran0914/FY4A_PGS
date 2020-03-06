#ifndef _PPS_FHS_ERROR_H 
#define _PPS_FHS_ERROR_H

#define  ZESUCCESSFHS        0         /* 执行成功           */

#define  ZEOFFSETOFFHS         10       /* 错误起始偏移量信息  热源点检测（3810-3900）*/



#define ZEFIREPARA						ZEOFFSETOFFHS + 1   /*执行参数个数错误*/
#define ZELOGNINITOFFIRE			ZEOFFSETOFFHS + 2   /*日志初始化失败*/
#define ZECONFIGWRITE					ZEOFFSETOFFHS + 3   /*写TXT输入输出接口文件失败*/
#define ZECONFIGOPEN					ZEOFFSETOFFHS + 4   /*打开TXT输入输出接口文件失败*/
#define ZEOKWRITE							ZEOFFSETOFFHS + 5   /*写OK文件失败*/
#define ZEOKOPEN							ZEOFFSETOFFHS + 6   /*打开OK文件失败*/
#define ZEHDFOPEN							ZEOFFSETOFFHS + 7   /*打开HDF文件失败*/
#define ZEHDFWRITE						ZEOFFSETOFFHS + 8   /*写HDF文件失败*/
#define ZENCWRITE 						ZEOFFSETOFFHS + 9   /*写NC文件失败*/
#define ZESDSOFNCWRITE 				ZEOFFSETOFFHS + 10   /*写NC数据集文件失败*/
#define ZEATTOFNCWRITE 				ZEOFFSETOFFHS + 11   /*写NC全局属性文件失败*/

#define ZEREADOFFIRE   				ZEOFFSETOFFHS + 12   /*读数据文件失败*/
#define ZEREADCLMOFFIRE   		ZEOFFSETOFFHS + 13   /*获取CLM数据文件失败*/

#define ZEREAD2KMHDFATT   		ZEOFFSETOFFHS + 14   /*获取2km文件属性失败*/

#define ZEGETWIDHEIOFFHS			ZEOFFSETOFFHS + 15   /*获取数据大小失败*/
#define ZEGET2KMDATAOFFHS			ZEOFFSETOFFHS + 16   /*获取2km数据集数据失败*/

#define ZEWRITETXTAOFFHS			ZEOFFSETOFFHS + 17   /*写火点TXT列表失败*/
/*************************文件不存在 50**********************************/
#define ZENOTFILEOFFIRE				50
#define ZEIJTOLATLONOFFIRE   	ZENOTFILEOFFIRE + 1   /*读行列号转经纬度文件不存在或获取失败*/
#define ZESURFTYPEOFFIRE   		ZENOTFILEOFFIRE + 2   /*读地表类型文件不存在或获取失败*/
#define ZESATZENOFFIRE   			ZENOTFILEOFFIRE + 3   /*读卫星天顶角文件不存在或获取失败*/
#define ZENLANDIMGOFFIRE   		ZENOTFILEOFFIRE + 4   /*读全球植被文件不存在或获取失败*/
#define ZEXJRASTEROFFIRE   		ZENOTFILEOFFIRE + 5   /*读县界DAT文件不存在或获取失败*/
#define ZEXJTXTOFFIRE   			ZENOTFILEOFFIRE + 6   /*读县界TXT文件不存在或获取失败*/
#define ZELANDCOVEROFFIRE   	ZENOTFILEOFFIRE + 7   /*读地表类型文件不存在或获取失败*/
#define ZELANDIMGOFFIRE   		ZENOTFILEOFFIRE + 8   /*读landimg不存在或获取失败*/
#define ZE2KMHDFOFFIRE   			ZENOTFILEOFFIRE + 9   /*读2km分辨率HDF不存在或获取失败*/
#define ZE4KMHDFOFFIRE   			ZENOTFILEOFFIRE + 10   /*读4km分辨率HDF不存在或获取失败*/
#define ZETXTPATHOFFIRE   		ZENOTFILEOFFIRE + 11   /*TXT输入输出接口文件的目录不存在或获取失败*/
#define ZEPARAMOFFIRE   			ZENOTFILEOFFIRE + 12   /*系数文件不存在或获取失败*/
#define ZEGEOOFFIRE   				ZENOTFILEOFFIRE + 13   /*GEO文件不存在或获取失败*/
#define ZESFTFIRE   		      ZENOTFILEOFFIRE + 14   /*读SFT不存在或获取失败*/
/**************************图像操作（3880-3900）***************************/
#define ZEOFFSETOFJPEG					40
#define ZEOPENFLAGOFJPEG				ZEOFFSETOFJPEG + 1   /*打开JPEG标记文件失败*/
#define ZEOPENPOWEROFJPEG				ZEOFFSETOFJPEG + 2   /*打开JPEG强度文件失败*/
#define ZEWRITEFLAGOFJPEG				ZEOFFSETOFJPEG + 3   /*写JPEG标记文件失败*/
#define ZEWRITEPOWEROFJPEG			ZEOFFSETOFJPEG + 4   /*写JPEG强度文件失败*/


#define  FILL_VALUE          40
#define  OUTERSPACE          65535
#define  SATAZEN80           50      
    
#define  SUNZEN_MASK         65532
#define  OCEAN_MASK          65531
#define  LAND_MASK           65530
#define  SNOW_MASK           65529






#define ZEPROCESSOFFIRE			255   /*火点处理失败*/
#endif      /* #ifndef _PPS_FHS_ERROR_H */
