/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#pragma GCC push_options
#pragma GCC optimize("O0")

#include "derivative.h" /* include peripheral declarations */
#include "Blazar_TFTLCD.h"
#include "KL2x_gpio.h"
#include "MMA8451Q.h"/*MMA8451Q */
#include "light.h"
#include "systick.h"
#include "judge.h"
#include "button.h"


extern unsigned char buttonstatus;
extern unsigned char chartbuffer[][32];
extern unsigned char chart[][260];
unsigned char dtoa(unsigned char c1)
{
  return (c1>=10)? (c1+'A'-10):(c1+'0');
  //�˴�ʹ�����ʺű���ʽ���൱��if/else
}

void main(void)
{
	//initial operations
	gpio_enable_port();
	LCD_Init();   //TFT��ʾ
	LCD_Clear(WHITE);//ˢ��Ϊ��ɫ
	MMA8451Q_Init();  
	LIGHT_INIT();
	BUTTON_INIT();
	
	
	char str[7];
	str[6]='\0';
	unsigned char MMA8451Q_data[6];
	//initial chart buffer
	int i=0;
	chartbuffer[0][i]=chart[0][i];
	chartbuffer[1][i]=chart[1][i];
	chartbuffer[2][i]=chart[2][i];
	chartbuffer[3][i]=0;
	i++;
	for(;i<32;i++){
		chartbuffer[0][i]=chart[0][i];
		chartbuffer[1][i]=chart[1][i];
		chartbuffer[2][i]=chart[2][i]+chartbuffer[2][i-1];
		chartbuffer[3][i]=0;			
	}
	
	//initial finish


	//user interface
	LCD_ShowStringColor(60,100,"Click Any Button",RED);
	LCD_ShowStringColor(100,140,"To START!",RED);
	for(;;){
		if(buttonstatus!=0){ 
			buttonstatus=0;
			break;//click any button to begin
		}
	}
	LCD_ShowStringColor(60,100,"Click Any Button",WHITE);
	LCD_ShowStringColor(100,140,"To START!",WHITE);
	
	//begin playing
	SYSTICK_INIT();
	TPM0_INIT();
	Draw_Circle(160,120,100);
	for(;;){//ˢ��
		//test codes for accelerator and attitude control
		/*
		MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis
    	MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x02);//LSB of X-Axis
    	
    	MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
    	MMA8451Q_data[3]=MMA8451Q_ReadRegister(0x04);//LSB of Y-Axis

    	MMA8451Q_data[4]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
    	MMA8451Q_data[5]=MMA8451Q_ReadRegister(0x06);//LSB of Z-Axis
    	
    	str[0]='X';
    	str[1]=':';
    	if (MMA8451Q_data[0]&0x80) { 
    		MMA8451Q_data[0]=(~MMA8451Q_data[0])+1;	//��������
    		str[2]='-';
    	} else 
    		str[2]=' ';     			
	    str[5]=dtoa(MMA8451Q_data[0]%10);
	    str[4]=dtoa((MMA8451Q_data[0]/10)%10);
	    str[3]=dtoa((MMA8451Q_data[0]/100)%10);
	    LCD_ShowStringColor(180,10,str,GREEN);
	    
	    str[0]='Y';
    	str[1]=':';
    	if (MMA8451Q_data[2]&0x80) { 
    		MMA8451Q_data[2]=(~MMA8451Q_data[2])+1;	//��������
    		str[2]='-';
    	} else 
    		str[2]=' ';     			
	    str[5]=dtoa(MMA8451Q_data[2]%10);
	    str[4]=dtoa((MMA8451Q_data[2]/10)%10);
	    str[3]=dtoa((MMA8451Q_data[2]/100)%10);
	    LCD_ShowStringColor(180,20,str,RED);
	    
	    str[0]='Z';
    	str[1]=':';
    	if (MMA8451Q_data[4]&0x80) { 
    		MMA8451Q_data[4]=(~MMA8451Q_data[4])+1;	//��������
    		str[2]='-';
    	} else 
    		str[2]=' ';     			
	    str[5]=dtoa(MMA8451Q_data[4]%10);
	    str[4]=dtoa((MMA8451Q_data[4]/10)%10);
	    str[3]=dtoa((MMA8451Q_data[4]/100)%10);
	    LCD_ShowStringColor(180,30,str,CYAN);
	    */
 	}
}
#pragma GCC pop_options
