#ifndef __LCD_H
#define __LCD_H		

#include <MKL25Z4.h>

/////////////////////////////////////用户配置区///////////////////////////////////	 
//以下2个宏定义，定义屏幕的显示方式及IO速度
#define USE_HORIZONTAL  0	//定义是否使用横屏 		0,不使用.1,使用.
#define LCD_FAST_IO     1 	//定义是否使用快速IO	0,不实用.1,使用
//////////////////////////////////////////////////////////////////////////////////	 


#define BIT0 0x01
#define BIT1 (1<<1)
#define BIT2 (1<<2)
#define BIT3 (1<<3)
#define BIT4 (1<<4)
#define BIT5 (1<<5)
#define BIT6 (1<<6)
#define BIT7 (1<<7)
#define BIT8 (1<<8)
#define BIT9 (1<<9)
#define BIT10 (1<<10)
#define BIT11 (1<<11)
#define BIT12 (1<<12)
#define BIT13 (1<<13)
#define BIT14 (1<<14)
#define BIT15 (1<<15)
#define BIT16 (1<<16)
#define BIT17 (1<<17)
#define BIT18 (1<<18)
#define BIT19 (1<<19)
#define BIT20 (1<<20)
#define BIT21 (1<<21)
#define BIT22 (1<<22)
#define BIT23 (1<<23)
#define BIT24 (1<<24)
#define BIT25 (1<<25)
#define BIT26 (1<<26)
#define BIT27 (1<<27)
#define BIT28 (1<<28)
#define BIT29 (1<<29)
#define BIT30 (1<<30)
#define BIT31 (1<<31)
 
#define pic_x 128
#define pic_y 96

//定义LCD的尺寸
#if USE_HORIZONTAL==1	//使用横屏
#define LCD_W 320
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 320
#endif
////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
//如果使用快速IO，则定义下句，如果不使用，则去掉即可！
//使用快速IO，刷屏速率可以达到28帧每秒！
//普通IO，只能14帧每秒！   
//-----------------LCD端口定义---------------- 
#define	LCD_LED PTA19_OUT //LCD背光    	
#define	LCD_CS	PTB9_OUT  //片选端口  	     
#define	LCD_RS	PTB8_OUT  //数据/命令           
#define	LCD_WR	PTB11_OUT  //写数据	
#define	LCD_RD	PTB10_OUT  //读数据	
#define LCD_RST PTE31_OUT

#define LCD_RST_SET (GPIOE_PDOR|=BIT31)
#define LCD_RST_CLR (GPIOE_PDOR&=~BIT31)
#define LCD_LED_SET (GPIOA_PDOR|=BIT19)
#define LCD_LED_CLR (GPIOA_PDOR&=~BIT19)
#define LCD_CS_SET (GPIOB_PDOR|=BIT9)
#define LCD_CS_CLR (GPIOB_PDOR&=~BIT9)
#define LCD_RS_SET (GPIOB_PDOR|=BIT8)
#define LCD_RS_CLR (GPIOB_PDOR&=~BIT8)
#define LCD_WR_SET (GPIOB_PDOR|=BIT11)
#define LCD_WR_CLR (GPIOB_PDOR&=~BIT11)
#define LCD_RD_SET (GPIOB_PDOR|=BIT10)
#define LCD_RD_CLR (GPIOB_PDOR&=~BIT10)


//PD0~7,作为数据线
#define DATAOUT(x) GPIO_PDOR_REG(PTD_BASE_PTR)=(x&0xff); //数据输出
#define DATAIN     GPIO_PDIR_REG(PTD_BASE_PTR);   //数据输入

//////////////////////////////////////////////////////////////////////
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 	         0XFFE0
#define GBLUE	         0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(unsigned int Color);	
void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos);
void LCD_DrawPoint(unsigned int x,unsigned int y);//画点
void LCD_DrawPointColor(unsigned int x,unsigned int y, unsigned int color);
//unsigned int  LCD_ReadPoint(unsigned int x,unsigned int y); //读点
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned int r);
void Draw_CircleColor(unsigned int x0,unsigned int y0,unsigned int r,unsigned int color);
float tan(unsigned int a);
void LCD_DrawArcFromZero(unsigned int x0,unsigned int y0,unsigned int r,unsigned int degree);
void LCD_DrawArcFromZeroColor(unsigned int x0,unsigned int y0,unsigned int r,unsigned int degree,unsigned int color);
void LCD_DrawArc(unsigned int x0,unsigned int y0,unsigned int r,unsigned int degreel,unsigned int degreeh);
void LCD_DrawArcColor(unsigned int x0,unsigned int y0,unsigned int r,unsigned int degreel,unsigned int degreeh,unsigned int color);
void LCD_DrawNoteColor(unsigned char mode,unsigned short r,unsigned int color);
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_DrawLineColor(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);		   
void LCD_DrawRectangleColor(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color);
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
void LCD_DrawDragNoteColor(unsigned short mode,unsigned short r, unsigned int color);
void LCD_DrawInsideNoteColor(unsigned short mode,unsigned short r,unsigned int color);
void LCD_DrawOutsideNoteColor(unsigned short mode,unsigned short degree,unsigned int color);
void LCD_DrawRotateNoteColor(unsigned short mode,unsigned short degree,unsigned int color);
void LCD_DrawStopNoteColor(unsigned short parameter,unsigned short color);
void LCD_ShowChar(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode);//显示一个字符
void LCD_ShowCharColor(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode, unsigned int color);
void LCD_ShowNum(unsigned int x,unsigned int y,unsigned long num,unsigned int len,unsigned int size);  //显示一个数字
void LCD_ShowNumColor(unsigned int x,unsigned int y,unsigned long num,unsigned int len,unsigned int size,unsigned int color);
void LCD_Show2Num(unsigned int x,unsigned int y,unsigned int num,unsigned int len,unsigned int size,unsigned int mode);//显示2个数字
void LCD_Show2NumColor(unsigned int x,unsigned int y,unsigned int num,unsigned int len,unsigned int size,unsigned int mode,unsigned int color);
void LCD_ShowString(unsigned char lx,unsigned char ly,char *sch);		 //显示一个字符串,16字体
void LCD_ShowStringColor(unsigned char lx,unsigned char ly,char *sch,unsigned int color);
									    
void LCD_WriteReg(unsigned int LCD_Reg, unsigned int LCD_RegValue);
unsigned int LCD_ReadReg(unsigned int LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(unsigned int RGB_Code);
//unsigned int LCD_ReadRAM(void);		   
unsigned int LCD_BGR2RGB(unsigned int c);
void LCD_WR_DATA(unsigned char data);
void LCD_WR_TDATA(unsigned int data);
				
#define x0 160
#define y0 120
//9320/9325 LCD寄存器  
#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5							  		 
#endif  
	 
	 



