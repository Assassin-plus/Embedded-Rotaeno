/*
 * judge.c
 *
 *  Created on: May 26, 2022
 *      Author: dell
 */
#include "Blazar_TFTLCD.h"
extern unsigned short frameSerial;
extern char MMA8451Q_databuffer[3],MMA8451Q_data[3];
extern unsigned char chartbuffer[][32];
unsigned char judgetap(unsigned char statustemp,unsigned short i){
	switch((unsigned short)chartbuffer[1][i]){
	case 0:
		if(statustemp&(0x08)){//U
			LCD_DrawInsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
			return 0x01;
		}else
			return 0x00;
		break;
	case 1:
		if((statustemp&(0x08|0x40))||(statustemp&(0x80|0x04))){//U+l | u+L
			LCD_DrawInsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
			return 0x01;
		}else
			return 0x00;
		break;
	case 2:
			if(statustemp&(0x04)){//L
				LCD_DrawInsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 3:
			if((statustemp&(0x40|0x02))||(statustemp&(0x04|0x20))){//l+D | L+d
				LCD_DrawInsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 4:
			if(statustemp&=(0x02)){//D
				LCD_DrawInsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 5:
			if((statustemp&(0x02|0x10))||(statustemp&(0x20|0x01))){//D+r | d+R
				LCD_DrawInsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 6:
			if(statustemp&=(0x01)){//R
				LCD_DrawInsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 7:
			if((statustemp&(0x01|0x80))||(statustemp&(0x10|0x08))){//R+u | r+U
				LCD_DrawInsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 8:
			if(statustemp&=(0x08|0x40)){//U+u
				LCD_DrawOutsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 9:
			if(statustemp&=(0x04|0x40)){//L+l
				LCD_DrawOutsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 10:
			if(statustemp&=(0x02|0x20)){//D+d
				LCD_DrawOutsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 11:
			if(statustemp&=(0x01|0x10)){//R+r
				LCD_DrawOutsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 12:
			if(statustemp&=(0x08|0x20)){//U+d
				LCD_DrawOutsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 13:
			if(statustemp&=(0x04|0x10)){//L+r
				LCD_DrawOutsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 14:
			if(statustemp&=(0x02|0x80)){//D+u
				LCD_DrawOutsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	case 15:
			if(statustemp&=(0x01|0x40)){//R+l
				LCD_DrawOutsideNoteColor((unsigned short)chartbuffer[1][i],(unsigned short)(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
				return 0x01;
			}else
				return 0x00;
			break;
	default :
		return 0x00;
		break;
	}
	
}
unsigned char judgemma(unsigned char channel,unsigned short i){
	switch((unsigned short) channel){
	//16->right clockwise 17->left clockwise 18->right anti 19->left anti 20-27->drag uldr 28->stop
	case 16:
	case 17:
		if (MMA8451Q_databuffer[0]-MMA8451Q_data[0]>=10){//clockwise
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;
	case 18:
	case 19:
		if (-MMA8451Q_databuffer[0]+MMA8451Q_data[0]>=10){//anti clockwise
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;
	case 20:
		if ((MMA8451Q_databuffer[0]-MMA8451Q_data[0]<10)&&(-MMA8451Q_databuffer[0]+MMA8451Q_data[0]<10)&&
				(MMA8451Q_databuffer[1]-MMA8451Q_data[1]>=10)){//lean up
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;
	case 21:
		if ((-MMA8451Q_databuffer[0]+MMA8451Q_data[0]>=10)&&
				(MMA8451Q_databuffer[1]-MMA8451Q_data[1]>=10)){//lean up&left
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;	
	case 22:
		if ((-MMA8451Q_databuffer[0]+MMA8451Q_data[0]>=10)&&
				(MMA8451Q_databuffer[1]-MMA8451Q_data[1]<10)&&
				(-MMA8451Q_databuffer[1]+MMA8451Q_data[1]<10)){//lean left
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;
	case 23:
		if ((-MMA8451Q_databuffer[0]+MMA8451Q_data[0]>=10)&&
				(-MMA8451Q_databuffer[1]+MMA8451Q_data[1]>=10)){//lean left&down
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;
	case 24:
		if ((MMA8451Q_databuffer[0]-MMA8451Q_data[0]<10)&&(-MMA8451Q_databuffer[0]+MMA8451Q_data[0]<10)&&
				(-MMA8451Q_databuffer[1]+MMA8451Q_data[1]>=10)){//lean down
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;
	case 25:
		if ((MMA8451Q_databuffer[0]-MMA8451Q_data[0]>=10)&&
				(-MMA8451Q_databuffer[1]+MMA8451Q_data[1]>=10)){//lean down&right
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;
	case 26:
		if ((MMA8451Q_databuffer[0]-MMA8451Q_data[0]>=10)&&
				(MMA8451Q_databuffer[1]-MMA8451Q_data[1]<10)&&
				(-MMA8451Q_databuffer[1]+MMA8451Q_data[1]<10)){//lean right
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;
	case 27:
		if ((MMA8451Q_databuffer[0]-MMA8451Q_data[0]>=10)&&
				(MMA8451Q_databuffer[1]-MMA8451Q_data[1]>=10)){//lean right&up
			LCD_DrawRotateNoteColor((unsigned short)channel-16,(28-chartbuffer[2][i]*9+frameSerial)*3,WHITE);
			return 0x01;
		}else{
			return 0x00;
		}	
		break;
	case 28:/*
		if((MMA8451Q_databuffer[2]-MMA8451Q_data[2]>=10)){
			return 0x01;
		}else{
			return 0x00;
		}
		break;*/
		return 0x00;
	}
}
