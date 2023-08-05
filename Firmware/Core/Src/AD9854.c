//=====================================================================
//                    AD9854 �����������

//AD9854.c
//�������ӹ�����
//˵�������������Ӳ������Ӿ���Ϊ20MHZ
//=====================================================================

#include "AD9854.h"      //STC��Ƭ��ͷ�ļ�	
#include "Usart1.h"


uchar FreqWord[6];				 //6���ֽ�Ƶ�ʿ�����

//**********************����Ϊϵͳʱ���Լ�����ر�������**************************

/* 
      �˴������Լ�����Ҫ����ϵͳʱ���Լ�������ص����ӣ�һ������ֻ�迪��һ��   
      CLK_SetΪʱ�ӱ�Ƶ���ã�������4~20����Ƶ��������ܳ���300MHZ
      Freq_mult_ulong��Freq_mult_doulle��Ϊ2��48�η�����ϵͳʱ�ӣ�һ��Ϊ�����Σ�һ��Ϊ˫������
*/

/*
#define      CLK_Set            4
const ulong  Freq_mult_ulong  = 3518437;
const double Freq_mult_doulle = 3518437.2088832;
*/

/*
#define      CLK_Set            5
const ulong  Freq_mult_ulong  = 2814750;
const double Freq_mult_doulle = 2814749.76710656;
*/

 /*
#define      CLK_Set            6
const ulong  Freq_mult_ulong  = 2345625;
const double Freq_mult_doulle = 2345624.80592213;
  */

/*
#define      CLK_Set            7
const ulong  Freq_mult_ulong  = 2010536;
const double Freq_mult_doulle = 2010535.54793326;
*/

/*
#define      CLK_Set            8
const ulong  Freq_mult_ulong  = 1759219;
const double Freq_mult_doulle = 1759218.6044416;
*/

/*
#define      CLK_Set            9
const ulong  Freq_mult_ulong  = 1563750;
const double Freq_mult_doulle = 1563749.87061476;
*/

/*
#define      CLK_Set            10
const ulong  Freq_mult_ulong  = 1407375;
const double Freq_mult_doulle = 1407374.88355328;
*/

/*
#define      CLK_Set            11
const ulong  Freq_mult_ulong  = 1279432;
const double Freq_mult_doulle = 1279431.712321164;
*/


#define      CLK_Set            0x4C   //0x0C 
const ulong  Freq_mult_ulong  = 1172812;
const double Freq_mult_doulle = 1172812.402961067;


/*
#define      CLK_Set            13
const ulong  Freq_mult_ulong  = 1082596;
const double Freq_mult_doulle = 1082596.064271754;
*/

/*
#define      CLK_Set            14
const ulong  Freq_mult_ulong  = 1005268;
const double Freq_mult_doulle = 1005267.773966629;
*/

/*
#define      CLK_Set            15
const ulong  Freq_mult_ulong  = 938250;
const double Freq_mult_doulle = 938249.9223688533;

 */
//**************************�޸�Ӳ��ʱҪ�޸ĵĲ���********************************



//**************************���²���Ϊ��������********************************

static void AD9854_WR_Byte(u8 addr,u8 dat);				  
static void Freq_convert(long Freq);	         	  
extern void AD9854_SetSine(ulong Freq,uint Shape);	  
static void Freq_double_convert(double Freq);		  
extern void AD9854_SetSine_double(double Freq,uint Shape);
extern void AD9854_InitFSK(void);				
extern void AD9854_SetFSK(ulong Freq1,ulong Freq2);					  
extern void AD9854_InitBPSK(void);					  
extern void AD9854_SetBPSK(uint Phase1,uint Phase2);					
extern void AD9854_InitOSK(void);					 
extern void AD9854_SetOSK(uchar RateShape);					  
extern void AD9854_InitAM(void);					 
extern void AD9854_SetAM(uint Shape);					
extern void AD9854_InitRFSK(void);					 
extern void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate);				


//====================================================================================
//��������:void AD9854_WR_Byte(uchar addr,uchar dat)
//��������:AD9854���п�д������
//��ڲ���:addr     6λ��ַ
//         dat      д�������
//���ڲ���:��
//====================================================================================
	u32 dat1,dat2;

