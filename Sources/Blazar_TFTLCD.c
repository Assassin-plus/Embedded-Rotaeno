#include "Blazar_TFTLCD.h"
#include "font.h"
#include "KL2x_gpio.h"
/*
 * Original source by Project Blazar.
 * Modified by MEMo_Official ©2022 , All rights reserved.
 * Add colour mode to every draw functions.
 * no eraser function <-> ( colour = back_color )
 * Add circle arc draw function. Bezier curve needed to add.
 */

unsigned int POINT_COLOR = BLACK, BACK_COLOR = WHITE;
unsigned int DeviceCode;

void delay_ms(int time)
{
	int i, j;
	for (i = 0; i < time / 4.9; i++)
	{
		for (j = 48000; j > 0; j--)
		{
			asm("nop");
		}
	}
}

//閸愭瑥鐦庣�妯烘珤閸戣姤鏆�
void LCD_RESET(void)
{
	LCD_RST_SET; //妤傛鏁搁敓锟�
				 // delay_ms(1);
	LCD_RST_CLR; //娴ｅ海鏁搁敓锟�	//delay_ms(20);
	LCD_RST_SET; //妤傛鏁搁敓锟�
				 // delay_ms(20);
}

//閸愭瑥鐦庣�妯烘珤閸戣姤鏆�
void LCD_WR_REG(unsigned int data)
{
	LCD_RS_CLR; //閸愭瑥婀撮崸锟�
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}

//閿燂拷娴ｅ秵鏆熼幑顔煎毐閿燂�?
//閻劌鐣�姘�?,閹绘劙鐝柅鐔峰.

void LCD_WR_DATA(unsigned char data)
{
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}

void LCD_WR_TDATA(unsigned int data)
{
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data & 0xFF00);
	LCD_WR_CLR;
	LCD_WR_SET;
	DATAOUT(data << 8);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}
//閸愭瑥鐦庣�妯烘�?
void LCD_WriteReg(unsigned int LCD_Reg, unsigned int LCD_RegValue)
{
	LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}
//鐠囪鐦庣�妯烘�?
unsigned int LCD_ReadReg(unsigned int LCD_Reg)
{
	unsigned int t;
	LCD_WR_REG(LCD_Reg); //閸愭瑥鍙嗙憰浣筋嚢閻ㄥ嫬鐦庣�妯烘珤閿燂�?
						 //    GPIOC->CRL=0X88888888; //PC0-7  娑撳﹥濯烘潏鎾冲�?
						 //    GPIOC->CRH=0X88888888; //PC8-15 娑撳﹥濯烘潏鎾冲�?
						 //    GPIOC->ODR=0XFFFF;    //閸忋劑鍎存潏鎾冲毉閿燂拷
						 //鐠佸墽鐤嗛弫鐗堝祦鏉堟挸鍤璉O娑撶儤甯归幐鍊熺翻閸戠尨绱�
	gpio_init(PTD0, GPI_UP, 1);
	gpio_init(PTD1, GPI_UP, 1);
	gpio_init(PTD2, GPI_UP, 1);
	gpio_init(PTD3, GPI_UP, 1);
	gpio_init(PTD4, GPI_UP, 1);
	gpio_init(PTD5, GPI_UP, 1);
	gpio_init(PTD6, GPI_UP, 1);
	gpio_init(PTD7, GPI_UP, 1);

	LCD_RS_SET;
	LCD_CS_CLR;
	//鐠囪褰囬弫鐗堝�?(鐠囪鐦庣�妯烘珤閿燂拷楠炴湹绗夐棁锟筋渽鐠囷拷閿燂�?
	LCD_RS_CLR;
	LCD_RS_SET;
	t = DATAIN;
	LCD_CS_SET;
	//    GPIOC->CRL=0X33333333; //PC0-7  娑撳﹥濯烘潏鎾冲�?
	//    GPIOC->CRH=0X33333333; //PC8-15 娑撳﹥濯烘潏鎾冲�?
	//    GPIOC->ODR=0XFFFF;    //閸忋劑鍎存潏鎾冲毉閿燂拷
	gpio_init(PTD0, GPO, 1);
	gpio_init(PTD1, GPO, 1);
	gpio_init(PTD2, GPO, 1);
	gpio_init(PTD3, GPO, 1);
	gpio_init(PTD4, GPO, 1);
	gpio_init(PTD5, GPO, 1);
	gpio_init(PTD6, GPO, 1);
	gpio_init(PTD7, GPO, 1);
	return t;
}

//瀵拷顬婇崘姗AM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(0x2c);
}
// LCD閸愭─RAM
void LCD_WriteRAM(unsigned int RGB_Code)
{
	LCD_WR_DATA(RGB_Code); //閸愭瑥宕勯崗顓濈秴GRAM
}
//娴犲丢LI93xx鐠囪鍤惃鍕殶閹诡喕璐烥BR閺嶇厧绱￠敍宀冿拷閹存垳婊戦崘娆忓弳閻ㄥ嫭妞傞崐娆庤礋RGB閺嶇厧绱￠敓锟�//闁俺绻冪拠銉ュ毐閺佹媽娴嗛敓锟�?//c:GBR閺嶇厧绱￠惃鍕杹閼硅鎷�
//鏉╂柨娲栭崐纭风窗RGB閺嶇厧绱￠惃鍕杹閼硅鎷�
unsigned int LCD_BGR2RGB(unsigned int c)
{
	unsigned int r, g, b, rgb;
	b = (c >> 0) & 0x1f;
	g = (c >> 5) & 0x3f;
	r = (c >> 11) & 0x1f;
	rgb = (b << 11) + (g << 5) + (r << 0);
	return (rgb);
}
//鐠囪褰囨稉顏呯厙閻愬湱娈戞０婊嗗閿燂�?
// x:0~239
// y:0~319
//鏉╂柨娲栭敓锟藉銈囧仯閻ㄥ嫰顤侀敓锟�?
/*unsigned int LCD_ReadPoint(unsigned int x,unsigned int y)
{
	unsigned int t;
	if(x>=LCD_W||y>=LCD_H)return 0;//鐡掑懓绻冩禍鍡氬瘱閿燂拷閻╁瓨甯存潻鏂挎礀
	LCD_SetCursor(x,y);
	LCD_WR_REG(R34);       //闁瀚℅RAM閸︽澘娼�?
	GPIOB->CRL=0X88888888; //PB0-7  娑撳﹥濯烘潏鎾冲�?
	GPIOB->CRH=0X88888888; //PB8-15 娑撳﹥濯烘潏鎾冲�?
	GPIOB->ODR=0XFFFF;     //閸忋劑鍎存潏鎾冲毉閿燂拷
#if LCD_FAST_IO==1 //韫囶偊锟絀O
	LCD_RS_SET;
	LCD_CS_CLR;
	//鐠囪褰囬弫鐗堝�?(鐠囩眾RAM閿燂拷闂囷拷顪呯拠锟介敓锟�
	LCD_RD_CLR;
	LCD_RD_SET;
	delay_us(2);//FOR 9320,瀵よ埖妞�us
	//dummy READ
	LCD_RD_CLR;
	delay_us(2);//FOR 8989,瀵よ埖妞�us
	LCD_RD_SET;
	t=DATAIN;
	LCD_CS_SET;
#else
	LCD_RS_SET;
	LCD_CS_CLR;
	//鐠囪褰囬弫鐗堝�?(鐠囩眾RAM閿燂拷闂囷拷顪呯拠锟介敓锟�
	LCD_RS_CLR;
	LCD_RS_SET;
	//dummy READ
	LCD_RS_CLR;
	LCD_RS_SET;
	t=DATAIN;
	LCD_CS_SET;
#endif
	GPIOB->CRL=0X33333333; //PB0-7  娑撳﹥濯烘潏鎾冲�?
	GPIOB->CRH=0X33333333; //PB8-15 娑撳﹥濯烘潏鎾冲�?
	GPIOB->ODR=0XFFFF;    //閸忋劑鍎存潏鎾冲毉閿燂拷
	if(DeviceCode==0X4535||DeviceCode==0X4531||DeviceCode==0X8989||DeviceCode==0XB505)return t;//鏉╂瑥鍤戠粔宀籆閻╁瓨甯存潻鏂挎礀妫版粏澹婇敓锟�	else return LCD_BGR2RGB(t);
}*/
// LCD瀵拷鎯庨弰鎾呮�??
void LCD_DisplayOn(void)
{
	LCD_WriteReg(R7, 0x0173); // 26娑撳洩澹婇弰鍓с仛瀵拷鎷�?
}
// LCD閸忔娊妫撮弰鍓с仛
void LCD_DisplayOff(void)
{
	LCD_WriteReg(R7, 0x0); //閸忔娊妫撮弰鍓с仛
}
//鐠佸墽鐤嗛崗澶嬬垼娴ｅ秶鐤�
// Xpos:濡亜娼楅敓锟�//Ypos:缁鹃潧娼楅敓锟�
void LCD_SetCursor(unsigned int Xpos, unsigned int Ypos)
{
	LCD_WR_REG(0x2b);
	LCD_WR_DATA(Xpos >> 8);
	LCD_WR_DATA(Xpos & 0xff);
	LCD_WR_DATA(Xpos >> 8);
	LCD_WR_DATA(Xpos & 0xff);

	LCD_WR_REG(0x2a);
	LCD_WR_DATA(Ypos >> 8);
	LCD_WR_DATA(Ypos & 0xff);
	LCD_WR_DATA(Ypos >> 8);
	LCD_WR_DATA(Ypos & 0xff);
}
//閻㈣崵鍋�?
// x:0~239
// y:0~319
// POINT_COLOR:濮濄倗鍋ｉ惃鍕杹閿燂拷

