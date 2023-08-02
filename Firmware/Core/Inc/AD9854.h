#ifndef __AD9854_H
#define __AD9854_H	 
#include "main.h"
#include "sys.h"



#define AD9854_RST    PAout(11)   //AD9854��λ�˿�
#define AD9854_UDCLK  PAout(12)   //AD9854����ʱ��
#define AD9854_WR     PAout(15)  //AD9854дʹ�ܣ�����Ч
#define AD9854_RD     PBout(0)   //AD9854��ʹ�ܣ�����Ч
#define AD9854_OSK    PBout(1)   //AD9854 OSK���ƶ�
#define AD9854_FDATA  PBout(11)   //AD9854 FSK,PSK����

#define AD9854_DataBus GPIOA->BSRR	//PA7~PA9
#define AD9854_AdrBus  GPIOB->BSRR	//PB3_PB8

#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long

struct Uart_Data_Struc
{
	uint8_t cmd;         //���ڴ��������
	union{
		uint32_t Freq1;         //Ƶ��1
		uint16_t Phase1;          //��λ1
		uint8_t OSK_Slop;         //OSKб��
		uint16_t AM_Amplitude;    //AM����
		uint32_t Freq_Low;      //��Ƶ��
	}Data1;

	union{
		uint16_t Amplitude;      //����
		uint32_t Freq2;        //Ƶ��2
		uint16_t Phase2;         //��λ2
		uint32_t Freq_High;    //��Ƶ��
	}Data2;

	uint32_t StepFreq;       //����Ƶ��
	
	uint32_t Clock_Freq_Control;      //Ƶ��ʱ�ӿ���
};
	
	


typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

void AD9854_Init(void);
static void Freq_convert(long Freq);	         	  
extern void AD9854_SetSine(ulong Freq,uint Shape);	              //������Ҳ�  ����Ƶ�ʣ�������  ��   12bit����
static void Freq_double_convert(double Freq);		  
extern void AD9854_SetSine_double(double Freq,uint Shape);        //������Ҳ�  ����Ƶ�ʣ�double��  ��   12bit����
extern void AD9854_InitFSK(void);				                          //FSK��ʼ��
extern void AD9854_SetFSK(ulong Freq1,ulong Freq2);					      //FSK���     ����Ƶ��1   ����Ƶ��2
extern void AD9854_InitBPSK(void);					                      //BPSK��ʼ��
extern void AD9854_SetBPSK(uint Phase1,uint Phase2);					    //BPSK��λ1     BPSK��λ2
extern void AD9854_InitOSK(void);					                        //OSK��ʼ��
extern void AD9854_SetOSK(uchar RateShape);					              //����OSK��б��
extern void AD9854_InitAM(void);					                        //AM��ʼ��
extern void AD9854_SetAM(uint Shape);					                    //����AM�ķ���
extern void AD9854_InitRFSK(void);					                      //RFSK��ʼ��
extern void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate);		    //̫�������嵽����ʵ�ֿ���

//�Լ���д�ĺ���
uint8_t AD9854_Uartdata_Analysis(struct Uart_Data_Struc *uart_data_struc);
void AD9854_Cmd_Execute(struct Uart_Data_Struc *uart_data_struc);
#endif

