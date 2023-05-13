/*
 * systick.c
 *
 *  Created on: May 28, 2022
 *      Author: Luziqi
 */
//core codes

#include "Blazar_TFTLCD.h"
#include "derivative.h" 
#include "MMA8451Q.h"

extern unsigned char buttonstatus;//date structure: uldrULDR
extern const unsigned short note[];
extern const unsigned char duration[];
extern const unsigned char volume[];
extern const unsigned char chart[][260];
//extern const unsigned char chart[][];
//63.81fps 1/63 9frame per music unit
//105bpm 0x2DB602 1/7sec
unsigned short pNote=0,nDuration=0,pChart=32;
unsigned short frameSerial=0;
char MMA8451Q_databuffer[3]={0},MMA8451Q_data[3]={0};
unsigned short combo=0;
unsigned char statustemp=0;
unsigned char chartbuffer[][32]
={//buffer up to 32 notes
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
		},
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
		},
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
		},
		{
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	
		}
};
/* chart data structure:			chartbuffer structure:
 * 	0unsigned char mode;			0unsigned char mode;
		intap=0,
		outtap=1,					1unsigned char channel;
		rotate=2,					2unsigned char time=sigma(duration);
		drag=3,						3unsigned char (bool) ondisplay;
		stop=4						
	1unsigned char channel;	
	 0-7->inside uldr 			0x0-
	  8-15 outside 				0x0-
	  uldr(anti)uldr 
	  16->right clockwise 		0x10
	  17->left clockwise 		0x11
	  18->right anti 			0x12
	  19->left anti				0x13
	20-27->drag uldr 	0x14-0x1B
	28->stop		0x1c
	2unsigned char duration;
	*/

/**
 * @brief integrated function to show note
 * 
 * @param mode 
 * @param channel 
 * @param parameter 
 */
void LCD_ShowNote(unsigned char mode,unsigned char channel,unsigned char parameter){
	switch(mode){
	case 0:
		LCD_DrawInsideNoteColor((unsigned short)channel,(unsigned short)parameter-3,WHITE);
		LCD_DrawInsideNoteColor((unsigned short)channel,(unsigned short)parameter,LIGHTBLUE);
		break;
	case 1:
		LCD_DrawOutsideNoteColor((unsigned short)channel,(unsigned short)parameter,MAGENTA);
			break;
	case 2:
		LCD_DrawRotateNoteColor((unsigned short)channel-16,(unsigned short)parameter,RED);
			break;
	case 3:
		LCD_DrawDragNoteColor((unsigned short)(channel-20),(unsigned short)parameter-3,WHITE);
		LCD_DrawDragNoteColor((unsigned short)(channel-20),(unsigned short)parameter,LIGHTBLUE);
			break;
	case 4:
		LCD_DrawStopNoteColor((unsigned short)parameter+1,WHITE);
		LCD_DrawStopNoteColor((unsigned short)parameter,RED);
			break;
	}
}
/**
 * @brief functions to move the notes
 * 
 * @param i frameSerial 
 */
void notemove(unsigned short i){
	switch(chartbuffer[0][i]){
		case 0:
		LCD_ShowNote(chartbuffer[0][i],chartbuffer[1][i],(30-chartbuffer[2][i]*9+frameSerial)*3+10+1);
		LCD_ShowNote(chartbuffer[0][i],chartbuffer[1][i],(30-chartbuffer[2][i]*9+frameSerial)*3+10);
		break;
		case 1:
		LCD_ShowNote(chartbuffer[0][i],chartbuffer[1][i],28-chartbuffer[2][i]*9+frameSerial);
		
		break;
		case 2:
		LCD_ShowNote(chartbuffer[0][i],chartbuffer[1][i],(28-chartbuffer[2][i]*9+frameSerial)*3);
		break;
		case 3:
		LCD_ShowNote(chartbuffer[0][i],chartbuffer[1][i],(30-chartbuffer[2][i]*9+frameSerial)*3+10);
		break;
		case 4:
		LCD_ShowNote(chartbuffer[0][i],chartbuffer[1][i],chartbuffer[2][i]*9-frameSerial);
		break;
	}
}
/**
 * @brief destroy the notes
 * 
 * @param i 
 */