//**********************************************************************************************************
/**
 * @brief 
 * 
 * @param x 
 * @param y 
 */
void LCD_DrawPoint(unsigned int x, unsigned int y)
{
	LCD_SetCursor(x, y); //鐠佸墽鐤嗛崗澶嬬垼娴ｅ秶鐤�
	LCD_WR_REG(0x2c);	 //瀵拷顬婇崘娆忓弳GRAM
	LCD_WR_DATA(POINT_COLOR >> 8);
	LCD_WR_DATA(POINT_COLOR & 0xff);
}

void LCD_DrawPointColor(unsigned int x, unsigned int y, unsigned int color)
{
	LCD_SetCursor(x, y); //鐠佸墽鐤嗛崗澶嬬垼娴ｅ秶鐤�
	LCD_WR_REG(0x2c);	 //瀵拷顬婇崘娆忓弳GRAM
	LCD_WR_DATA(color >> 8);
	LCD_WR_DATA(color & 0xff);
}

#define DELAY delay_ms
//閸掓繂顬婇崠鏉攃d
//鐠囥儱鍨垫慨瀣閸戣姤鏆熼崣顖欎簰閸掓繂顬婇崠鏍ф倗缁夊不LI93XX濞戝弶娅�娴ｅ棙妲搁崗鏈电铂閸戣姤鏆熼弰顖氱唨娴滃丢LI9320閿燂�?!!
//閸︺劌鍙炬禒鏍х�閸欓娈戞す鍗炲З閼侯垳澧栨稉濠冪梾閺堝绁撮敓锟�?

void LCD_Init(void)
{
	gpio_init(PTB11, GPO, 1); //鐠佸墽鐤哛W 閹恒劍灏虫潏鎾冲毉閿涘苯鍨垫慨瀣翻閸戞椽鐝悽闈涢挬
	gpio_init(PTB8, GPO, 1);  //鐠佸墽鐤哛S
	gpio_init(PTB10, GPO, 1); //鐠佸墽鐤哛D
	gpio_init(PTB9, GPO, 1);  //鐠佸墽鐤咰S
	gpio_init(PTA19, GPO, 1); //鐠佸墽鐤咮K閼冲苯鍘滈幒褍鍩�?

	//鐠佸墽鐤嗛弫鐗堝祦鏉堟挸鍤璉O娑撶儤甯归幐鍊熺翻閸戠尨绱�
	gpio_init(PTD0, GPO, 1);
	gpio_init(PTD1, GPO, 1);
	gpio_init(PTD2, GPO, 1);
	gpio_init(PTD3, GPO, 1);
	gpio_init(PTD4, GPO, 1);
	gpio_init(PTD5, GPO, 1);
	gpio_init(PTD6, GPO, 1);
	gpio_init(PTD7, GPO, 1);
	// TFT婢跺秳缍�?
	gpio_init(PTE31, GPO, 1); //鐠佸墽鐤哛ST楠炶埖濯烘担搴ょ箻鐞涘苯顦炬担锟�?
	LCD_RESET();			  //婢跺秳缍�?

	delay_ms(15); // delay 50 ms
	LCD_WriteReg(0x00, 0x01);
	delay_ms(15); // delay 50 ms
	DeviceCode = LCD_ReadReg(0x04);
	//	printf(" LCD ID:%x\n",DeviceCode);

	//************* Start Initial Sequence **********//   ILI9341+IVO2.8
	LCD_WR_REG(0xCF);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xD9);
	LCD_WR_DATA(0X30);

	LCD_WR_REG(0xED);
	LCD_WR_DATA(0x64);
	LCD_WR_DATA(0x03);
	LCD_WR_DATA(0X12);
	LCD_WR_DATA(0X81);

	LCD_WR_REG(0xE8);
	LCD_WR_DATA(0x85);
	LCD_WR_DATA(0x10);
	LCD_WR_DATA(0x78);

	LCD_WR_REG(0xCB);
	LCD_WR_DATA(0x39);
	LCD_WR_DATA(0x2C);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x34);
	LCD_WR_DATA(0x02);

	LCD_WR_REG(0xF7);
	LCD_WR_DATA(0x20);

	LCD_WR_REG(0xEA);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0xC0);  // Power control
	LCD_WR_DATA(0x21); // VRH[5:0]

	LCD_WR_REG(0xC1);  // Power control
	LCD_WR_DATA(0x12); // SAP[2:0];BT[3:0]

	LCD_WR_REG(0xC5); // VCM control
	LCD_WR_DATA(0x32);
	LCD_WR_DATA(0x3C);

	LCD_WR_REG(0xC7); // VCM control2
	LCD_WR_DATA(0XC1);

	LCD_WR_REG(0x36);  // Memory Access Control
	LCD_WR_DATA(0x88); // 0x08(娴犲骸涔忛崚鏉垮�?,娴犲簼绗傞崚棰佺�?) -> 0x88(娴犲骸涔忛崚鏉垮�?,娴犲簼绗呴崚棰佺�?)

	LCD_WR_REG(0x3A);
	LCD_WR_DATA(0x55);

	LCD_WR_REG(0xB1);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x18);

	LCD_WR_REG(0xB6); // Display Function Control
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0xA2);

	LCD_WR_REG(0xF2); // 3Gamma Function Disable
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0x26); // Gamma curve selected
	LCD_WR_DATA(0x01);

	LCD_WR_REG(0xE0); // Set Gamma
	LCD_WR_DATA(0x0F);
	LCD_WR_DATA(0x20);
	LCD_WR_DATA(0x1E);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x0B);
	LCD_WR_DATA(0x50);
	LCD_WR_DATA(0XBA);
	LCD_WR_DATA(0x44);
	LCD_WR_DATA(0x09);
	LCD_WR_DATA(0x14);
	LCD_WR_DATA(0x05);
	LCD_WR_DATA(0x23);
	LCD_WR_DATA(0x21);
	LCD_WR_DATA(0x00);

	LCD_WR_REG(0XE1); // Set Gamma
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x19);
	LCD_WR_DATA(0x19);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x12);
	LCD_WR_DATA(0x07);
	LCD_WR_DATA(0x2D);
	LCD_WR_DATA(0x28);
	LCD_WR_DATA(0x3F);
	LCD_WR_DATA(0x02);
	LCD_WR_DATA(0x0A);
	LCD_WR_DATA(0x08);
	LCD_WR_DATA(0x25);
	LCD_WR_DATA(0x2D);
	LCD_WR_DATA(0x0F);

	LCD_WR_REG(0x11); // Exit Sleep
	DELAY(120);
	LCD_WR_REG(0x29); // Display on

	LCD_LED_CLR; //閻愰�瀵掗懗灞藉帨
				 //	LCD_Clear(WHITE);
}

