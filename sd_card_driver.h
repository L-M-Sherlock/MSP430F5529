#ifndef SD_CARD_DRIVER_H_
#define SD_CARD_DRIVER_H_
#include "utils.h"
#include <msp430.h>
#include "dr_sdcard/ff.h"
#include <string.h>
#include <stdio.h>
#include <string.h>
// Pins from MSP430 connected to the SD Card
#define SPI_SIMO        BIT1
#define SPI_SOMI        BIT2
#define SPI_CLK         BIT3
#define SD_CS           BIT0

// Ports
#define SPI_SEL         P4SEL
#define SPI_DIR         P4DIR
#define SPI_OUT         P4OUT
#define SPI_REN         P4REN
#define SD_CS_SEL       P4SEL
#define SD_CS_OUT       P4OUT
#define SD_CS_DIR       P4DIR
#define MCLK_FREQ 20000000

unsigned char  SPI_WriteByte(unsigned char val)
{
 	while(!(UCB1IFG & UCTXIFG)); //�ȴ����ͻ�������
 	UCB1IFG&=~UCTXIFG;
 	UCB1TXBUF = val; //���͵�λ
 	while(UCB1STAT & UCBBUSY); //�ȴ����һλʵ���ͳ�
 	__delay_cycles(MCLK_FREQ / 1000*3);
	return(UCB1RXBUF);

}

//*****************���������****************************
unsigned char SD_SendCommand(unsigned char cmd,unsigned long arg)
{
 unsigned char r1;
 unsigned char retry1=0;      //�ظ���������
 SPI_WriteByte(cmd|0x40);   //�ֱ�д������
 SPI_WriteByte(arg>>24);      //���ݶε�4�ֽ�
 SPI_WriteByte(arg>>16);      //���ݶε�3�ֽ�
 SPI_WriteByte(arg>>8);       //���ݶε�2�ֽ�
 SPI_WriteByte(arg);          //���ݶε�1�ֽ�
 SPI_WriteByte(0x95);         //CRCЧ���
 while((r1=SPI_WriteByte(0xff))==0xff)
	 if(retry1++>200)break;                     //���Ƭѡ�ź�
 return r1;                   //����״ֵ̬
}

unsigned char SD_SendCommand1(unsigned char cmd,unsigned long arg)
{
 unsigned char r1;
 unsigned char retry1=0;      //�ظ���������
 SPI_WriteByte(cmd|0x40);   //�ֱ�д������
 SPI_WriteByte(arg>>24);      //���ݶε�4�ֽ�
 SPI_WriteByte(arg>>16);      //���ݶε�3�ֽ�
 SPI_WriteByte(arg>>8);       //���ݶε�2�ֽ�
 SPI_WriteByte(arg);          //���ݶε�1�ֽ�
 SPI_WriteByte(0xff);         //CRCЧ���
 while((r1=SPI_WriteByte(0xff))==0xff)
	 if(retry1++>200)break;                     //���Ƭѡ�ź�
 return r1;                   //����״ֵ̬
}

unsigned char sd_reset()
{
	unsigned char i;
	unsigned char retry;            		//�ظ�����
	unsigned char r1=0;
	retry=0;
	do
	{
		SD_CS_OUT |= SD_CS;				//Ƭѡ�ź�CS�ø�
		for(i=0;i<20;i++)					//����74+ʱ���ź�
		{
		   SPI_WriteByte(0xff);
		}
		SD_CS_OUT &= ~SD_CS;			//Ƭѡ�ź�CS����
		r1 = SD_SendCommand(0,0);	//��CMD0�������ȡӦ������Ӧ��Ϊ0x01
		retry++;
		if(retry>200) return 1;   //��ʱ�˳�
	} while(r1 != 0x01); //�ȴ�IDLE�����
	retry = 0;
	while(1)
	{
		SD_CS_OUT |= SD_CS;					//Ƭѡ�ź�CS�ø�
		SPI_WriteByte(0xff);				//ʱ�Ӳ���
		SD_CS_OUT &= ~SD_CS;				//Ƭѡ�ź�CS����
		r1 = SD_SendCommand1(1, 0);         //��CMD1�������ȡӦ����Ӧ0x00����SPIģʽ
		retry++;
		if(retry>200) return 1;
		if(r1==0x00) break;
	}
	SD_CS_OUT |= SD_CS;						//Ƭѡ�ź�CS�ø�
	SPI_WriteByte(0xff);					//ʱ�Ӳ���
 return 0;                              //��������
}

void SDCard_init(void)
{
	SPI_DIR |= SPI_CLK + SPI_SIMO;						   // CLK,SIMO��������Ϊ�����
	SPI_REN |= SPI_SOMI;                                   // ʹ��SD��SOMI������������
	SPI_OUT |= SPI_SOMI;                                   // SOMI��������ߵ�ƽ
	SPI_SEL |= SPI_CLK + SPI_SOMI + SPI_SIMO;			   // CLK,SOMI,SIMO����Ϊ���蹦��
	SD_CS_SEL &= ~SD_CS;								   // CS������Ϊ��ͨIO��
	SPI_REN |=SD_CS;									   // ʹ��CS���ŵ���������
	SD_CS_DIR |= SD_CS;									   // CS��������Ϊ�������
	SD_CS_OUT |= SD_CS;									   // CS��������ߵ�ƽ
	// ��ʼ�� USCI_B1 Ϊ SPI ����ģʽ
	UCB1CTL1 |= UCSWRST;                                   //����USCI ״̬
	UCB1CTL0 = UCCKPL + UCMSB + UCMST + UCMODE_0 + UCSYNC; // ����, 8λ SPI ����ģʽ
	// ʱ�Ӽ���ѡ�� - �ߵ�ƽΪ����״̬
	//��λ��ǰ����λ�ں�
	UCB1CTL1 = UCSWRST + UCSSEL_2;                         // ʹ��SMCLK
	UCB1BR0 = 200;                                          // ��ʼ��SD��SPI���� <400kHz
	UCB1BR1 = 1;                                           // f_UCxCLK = 20MHz/100 = 200kHz
	UCB1CTL1 &= ~UCSWRST;                                  // �ͷ� USCI ״̬
	UCB1IFG &= ~UCRXIFG;                                   // ������ݵ�ĩβ�����������ݣ������ط�������
	sd_reset();
}



#endif