void AD9854_WR_Byte(u8 addr,u8 dat)
{   //Ϊ������PA0~PA7�����ı���������״̬��Ӧ����BSRR�ĵ�16~23λ��1���Ӷ�����Ͱ�λ��Ȼ���ڰѵڰ�λ��������һ�¾Ϳ�����
	AD9854_DataBus |= 0x00FF0000;               //���PA0~PA7
	AD9854_DataBus |= dat;
	AD9854_AdrBus |= 0x01F80000;                       //���PB3~PB8    //0000 0001 1111 1000
	AD9854_AdrBus |= addr << 3;                         //��6λ  0~5 -> 3~8
	AD9854_WR      = 0;                         //Ȼ��WR�ϸ�һ�����壬�������ݵ�AD9854��Buffer
	AD9854_WR      = 1;
}

//====================================================================================
//��������:void AD9854_Init(void)
//��������:AD9854��ʼ��
//��ڲ���:��
//���ڲ���:��
// �����ʼ״̬�ɼ�AD9854�ֲ��Figure35
//====================================================================================
void AD9854_Init(void)
{
	AD9854_WR=1;                  //������д���ƶ˿���Ϊ��Ч
	AD9854_RD=1;
	AD9854_UDCLK=0;
	AD9854_RST=1;                 //��λAD9854
	AD9854_RST=0;

	AD9854_WR_Byte(0x1d,0x00);					//�򿪱Ƚ������翪�أ�QDAC���翪�أ�DAC���翪�أ����ֹ��翪�ء�
	//	AD9854_WR_Byte(0x1d,0x10);	    //�رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   		//����ϵͳʱ�ӱ�Ƶ        0C  0000_1100      ��Ƶ01100������12����Ƶ��240MHz�������Ѿ�����200MHz�ˣ�PLL range bitӦ��Ϊ1��Ҳ����Ӧ������Ϊ0100_1100 Ӧ����4C
	AD9854_WR_Byte(0x1f,0x00);	   			//����ϵͳΪģʽ0�����ⲿ����     ACC1 AC2 Triangle SRC_QADC��û���õ�
	AD9854_WR_Byte(0x20,0x60);	   			//����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����     //0110_0000   �ڲ���sinc�˲�����·���ˣ�Ȼ��OSK EN��Ϊһ�����Կ��Ʒ���  �����ʵ������һ����sinc�˲���

	AD9854_UDCLK=1;               			//����AD9854���
	AD9854_UDCLK=0;
}
//====================================================================================
//��������:void Freq_convert(long Freq)
//��������:�����ź�Ƶ������ת��
//��ڲ���:Freq   ��Ҫת����Ƶ�ʣ�ȡֵ��0~SYSCLK/2
//���ڲ���:��   ����Ӱ��ȫ�ֱ���FreqWord[6]��ֵ
//˵����   ���㷨λ���ֽ�����㷨���й�ʽFTW = (Desired Output Frequency �� 2N)/SYSCLK
//         �õ����㷨������N=48��Desired Output Frequency Ϊ����Ҫ��Ƶ�ʣ���Freq��SYSCLK
//         Ϊ�ɱ�̵�ϵͳʱ�ӣ�FTWΪ48Bit��Ƶ�ʿ����֣���FreqWord[6]
//====================================================================================
void Freq_convert(long Freq)   
{
	ulong FreqBuf;
	ulong Temp=Freq_mult_ulong;   	       

	uchar Array_Freq[4];			     //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�
	Array_Freq[0]=(uchar)Freq;
	Array_Freq[1]=(uchar)(Freq>>8);
	Array_Freq[2]=(uchar)(Freq>>16);
	Array_Freq[3]=(uchar)(Freq>>24);

	FreqBuf=Temp*Array_Freq[0];                  
	FreqWord[0]=FreqBuf;    
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[1]);
	FreqWord[1]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[2]);
	FreqWord[2]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[3]);
	FreqWord[3]=FreqBuf;
	FreqBuf>>=8;

	FreqWord[4]=FreqBuf;
  FreqWord[5]=FreqBuf>>8;			
}  