//濞撳懎鐫嗛崙鑺ユ�?
// Color:鐟曚焦绔荤仦蹇曟畱婵夘偄鍘栭敓锟�?
void LCD_Clear(unsigned int Color)
{
	/*	unsigned long index=0;
			LCD_SetCursor(0,0);
			LCD_WriteRAM_Prepare();     //瀵拷顬婇崘娆忓弳GRAM
		for(index=0;index<76800;index++)
		{
			  LCD_WR_DATA(Color>>8);
			  LCD_WR_DATA(Color&0xff);
		}
	*/
	unsigned int index, index1;
	for (index = 0; index < 320; index++)
	{
		for (index1 = 0; index1 < 240; index1++)
		{
			LCD_SetCursor(index, index1); //鐠佸墽鐤嗛崗澶嬬垼娴ｅ秶鐤�
			LCD_WR_REG(0x2c);			  //瀵拷顬婇崘娆忓弳GRAM
			LCD_WR_DATA(Color >> 8);
			LCD_WR_DATA(Color & 0xff);
		}
	}
}
//閸︺劍瀵氱�姘隘閸╃喎鍞存繅顐㈠帠閹稿洤鐣炬０婊嗗�?
//閸栧搫鐓欐径褍鐨�?
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color)
{
	unsigned int i, j;
	unsigned int xlen = 0;
#if USE_HORIZONTAL == 1
	xlen = yend - ysta + 1;
	for (i = xsta; i <= xend; i++)
	{

		for (j = ysta; j < yend; j++)
		{
			LCD_SetCursor(i, j); //鐠佸墽鐤嗛崗澶嬬垼娴ｅ秶鐤�
			LCD_WR_REG(0x2c);	 //瀵拷顬婇崘娆忓弳GRAM
			LCD_WR_DATA(color >> 8);
			LCD_WR_DATA(color & 0xff);
		}
	}
#else
	xlen = xend - xsta + 1;
	for (i = ysta; i <= yend; i++)
	{
		for (j = ysta; j < yend; j++)
		{
			LCD_SetCursor(i, j); //鐠佸墽鐤嗛崗澶嬬垼娴ｅ秶鐤�
			LCD_WR_REG(0x2c);	 //瀵拷顬婇崘娆忓弳GRAM
			LCD_WR_DATA(color >> 8);
			LCD_WR_DATA(color & 0xff);
		}
	}
#endif
}
//閻㈣崵鍤�?
// x1,y1:鐠ч鍋ｉ崸鎰垼
// x2,y2:缂佸牏鍋ｉ崸鎰�?
void LCD_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	unsigned int t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;

	delta_x = x2 - x1; //鐠侊紕鐣婚崸鎰垼婢х偤鍣�?
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //鐠佸墽鐤嗛崡鏇燁劄閺傜懓鎮�
	else if (delta_x == 0)
		incx = 0; //閸ㄥ倻娲块敓锟�
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //濮樻潙閽╅敓锟�
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; //闁褰囬崺鐑樻拱婢х偤鍣洪崸鎰垼閿燂拷
	else
		distance = delta_y;
	for (t = 0; t <= distance + 1; t++) //閻㈣崵鍤庢潏鎾冲�?
	{
		LCD_DrawPoint(uRow, uCol); //閻㈣崵鍋�?
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}
void LCD_DrawLineColor(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
	unsigned int t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;

	delta_x = x2 - x1; //鐠侊紕鐣婚崸鎰垼婢х偤鍣�?
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //鐠佸墽鐤嗛崡鏇燁劄閺傜懓鎮�
	else if (delta_x == 0)
		incx = 0; //閸ㄥ倻娲块敓锟�
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //濮樻潙閽╅敓锟�
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; //闁褰囬崺鐑樻拱婢х偤鍣洪崸鎰垼閿燂拷
	else
		distance = delta_y;
	for (t = 0; t <= distance + 1; t++) //閻㈣崵鍤庢潏鎾冲�?
	{
		LCD_DrawPointColor(uRow, uCol, color); //閻㈣崵鍋�?
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}
//閻㈣崵鐓╅敓锟�
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	LCD_DrawLine(x1, y1, x2, y1);
	LCD_DrawLine(x1, y1, x1, y2);
	LCD_DrawLine(x1, y2, x2, y2);
	LCD_DrawLine(x2, y1, x2, y2);
}
void LCD_DrawRectangleColor(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color)
{
	LCD_DrawLineColor(x1, y1, x2, y1, color);
	LCD_DrawLineColor(x1, y1, x1, y2, color);
	LCD_DrawLineColor(x1, y2, x2, y2, color);
	LCD_DrawLineColor(x2, y1, x2, y2, color);
}
//閸︺劍瀵氱�姘秴缂冾喚鏁炬稉锟介嚋閹稿洤鐣炬径褍鐨惃鍕舵嫹?
//(x,y):娑擃厼绺鹃敓锟�//r    :閸楀﹤绶�?
/**
 * @brief 
 * 
 * @param x1 
 * @param y1 
 * @param r 
 */
void Draw_Circle(unsigned int x1, unsigned int y1, unsigned int r)
{
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); //閸掋倖鏌囨稉瀣╅嚋閻愰�缍呯純顔炬畱閺嶅洤绻�?
	while (a <= b)
	{
		LCD_DrawPoint(x1 - b, y1 - a); // 3
		LCD_DrawPoint(x1 + b, y1 - a); // 0
		LCD_DrawPoint(x1 - a, y1 + b); // 1
		LCD_DrawPoint(x1 - b, y1 - a); // 7
		LCD_DrawPoint(x1 - a, y1 - b); // 2
		LCD_DrawPoint(x1 + b, y1 + a); // 4
		LCD_DrawPoint(x1 + a, y1 - b); // 5
		LCD_DrawPoint(x1 + a, y1 + b); // 6
		LCD_DrawPoint(x1 - b, y1 + a);
		a++;
		//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
		if (di < 0)
			di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
		LCD_DrawPoint(x1 + a, y1 + b);
	}
}
void Draw_CircleColor(unsigned int x1, unsigned int y1, unsigned int r, unsigned int color)
{
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); //閸掋倖鏌囨稉瀣╅嚋閻愰�缍呯純顔炬畱閺嶅洤绻�?
	while (a <= b)
	{
		LCD_DrawPointColor(x1 - b, y1 - a, color); // 3
		LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
		LCD_DrawPointColor(x1 - a, y1 + b, color); // 1
		LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
		LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
		LCD_DrawPointColor(x1 + b, y1 + a, color); // 4
		LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
		LCD_DrawPointColor(x1 + a, y1 + b, color); // 6
		LCD_DrawPointColor(x1 - b, y1 + a, color);
		a++;
		//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
		if (di < 0)
			di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
		LCD_DrawPointColor(x1 + a, y1 + b, color);
	}
}
/*
*all sources above are written by Blazar Project
*functions with suffix *Color are modified by Luziqi
*/

