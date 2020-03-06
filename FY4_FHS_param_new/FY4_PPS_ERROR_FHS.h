#ifndef _PPS_FHS_ERROR_H 
#define _PPS_FHS_ERROR_H

#define  ZESUCCESSFHS        0         /* ִ�гɹ�           */

#define  ZEOFFSETOFFHS         10       /* ������ʼƫ������Ϣ  ��Դ���⣨3810-3900��*/



#define ZEFIREPARA						ZEOFFSETOFFHS + 1   /*ִ�в�����������*/
#define ZELOGNINITOFFIRE			ZEOFFSETOFFHS + 2   /*��־��ʼ��ʧ��*/
#define ZECONFIGWRITE					ZEOFFSETOFFHS + 3   /*дTXT��������ӿ��ļ�ʧ��*/
#define ZECONFIGOPEN					ZEOFFSETOFFHS + 4   /*��TXT��������ӿ��ļ�ʧ��*/
#define ZEOKWRITE							ZEOFFSETOFFHS + 5   /*дOK�ļ�ʧ��*/
#define ZEOKOPEN							ZEOFFSETOFFHS + 6   /*��OK�ļ�ʧ��*/
#define ZEHDFOPEN							ZEOFFSETOFFHS + 7   /*��HDF�ļ�ʧ��*/
#define ZEHDFWRITE						ZEOFFSETOFFHS + 8   /*дHDF�ļ�ʧ��*/
#define ZENCWRITE 						ZEOFFSETOFFHS + 9   /*дNC�ļ�ʧ��*/
#define ZESDSOFNCWRITE 				ZEOFFSETOFFHS + 10   /*дNC���ݼ��ļ�ʧ��*/
#define ZEATTOFNCWRITE 				ZEOFFSETOFFHS + 11   /*дNCȫ�������ļ�ʧ��*/

#define ZEREADOFFIRE   				ZEOFFSETOFFHS + 12   /*�������ļ�ʧ��*/
#define ZEREADCLMOFFIRE   		ZEOFFSETOFFHS + 13   /*��ȡCLM�����ļ�ʧ��*/

#define ZEREAD2KMHDFATT   		ZEOFFSETOFFHS + 14   /*��ȡ2km�ļ�����ʧ��*/

#define ZEGETWIDHEIOFFHS			ZEOFFSETOFFHS + 15   /*��ȡ���ݴ�Сʧ��*/
#define ZEGET2KMDATAOFFHS			ZEOFFSETOFFHS + 16   /*��ȡ2km���ݼ�����ʧ��*/

#define ZEWRITETXTAOFFHS			ZEOFFSETOFFHS + 17   /*д���TXT�б�ʧ��*/
/*************************�ļ������� 50**********************************/
#define ZENOTFILEOFFIRE				50
#define ZEIJTOLATLONOFFIRE   	ZENOTFILEOFFIRE + 1   /*�����к�ת��γ���ļ������ڻ��ȡʧ��*/
#define ZESURFTYPEOFFIRE   		ZENOTFILEOFFIRE + 2   /*���ر������ļ������ڻ��ȡʧ��*/
#define ZESATZENOFFIRE   			ZENOTFILEOFFIRE + 3   /*�������춥���ļ������ڻ��ȡʧ��*/
#define ZENLANDIMGOFFIRE   		ZENOTFILEOFFIRE + 4   /*��ȫ��ֲ���ļ������ڻ��ȡʧ��*/
#define ZEXJRASTEROFFIRE   		ZENOTFILEOFFIRE + 5   /*���ؽ�DAT�ļ������ڻ��ȡʧ��*/
#define ZEXJTXTOFFIRE   			ZENOTFILEOFFIRE + 6   /*���ؽ�TXT�ļ������ڻ��ȡʧ��*/
#define ZELANDCOVEROFFIRE   	ZENOTFILEOFFIRE + 7   /*���ر������ļ������ڻ��ȡʧ��*/
#define ZELANDIMGOFFIRE   		ZENOTFILEOFFIRE + 8   /*��landimg�����ڻ��ȡʧ��*/
#define ZE2KMHDFOFFIRE   			ZENOTFILEOFFIRE + 9   /*��2km�ֱ���HDF�����ڻ��ȡʧ��*/
#define ZE4KMHDFOFFIRE   			ZENOTFILEOFFIRE + 10   /*��4km�ֱ���HDF�����ڻ��ȡʧ��*/
#define ZETXTPATHOFFIRE   		ZENOTFILEOFFIRE + 11   /*TXT��������ӿ��ļ���Ŀ¼�����ڻ��ȡʧ��*/
#define ZEPARAMOFFIRE   			ZENOTFILEOFFIRE + 12   /*ϵ���ļ������ڻ��ȡʧ��*/
#define ZEGEOOFFIRE   				ZENOTFILEOFFIRE + 13   /*GEO�ļ������ڻ��ȡʧ��*/
#define ZESFTFIRE   		      ZENOTFILEOFFIRE + 14   /*��SFT�����ڻ��ȡʧ��*/
/**************************ͼ�������3880-3900��***************************/
#define ZEOFFSETOFJPEG					40
#define ZEOPENFLAGOFJPEG				ZEOFFSETOFJPEG + 1   /*��JPEG����ļ�ʧ��*/
#define ZEOPENPOWEROFJPEG				ZEOFFSETOFJPEG + 2   /*��JPEGǿ���ļ�ʧ��*/
#define ZEWRITEFLAGOFJPEG				ZEOFFSETOFJPEG + 3   /*дJPEG����ļ�ʧ��*/
#define ZEWRITEPOWEROFJPEG			ZEOFFSETOFJPEG + 4   /*дJPEGǿ���ļ�ʧ��*/


#define  FILL_VALUE          40
#define  OUTERSPACE          65535
#define  SATAZEN80           50      
    
#define  SUNZEN_MASK         65532
#define  OCEAN_MASK          65531
#define  LAND_MASK           65530
#define  SNOW_MASK           65529






#define ZEPROCESSOFFIRE			255   /*��㴦��ʧ��*/
#endif      /* #ifndef _PPS_FHS_ERROR_H */