//====================================================================================
//��������:void AD9854_SetSine(ulong Freq,uint Shape)
//��������:AD9854���Ҳ���������
//��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~(1/2)*SYSCLK
//         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095) ,ȡֵԽ��,����Խ�� 
//���ڲ���:��
//====================================================================================
void AD9854_SetSine(ulong Freq,uint Shape)
{
	uchar count;
	uchar Adress;

	Adress = 0x04;                        //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

	Freq_convert(Freq);		           //Ƶ��ת��

	for(count=6;count>0;)	          //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress++,FreqWord[--count]);   //д��Ƶ��FTW1
	}

	AD9854_WR_Byte(0x21,Shape>>8);	  //����Iͨ������
	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));

	AD9854_WR_Byte(0x23,Shape>>8);	  //����Qͨ������
	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

	AD9854_UDCLK=1;                    //����AD9854���
	AD9854_UDCLK=0;
}

//====================================================================================
//��������:void Freq_doublt_convert(double Freq)
//��������:�����ź�Ƶ������ת��
//��ڲ���:Freq   ��Ҫת����Ƶ�ʣ�ȡֵ��0~SYSCLK/2
//���ڲ���:��   ����Ӱ��ȫ�ֱ���FreqWord[6]��ֵ
//˵����   �й�ʽFTW = (Desired Output Frequency �� 2N)/SYSCLK�õ��ú�����
//         ����N=48��Desired Output Frequency Ϊ����Ҫ��Ƶ�ʣ���Freq��SYSCLK
//         Ϊ�ɱ�̵�ϵͳʱ�ӣ�FTWΪ48Bit��Ƶ�ʿ����֣���FreqWord[6]
//ע�⣺   �ú��������溯��������Ϊ�ú�������ڲ���Ϊdouble����ʹ�źŵ�Ƶ�ʸ���ȷ
//         ������100HZ�����ñ��������ڸ���100HZ��������ú���void Freq_convert(long Freq)
//====================================================================================
void Freq_double_convert(double Freq)   
{
	ulong Low32;
	uint  High16;
	double Temp=Freq_mult_doulle;   	            //23ca99Ϊ2��48�η�����120M
	Freq*=(double)(Temp);
	//	1 0000 0000 0000 0000 0000 0000 0000 0000 = 4294967295
	High16 =(int)(Freq/4294967295);                  //2^32 = 4294967295
	Freq -= (double)High16*4294967295;
	Low32 = (ulong)Freq;

	FreqWord[0]=Low32;	     
	FreqWord[1]=Low32>>8;
	FreqWord[2]=Low32>>16;
	FreqWord[3]=Low32>>24;
	FreqWord[4]=High16;
	FreqWord[5]=High16>>8;			
} 
//====================================================================================
//��������:void AD9854_SetSine_double(double Freq,uint Shape)
//��������:AD9854���Ҳ���������
//��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~1/2*SYSCLK
//         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095) 
//���ڲ���:��
//====================================================================================
void AD9854_SetSine_double(double Freq,uint Shape)
{
	uchar count=0;
	uchar Adress;

	Adress=0x04;						     //ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ

	Freq_double_convert(Freq);		           //Ƶ��ת��

	for(count=6;count>0;)	                    //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
	}

	AD9854_WR_Byte(0x21,Shape>>8);	  //����Iͨ������
	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));

	AD9854_WR_Byte(0x23,Shape>>8);	  //����Qͨ������
	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

	AD9854_UDCLK=1;                    //����AD9854���
	AD9854_UDCLK=0;
}

//====================================================================================
//��������:void AD9854_InitFSK(void)
//��������:AD9854��FSK��ʼ��
//��ڲ���:��
//���ڲ���:��
// �����ǳ�ʼ����FSK���ѣ���û������FSK��FTW1��FTW2��
//====================================================================================
void AD9854_InitFSK(void)
{
	AD9854_WR=1;                        //������д���ƶ˿���Ϊ��Ч
	AD9854_RD=1;
	AD9854_UDCLK=0;
	AD9854_RST=1;                        //��λAD9854
	AD9854_RST=0;	

	AD9854_WR_Byte(0x1d,0x00);	       //�򿪱Ƚ���   
	AD9854_WR_Byte(0x1e,CLK_Set);	   //����ϵͳʱ�ӱ�Ƶ  
	AD9854_WR_Byte(0x1f,0x02);	       //����ϵͳΪģʽ1�����ⲿ����   
	AD9854_WR_Byte(0x20,0x60);	      //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����  

	AD9854_UDCLK=1;                  //����AD9854���
	AD9854_UDCLK=0;
}