//************************************************************************************************
/*
* function: tan uses CORDIC method to get tangent values
* J. E. Volder, "The CORDIC Trigonometric Computing Technique," in IRE Transactions on Electronic Computers, vol. EC-8, no. 3, pp. 330-334, Sept. 1959, doi: 10.1109/TEC.1959.5222693.
*/
//************************************************************************************************
float angle[] = {180 / 8, 180 / 16,
				 180 / 32, 180 / 64, 180 / 128,
				 180 / 256, 180 / 512, 180 / 1024,
				 180 / 2048, 180 / 4096, 180 / 8192, 180 / 16384};

float tang[] = {0.4142135623731, 0.19891236737966,
				0.098491403357164, 0.049126849769467,
				0.024548622108925, 0.012272462379566,
				0.0061360001576234, 0.0030679712014227,
				0.0015339819910887, 0.00076699054434309,
				0.00038349521577144, 0.00019174760083571};
/**
 * @brief trigonometric function
 * 
 * @param a degree of an angle
 * @return float 
 */
float tan(unsigned int a)
{
	//开�? CORDIC 算法
	float x = 10;
	float y = 0;
	float theta = 0;
	int i = 0;
	for (; i < 13; i++)
	{ //开始迭�?
		float orix, oriy;
		while (theta < a)
		{ //当前角度小于a
			orix = x;
			oriy = y;
			//坐标旋转
			x = orix - tang[i] * oriy;
			y = tang[i] * orix + oriy;
			theta += angle[i];
		}
		if (theta == a)
		{
			return (y / x);
		}
		else
		{
			//旋转的弧度超过了a，退回原来增加的角度，同时进入下一次迭�?
			theta -= angle[i];
			x = orix;
			y = oriy;
		}
	}
}
//**************************************************************************************************************************************************
/**
 * @brief arc draw from degree zero
 * 		develop on the basis of function Draw_Circle
 * 		needs to be optimized
 * @param x1 x-axis of the center of the arc
 * @param y1 y-axis of the center of the arc
 * @param r radius of the arc
 * @param degree end degree of the arc
 */
void LCD_DrawArcFromZero(unsigned int x1, unsigned int y1, unsigned int r, unsigned int degree)
{
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); //閸掋倖鏌囨稉瀣╅嚋閻愰�缍呯純顔炬畱閺嶅洤绻�?
	float tangent = tan(degree % 45);
	switch (degree / 45)
	{
	case 0:
		while (a <= tangent * b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 1:
		tangent = tan(45 - degree % 45);
		while (a <= tangent * b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 2:
		while (a <= tangent * b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 3:
		tangent = tan(45 - degree % 45);
		while (a <= tangent * b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - b, y1 - a); // 7
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 4:
		while (a <= tangent * b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - b, y1 + a); // 1
			LCD_DrawPoint(x1 - b, y1 - a); // 7
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - b, y1 - a); // 7
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 5:
		tangent = tan(45 - degree % 45);
		while (a <= tangent * b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - b, y1 + a); // 1
			LCD_DrawPoint(x1 - b, y1 - a); // 7
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - a, y1 + b); // 1
			LCD_DrawPoint(x1 - b, y1 - a); // 7
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			LCD_DrawPoint(x1 - b, y1 + a);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 6:
		while (a <= tangent * b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - a, y1 + b); // 1
			LCD_DrawPoint(x1 - b, y1 - a); // 7
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			LCD_DrawPoint(x1 - b, y1 + a);
			LCD_DrawPoint(x1 + a, y1 + b);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - a, y1 + b); // 1
			LCD_DrawPoint(x1 - b, y1 - a); // 7
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			LCD_DrawPoint(x1 - b, y1 + a);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 7:
		tangent = tan(45 - degree % 45);
		while (a <= tangent * b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - a, y1 + b); // 1
			LCD_DrawPoint(x1 - b, y1 - a); // 7
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			LCD_DrawPoint(x1 - b, y1 + a);
			LCD_DrawPoint(x1 + a, y1 + b);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPoint(x1 + b, y1 - a); // 0
			LCD_DrawPoint(x1 - a, y1 + b); // 1
			LCD_DrawPoint(x1 - b, y1 - a); // 7
			LCD_DrawPoint(x1 - a, y1 - b); // 2
			LCD_DrawPoint(x1 + a, y1 - b); // 5
			LCD_DrawPoint(x1 - b, y1 + a);
			LCD_DrawPoint(x1 + a, y1 + b);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPoint(x1 + b, y1 + a);
		}
		break;
	default:
		degree = degree % 360;
		LCD_DrawArcFromZero(x1, y1, r, degree);
		break;
	}
}
void LCD_DrawArcFromZeroColor(unsigned int x1, unsigned int y1, unsigned int r, unsigned int degree, unsigned int color)
{
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); //閸掋倖鏌囨稉瀣╅嚋閻愰�缍呯純顔炬畱閺嶅洤绻�?
	float tangent = tan(degree % 45);
	switch (degree / 45)
	{
	case 0:
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 1:
		tangent = tan(45 - degree % 45);
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 2:
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 3:
		tangent = tan(45 - degree % 45);
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 4:
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - b, y1 + a, color); // 1
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 5:
		tangent = tan(45 - degree % 45);
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - b, y1 + a, color); // 1
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - a, y1 + b, color); // 1
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			LCD_DrawPointColor(x1 - b, y1 + a, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 6:
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - a, y1 + b, color); // 1
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			LCD_DrawPointColor(x1 - b, y1 + a, color);
			LCD_DrawPointColor(x1 + a, y1 + b, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - a, y1 + b, color); // 1
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			LCD_DrawPointColor(x1 - b, y1 + a, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 7:
		tangent = tan(45 - degree % 45);
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - a, y1 + b, color); // 1
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			LCD_DrawPointColor(x1 - b, y1 + a, color);
			LCD_DrawPointColor(x1 + a, y1 + b, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		while (a <= b)
		{
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			LCD_DrawPointColor(x1 - a, y1 + b, color); // 1
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			LCD_DrawPointColor(x1 - b, y1 + a, color);
			LCD_DrawPointColor(x1 + a, y1 + b, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 + b, y1 + a, color);
		}
		break;
	default:
		degree = degree % 360;
		LCD_DrawArcFromZeroColor(x1, y1, r, degree, color);
		break;
	}
}

/**
 * @brief universal arc draw
 * 
 * @param x1 x-axis of the center of the arc
 * @param y1 y-axis of the center of the arc
 * @param r radius of the arc
 * @param degreel begin degree of the arc
 * @param degreeh end degree of the arc
 */