void notedestroy(unsigned short i){
	switch(chartbuffer[0][i]){
	case 0:
		LCD_DrawInsideNoteColor(chartbuffer[1][i],(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);	//
		LCD_DrawInsideNoteColor(chartbuffer[1][i],(30-chartbuffer[2][i]*9+frameSerial)*3+10+1,WHITE);//
		break;
	case 1:
		LCD_DrawOutsideNoteColor(chartbuffer[1][i],28-chartbuffer[2][i]*9+frameSerial,WHITE);
		Draw_CircleColor(160,120,110,WHITE);
		break;
	case 2:
		Draw_CircleColor(160,120,130,WHITE);
		LCD_DrawRotateNoteColor(chartbuffer[1][i]-16,108,WHITE);
		break;
	case 3:
		LCD_DrawDragNoteColor((chartbuffer[1][i]-20),(21-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
		LCD_DrawDragNoteColor((chartbuffer[1][i]-20),(30-chartbuffer[2][i]*9+frameSerial)*3+10,WHITE);
		break;
	case 4:
		LCD_DrawPointColor(160,120,WHITE);
		break;
	}
}

void SYSTICK_INIT(void){
	//MUSIC (PWM0) INIT
	SYST_RVR = 0x51439; //0.016sec 1frame  @ 20.97MHz
	SYST_CVR = 0x00; //Clear Value
	SYST_CSR = 0x07; //Enable Clock
	//SET systick INTERRUPT PRIORITY
	SCB_SHPR3|=(1<<31|1<<30);
}
unsigned char j=0,k=0;
/**
 * @brief refresh and judge the notes
 * 
 */
void SysTick_Handler(void){
	unsigned int i=0;
	switch(frameSerial){
	case 0:	
		
	//MUSIC (PWM0) CONFIGURE
		if(nDuration==0) {
			//replay
			if(note[pNote]==0xff) pNote=0;
			//���ò����µ���
			TPM0_SET((unsigned short)( note[pNote]),(unsigned char) volume[pNote]); 
			nDuration = duration[pNote];
			pNote++;
		} 
		nDuration--; 
		
		i=0;
		for(;i<32;i++){
	 		//timing
			chartbuffer[2][i]--;
			//note emerge
			if(chartbuffer[2][i]<=0x03){
				chartbuffer[3][i]=0x01;
			}
			//note display & move
			if(chartbuffer[3][i]==0x01){
				notemove(i);
			}
		}
		//note judge
		//intap & outtap judge
		statustemp=buttonstatus;//keep status freezed
		if(statustemp){
			i=0;
			for(;i<32;i++){
				if(chartbuffer[2][i]==0x00){
					// judge whether button is corresponding to note
					if(judgetap(statustemp,i)==0x01){
						combo++;
						 
						LCD_ShowNumColor(2,2,combo,3,20,YELLOW);//show combo
						StatusDisplay((unsigned char)1);//show perfect
						notedestroy(i);chartbuffer[2][i]=0;//destroy note
						
						//load new note
						chartbuffer[0][i]=chart[0][pChart];
						chartbuffer[1][i]=chart[1][pChart];
						chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
						chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
					}
				}
			}
			statustemp=0;
			buttonstatus=0;
		}
		//rotate,stop & drag judge
		MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
		MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
		MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
		if (MMA8451Q_data[0]&0x80) { 
			MMA8451Q_data[0]=-((~MMA8451Q_data[0])+1);	//��������
		}
		if (MMA8451Q_data[1]&0x80) { 
		    MMA8451Q_data[1]=-((~MMA8451Q_data[1])+1);	//��������
		}
		if (MMA8451Q_data[2]&0x80) { 
			MMA8451Q_data[2]=-((~MMA8451Q_data[2])+1);	//��������
		}
		//judge mma
		i=0;
		for(;i<32;i++){
			if(chartbuffer[2][i]==0x00){
				if(judgemma(chartbuffer[1][i],i)==0x01){
					combo++;
					 
					LCD_ShowNumColor(2,2,combo,3,20,YELLOW);//show combo
					StatusDisplay((unsigned char)1);//show perfect
					notedestroy(i);chartbuffer[2][i]=0;//destroy note
					
					//load new note
					chartbuffer[0][i]=chart[0][pChart];
					chartbuffer[1][i]=chart[1][pChart];
					chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
					chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
				}		
			}
		}
	break;
	case 1:
		//note display & move
		i=0;
		for(;i<32;i++){
		if(chartbuffer[3][i]==0x01){
			notemove(i);
		}		
		}
		//note judge
		//intap & outtap judge
		statustemp=buttonstatus;//keep status freezed
		if(statustemp){
			i=0;
			for(;i<32;i++){
				if(chartbuffer[2][i]==0x00){
					// judge whether button is corresponding to note
					if(judgetap(statustemp,i)==0x01){
						combo++;
						 
						LCD_ShowNumColor(2,2,combo,3,20,YELLOW);//show combo
						StatusDisplay((unsigned char)1);//show perfect
						notedestroy(i);chartbuffer[2][i]=0;//destroy note
						
						//load new note
						chartbuffer[0][i]=chart[0][pChart];
						chartbuffer[1][i]=chart[1][pChart];
						chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
						chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
					}
				}
			}
			statustemp=0;
			buttonstatus=0;
		}
		//rotate,stop & drag judge
		MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
		MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
		MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
		if (MMA8451Q_data[0]&0x80) { 
			MMA8451Q_data[0]=-((~MMA8451Q_data[0])+1);	//��������
		}
		if (MMA8451Q_data[1]&0x80) { 
		    MMA8451Q_data[1]=-((~MMA8451Q_data[1])+1);	//��������
		}
		if (MMA8451Q_data[2]&0x80) { 
			MMA8451Q_data[2]=-((~MMA8451Q_data[2])+1);	//��������
		}
		//judge mma
		i=0;
		for(;i<32;i++){
			if(chartbuffer[2][i]==0x00){
				if(judgemma(chartbuffer[1][i],i)==0x01){
					combo++;
					 
					LCD_ShowNumColor(2,2,combo,3,20,YELLOW);//show combo
					StatusDisplay((unsigned char)1);//show perfect
					notedestroy(i);chartbuffer[2][i]=0;//destroy note
					
					//load new note
					chartbuffer[0][i]=chart[0][pChart];
					chartbuffer[1][i]=chart[1][pChart];
					chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
					chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
				}		
			}
		}
		MMA8451Q_databuffer[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
		MMA8451Q_databuffer[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
		MMA8451Q_databuffer[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
		
		break;
	case 2:
		//note display & move
		i=0;
		for(;i<32;i++){
		if(chartbuffer[3][i]==0x01){
			notemove(i);
		}		
		}	
		//note judge
		//intap & outtap judge
		statustemp=buttonstatus;//keep status freezed
		if(statustemp){
			i=0;
			for(;i<32;i++){
				if(chartbuffer[2][i]==0x00){
					// judge whether button is corresponding to note
					if(judgetap(statustemp,i)==0x01){
						combo++;
						 
						LCD_ShowNumColor(2,2,combo,3,20,MAGENTA);//show combo
						StatusDisplay((unsigned char)2);//show good
						notedestroy(i);chartbuffer[2][i]=0;//destroy note
						
						//load new note
						chartbuffer[0][i]=chart[0][pChart];
						chartbuffer[1][i]=chart[1][pChart];
						chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
						chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
					}
				}
			}
			statustemp=0;
			buttonstatus=0;
		}
		//rotate,stop & drag judge
		MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
		MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
		MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
		if (MMA8451Q_data[0]&0x80) { 
			MMA8451Q_data[0]=-((~MMA8451Q_data[0])+1);	//��������
		}
		if (MMA8451Q_data[1]&0x80) { 
		    MMA8451Q_data[1]=-((~MMA8451Q_data[1])+1);	//��������
		}
		if (MMA8451Q_data[2]&0x80) { 
			MMA8451Q_data[2]=-((~MMA8451Q_data[2])+1);	//��������
		}
		//judge mma
		i=0;
		for(;i<32;i++){
			if(chartbuffer[2][i]==0x00){
				if(judgemma(chartbuffer[1][i],i)==0x01){
					combo++;
					 
					LCD_ShowNumColor(2,2,combo,3,20,MAGENTA);//show combo
					StatusDisplay((unsigned char)2);//show good
					notedestroy(i);chartbuffer[2][i]=0;//destroy note
					
					//load new note
					chartbuffer[0][i]=chart[0][pChart];
					chartbuffer[1][i]=chart[1][pChart];
					chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
					chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
				}		
			}
		}
		break;
	case 3: 
		//note display & move
		i=0;
				for(;i<32;i++){
				if(chartbuffer[3][i]==0x01){
					notemove(i);
				}		
				}		
		//note judge
		//intap & outtap judge
		statustemp=buttonstatus;//keep status freezed
		if(statustemp){
			i=0;
			for(;i<32;i++){
				if(chartbuffer[2][i]==0x00){
					// judge whether button is corresponding to note
					if(judgetap(statustemp,i)==0x01){
						combo++;
						 
						LCD_ShowNumColor(2,2,combo,3,20,MAGENTA);//show combo
						StatusDisplay((unsigned char)2);//show good
						notedestroy(i);chartbuffer[2][i]=0;//destroy note
						
						//load new note
						chartbuffer[0][i]=chart[0][pChart];
						chartbuffer[1][i]=chart[1][pChart];
						chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
						chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
					}
				}
			}
			if(i==32){
				i=0;
				for(;i<32;i++){
				if(chartbuffer[2][i]==0x01){
					// judge whether button is corresponding to note
					if(judgetap(statustemp,i)==0x01){
						combo=0;
						 
						LCD_ShowNumColor(2,2,combo,3,20,RED);//show combo
						StatusDisplay((unsigned char)3);//show bad
						notedestroy(i);chartbuffer[2][i]=0;//destroy note
						
						//load new note
						chartbuffer[0][i]=chart[0][pChart];
						chartbuffer[1][i]=chart[1][pChart];
						chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
						chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
					}
				}
			}
			}
			statustemp=0;
			buttonstatus=0;
		}
		//rotate,stop & drag judge
		MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
		MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
		MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
		if (MMA8451Q_data[0]&0x80) { 
			MMA8451Q_data[0]=-((~MMA8451Q_data[0])+1);	//��������
		}
		if (MMA8451Q_data[1]&0x80) { 
		    MMA8451Q_data[1]=-((~MMA8451Q_data[1])+1);	//��������
		}
		if (MMA8451Q_data[2]&0x80) { 
			MMA8451Q_data[2]=-((~MMA8451Q_data[2])+1);	//��������
		}
		//judge mma
		i=0;
		for(;i<32;i++){
			if(chartbuffer[2][i]==0x00){
				if(judgemma(chartbuffer[1][i],i)==0x01){
					combo++;
					 
					LCD_ShowNumColor(2,2,combo,3,20,MAGENTA);//show combo
					StatusDisplay((unsigned char)2);//show good
					notedestroy(i);chartbuffer[2][i]=0;//destroy note
					
					//load new note
					chartbuffer[0][i]=chart[0][pChart];
					chartbuffer[1][i]=chart[1][pChart];
					chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
					chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
				}		
			}
		}
		if(i==32){
			i=0;
			for(;i<32;i++){
			if(chartbuffer[2][i]==0x01){
				if(judgemma(chartbuffer[1][i],i)==0x01){
					combo=0;
					 
					LCD_ShowNumColor(2,2,combo,3,20,RED);//show combo
					StatusDisplay((unsigned char)3);//show bad
					notedestroy(i);chartbuffer[2][i]=0;//destroy note
					
					//load new note
					chartbuffer[0][i]=chart[0][pChart];
					chartbuffer[1][i]=chart[1][pChart];
					chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
					chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
				}		
			}
			}
		}
		MMA8451Q_databuffer[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
		MMA8451Q_databuffer[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
		MMA8451Q_databuffer[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
		break;
	case 4: 
		//note display & move
		i=0;
				for(;i<32;i++){
				if(chartbuffer[3][i]==0x01){
					notemove(i);
				}		
				}		
		//note judge
		//intap & outtap judge
		statustemp=buttonstatus;//keep status freezed
		if(statustemp){
			i=0;
			for(;i<32;i++){
				if(chartbuffer[2][i]==0x00){
					// judge whether button is corresponding to note
					if(judgetap(statustemp,i)==0x01){
						combo=0;
						 
						LCD_ShowNumColor(2,2,combo,3,20,RED);//show combo
						StatusDisplay((unsigned char)3);//show bad
						notedestroy(i);chartbuffer[2][i]=0;//destroy note
						
						//load new note
						chartbuffer[0][i]=chart[0][pChart];
						chartbuffer[1][i]=chart[1][pChart];
						chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
						chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
					}
				}
			}
			if(i==32){
				i=0;
				for(;i<32;i++){
				if(chartbuffer[2][i]==0x01){
					// judge whether button is corresponding to note
					if(judgetap(statustemp,i)==0x01){
						combo=0;
						 
						LCD_ShowNumColor(2,2,combo,3,20,RED);//show combo
						StatusDisplay((unsigned char)3);//show bad
						notedestroy(i);chartbuffer[2][i]=0;//destroy note
						
						//load new note
						chartbuffer[0][i]=chart[0][pChart];
						chartbuffer[1][i]=chart[1][pChart];
						chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
						chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
					}
				}
			}
			}
			statustemp=0;
			buttonstatus=0;
		}
		//rotate,stop & drag judge
		MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
		MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
		MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
		if (MMA8451Q_data[0]&0x80) { 
			MMA8451Q_data[0]=-((~MMA8451Q_data[0])+1);	//��������
		}
		if (MMA8451Q_data[1]&0x80) { 
		    MMA8451Q_data[1]=-((~MMA8451Q_data[1])+1);	//��������
		}
		if (MMA8451Q_data[2]&0x80) { 
			MMA8451Q_data[2]=-((~MMA8451Q_data[2])+1);	//��������
		}
		//judge mma
		i=0;
		for(;i<32;i++){
			if(chartbuffer[2][i]==0x00){
				if(judgemma(chartbuffer[1][i],i)==0x01){
					combo=0;
					 
					LCD_ShowNumColor(2,2,combo,3,20,RED);//show combo
					StatusDisplay((unsigned char)3);//show bad
					notedestroy(i);chartbuffer[2][i]=0;//destroy note
					
					//load new note
					chartbuffer[0][i]=chart[0][pChart];
					chartbuffer[1][i]=chart[1][pChart];
					chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
					chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
				}		
			}
		}
		if(i==32){
			i=0;
			for(;i<32;i++){
			if(chartbuffer[2][i]==0x01){
				if(judgemma(chartbuffer[1][i],i)==0x01){
					combo=0;
					 
					LCD_ShowNumColor(2,2,combo,3,20,RED);//show combo
					StatusDisplay((unsigned char)3);//show bad
					notedestroy(i);chartbuffer[2][i]=0;//destroy note
					
					//load new note
					chartbuffer[0][i]=chart[0][pChart];
					chartbuffer[1][i]=chart[1][pChart];
					chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
					chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
				}		
			}
			}
		}
		break;
	case 5: 
		//note display & move
		i=0;
				for(;i<32;i++){
				if(chartbuffer[3][i]==0x01){
					notemove(i);
				}		
				}		
				//note judge
				//intap & outtap judge
				statustemp=buttonstatus;//keep status freezed
				if(statustemp){
					i=0;
					for(;i<32;i++){
						if(chartbuffer[2][i]==0x01){
							// judge whether button is corresponding to note
							if(judgetap(statustemp,i)==0x01){
								combo++;
								 
								LCD_ShowNumColor(2,2,combo,3,20,MAGENTA);//show combo
								StatusDisplay((unsigned char)2);//show good
								notedestroy(i);chartbuffer[2][i]=0;//destroy note
								
								//load new note
								chartbuffer[0][i]=chart[0][pChart];
								chartbuffer[1][i]=chart[1][pChart];
								chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
								chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
							}
						}
					}
					if(i==32){
						i=0;
						for(;i<32;i++){
						if(chartbuffer[2][i]==0x00){
							// judge whether button is corresponding to note
							if(judgetap(statustemp,i)==0x01){
								combo=0;
								 
								LCD_ShowNumColor(2,2,combo,3,20,RED);//show combo
								StatusDisplay((unsigned char)3);//show bad
								notedestroy(i);chartbuffer[2][i]=0;//destroy note
								
								//load new note
								chartbuffer[0][i]=chart[0][pChart];
								chartbuffer[1][i]=chart[1][pChart];
								chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
								chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
							}
						}
					}
					}
					statustemp=0;
					buttonstatus=0;
				}
				//rotate,stop & drag judge
				MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
				MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
				MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
				if (MMA8451Q_data[0]&0x80) { 
					MMA8451Q_data[0]=-((~MMA8451Q_data[0])+1);	//��������
				}
				if (MMA8451Q_data[1]&0x80) { 
				    MMA8451Q_data[1]=-((~MMA8451Q_data[1])+1);	//��������
				}
				if (MMA8451Q_data[2]&0x80) { 
					MMA8451Q_data[2]=-((~MMA8451Q_data[2])+1);	//��������
				}
				//judge mma
				i=0;
				for(;i<32;i++){
					if(chartbuffer[2][i]==0x01){
						if(judgemma(chartbuffer[1][i],i)==0x01){
							combo++;
							 
							LCD_ShowNumColor(2,2,combo,3,20,MAGENTA);//show combo
							StatusDisplay((unsigned char)2);//show good
							notedestroy(i);chartbuffer[2][i]=0;//destroy note
							
							//load new note
							chartbuffer[0][i]=chart[0][pChart];
							chartbuffer[1][i]=chart[1][pChart];
							chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
							chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
						}		
					}
				}
				if(i==32){
					i=0;
					for(;i<32;i++){
					if(chartbuffer[2][i]==0x00){
						if(judgemma(chartbuffer[1][i],i)==0x00){
							combo=0;
							 
							LCD_ShowNumColor(2,2,combo,3,20,RED);//show combo
							StatusDisplay((unsigned char)3);//show bad
							notedestroy(i);chartbuffer[2][i]=0;//destroy note
							
							//load new note
							chartbuffer[0][i]=chart[0][pChart];
							chartbuffer[1][i]=chart[1][pChart];
							chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
							chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
						}		
					}
					}
				}
				MMA8451Q_databuffer[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
				MMA8451Q_databuffer[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
				MMA8451Q_databuffer[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
		break;
	case 6: 
		//note display & move
		i=0;
				for(;i<32;i++){
				if(chartbuffer[3][i]==0x01){
					notemove(i);
				}		
				}		
				//note judge
				//intap & outtap judge
				statustemp=buttonstatus;//keep status freezed
				if(statustemp){
					i=0;
					for(;i<32;i++){
						if(chartbuffer[2][i]==0x01){
							// judge whether button is corresponding to note
							if(judgetap(statustemp,i)==0x01){
								combo++;
								 
								LCD_ShowNumColor(2,2,combo,3,20,MAGENTA);//show combo
								StatusDisplay((unsigned char)2);//show good
								notedestroy(i);chartbuffer[2][i]=0;//destroy note
								
								//load new note
								chartbuffer[0][i]=chart[0][pChart];
								chartbuffer[1][i]=chart[1][pChart];
								chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
								chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
							}
						}
					}
					if(i==32){
						i=0;
						for(;i<32;i++){
						if(chartbuffer[2][i]==0x00){
							// judge whether button is corresponding to note
							if(judgetap(statustemp,i)==0x01){
								combo=0;
								 
								LCD_ShowNumColor(2,2,combo,3,20,WHITE);//show combo
								StatusDisplay((unsigned char)4);//show miss
								notedestroy(i);chartbuffer[2][i]=0;//destroy note
								
								//load new note
								chartbuffer[0][i]=chart[0][pChart];
								chartbuffer[1][i]=chart[1][pChart];
								chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
								chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
							}
						}
					}
					}
					statustemp=0;
					buttonstatus=0;
				}
				//rotate,stop & drag judge
				MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
				MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
				MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
				if (MMA8451Q_data[0]&0x80) { 
					MMA8451Q_data[0]=-((~MMA8451Q_data[0])+1);	//��������
				}
				if (MMA8451Q_data[1]&0x80) { 
				    MMA8451Q_data[1]=-((~MMA8451Q_data[1])+1);	//��������
				}
				if (MMA8451Q_data[2]&0x80) { 
					MMA8451Q_data[2]=-((~MMA8451Q_data[2])+1);	//��������
				}
				//judge mma
				i=0;
				for(;i<32;i++){
					if(chartbuffer[2][i]==0x01){
						if(judgemma(chartbuffer[1][i],i)==0x01){
							combo++;
							 
							LCD_ShowNumColor(2,2,combo,3,20,MAGENTA);//show combo
							StatusDisplay((unsigned char)2);//show good
							notedestroy(i);chartbuffer[2][i]=0;//destroy note
							
							//load new note
							chartbuffer[0][i]=chart[0][pChart];
							chartbuffer[1][i]=chart[1][pChart];
							chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
							chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
						}		
					}
				}
				if(i==32){
					i=0;
					for(;i<32;i++){
					if(chartbuffer[2][i]==0x00){
						if(judgemma(chartbuffer[1][i],i)==0x01){
							combo=0;
							 
							LCD_ShowNumColor(2,2,combo,3,20,WHITE);//show combo
							StatusDisplay((unsigned char)4);//show MISS
							notedestroy(i);chartbuffer[2][i]=0;//destroy note
							
							//load new note
							chartbuffer[0][i]=chart[0][pChart];
							chartbuffer[1][i]=chart[1][pChart];
							chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
							chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
						}		
					}
					}
				}
		break;
	case 7: 
		//note display & move
		i=0;
				for(;i<32;i++){
				if(chartbuffer[3][i]==0x01){
					notemove(i);
				}		
				}		
				//note judge
				//intap & outtap judge
				statustemp=buttonstatus;//keep status freezed
				if(statustemp){
					i=0;
					for(;i<32;i++){
						if(chartbuffer[2][i]==0x01){
							// judge whether button is corresponding to note
							if(judgetap(statustemp,i)==0x01){
								combo++;
								 
								LCD_ShowNumColor(2,2,combo,3,20,YELLOW);//show combo
								StatusDisplay((unsigned char)1);//show PERFECT
								notedestroy(i);chartbuffer[2][i]=0;//destroy note
								
								//load new note
								chartbuffer[0][i]=chart[0][pChart];
								chartbuffer[1][i]=chart[1][pChart];
								chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
								chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
							}
						}
					}
					if(i==32){
						i=0;
						for(;i<32;i++){
						if(chartbuffer[2][i]==0x00){
							// judge whether button is corresponding to note
							if(judgetap(statustemp,i)==0x01){
								combo=0;
								 
								LCD_ShowNumColor(2,2,combo,3,20,WHITE);//show combo
								StatusDisplay((unsigned char)4);//show MISS
								notedestroy(i);chartbuffer[2][i]=0;//destroy note
								
								//load new note
								chartbuffer[0][i]=chart[0][pChart];
								chartbuffer[1][i]=chart[1][pChart];
								chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
								chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
							}
						}
					}
					}
					statustemp=0;
					buttonstatus=0;
				}
				//rotate,stop & drag judge
				MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
				MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
				MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
				if (MMA8451Q_data[0]&0x80) { 
					MMA8451Q_data[0]=-((~MMA8451Q_data[0])+1);	//��������
				}
				if (MMA8451Q_data[1]&0x80) { 
				    MMA8451Q_data[1]=-((~MMA8451Q_data[1])+1);	//��������
				}
				if (MMA8451Q_data[2]&0x80) { 
					MMA8451Q_data[2]=-((~MMA8451Q_data[2])+1);	//��������
				}
				//judge mma
				i=0;
				for(;i<32;i++){
					if(chartbuffer[2][i]==0x01){
						if(judgemma(chartbuffer[1][i],i)==0x01){
							combo++;
							 
							LCD_ShowNumColor(2,2,combo,3,20,YELLOW);//show combo
							StatusDisplay((unsigned char)1);//show PERFECT
							notedestroy(i);chartbuffer[2][i]=0;//destroy note
							
							//load new note
							chartbuffer[0][i]=chart[0][pChart];
							chartbuffer[1][i]=chart[1][pChart];
							chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
							chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
						}		
					}
				}
				if(i==32){
					i=0;
					for(;i<32;i++){
					if(chartbuffer[2][i]==0x00){
						if(judgemma(chartbuffer[1][i],i)==0x01){
							combo=0;
							 
							LCD_ShowNumColor(2,2,combo,3,20,WHITE);//show combo
							StatusDisplay((unsigned char)4);//show MISS
							notedestroy(i);chartbuffer[2][i]=0;//destroy note
							
							//load new note
							chartbuffer[0][i]=chart[0][pChart];
							chartbuffer[1][i]=chart[1][pChart];
							chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
							chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
						}		
					}
					}
				}
				MMA8451Q_databuffer[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
				MMA8451Q_databuffer[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
				MMA8451Q_databuffer[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
		break;
	case 8: //miss
		//note display & move
		i=0;
				for(;i<32;i++){
				if(chartbuffer[3][i]==0x01){
					notemove(i);
				}		
				}		
				//note judge
				//intap & outtap judge
				statustemp=buttonstatus;//keep status freezed
				if(statustemp){
					i=0;
					for(;i<32;i++){
						if(chartbuffer[2][i]==0x00){
							// judge whether button is corresponding to note
							if(judgetap(statustemp,i)==0x01){
								combo++;
								 
								LCD_ShowNumColor(2,2,combo,3,20,MAGENTA);//show combo
								StatusDisplay((unsigned char)2);//show good
								notedestroy(i);chartbuffer[2][i]=0;//destroy note
								
								//load new note
								chartbuffer[0][i]=chart[0][pChart];
								chartbuffer[1][i]=chart[1][pChart];
								chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
								chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
							}
						}
					}
					statustemp=0;
					buttonstatus=0;
				}
				//rotate,stop & drag judge
				MMA8451Q_data[0]=MMA8451Q_ReadRegister(0x01);//MSB of X-Axis 	
				MMA8451Q_data[1]=MMA8451Q_ReadRegister(0x03);//MSB of Y-Axis
				MMA8451Q_data[2]=MMA8451Q_ReadRegister(0x05);//MSB of Z-Axis
				if (MMA8451Q_data[0]&0x80) { 
					MMA8451Q_data[0]=-((~MMA8451Q_data[0])+1);	//��������
				}
				if (MMA8451Q_data[1]&0x80) { 
				    MMA8451Q_data[1]=-((~MMA8451Q_data[1])+1);	//��������
				}
				if (MMA8451Q_data[2]&0x80) { 
					MMA8451Q_data[2]=-((~MMA8451Q_data[2])+1);	//��������
				}
				//judge mma
				i=0;
				for(;i<32;i++){
					if(chartbuffer[2][i]==0x01){
						if(judgemma(chartbuffer[1][i],i)==0x01){
							combo++;
							 
							LCD_ShowNumColor(2,2,combo,3,20,YELLOW);//show combo
							StatusDisplay((unsigned char)1);//show PERFECT
							notedestroy(i);chartbuffer[2][i]=0;//destroy note
							
							//load new note
							chartbuffer[0][i]=chart[0][pChart];
							chartbuffer[1][i]=chart[1][pChart];
							chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
							chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
						}		
					}
				}
				
					i=0;
					for(;i<32;i++){
					if(chartbuffer[2][i]==0x00){
						
							combo=0;
							 
							LCD_ShowNumColor(2,2,combo,3,20,WHITE);//show combo
							StatusDisplay((unsigned char)4);//show MISS
							notedestroy(i);chartbuffer[2][i]=0;//destroy note
							
							//load new note
							chartbuffer[0][i]=chart[0][pChart];
							chartbuffer[1][i]=chart[1][pChart];
							chartbuffer[2][i]=chart[2][pChart]+chartbuffer[2][(i+31)%32];
							chartbuffer[3][i]=0;pChart++;if(chart[2][pChart]==0x00)pChart=0;
								
					}
					}
				
		break;
	default : 
		break;
	}
	frameSerial++;
	frameSerial=frameSerial %9;
	i = SYST_CSR; //�����������־λ
}