//====================================================================================
//��������:void AD9854_SetFSK(ulong Freq1,ulong Freq2)
//��������:AD9854��FSK����
//��ڲ���:Freq1   FSKƵ��1
//         Freq2   FSKƵ��2
//���ڲ���:��
// ����Freq1����С��Freq2
//====================================================================================
void AD9854_SetFSK(ulong Freq1,ulong Freq2)
{
	uchar count=6;
	uchar Adress1,Adress2;

	const uint Shape=4000;	      //��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

	Adress1=0x04;				 //ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ
	Adress2=0x0a;				 //ѡ��Ƶ�ʿ�����2��ַ�ĳ�ֵ

	Freq_convert(Freq1);               //Ƶ��ת��1

	for(count=6;count>0;)	          //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
	}

	Freq_convert(Freq2);               //Ƶ��ת��2

	for(count=6;count>0;)	          //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress2++,FreqWord[--count]);
	}

	AD9854_WR_Byte(0x21,Shape>>8);	      //����Iͨ������
	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));

	AD9854_WR_Byte(0x23,Shape>>8);	     //����Qͨ������
	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

	AD9854_UDCLK=1;                    //����AD9854���
	AD9854_UDCLK=0;		
}

//====================================================================================
//��������:void AD9854_InitBPSK(void)
//��������:AD9854��BPSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitBPSK(void)
{
	AD9854_WR=1;                    //������д���ƶ˿���Ϊ��Ч
	AD9854_RD=1;
	AD9854_UDCLK=0;
	AD9854_RST=1;                   //��λAD9854
	AD9854_RST=0;

	AD9854_WR_Byte(0x1d,0x00);	       //�򿪱Ƚ������������
	AD9854_WR_Byte(0x1e,CLK_Set);	   //����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x08);	      //����ϵͳΪģʽ4�����ⲿ����
	AD9854_WR_Byte(0x20,0x60);	      //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

	AD9854_UDCLK=1;                //����AD9854���
	AD9854_UDCLK=0;
}

//====================================================================================
//��������:void AD9854_SetBPSK(uint Phase1,uint Phase2)
//��������:AD9854��BPSK����
//��ڲ���:Phase1   ������λ1
//         Phase2	������λ2
//���ڲ���:��
//˵����   ��λΪ14Bit��ȡֵ��0~16383���������ñ�������ʱ��Phase1����Ϊ0��
//         ��Phase1����Ϊ8192��180����λ
//====================================================================================
void AD9854_SetBPSK(uint Phase1,uint Phase2)
{
	uchar count;

	const ulong Freq=60000;
	const uint Shape=4000;

	uchar Adress;
	Adress=0x04;                           //ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ

	AD9854_WR_Byte(0x00,Phase1>>8);	           //������λ1
	AD9854_WR_Byte(0x01,(uchar)(Phase1&0xff));

	AD9854_WR_Byte(0x02,Phase2>>8);	          //������λ2
	AD9854_WR_Byte(0x03,(uchar)(Phase2&0xff));

	Freq_convert(Freq);                            //Ƶ��ת��

	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
	}

	AD9854_WR_Byte(0x21,Shape>>8);	                  //����Iͨ������
	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));

	AD9854_WR_Byte(0x23,Shape>>8);	               //����Qͨ������
	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

	AD9854_UDCLK=1;                                //����AD9854���
	AD9854_UDCLK=0;	
}

//====================================================================================
//��������:void AD9854_InitOSK(void)
//��������:AD9854��OSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitOSK(void)
{
	AD9854_WR=1;                           //������д���ƶ˿���Ϊ��Ч
	AD9854_RD=1;
	AD9854_UDCLK=0;
	AD9854_RST=1;                          //��λAD9854
	AD9854_RST=0;

	AD9854_WR_Byte(0x1d,0x00);	           //�򿪱Ƚ���
	AD9854_WR_Byte(0x1e,CLK_Set);	       //����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x00);	           //����ϵͳΪģʽ0�����ⲿ����
	AD9854_WR_Byte(0x20,0x70);	           //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,ͨ�������ڲ�����    0111_0000   OSKб���ڲ����Σ��������Ϊ0110_0000����ô������I·ͨ����Q·ͨ����������  �����������ó�AMģʽ��

	AD9854_UDCLK=1;                        //����AD9854���
	AD9854_UDCLK=0;
}

