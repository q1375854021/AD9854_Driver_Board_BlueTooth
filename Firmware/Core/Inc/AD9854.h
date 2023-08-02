#ifndef __AD9854_H
#define __AD9854_H	 
#include "main.h"
#include "sys.h"



#define AD9854_RST    PAout(11)   //AD9854复位端口
#define AD9854_UDCLK  PAout(12)   //AD9854更新时钟
#define AD9854_WR     PAout(15)  //AD9854写使能，低有效
#define AD9854_RD     PBout(0)   //AD9854读使能，低有效
#define AD9854_OSK    PBout(1)   //AD9854 OSK控制端
#define AD9854_FDATA  PBout(11)   //AD9854 FSK,PSK控制

#define AD9854_DataBus GPIOA->BSRR	//PA7~PA9
#define AD9854_AdrBus  GPIOB->BSRR	//PB3_PB8

#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long

struct Uart_Data_Struc
{
	uint8_t cmd;         //用于存放命令字
	union{
		uint32_t Freq1;         //频率1
		uint16_t Phase1;          //相位1
		uint8_t OSK_Slop;         //OSK斜率
		uint16_t AM_Amplitude;    //AM幅度
		uint32_t Freq_Low;      //低频率
	}Data1;

	union{
		uint16_t Amplitude;      //幅度
		uint32_t Freq2;        //频率2
		uint16_t Phase2;         //相位2
		uint32_t Freq_High;    //高频率
	}Data2;

	uint32_t StepFreq;       //步进频率
	
	uint32_t Clock_Freq_Control;      //频率时钟控制
};
	
	


typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

void AD9854_Init(void);
static void Freq_convert(long Freq);	         	  
extern void AD9854_SetSine(ulong Freq,uint Shape);	              //输出正弦波  输入频率（整数）  和   12bit幅度
static void Freq_double_convert(double Freq);		  
extern void AD9854_SetSine_double(double Freq,uint Shape);        //输出正弦波  输入频率（double）  和   12bit幅度
extern void AD9854_InitFSK(void);				                          //FSK初始化
extern void AD9854_SetFSK(ulong Freq1,ulong Freq2);					      //FSK输出     输入频率1   输入频率2
extern void AD9854_InitBPSK(void);					                      //BPSK初始化
extern void AD9854_SetBPSK(uint Phase1,uint Phase2);					    //BPSK相位1     BPSK相位2
extern void AD9854_InitOSK(void);					                        //OSK初始化
extern void AD9854_SetOSK(uchar RateShape);					              //设置OSK的斜率
extern void AD9854_InitAM(void);					                        //AM初始化
extern void AD9854_SetAM(uint Shape);					                    //设置AM的幅度
extern void AD9854_InitRFSK(void);					                      //RFSK初始化
extern void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate);		    //太长，具体到函数实现看吧

//自己编写的函数
uint8_t AD9854_Uartdata_Analysis(struct Uart_Data_Struc *uart_data_struc);
void AD9854_Cmd_Execute(struct Uart_Data_Struc *uart_data_struc);
#endif