void LCD_DrawArc(unsigned int x1, unsigned int y1, unsigned int r, unsigned int degreel, unsigned int degreeh)
{
	// degreeh>degreel
	LCD_DrawArcFromZero(x1, y1, r, degreeh);
	LCD_DrawArcFromZeroColor(x1, y1, r, degreel, BACK_COLOR);
}
void LCD_DrawArcColor(unsigned int x1, unsigned int y1, unsigned int r, unsigned int degreel, unsigned int degreeh, unsigned int color)
{
	// degree-h > degree-l
	if ((degreeh - degreel) == 1)
	{
		int a, b;
		int di;
		a = 0;
		b = r;
		di = 3 - (r << 1); //閸掋倖鏌囨稉瀣╅嚋閻愰�缍呯純顔炬畱閺嶅洤绻�?
		float tangent = tan(degreeh % 45);
		switch (degreeh / 45)
		{
		case 0:
			while (a < tangent * b)
			{
				a++;
				//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
				if (di < 0)
					di += 4 * a + 6;
				else
				{
					di += 10 + 4 * (a - b);
					b--;
				}
			}
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 + b, y1 - a, color); // 0
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 + b, y1 - a, color);
			break;
		case 1:
			tangent = tan(45 - degreeh % 45);
			while (a < tangent * b)
			{
				a++;
				//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
				if (di < 0)
					di += 4 * a + 6;
				else
				{
					di += 10 + 4 * (a - b);
					b--;
				}
			}
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 + a, y1 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 + a, y1 - b, color);
			break;
		case 2:
			while (a < tangent * b)
			{
				a++;
				//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
				if (di < 0)
					di += 4 * a + 6;
				else
				{
					di += 10 + 4 * (a - b);
					b--;
				}
			}
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 - a, y1 - b, color); // 2
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 - a, y1 - b, color);
			break;
		case 3:
			tangent = tan(45 - degreeh % 45);
			while (a < tangent * b)
			{
				a++;
				if (di < 0)
					di += 4 * a + 6;
				else
				{
					di += 10 + 4 * (a - b);
					b--;
				}
			}
			LCD_DrawPointColor(x1 - b, y1 - a, color); // 7
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 - b, y1 - a, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 - b, y1 - a, color);
			break;
		case 4:
			while (a < tangent * b)
			{
				a++;
				//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
				if (di < 0)
					di += 4 * a + 6;
				else
				{
					di += 10 + 4 * (a - b);
					b--;
				}
			}
			LCD_DrawPointColor(x1 - b, y1 + a, color); // 1
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 - b, y1 + a, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 - b, y1 + a, color);
			break;
		case 5:
			tangent = tan(45 - degreeh % 45);
			while (a < tangent * b)
			{
				a++;
				//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
				if (di < 0)
					di += 4 * a + 6;
				else
				{
					di += 10 + 4 * (a - b);
					b--;
				}
			}
			LCD_DrawPointColor(x1 - a, y1 + b, color); // 1 �?
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 - a, y1 + b, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 - a, y1 + b, color);
			break;
		case 6:
			while (a < tangent * b)
			{
				a++;
				//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
				if (di < 0)
					di += 4 * a + 6;
				else
				{
					di += 10 + 4 * (a - b);
					b--;
				}
			}
			LCD_DrawPointColor(x1 + a, y1 + b, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 + a, y1 + b, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 + a, y1 + b, color);
			break;
		case 7:
			tangent = tan(45 - degreeh % 45);
			while (a <= tangent * b)
			{
				a++;
				//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
				if (di < 0)
					di += 4 * a + 6;
				else
				{
					di += 10 + 4 * (a - b);
					b--;
				}
			}
			LCD_DrawPointColor(x1 + b, y1 + a, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 + b, y1 + a, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
			LCD_DrawPointColor(x1 + b, y1 + a, color);
			break;
		}
	}
	else
	{
		LCD_DrawArcFromZeroColor(x1, y1, r, degreeh, color);
		LCD_DrawArcFromZeroColor(x1, y1, r, degreel, BACK_COLOR);
	}
}
//***************************************************************************************************************************************************
//functions below are integrated ones to draw graphics specified for the game
/**
 * @brief a specified arc
 * 
 * @param mode see the annotation inside the function
 * @param r radius of the note
 * @param color 
 */
void LCD_DrawInsideNoteColor(unsigned short mode, unsigned short r, unsigned int color)
{
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1); //閸掋倖鏌囨稉瀣╅嚋閻愰�缍呯純顔炬畱閺嶅洤绻�?
	float tangent = tan(15), tangent1 = tan(30);
	if (r == 100)
	{
		color = WHITE;
	}
	switch (mode)
	{
	/*
	 * 0-7 inside arc
	 * mode = 0->up arc
	 * mode = 1->up&left arc
	 * mode = 2->left arc
	 * mode = 3->left&down arc
	 * mode = 4->down arc
	 * mode = 5->down&right arc
	 * mode = 6->right arc
	 * mode = 7->right&up arc
	 */
	case 0:
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x0 - a, y0 - b, color); // 2
			LCD_DrawPointColor(x0 + a, y0 - b, color); // 5
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 1:
		a = b / 1.41421;
		b = b / 1.41421;
		while (a >= tangent1 * b)
		{
			LCD_DrawPointColor(x0 - b, y0 - a, color); // 3
			LCD_DrawPointColor(x0 - a, y0 - b, color); // 2
			a--;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b++;
			}
		}
		break;
	case 2:
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x0 - b, y0 - a, color); // 3
			LCD_DrawPointColor(x0 - b, y0 + a, color);
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 3:
		a = b / 1.41421;
		b = b / 1.41421;
		while (a >= tangent1 * b)
		{
			LCD_DrawPointColor(x0 - a, y0 + b, color); // 1
			LCD_DrawPointColor(x0 - b, y0 + a, color);
			a--;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b++;
			}
		}
		break;
	case 4:
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x0 - a, y0 + b, color); // 1
			LCD_DrawPointColor(x0 + a, y0 + b, color); // 6
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 5:
		a = b / 1.41421;
		b = b / 1.41421;
		while (a >= tangent1 * b)
		{
			LCD_DrawPointColor(x0 + b, y0 + a, color); // 4
			a--;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b++;
			}
			LCD_DrawPointColor(x0 + a, y0 + b, color);
		}
		break;
	case 6:
		while (a <= tangent * b)
		{
			LCD_DrawPointColor(x0 + b, y0 - a, color); // 0
			LCD_DrawPointColor(x0 + b, y0 + a, color); // 4
			a++;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b--;
			}
		}
		break;
	case 7:
		a = b / 1.41421;
		b = b / 1.41421;
		while (a >= tangent1 * b)
		{
			LCD_DrawPointColor(x0 + b, y0 - a, color); // 0
			LCD_DrawPointColor(x0 + a, y0 - b, color); // 5
			a--;
			//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
			if (di < 0)
				di += 4 * a + 6;
			else
			{
				di += 10 + 4 * (a - b);
				b++;
			}
		}
		break;
	}
	if (r == 100)
	{
		Draw_Circle(160, 120, 100);
	}
}
/**
 * @brief a specified arrow
 * 
 * @param mode eight directions from 0-7
 * @param r the outside circle radius of the arrow
 * @param color 
 */