//====================================================================================
//��������:void AD9854_SetOSK(uchar RateShape)
//��������:AD9854��OSK����
//��ڲ���: RateShape    OSKб��,ȡֵΪ4~255��С��4����Ч
//���ڲ���:��
//��Ҫ������OSK EN bit��OSK INT
//====================================================================================
void AD9854_SetOSK(uchar RateShape)
{
	uchar count;

	const ulong Freq=60000;			 //������Ƶ
	const uint  Shape=4000;			//��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

	uchar Adress;
	Adress=0x04;               //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

	Freq_convert(Freq);                       //Ƶ��ת��

	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
	}

	AD9854_WR_Byte(0x21,Shape>>8);	                  //����Iͨ������
	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));

	AD9854_WR_Byte(0x23,Shape>>8);	                  //����Qͨ������
	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));  	 


	AD9854_WR_Byte(0x25,RateShape);				       //����OSKб��

	AD9854_UDCLK=1;                                //����AD9854���
	AD9854_UDCLK=0;	
}

//====================================================================================
//��������:void AD9854_InitAM(void)
//��������:AD9854��AM��ʼ��
//��ڲ���:��
//���ڲ���:��
//������ȫ��ΪAMģʽ������ֻ���Ƿ��������Ȼ����Լ����õ�
//====================================================================================
void AD9854_InitAM(void)
{
	uchar count;

	const ulong Freq=60000;			 //������Ƶ

	uchar  Adress;
	Adress=0x04;      //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

	AD9854_WR=1;    //������д���ƶ˿���Ϊ��Ч
	AD9854_RD=1;
	AD9854_UDCLK=0;
	AD9854_RST=1;     //��λAD9854
	AD9854_RST=0;

	AD9854_WR_Byte(0x1d,0x00);	                  //�򿪱Ƚ���
	AD9854_WR_Byte(0x1e,CLK_Set);	             //����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x00);	                 //����ϵͳΪģʽ0�����ⲿ����
	AD9854_WR_Byte(0x20,0x60);	                  //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

	Freq_convert(Freq);                            //Ƶ��ת��

	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
	}

	AD9854_UDCLK=1;                             //����AD9854���
	AD9854_UDCLK=0;
}

//====================================================================================
//��������:void AD9854_SetAM(uchar Shape)
//��������:AD9854��AM����
//��ڲ���:Shape   12Bit����,ȡֵ��0~4095   
//���ڲ���:��
//====================================================================================
void AD9854_SetAM(uint Shape)
{
	AD9854_WR_Byte(0x21,Shape>>8);	                  //����Iͨ������
	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));

	AD9854_WR_Byte(0x23,Shape>>8);	                  //����Qͨ������
	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

	AD9854_UDCLK=1;                                   //����AD9854���
	AD9854_UDCLK=0;			
}

//====================================================================================
//��������:void AD9854_InitRFSK(void)
//��������:AD9854��RFSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//б��FSKģʽ����������Triange������˫б��chirp��Ƶ���ɵͱ�ߣ����ɸ߱�ͣ�ֻ�������FSK����û��ʲô��ϵ��
//====================================================================================
void AD9854_InitRFSK(void)
{
	AD9854_WR=1;                      //������д���ƶ˿���Ϊ��Ч
	AD9854_RD=1;
	AD9854_UDCLK=0;
	AD9854_RST=1;                     //��λAD9854
	AD9854_RST=0;

	AD9854_WR_Byte(0x1d,0x00);	       //�򿪱Ƚ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   //����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x24);	        //����ϵͳΪģʽ2�����ⲿ����,ʹ�����ǲ�ɨƵ����   0010_0100  Triangleʹ�ܣ�010ģʽ��
	AD9854_WR_Byte(0x20,0x60);	        //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����	

	AD9854_UDCLK=1;                   //����AD9854���
	AD9854_UDCLK=0;
}

