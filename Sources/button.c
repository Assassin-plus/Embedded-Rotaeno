/*
 * button.c
 *
 *  Created on: May 26, 2022
 *      Author: Luziqi
 */
#include "derivative.h"

unsigned char buttonstatus=0;//8-bit data to save button status
//date structure: uldrULDR

void BUTTON_INIT(void){
	SIM_SCGC5|=(1<<9);//ENABLE PORTA CLK
	//SET BUTTON INTERRUPT PRIORITY
	NVIC_IPR7&=~(1<<23);
	NVIC_IPR7|=(1<<22);
	//BUTTON ->I/O PULL 
	PORTA_PCR1 =0x102;
	PORTA_PCR12=0x102;
	PORTA_PCR5 =0x102;
	PORTA_PCR13=0x102;
	PORTA_PCR17=0x102;
	PORTA_PCR14=0x102;
	PORTA_PCR15=0x102;
	PORTA_PCR16=0x102;
	
	//SET PORTA(BUTTON) TO INPUT
	GPIOA_PDDR&=~(1<<1|1<<12|1<<5|1<<13|1<<17|1<<14|1<<15|1<<16);
	
	/*
	 * u -> PTA1
	 * l -> PTA12
	 * d -> PTA5
	 * r -> PTA13
	 * U -> PTA17
	 * L -> PTA14
	 * D -> PTA15
	 * R -> PTA16
	 */
	
	asm("CPSIE i");//OPEN GLOBAL INTERRUPT
	NVIC_ISER |= 0x40000000;//OPEN PORTA INTERRUPT
	
	//BUTTON INTERRUPT CONFIGURATION
	PORTA_PCR1 =0x0A0102;//u(PTA1 ) DOWNSIDE INTERRUPT
	PORTA_PCR12=0x0A0102;//l(PTA12) DOWNSIDE INTERRUPT
	PORTA_PCR5 =0x0A0102;//d(PTA5 ) DOWNSIDE INTERRUPT
	PORTA_PCR13=0x0A0102;//r(PTA13) DOWNSIDE INTERRUPT
	PORTA_PCR17=0x0A0102;//U(PTA17) DOWNSIDE INTERRUPT
	PORTA_PCR14=0x0A0102;//L(PTA14) DOWNSIDE INTERRUPT
	PORTA_PCR15=0x0A0102;//D(PTA15) DOWNSIDE INTERRUPT
	PORTA_PCR16=0x0A0102;//R(PTA16) DOWNSIDE INTERRUPT
	
}
void PORTA_IRQHandler(void){
		  if((GPIOA_PDIR&(1<<1)) ==0)//BUTTON u INTERRUPT
	{
		buttonstatus |= 0x80;
	}
		  if((GPIOA_PDIR&(1<<12))==0)//BUTTON l INTERRUPT
	{
		buttonstatus |= 0x40;
	}
		  if((GPIOA_PDIR&(1<<5)) ==0)//BUTTON d INTERRUPT
	{
		buttonstatus |= 0x20;
	}
		  if((GPIOA_PDIR&(1<<13))==0)//BUTTON r INTERRUPT
	{
		buttonstatus |= 0x10;
	}
		  if((GPIOA_PDIR&(1<<17))==0)//BUTTON U INTERRUPT
	{
		buttonstatus |= 0x08;
	}
		  if((GPIOA_PDIR&(1<<14))==0)//BUTTON L INTERRUPT
	{
		buttonstatus |= 0x04;
	}
		  if((GPIOA_PDIR&(1<<15))==0)//BUTTON D INTERRUPT
	{
		buttonstatus |= 0x02;
	}
		if((GPIOA_PDIR&(1<<16))==0)//BUTTON R INTERRUPT
	{
		buttonstatus |= 0x01;
	}
	//clear interrupt flag
		PORTA_PCR1 |= 0X01000000;
		PORTA_PCR12|= 0X01000000;
		PORTA_PCR5 |= 0X01000000;
		PORTA_PCR13|= 0X01000000;
		PORTA_PCR17|= 0X01000000;
		PORTA_PCR14|= 0X01000000;
		PORTA_PCR15|= 0X01000000;
		PORTA_PCR16|= 0X01000000;
}