void LCD_DrawDragNoteColor(unsigned short mode, unsigned short r, unsigned int color)
{
	switch (mode)
	{
	case 0:
		LCD_DrawLineColor(160, 120, 160, 120 - r, color);
		LCD_DrawLineColor(160, 120 - r, 160 - r / 6, 120 - r * 2 / 3, color);
		LCD_DrawLineColor(160, 120 - r, 160 + r / 6, 120 - r * 2 / 3, color);
		if (r == 100)
		{
			LCD_DrawLineColor(160, 120, 160, 120 - r, BACK_COLOR);
			LCD_DrawLineColor(160, 120 - r, 160 - r / 6, 120 - r * 2 / 3, BACK_COLOR);
			LCD_DrawLineColor(160, 120 - r, 160 + r / 6, 120 - r * 2 / 3, BACK_COLOR);
			LCD_DrawPointColor(160, 120 - r, BLACK);
		}

		break;
	case 1:
		LCD_DrawLineColor(160, 120, 160 - r / 1.41421, 120 - r / 1.41421, color);
		LCD_DrawLineColor(160 - r / 1.41421, 120 - r / 1.41421, 160 - r * 7 / 12, 120 - r / 3, color);
		LCD_DrawLineColor(160 - r / 1.41421, 120 - r / 1.41421, 160 - r / 3, 120 - r * 7 / 12, color);
		if (r == 100)
		{
			LCD_DrawLineColor(160, 120, 160 - r / 1.41421, 120 - r / 1.41421, BACK_COLOR);
			LCD_DrawLineColor(160 - r / 1.41421, 120 - r / 1.41421, 160 - r * 7 / 12, 120 - r / 3, BACK_COLOR);
			LCD_DrawLineColor(160 - r / 1.41421, 120 - r / 1.41421, 160 - r / 3, 120 - r * 7 / 12, BACK_COLOR);
			LCD_DrawPointColor(160 - r / 1.41421, 120 - r / 1.41421, BLACK);
		}
		break;
	case 2:
		LCD_DrawLineColor(160, 120, 160 - r, 120, color);
		LCD_DrawLineColor(160 - r, 120, 160 - r * 2 / 3, 120 + r / 6, color);
		LCD_DrawLineColor(160 - r, 120, 160 - r * 2 / 3, 120 - r / 6, color);
		if (r == 100)
		{
			LCD_DrawLineColor(160, 120, 160 - r, 120, BACK_COLOR);
			LCD_DrawLineColor(160 - r, 120, 160 - r * 2 / 3, 120 + r / 6, BACK_COLOR);
			LCD_DrawLineColor(160 - r, 120, 160 - r * 2 / 3, 120 - r / 6, BACK_COLOR);
			LCD_DrawPointColor(160 - r, 120, BLACK);
		}
		break;
	case 3:
		LCD_DrawLineColor(160, 120, 160 - r / 1.41421, 120 + r / 1.41421, color);
		LCD_DrawLineColor(160 - r / 1.41421, 120 + r / 1.41421, 160 - r / 3, 120 + r * 7 / 12, color);
		LCD_DrawLineColor(160 - r / 1.41421, 120 + r / 1.41421, 160 - r * 7 / 12, 120 + r / 3, color);
		if (r == 100)
		{
			LCD_DrawLineColor(160, 120, 160 - r / 1.41421, 120 + r / 1.41421, BACK_COLOR);
			LCD_DrawLineColor(160 - r / 1.41421, 120 + r / 1.41421, 160 - r / 3, 120 + r * 7 / 12, BACK_COLOR);
			LCD_DrawLineColor(160 - r / 1.41421, 120 + r / 1.41421, 160 - r * 7 / 12, 120 + r / 3, BACK_COLOR);
			LCD_DrawPointColor(160 - r / 1.41421, 120 + r / 1.41421, BLACK);
		}
		break;
	case 4:
		LCD_DrawLineColor(160, 120, 160, 120 + r, color);
		LCD_DrawLineColor(160, 120 + r, 160 - r / 6, 120 + r * 2 / 3, color);
		LCD_DrawLineColor(160, 120 + r, 160 + r / 6, 120 + r * 2 / 3, color);
		if (r == 100)
		{
			LCD_DrawLineColor(160, 120, 160, 120 + r, BACK_COLOR);
			LCD_DrawLineColor(160, 120 + r, 160 - r / 6, 120 + r * 2 / 3, BACK_COLOR);
			LCD_DrawLineColor(160, 120 + r, 160 + r / 6, 120 + r * 2 / 3, BACK_COLOR);
			LCD_DrawPointColor(160, 120 + r, BLACK);
		}
		break;
	case 5:
		LCD_DrawLineColor(160, 120, 160 + r / 1.41421, 120 + r / 1.41421, color);
		LCD_DrawLineColor(160 + r / 1.41421, 120 + r / 1.41421, 160 + r / 3, 120 + r * 7 / 12, color);
		LCD_DrawLineColor(160 + r / 1.41421, 120 + r / 1.41421, 160 + r * 7 / 12, 120 + r / 3, color);
		if (r == 100)
		{
			LCD_DrawLineColor(160, 120, 160 + r / 1.41421, 120 + r / 1.41421, BACK_COLOR);
			LCD_DrawLineColor(160 + r / 1.41421, 120 + r / 1.41421, 160 + r / 3, 120 + r * 7 / 12, BACK_COLOR);
			LCD_DrawLineColor(160 + r / 1.41421, 120 + r / 1.41421, 160 + r * 7 / 12, 120 + r / 3, BACK_COLOR);
			LCD_DrawPointColor(160 + r / 1.41421, 120 + r / 1.41421, BLACK);
		}
		break;
	case 6:
		LCD_DrawLineColor(160, 120, 160 + r, 120, color);
		LCD_DrawLineColor(160 + r, 120, 160 + r * 2 / 3, 120 + r / 6, color);
		LCD_DrawLineColor(160 + r, 120, 160 + r * 2 / 3, 120 - r / 6, color);
		if (r == 100)
		{
			LCD_DrawLineColor(160, 120, 160 + r, 120, BACK_COLOR);
			LCD_DrawLineColor(160 + r, 120, 160 + r * 2 / 3, 120 + r / 6, BACK_COLOR);
			LCD_DrawLineColor(160 + r, 120, 160 + r * 2 / 3, 120 - r / 6, BACK_COLOR);
			LCD_DrawPointColor(160 + r, 120, BLACK);
		}
		break;
	case 7:
		LCD_DrawLineColor(160, 120, 160 + r / 1.41421, 120 - r / 1.41421, color);
		LCD_DrawLineColor(160 + r / 1.41421, 120 - r / 1.41421, 160 + r / 3, 120 - r * 7 / 12, color);
		LCD_DrawLineColor(160 + r / 1.41421, 120 - r / 1.41421, 160 + r * 7 / 12, 120 - r / 3, color);
		if (r == 100)
		{
			LCD_DrawLineColor(160, 120, 160 + r / 1.41421, 120 - r / 1.41421, BACK_COLOR);
			LCD_DrawLineColor(160 + r / 1.41421, 120 - r / 1.41421, 160 + r / 3, 120 - r * 7 / 12, BACK_COLOR);
			LCD_DrawLineColor(160 + r / 1.41421, 120 - r / 1.41421, 160 + r * 7 / 12, 120 - r / 3, BACK_COLOR);
			LCD_DrawPointColor(160 + r / 1.41421, 120 - r / 1.41421, BLACK);
		}
		break;
	}
}
/**
 * @brief a specified arc arrow
 * 
 * @param mode 
 * @param degree 
 * @param color 
 */