//====================================================================================
//��������:void AD9854_SetRFSK(void)
//��������:AD9854��RFSK����
//��ڲ���:Freq_Low          RFSK��Ƶ��	   48Bit
//         Freq_High         RFSK��Ƶ��	   48Bit
//         Freq_Up_Down		 ����Ƶ��	   48Bit
//		   FreRate           б��ʱ�ӿ���  20Bit
//���ڲ���:��
//ע��     ÿ��������֮���ʱ����������ʽ��ʾ��FreRate +1��*��System Clock ����һ������,
//         Ƶ�� ���������½� һ������Ƶ��
//   ����ϵͳʱ��300MHz����ôһ�����300M��Sample�����FreRate��1��һ�������Ȼ��300M��Sample�������2����ôֻ���150M��Sample
//====================================================================================
void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate)
{
	uchar count=6;
	uchar Adress1,Adress2,Adress3;
	const uint  Shape=4000;			   //��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

	Adress1=0x04;		     //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ 
	Adress2=0x0a;
	Adress3=0x10;

	Freq_convert(Freq_Low);                             //Ƶ��1ת��

	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
	}

	Freq_convert(Freq_High);                             //Ƶ��2ת��

	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress2++,FreqWord[--count]);
	}

	Freq_convert(Freq_Up_Down);                             //����Ƶ��ת��

	for(count=6;count>0;)	                               //д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress3++,FreqWord[--count]);
	}

	// ���õ�Ramp Rate Clock��20��bit
	AD9854_WR_Byte(0x1a,(uchar)((FreRate>>16)&0x0f));				   //����б������
	AD9854_WR_Byte(0x1b,(uchar)(FreRate>>8));
	AD9854_WR_Byte(0x1c,(uchar)FreRate);				    

	AD9854_WR_Byte(0x21,Shape>>8);	                  //����Iͨ������
	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));

	AD9854_WR_Byte(0x23,Shape>>8);	                  //����Qͨ������
	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

	AD9854_UDCLK=1;                                //����AD9854���
	AD9854_UDCLK=0;	
}

