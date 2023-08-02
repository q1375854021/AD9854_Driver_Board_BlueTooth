/**********************************************************
* @ File name -> sys.h
* @ Version   -> V1.0
* @ Date      -> 12-26-2013
* @ Brief     -> ϵͳ������صĺ���ͷ�ļ�
**********************************************************/

#ifndef _sys_h_
#define _sys_h_

/**********************************************************
                     �ⲿ����ͷ�ļ�                        
**********************************************************/
	

#include "string.h"
#include "stdio.h"


/**********************************************************
                    λ��������غ궨��
              �ο���CM3Ȩ��ָ�ϡ���87 ~ 92ҳ
**********************************************************/

#define BITBAND(addr, bitnum)			((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)					*((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)			MEM_ADDR(BITBAND(addr, bitnum)) 

/**********************************************************
                       GPIO��ַӳ��
              ����ַ���ϼĴ���ƫ�Ƶ�ַ���
**********************************************************/

#define GPIOA_ODR_Addr    (GPIOA_BASE+12)	//0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12)	//0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12)	//0x4001100C 


#define GPIOA_IDR_Addr    (GPIOA_BASE+8)	//0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)	//0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)	//0x40011008 


/**********************************************************
             ʵ�ֵ�һIO������������51��IO����
                   nֵҪС��IO������Ŀ
**********************************************************/ 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#endif