void LCD_DrawOutsideNoteColor(unsigned short mode, unsigned short degree, unsigned int color)
{
	int a, b;
	int di;
	a = 0;
	b = 110;
	di = 3 - (110 << 1);
	float tangent = tan(15);
	while (a <= tangent * b)
	{
		a++;
		//娴ｈ法鏁resenham缁犳纭堕悽璇叉�?
		if (di < 0)
			di += 4 * a + 6;
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}
	} // a=sin(15)*110,b=cos(15)*110;
	switch (mode)
	{
	/*
	 * 8-11 outside arc anti-clockwise
	 * 12-15 outside arc clockwise
	 * mode = 8->up arc
	 * mode = 9->left arc
	 * mode = 10->down arc
	 * mode = 11->right arc
	 * mode = 12->up arc
	 * mode = 13->left arc
	 * mode = 14->down arc
	 * mode = 15->right arc
	 *
	 * 0<=degree<=30
	 */
	case 8:
		// generate
		Draw_CircleColor(160 - a, 120 - b, 3, color);
		if (degree == 36)
		{
			// destroy
			LCD_DrawArcColor(x0, y0, 110, 75, 111, BACK_COLOR);
		}
		else
			LCD_DrawArcColor(x0, y0, 110, 74 + degree, 75 + degree, color);
		break;
	case 9:
		// generate
		Draw_CircleColor(160 - b, 120 + a, 3, color);
		if (degree == 36)
		{
			// destroy
			LCD_DrawArcColor(x0, y0, 110, 165, 201, BACK_COLOR);
		}
		else
			LCD_DrawArcColor(x0, y0, 110, 164 + degree, 165 + degree, color);
		break;
	case 10:
		// generate
		Draw_CircleColor(160 + a, 120 + b, 3, color);
		if (degree == 36)
		{
			// destroy
			LCD_DrawArcColor(x0, y0, 110, 255, 291, BACK_COLOR);
		}
		else
			LCD_DrawArcColor(x0, y0, 110, 254 + degree, 255 + degree, color);
		break;
	case 11:
		// generate
		Draw_CircleColor(160 + b, 120 - a, 3, color);
		if (degree == 36)
		{
			// destroy
			LCD_DrawArcColor(x0, y0, 110, 345, 359, BACK_COLOR);
			LCD_DrawArcColor(x0, y0, 110, 0, 21, BACK_COLOR);
		}
		else
		{
			if (degree < 15)
				LCD_DrawArcColor(x0, y0, 110, 344 + degree, 345 + degree, color);
			else
				LCD_DrawArcColor(x0, y0, 110, -15 + degree, -14 + degree, color);
		}
		break;
	case 12:
		// generate
		Draw_CircleColor(160 + a, 120 - b, 3, color);
		if (degree == 36)
		{
			// destroy
			LCD_DrawArcColor(x0, y0, 110, 75, 111, BACK_COLOR);
		}
		else
			LCD_DrawArcColor(x0, y0, 110, 104 - degree, 105 - degree, color);
		break;
	case 13:
		// generate
		Draw_CircleColor(160 - b, 120 - a, 3, color);
		if (degree == 36)
		{
			// destroy
			LCD_DrawArcColor(x0, y0, 110, 165, 201, BACK_COLOR);
		}
		else
			LCD_DrawArcColor(x0, y0, 110, 194 - degree, 195 - degree, color);
		break;
	case 14:
		// generate
		Draw_CircleColor(160 - a, 120 + b, 3, color);
		if (degree == 36)
		{
			// destroy
			LCD_DrawArcColor(x0, y0, 110, 255, 291, BACK_COLOR);
		}
		else
			LCD_DrawArcColor(x0, y0, 110, 284 - degree, 285 - degree, color);
		break;
	case 15:
		// generate
		Draw_CircleColor(160 + b, 120 + a, 3, color);
		if (degree == 36)
		{
			// destroy
			LCD_DrawArcColor(x0, y0, 110, 345, 359, BACK_COLOR);
			LCD_DrawArcColor(x0, y0, 110, 0, 21, BACK_COLOR);
		}
		else
		{
			if (degree < 15)
				LCD_DrawArcColor(x0, y0, 110, 14 - degree, 15 - degree, color);
			else
				LCD_DrawArcColor(x0, y0, 110, 374 - degree, 375 - degree, color);
		}
		break;
	}
}
/**
 * @brief a specified arc arrow
 * 
 * @param mode 
 * @param degree 
 * @param color 
 */
void LCD_DrawRotateNoteColor(unsigned short mode, unsigned short degree, unsigned int color)
{
	/*mode: 0->right clockwise
	 * 		1->left clockwise
	 * 		2->right anti-clockwise
	 * 		3->left anti-clockwise
	 */

	switch (mode)
	{
	case 0:
		Draw_CircleColor(160 + 130 / 1.41421, 120 + 130 / 1.41421, 5, color);
		if (degree == 108)
		{
			// destroy
			LCD_DrawArcColor(x0, y0, 130, 0, 45, BACK_COLOR);
			LCD_DrawArcColor(x0, y0, 130, 297, 359, BACK_COLOR);
			Draw_CircleColor(160 + 130 / 1.41421, 120 + 130 / 1.41421, 5, BACK_COLOR);
		}
		else if (degree < 45)
			LCD_DrawArcColor(x0, y0, 130, 44 - degree, 45 - degree, color);
		else
			LCD_DrawArcColor(x0, y0, 130, 404 - degree, 405 - degree, color);
		break;
	case 1:
		Draw_CircleColor(160 - 130 / 1.41421, 120 - 130 / 1.41421, 5, color);
		if (degree == 108)
		{
			// destroy
			LCD_DrawArcColor(160, 120, 130, 117, 225, BACK_COLOR);
			Draw_CircleColor(160 - 130 / 1.41421, 120 - 130 / 1.41421, 5, BACK_COLOR);
		}
		else
			LCD_DrawArcColor(x0, y0, 130, 224 - degree, 225 - degree, color);
		break;
	case 2:
		Draw_CircleColor(160 + 130 / 1.41421, 120 - 130 / 1.41421, 5, color);
		if (degree == 108)
		{
			// destroy
			LCD_DrawArcColor(160, 120, 130, 0, 63, BACK_COLOR);
			LCD_DrawArcColor(160, 120, 130, 314, 359, BACK_COLOR);
			Draw_CircleColor(160 + 130 / 1.41421, 120 - 130 / 1.41421, 5, BACK_COLOR);
		}
		else if (degree < 45)
			LCD_DrawArcColor(x0, y0, 130, 314 + degree, 315 + degree, color);
		else
			LCD_DrawArcColor(x0, y0, 130, degree - 44, degree - 45, color);
		break;
	case 3:
		Draw_CircleColor(160 - 130 / 1.41421, 120 + 130 / 1.41421, 5, color);
		if (degree == 108)
		{
			// destroy
			LCD_DrawArcColor(160, 120, 130, 135, 243, BACK_COLOR);
			Draw_CircleColor(160 - 130 / 1.41421, 120 + 130 / 1.41421, 5, BACK_COLOR);
		}
		else
			LCD_DrawArcColor(x0, y0, 130, 134 + degree, 135 + degree, color);
		break;
	}
}
/**
 * @brief a specified circle
 * 
 * @param parameter 
 * @param color 
 */
