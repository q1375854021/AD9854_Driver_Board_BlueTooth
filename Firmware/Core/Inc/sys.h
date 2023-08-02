/**********************************************************
* @ File name -> sys.h
* @ Version   -> V1.0
* @ Date      -> 12-26-2013
* @ Brief     -> 系统设置相关的函数头文件
**********************************************************/

#ifndef _sys_h_
#define _sys_h_

/**********************************************************
                     外部函数头文件                        
**********************************************************/
	

#include "string.h"
#include "stdio.h"


/**********************************************************
                    位带操作相关宏定义
              参考《CM3权威指南》第87 ~ 92页
**********************************************************/

#define BITBAND(addr, bitnum)			((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)					*((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)			MEM_ADDR(BITBAND(addr, bitnum)) 

/**********************************************************
                       GPIO地址映射
              基地址加上寄存器偏移地址组成
**********************************************************/

#define GPIOA_ODR_Addr    (GPIOA_BASE+12)	//0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12)	//0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12)	//0x4001100C 


#define GPIOA_IDR_Addr    (GPIOA_BASE+8)	//0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)	//0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)	//0x40011008 


/**********************************************************
             实现单一IO操作，类似于51的IO操作
                   n值要小于IO具体数目
**********************************************************/ 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#endif