uint8_t AD9854_Uartdata_Analysis(struct Uart_Data_Struc *uart_data_struc)
{
	uint8_t current_index=0;                        //���ڱ�־Ŀǰ��ȡ�Ĵ���λ��
	uint8_t temp_i=0;                               //����Forѭ��
	memset(uart_data_struc,0,sizeof(*uart_data_struc));        //����ṹ�壬��ֹ��λ��ֵ��ʱ���������
	if(USART1_Data_Recv[current_index]==0x00)             //û���յ�����
		return 0;               
	else if(USART1_Data_Recv[current_index]!=0x55)        //�յ����ݵ�����0x55
		return 1;    //Э��ͷ����
	else ;
	current_index++;         //��1����   ����
	uart_data_struc->cmd = USART1_Data_Recv[current_index];    // ��cmd������
	current_index++;        //��2���֣������ݲ�����
	
	switch( uart_data_struc->cmd)       //����cmd���������ݸ�˭��ֵ
	{
		// ����1   ��Ƶ�����
		case (0x01):
			for(temp_i=0;temp_i<4;temp_i++)          //����1 4��byte  Ƶ��1  1-150MHz   48bit
			{
				uart_data_struc->Data1.Freq1 = ( (uart_data_struc->Data1.Freq1)| USART1_Data_Recv[current_index]<<8*(4-1-temp_i));
				current_index++;
			}
			for(temp_i=0;temp_i<2;temp_i++)         //����2 2��byte  ���� 1-4095    12bit
			{
				uart_data_struc->Data2.Amplitude = ( (uart_data_struc->Data2.Amplitude)| USART1_Data_Recv[current_index]<<8*(2-1-temp_i));
				current_index++;
			}
			if(USART1_Data_Recv[current_index]!=0xCC)
				return 2;                       //����β����
			break;

		// ����2   FSK���
		case 0x02:
			for(temp_i=0;temp_i<4;temp_i++)          //����1 4��byte  Ƶ��1  1-150MHz   48bit
			{
				uart_data_struc->Data1.Freq1 = ( (uart_data_struc->Data1.Freq1)| USART1_Data_Recv[current_index]<<8*(4-1-temp_i));
				current_index++;
			}
			for(temp_i=0;temp_i<4;temp_i++)          //����1 4��byte  Ƶ��1  1-150MHz   48bit
			{
				uart_data_struc->Data2.Freq2 = ( (uart_data_struc->Data2.Freq2)| USART1_Data_Recv[current_index]<<8*(4-1-temp_i));
				current_index++;
			}
			if(USART1_Data_Recv[current_index]!=0xCC)
				return 2;                       //����β����
			break;

		// ����3   BPSK���
		case 0x03:
			for(temp_i=0;temp_i<2;temp_i++)          //����1 2��byte   ��λ1   1-16383  14bit
			{
				uart_data_struc->Data1.Phase1 = ( (uart_data_struc->Data1.Phase1)| USART1_Data_Recv[current_index]<<8*(2-1-temp_i));
				current_index++;
			}
			for(temp_i=0;temp_i<2;temp_i++)         //����2 2��byte      ��λ2   1-16383   14bit
			{
				uart_data_struc->Data2.Phase2 = ( (uart_data_struc->Data2.Phase2)| USART1_Data_Recv[current_index]<<8*(2-1-temp_i));
				current_index++;
			}
			if(USART1_Data_Recv[current_index]!=0xCC)
				return 2;                       //����β����
			break;

		// ����4   OSK���
		case 0x04:
			for(temp_i=0;temp_i<1;temp_i++)          //����1 1��byte   OSKб��  0-255  8bit
			{
				uart_data_struc->Data1.OSK_Slop = ( (uart_data_struc->Data1.OSK_Slop)| USART1_Data_Recv[current_index]<<8*(1-1-temp_i));
				current_index++;
			}
			if(USART1_Data_Recv[current_index]!=0xCC)
				return 2;                       //����β����
			break;

		// ����5   AM���
		case 0x05:
			for(temp_i=0;temp_i<2;temp_i++)          //����1 2��byte    ����   1-4095  12bit
			{
				uart_data_struc->Data1.AM_Amplitude = ( (uart_data_struc->Data1.AM_Amplitude)| USART1_Data_Recv[current_index]<<8*(2-1-temp_i));
				current_index++;
			}
			if(USART1_Data_Recv[current_index]!=0xCC)
				return 2;                       //����β����
			break;

		// ����6   RFSK���
		case 0x06:
			for(temp_i=0;temp_i<4;temp_i++)          //����1 4��byte    ��Ƶ��  1-150MHz   48bit
			{
				uart_data_struc->Data1.Freq_Low = ( (uart_data_struc->Data1.Freq_Low)| USART1_Data_Recv[current_index]<<8*(4-1-temp_i));
				current_index++;
			}
			for(temp_i=0;temp_i<4;temp_i++)         //����2 4��byte     ��Ƶ��  1-150MHz   48bit
			{
				uart_data_struc->Data2.Freq_High = ( (uart_data_struc->Data2.Freq_High)| USART1_Data_Recv[current_index]<<8*(4-1-temp_i));
				current_index++;
			}
			for(temp_i=0;temp_i<4;temp_i++)         //����3 4��byte     ����Ƶ��   1-150MHz   48bit
			{
				uart_data_struc->StepFreq = ( (uart_data_struc->StepFreq)| USART1_Data_Recv[current_index]<<8*(4-1-temp_i));
				current_index++;
			}
			for(temp_i=0;temp_i<3;temp_i++)         //����4 3��byte   Ƶ��ʱ�ӿ���  20bit
			{
				uart_data_struc->Clock_Freq_Control = ( (uart_data_struc->Clock_Freq_Control)| USART1_Data_Recv[current_index]<<8*(3-1-temp_i));
				current_index++;
			}
			if(USART1_Data_Recv[current_index]!=0xCC)
				return 2;                       //����β����
			break;

		default:return 0;
	}
	return 0;
}


void AD9854_Cmd_Execute(struct Uart_Data_Struc *uart_data_struc)
{
	switch(uart_data_struc->cmd)
	{
		case 0x01:
			AD9854_SetSine(uart_data_struc->Data1.Freq1,uart_data_struc->Data2.Amplitude); 
			break;
		case 0x02:
			AD9854_InitFSK();
			AD9854_SetFSK(uart_data_struc->Data1.Freq1,uart_data_struc->Data2.Freq2);
			break;
		case 0x03:
			AD9854_InitBPSK();
			AD9854_SetBPSK(uart_data_struc->Data1.Phase1,uart_data_struc->Data2.Phase2);
			break;
		case 0x04:
			AD9854_InitOSK();
			AD9854_SetOSK(uart_data_struc->Data1.OSK_Slop);
			break;
		case 0x05:
			AD9854_InitAM();
			AD9854_SetAM(uart_data_struc->Data1.AM_Amplitude);
			break;
		case 0x06:
			AD9854_InitRFSK();
			AD9854_SetRFSK(uart_data_struc->Data1.Freq_Low,uart_data_struc->Data2.Freq_High,uart_data_struc->StepFreq,uart_data_struc->Clock_Freq_Control);
			break;
		default: break;
	}
}