void LCD_DrawStopNoteColor(unsigned short parameter, unsigned short color)
{
	Draw_CircleColor(160, 120, parameter, color);
}
//********************************************************************************************************************************
/*
*all sources below are written by Blazar Project
*functions with suffix *Color are modified by Luziqi
*/
//閸︺劍瀵氱�姘秴缂冾喗妯夌粈杞扮娑擃亜鐡ч敓锟�//x:0~234
// y:0~308
// num:鐟曚焦妯夌粈铏规畱鐎涙顑�" "--->"~"
// size:鐎涙ぞ缍嬫径褍鐨�?12/16
// mode:閸欑姴濮為弬鐟扮础(1)鏉╂ɑ妲搁棃鐐插綌閸旂姵鏌熼敓锟�)
void LCD_ShowChar(unsigned int x, unsigned int y, unsigned int num, unsigned int size, unsigned int mode)
{
#if USE_HORIZONTAL == 1
#define MAX_CHAR_POSX 320
#define MAX_CHAR_POSY 240
#else
#define MAX_CHAR_POSX 240
#define MAX_CHAR_POSY 320
#endif
	unsigned int temp;
	unsigned int pos, t;
	unsigned int x1 = x;
	unsigned int colortemp = POINT_COLOR;
	if (x > MAX_CHAR_POSX || y > MAX_CHAR_POSY)
		return;
	//鐠佸墽鐤嗙粣妤��?
	num = num - ' '; //瀵版鍩岄崑蹇曅╅崥搴ｆ畱閿燂拷
	if (!mode)		 //闂堢偛褰旈崝鐘虫煙閿燂拷
	{
		for (pos = 0; pos < size; pos++)
		{
			if (size == 12)
				temp = asc2_1206[num][pos]; //鐠嬪啰鏁�?206鐎涙ぞ缍�?
			else
				temp = asc2_1608[num][pos]; //鐠嬪啰鏁�?608鐎涙ぞ缍�?
			for (t = 0; t < size / 2; t++)
			{
				if (temp & 0x01)
					POINT_COLOR = colortemp;
				else
					POINT_COLOR = BACK_COLOR;
				LCD_DrawPoint(x, y);
				temp >>= 1;
				x++;
			}
			x = x1;
			y++;
		}
	}
	else //閸欑姴濮為弬鐟扮础
	{
		for (pos = 0; pos < size; pos++)
		{
			if (size == 12)
				temp = asc2_1206[num][pos]; //鐠嬪啰鏁�?206鐎涙ぞ缍�?
			else
				temp = asc2_1608[num][pos]; //鐠嬪啰鏁�?608鐎涙ぞ缍�?
			for (t = 0; t < size / 2; t++)
			{
				if (temp & 0x01)
					LCD_DrawPoint(x + t, y + pos); //閻㈣绔存稉顏嗗�?
				temp >>= 1;
			}
		}
	}
	POINT_COLOR = colortemp;
}
void LCD_ShowCharColor(unsigned int x, unsigned int y, unsigned int num, unsigned int size, unsigned int mode, unsigned int color)
{
#if USE_HORIZONTAL == 1
#define MAX_CHAR_POSX 320
#define MAX_CHAR_POSY 240
#else
#define MAX_CHAR_POSX 240
#define MAX_CHAR_POSY 320
#endif
	unsigned int temp;
	unsigned int pos, t;
	unsigned int x1 = x;
	unsigned int colortemp = color;
	if (x > MAX_CHAR_POSX || y > MAX_CHAR_POSY)
		return;
	//鐠佸墽鐤嗙粣妤��?
	num = num - ' '; //瀵版鍩岄崑蹇曅╅崥搴ｆ畱閿燂拷
	if (!mode)		 //闂堢偛褰旈崝鐘虫煙閿燂拷
	{
		for (pos = 0; pos < size; pos++)
		{
			if (size == 12)
				temp = asc2_1206[num][pos]; //鐠嬪啰鏁�?206鐎涙ぞ缍�?
			else
				temp = asc2_1608[num][pos]; //鐠嬪啰鏁�?608鐎涙ぞ缍�?
			for (t = 0; t < size / 2; t++)
			{
				if (!(temp & 0x01))
					colortemp = BACK_COLOR;
				else
					colortemp = color;
				LCD_DrawPointColor(x, y, colortemp);
				temp >>= 1;
				x++;
			}
			x = x1;
			y++;
		}
	}
	else //閸欑姴濮為弬鐟扮础
	{
		for (pos = 0; pos < size; pos++)
		{
			if (size == 12)
				temp = asc2_1206[num][pos]; //鐠嬪啰鏁�?206鐎涙ぞ缍�?
			else
				temp = asc2_1608[num][pos]; //鐠嬪啰鏁�?608鐎涙ぞ缍�?
			for (t = 0; t < size / 2; t++)
			{
				if (temp & 0x01)
					LCD_DrawPointColor(x + t, y + pos, colortemp); //閻㈣绔存稉顏嗗�?
				temp >>= 1;
			}
		}
	}
}
// m^n閸戣姤鏆�?
unsigned long mypow(unsigned int m, unsigned int n)
{
	unsigned long result = 1;
	while (n--)
		result *= m;
	return result;
}
//閺勫墽銇�娑擃亝鏆熼敓锟�?//x,y :鐠ч鍋ｉ崸鎰垼
// len :閺佹澘鐡ч惃鍕秴閿燂�?
// size:鐎涙ぞ缍嬫径褍鐨�?
// color:妫版粏澹�?
// num:閺佸府鎷�?0~4294967295);
void LCD_ShowNum(unsigned int x, unsigned int y, unsigned long num, unsigned int len, unsigned int size)
{
	unsigned int t, temp;
	unsigned int enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(x + (size / 2) * t, y, ' ', size, 0);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, 0);
	}
}
void LCD_ShowNumColor(unsigned int x, unsigned int y, unsigned long num, unsigned int len, unsigned int size, unsigned int color)
{
	unsigned int t, temp;
	unsigned int enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				LCD_ShowCharColor(x + (size / 2) * t, y, ' ', size, 0, color);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowCharColor(x + (size / 2) * t, y, temp + '0', size, 0, color);
	}
}
//閺勫墽銇�娑擃亝鏆熼敓锟�?//x,y:鐠ч鍋ｉ崸鎰垼
// size:鐎涙ぞ缍嬫径褍鐨�?
// mode:濡�绱�?0,婵夘偄鍘栧Ο鈥崇础;1,閸欑姴濮炲Ο鈥崇础
// num:閺佸府鎷�?0~99);
void LCD_Show2Num(unsigned int x, unsigned int y, unsigned int num, unsigned int len, unsigned int size, unsigned int mode)
{
	unsigned int t, temp;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, mode);
	}
}
void LCD_Show2NumColor(unsigned int x, unsigned int y, unsigned int num, unsigned int len, unsigned int size, unsigned int mode, unsigned int color)
{
	unsigned int t, temp;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		LCD_ShowCharColor(x + (size / 2) * t, y, temp + '0', size, mode, color);
	}
}
//閺勫墽銇氱�妤冾儊閿燂拷
// x,y:鐠ч鍋ｉ崸鎰垼
//*p:鐎涙顑佹稉鑼舵崳婵婀撮崸锟�//閿燂�?6鐎涙ぞ缍�?

void LCD_ShowString(unsigned char lx, unsigned char ly, char *sch)
{
	char *p2ch = sch;
	unsigned char L_x = lx;
	while (*p2ch)
	{
		LCD_ShowChar(L_x, ly, *p2ch++, 12, 0);
		L_x += 12;
	}
}
void LCD_ShowStringColor(unsigned char lx, unsigned char ly, char *sch, unsigned int color)
{
	char *p2ch = sch;
	unsigned char L_x = lx;
	while (*p2ch)
	{
		LCD_ShowCharColor(L_x, ly, *p2ch++, 12, 0, color);
		L_x += 12;
	}
}

void LCD_ShowPicture(unsigned int x, unsigned int y, unsigned char *pic)
{
	unsigned int i, j;
	for (j = y; j < pic_y + y; j++)
	{
		for (i = x; i <= pic_x + x; i++)
		{
			LCD_SetCursor(i, j); //鐠佸墽鐤嗛崗澶嬬垼娴ｅ秶鐤�
			LCD_WR_REG(0x2c);	 //瀵拷顬婇崘娆忓弳GRAM
			LCD_WR_DATA(pic[2 * ((j - y) * pic_x + i - x) + 1]);
			LCD_WR_DATA(pic[2 * ((j - y) * pic_x + i - x)]);
		}
	}
}
