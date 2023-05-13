/***********************************************************
 * 文件名       ：K6x_gpio.h
 * 说明         ：IO端口应用
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://www.landzo.cn
 *库版本        ：V1.0
 *时间          ：15.5.26
************************************************************/
#ifndef __K6x_GPIO_H__
#define __K6x_GPIO_H__

/*
 * 定义管脚方向
 */

//得出：PTx = PTxn / 32 ; PTn = PTxn & 31
#define PTX(PTxn)           ((PTxn)>>5)              //PORTX PORTA PORTB PORTC PORTD PORTE
#define PTn(PTxn)           ((PTxn)&0x1f)           //PIN   0~31
#define PORTX_BASE(PTxn)     PORTX[PTX(PTxn)]       //PORT模块的地址
#define GPIOX_BASE(PTxn)     GPIOX[PTX(PTxn)]       //GPIOx模块的地址

typedef enum
{
    //中断方式和DMA请求方式，两者只能选其中一种（可以不选）
    //中断方式选择
    IRQ_ZERO     = 0x08 << PORT_PCR_IRQC_SHIFT,   //低电平触发
    IRQ_RISING   = 0x09 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    IRQ_FALLING  = 0x0A << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    IRQ_EITHER   = 0x0B << PORT_PCR_IRQC_SHIFT,   //跳变沿触发
    IRQ_ONE      = 0x0C << PORT_PCR_IRQC_SHIFT,   //高电平触发

    //DMA请求选择
    DMA_RISING   = 0x01 << PORT_PCR_IRQC_SHIFT,   //上升沿触发
    DMA_FALLING  = 0x02 << PORT_PCR_IRQC_SHIFT,   //下降沿触发
    DMA_EITHER   = 0x03 << PORT_PCR_IRQC_SHIFT,   //跳变沿触发


    HDS          = 0x01 << PORT_PCR_DSE_SHIFT,    //输出高驱动能力
    PF           = 0x01 << PORT_PCR_PFE_SHIFT,    //带无源滤波器
    SSR          = 0x01 << PORT_PCR_SRE_SHIFT,    //输出慢变化率          Slow slew rate

    //下拉上拉选择
    PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     //下拉
    PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     //上拉

    //功能复用选择(如果不需要改变功能复用选择，保留原先的功能复用，直接选择ALT0 )
    //需要查 K60 Signal Multiplexing and Pin Assignments
    ALT0         = 0x00 << PORT_PCR_MUX_SHIFT,
    ALT1         = 0x01 << PORT_PCR_MUX_SHIFT,    //GPIO
    ALT2         = 0x02 << PORT_PCR_MUX_SHIFT,
    ALT3         = 0x03 << PORT_PCR_MUX_SHIFT,
    ALT4         = 0x04 << PORT_PCR_MUX_SHIFT,
    ALT5         = 0x05 << PORT_PCR_MUX_SHIFT,
    ALT6         = 0x06 << PORT_PCR_MUX_SHIFT,
    ALT7         = 0x07 << PORT_PCR_MUX_SHIFT,
} port_cfg;

typedef enum GPIO_CFG
{
    //这里的值不能改！！！
    GPI         = 0,                                //定义管脚输入方向      GPIOx_PDDRn里，0表示输入，1表示输出
    GPO         = 1,                                //定义管脚输出方向
    
    GPI_DOWN    = 0x02,                       //输入下拉              PORTx_PCRn需要PE=1，PS=0
    GPI_UP      = 0x03,                       //输入上拉              PORTx_PCRn需要PE=1，PS=1
    GPI_PF      = 0x10,                       //输入，带无源滤波器,滤波范围：10 MHz ~ 30 MHz 。不支持高速接口（>=2MHz）  0b10000           Passive Filter Enable
    GPI_DOWN_PF = GPI_DOWN | GPI_PF ,         //输入下拉，带无源滤波器
    GPI_UP_PF   = GPI_UP   | GPI_PF ,         //输入上拉，带无源滤波器

    GPO_HDS     = 0x41,                        //输出高驱动能力   0b100 0001    High drive strength
    GPO_SSR     = 0x05,                        //输出慢变化率          0b101    Slow slew rate
    GPO_HDS_SSR = GPO_HDS | GPO_SSR,           //输出高驱动能力、慢变化率   
       
} GPIO_CFG;

#define DISABLED      IRQ_DISABLED    //中断禁止
#define DMARING       DMA_RISING     //DMA上升沿触发
#define DMAFALLING    DMA_FALLING     //DMA下降沿触发
#define DMAEITHER     DMA_EITHER     //DMA跳变沿触发
#define ZERO          IRQ_ZERO        //低电平触发
#define RING          IRQ_RISING      //上升沿触发
#define FALLING       IRQ_FALLING    //下降沿触发
#define EITHER        IRQ_EITHER    //跳变沿触发
#define ONE           IRQ_ONE         //高电平触发
#define Port_init(portx,cfg)     (PORT_PCR_REG(PORTX[portx>>5],(portx&0x1f)) = cfg) //配置PORT端口输出定义

/***********************************************************
 * 文件名       ：gpio_cfg.h
 * 说明         ：gpio配置模块
 * 作者         ：landzo 蓝电子
 * 淘宝店       ：http://landzo.taobao.com/
 * 公司网站     ：http://www.landzo.com/
 *论坛          ：http://bbs.landzo.com
 * 库版本       ：V1.0
 *时间          ：15.5.26
************************************************************/

#define PTX_BIT(X,n,reg)         (((Bittyp *)(&(GPIO_##reg##_REG(PT##X##_BASE_PTR)))) ->b##n)               //位操作
#define PTX_BYTE(X,n,reg)        (((Bittyp *)(&(GPIO_##reg##_REG(PT##X##_BASE_PTR)))) ->B[n])               //位操作
#define PTX_WORD(X,n,reg)        (((Bittyp *)(&(GPIO_##reg##_REG(PT##X##_BASE_PTR)))) ->W[n])               //位操作


//定义的端口
//A端口输出
#define PTA0_OUT             PTX_BIT(A,0,PDOR)
#define PTA1_OUT             PTX_BIT(A,1,PDOR)
#define PTA2_OUT             PTX_BIT(A,2,PDOR)
#define PTA3_OUT             PTX_BIT(A,3,PDOR)
#define PTA4_OUT             PTX_BIT(A,4,PDOR)
#define PTA5_OUT             PTX_BIT(A,5,PDOR)
#define PTA6_OUT             PTX_BIT(A,6,PDOR)
#define PTA7_OUT             PTX_BIT(A,7,PDOR)
#define PTA8_OUT             PTX_BIT(A,8,PDOR)
#define PTA9_OUT             PTX_BIT(A,9,PDOR)
#define PTA10_OUT            PTX_BIT(A,10,PDOR)
#define PTA11_OUT            PTX_BIT(A,11,PDOR)
#define PTA12_OUT            PTX_BIT(A,12,PDOR)
#define PTA13_OUT            PTX_BIT(A,13,PDOR)
#define PTA14_OUT            PTX_BIT(A,14,PDOR)
#define PTA15_OUT            PTX_BIT(A,15,PDOR)
#define PTA16_OUT            PTX_BIT(A,16,PDOR)
#define PTA17_OUT            PTX_BIT(A,17,PDOR)
#define PTA18_OUT            PTX_BIT(A,18,PDOR)
#define PTA19_OUT            PTX_BIT(A,19,PDOR)
#define PTA20_OUT            PTX_BIT(A,20,PDOR)
#define PTA21_OUT            PTX_BIT(A,21,PDOR)
#define PTA22_OUT            PTX_BIT(A,22,PDOR)
#define PTA23_OUT            PTX_BIT(A,23,PDOR)
#define PTA24_OUT            PTX_BIT(A,24,PDOR)
#define PTA25_OUT            PTX_BIT(A,25,PDOR)
#define PTA26_OUT            PTX_BIT(A,26,PDOR)
#define PTA27_OUT            PTX_BIT(A,27,PDOR)
#define PTA28_OUT            PTX_BIT(A,28,PDOR)
#define PTA29_OUT            PTX_BIT(A,29,PDOR)
#define PTA30_OUT            PTX_BIT(A,30,PDOR)
#define PTA31_OUT            PTX_BIT(A,31,PDOR)

//B端口输出
#define PTB0_OUT             PTX_BIT(B,0,PDOR)
#define PTB1_OUT             PTX_BIT(B,1,PDOR)
#define PTB2_OUT             PTX_BIT(B,2,PDOR)
#define PTB3_OUT             PTX_BIT(B,3,PDOR)
#define PTB4_OUT             PTX_BIT(B,4,PDOR)
#define PTB5_OUT             PTX_BIT(B,5,PDOR)
#define PTB6_OUT             PTX_BIT(B,6,PDOR)
#define PTB7_OUT             PTX_BIT(B,7,PDOR)
#define PTB8_OUT             PTX_BIT(B,8,PDOR)
#define PTB9_OUT             PTX_BIT(B,9,PDOR)
#define PTB10_OUT            PTX_BIT(B,10,PDOR)
#define PTB11_OUT            PTX_BIT(B,11,PDOR)
#define PTB12_OUT            PTX_BIT(B,12,PDOR)
#define PTB13_OUT            PTX_BIT(B,13,PDOR)
#define PTB14_OUT            PTX_BIT(B,14,PDOR)
#define PTB15_OUT            PTX_BIT(B,15,PDOR)
#define PTB16_OUT            PTX_BIT(B,16,PDOR)
#define PTB17_OUT            PTX_BIT(B,17,PDOR)
#define PTB18_OUT            PTX_BIT(B,18,PDOR)
#define PTB19_OUT            PTX_BIT(B,19,PDOR)
#define PTB20_OUT            PTX_BIT(B,20,PDOR)
#define PTB21_OUT            PTX_BIT(B,21,PDOR)
#define PTB22_OUT            PTX_BIT(B,22,PDOR)
#define PTB23_OUT            PTX_BIT(B,23,PDOR)
#define PTB24_OUT            PTX_BIT(B,24,PDOR)
#define PTB25_OUT            PTX_BIT(B,25,PDOR)
#define PTB26_OUT            PTX_BIT(B,26,PDOR)
#define PTB27_OUT            PTX_BIT(B,27,PDOR)
#define PTB28_OUT            PTX_BIT(B,28,PDOR)
#define PTB29_OUT            PTX_BIT(B,29,PDOR)
#define PTB30_OUT            PTX_BIT(B,30,PDOR)
#define PTB31_OUT            PTX_BIT(B,31,PDOR)

//C端口输出
#define PTC0_OUT             PTX_BIT(C,0,PDOR)
#define PTC1_OUT             PTX_BIT(C,1,PDOR)
#define PTC2_OUT             PTX_BIT(C,2,PDOR)
#define PTC3_OUT             PTX_BIT(C,3,PDOR)
#define PTC4_OUT             PTX_BIT(C,4,PDOR)
#define PTC5_OUT             PTX_BIT(C,5,PDOR)
#define PTC6_OUT             PTX_BIT(C,6,PDOR)
#define PTC7_OUT             PTX_BIT(C,7,PDOR)
#define PTC8_OUT             PTX_BIT(C,8,PDOR)
#define PTC9_OUT             PTX_BIT(C,9,PDOR)
#define PTC10_OUT            PTX_BIT(C,10,PDOR)
#define PTC11_OUT            PTX_BIT(C,11,PDOR)
#define PTC12_OUT            PTX_BIT(C,12,PDOR)
#define PTC13_OUT            PTX_BIT(C,13,PDOR)
#define PTC14_OUT            PTX_BIT(C,14,PDOR)
#define PTC15_OUT            PTX_BIT(C,15,PDOR)
#define PTC16_OUT            PTX_BIT(C,16,PDOR)
#define PTC17_OUT            PTX_BIT(C,17,PDOR)
#define PTC18_OUT            PTX_BIT(C,18,PDOR)
#define PTC19_OUT            PTX_BIT(C,19,PDOR)
#define PTC20_OUT            PTX_BIT(C,20,PDOR)
#define PTC21_OUT            PTX_BIT(C,21,PDOR)
#define PTC22_OUT            PTX_BIT(C,22,PDOR)
#define PTC23_OUT            PTX_BIT(C,23,PDOR)
#define PTC24_OUT            PTX_BIT(C,24,PDOR)
#define PTC25_OUT            PTX_BIT(C,25,PDOR)
#define PTC26_OUT            PTX_BIT(C,26,PDOR)
#define PTC27_OUT            PTX_BIT(C,27,PDOR)
#define PTC28_OUT            PTX_BIT(C,28,PDOR)
#define PTC29_OUT            PTX_BIT(C,29,PDOR)
#define PTC30_OUT            PTX_BIT(C,30,PDOR)
#define PTC31_OUT            PTX_BIT(C,31,PDOR)

//D端口输出
#define PTD0_OUT             PTX_BIT(D,0,PDOR)
#define PTD1_OUT             PTX_BIT(D,1,PDOR)
#define PTD2_OUT             PTX_BIT(D,2,PDOR)
#define PTD3_OUT             PTX_BIT(D,3,PDOR)
#define PTD4_OUT             PTX_BIT(D,4,PDOR)
#define PTD5_OUT             PTX_BIT(D,5,PDOR)
#define PTD6_OUT             PTX_BIT(D,6,PDOR)
#define PTD7_OUT             PTX_BIT(D,7,PDOR)
#define PTD8_OUT             PTX_BIT(D,8,PDOR)
#define PTD9_OUT             PTX_BIT(D,9,PDOR)
#define PTD10_OUT            PTX_BIT(D,10,PDOR)
#define PTD11_OUT            PTX_BIT(D,11,PDOR)
#define PTD12_OUT            PTX_BIT(D,12,PDOR)
#define PTD13_OUT            PTX_BIT(D,13,PDOR)
#define PTD14_OUT            PTX_BIT(D,14,PDOR)
#define PTD15_OUT            PTX_BIT(D,15,PDOR)
#define PTD16_OUT            PTX_BIT(D,16,PDOR)
#define PTD17_OUT            PTX_BIT(D,17,PDOR)
#define PTD18_OUT            PTX_BIT(D,18,PDOR)
#define PTD19_OUT            PTX_BIT(D,19,PDOR)
#define PTD20_OUT            PTX_BIT(D,20,PDOR)
#define PTD21_OUT            PTX_BIT(D,21,PDOR)
#define PTD22_OUT            PTX_BIT(D,22,PDOR)
#define PTD23_OUT            PTX_BIT(D,23,PDOR)
#define PTD24_OUT            PTX_BIT(D,24,PDOR)
#define PTD25_OUT            PTX_BIT(D,25,PDOR)
#define PTD26_OUT            PTX_BIT(D,26,PDOR)
#define PTD27_OUT            PTX_BIT(D,27,PDOR)
#define PTD28_OUT            PTX_BIT(D,28,PDOR)
#define PTD29_OUT            PTX_BIT(D,29,PDOR)
#define PTD30_OUT            PTX_BIT(D,30,PDOR)
#define PTD31_OUT            PTX_BIT(D,31,PDOR)

//E端口输出
#define PTE0_OUT             PTX_BIT(E,0,PDOR)
#define PTE1_OUT             PTX_BIT(E,1,PDOR)
#define PTE2_OUT             PTX_BIT(E,2,PDOR)
#define PTE3_OUT             PTX_BIT(E,3,PDOR)
#define PTE4_OUT             PTX_BIT(E,4,PDOR)
#define PTE5_OUT             PTX_BIT(E,5,PDOR)
#define PTE6_OUT             PTX_BIT(E,6,PDOR)
#define PTE7_OUT             PTX_BIT(E,7,PDOR)
#define PTE8_OUT             PTX_BIT(E,8,PDOR)
#define PTE9_OUT             PTX_BIT(E,9,PDOR)
#define PTE10_OUT            PTX_BIT(E,10,PDOR)
#define PTE11_OUT            PTX_BIT(E,11,PDOR)
#define PTE12_OUT            PTX_BIT(E,12,PDOR)
#define PTE13_OUT            PTX_BIT(E,13,PDOR)
#define PTE14_OUT            PTX_BIT(E,14,PDOR)
#define PTE15_OUT            PTX_BIT(E,15,PDOR)
#define PTE16_OUT            PTX_BIT(E,16,PDOR)
#define PTE17_OUT            PTX_BIT(E,17,PDOR)
#define PTE18_OUT            PTX_BIT(E,18,PDOR)
#define PTE19_OUT            PTX_BIT(E,19,PDOR)
#define PTE20_OUT            PTX_BIT(E,20,PDOR)
#define PTE21_OUT            PTX_BIT(E,21,PDOR)
#define PTE22_OUT            PTX_BIT(E,22,PDOR)
#define PTE23_OUT            PTX_BIT(E,23,PDOR)
#define PTE24_OUT            PTX_BIT(E,24,PDOR)
#define PTE25_OUT            PTX_BIT(E,25,PDOR)
#define PTE26_OUT            PTX_BIT(E,26,PDOR)
#define PTE27_OUT            PTX_BIT(E,27,PDOR)
#define PTE28_OUT            PTX_BIT(E,28,PDOR)
#define PTE29_OUT            PTX_BIT(E,29,PDOR)
#define PTE30_OUT            PTX_BIT(E,30,PDOR)
#define PTE31_OUT            PTX_BIT(E,31,PDOR)


//A端口输入
#define PTA0_INT             PTX_BIT(A,0,PDIR)
#define PTA1_INT             PTX_BIT(A,1,PDIR)
#define PTA2_INT             PTX_BIT(A,2,PDIR)
#define PTA3_INT             PTX_BIT(A,3,PDIR)
#define PTA4_INT             PTX_BIT(A,4,PDIR)
#define PTA5_INT             PTX_BIT(A,5,PDIR)
#define PTA6_INT             PTX_BIT(A,6,PDIR)
#define PTA7_INT             PTX_BIT(A,7,PDIR)
#define PTA8_INT             PTX_BIT(A,8,PDIR)
#define PTA9_INT             PTX_BIT(A,9,PDIR)
#define PTA10_INT            PTX_BIT(A,10,PDIR)
#define PTA11_INT            PTX_BIT(A,11,PDIR)
#define PTA12_INT            PTX_BIT(A,12,PDIR)
#define PTA13_INT            PTX_BIT(A,13,PDIR)
#define PTA14_INT            PTX_BIT(A,14,PDIR)
#define PTA15_INT            PTX_BIT(A,15,PDIR)
#define PTA16_INT            PTX_BIT(A,16,PDIR)
#define PTA17_INT            PTX_BIT(A,17,PDIR)
#define PTA18_INT            PTX_BIT(A,18,PDIR)
#define PTA19_INT            PTX_BIT(A,19,PDIR)
#define PTA20_INT            PTX_BIT(A,20,PDIR)
#define PTA21_INT            PTX_BIT(A,21,PDIR)
#define PTA22_INT            PTX_BIT(A,22,PDIR)
#define PTA23_INT            PTX_BIT(A,23,PDIR)
#define PTA24_INT            PTX_BIT(A,24,PDIR)
#define PTA25_INT            PTX_BIT(A,25,PDIR)
#define PTA26_INT            PTX_BIT(A,26,PDIR)
#define PTA27_INT            PTX_BIT(A,27,PDIR)
#define PTA28_INT            PTX_BIT(A,28,PDIR)
#define PTA29_INT            PTX_BIT(A,29,PDIR)
#define PTA30_INT            PTX_BIT(A,30,PDIR)
#define PTA31_INT            PTX_BIT(A,31,PDIR)

//B端口输入
#define PTB0_INT             PTX_BIT(B,0,PDIR)
#define PTB1_INT             PTX_BIT(B,1,PDIR)
#define PTB2_INT             PTX_BIT(B,2,PDIR)
#define PTB3_INT             PTX_BIT(B,3,PDIR)
#define PTB4_INT             PTX_BIT(B,4,PDIR)
#define PTB5_INT             PTX_BIT(B,5,PDIR)
#define PTB6_INT             PTX_BIT(B,6,PDIR)
#define PTB7_INT             PTX_BIT(B,7,PDIR)
#define PTB8_INT             PTX_BIT(B,8,PDIR)
#define PTB9_INT             PTX_BIT(B,9,PDIR)
#define PTB10_INT            PTX_BIT(B,10,PDIR)
#define PTB11_INT            PTX_BIT(B,11,PDIR)
#define PTB12_INT            PTX_BIT(B,12,PDIR)
#define PTB13_INT            PTX_BIT(B,13,PDIR)
#define PTB14_INT            PTX_BIT(B,14,PDIR)
#define PTB15_INT            PTX_BIT(B,15,PDIR)
#define PTB16_INT            PTX_BIT(B,16,PDIR)
#define PTB17_INT            PTX_BIT(B,17,PDIR)
#define PTB18_INT            PTX_BIT(B,18,PDIR)
#define PTB19_INT            PTX_BIT(B,19,PDIR)
#define PTB20_INT            PTX_BIT(B,20,PDIR)
#define PTB21_INT            PTX_BIT(B,21,PDIR)
#define PTB22_INT            PTX_BIT(B,22,PDIR)
#define PTB23_INT            PTX_BIT(B,23,PDIR)
#define PTB24_INT            PTX_BIT(B,24,PDIR)
#define PTB25_INT            PTX_BIT(B,25,PDIR)
#define PTB26_INT            PTX_BIT(B,26,PDIR)
#define PTB27_INT            PTX_BIT(B,27,PDIR)
#define PTB28_INT            PTX_BIT(B,28,PDIR)
#define PTB29_INT            PTX_BIT(B,29,PDIR)
#define PTB30_INT            PTX_BIT(B,30,PDIR)
#define PTB31_INT            PTX_BIT(B,31,PDIR)

//C端口输入
#define PTC0_INT             PTX_BIT(C,0,PDIR)
#define PTC1_INT             PTX_BIT(C,1,PDIR)
#define PTC2_INT             PTX_BIT(C,2,PDIR)
#define PTC3_INT             PTX_BIT(C,3,PDIR)
#define PTC4_INT             PTX_BIT(C,4,PDIR)
#define PTC5_INT             PTX_BIT(C,5,PDIR)
#define PTC6_INT             PTX_BIT(C,6,PDIR)
#define PTC7_INT             PTX_BIT(C,7,PDIR)
#define PTC8_INT             PTX_BIT(C,8,PDIR)
#define PTC9_INT             PTX_BIT(C,9,PDIR)
#define PTC10_INT            PTX_BIT(C,10,PDIR)
#define PTC11_INT            PTX_BIT(C,11,PDIR)
#define PTC12_INT            PTX_BIT(C,12,PDIR)
#define PTC13_INT            PTX_BIT(C,13,PDIR)
#define PTC14_INT            PTX_BIT(C,14,PDIR)
#define PTC15_INT            PTX_BIT(C,15,PDIR)
#define PTC16_INT            PTX_BIT(C,16,PDIR)
#define PTC17_INT            PTX_BIT(C,17,PDIR)
#define PTC18_INT            PTX_BIT(C,18,PDIR)
#define PTC19_INT            PTX_BIT(C,19,PDIR)
#define PTC20_INT            PTX_BIT(C,20,PDIR)
#define PTC21_INT            PTX_BIT(C,21,PDIR)
#define PTC22_INT            PTX_BIT(C,22,PDIR)
#define PTC23_INT            PTX_BIT(C,23,PDIR)
#define PTC24_INT            PTX_BIT(C,24,PDIR)
#define PTC25_INT            PTX_BIT(C,25,PDIR)
#define PTC26_INT            PTX_BIT(C,26,PDIR)
#define PTC27_INT            PTX_BIT(C,27,PDIR)
#define PTC28_INT            PTX_BIT(C,28,PDIR)
#define PTC29_INT            PTX_BIT(C,29,PDIR)
#define PTC30_INT            PTX_BIT(C,30,PDIR)
#define PTC31_INT            PTX_BIT(C,31,PDIR)

//D端口输入
#define PTD0_INT             PTX_BIT(D,0,PDIR)
#define PTD1_INT             PTX_BIT(D,1,PDIR)
#define PTD2_INT             PTX_BIT(D,2,PDIR)
#define PTD3_INT             PTX_BIT(D,3,PDIR)
#define PTD4_INT             PTX_BIT(D,4,PDIR)
#define PTD5_INT             PTX_BIT(D,5,PDIR)
#define PTD6_INT             PTX_BIT(D,6,PDIR)
#define PTD7_INT             PTX_BIT(D,7,PDIR)
#define PTD8_INT             PTX_BIT(D,8,PDIR)
#define PTD9_INT             PTX_BIT(D,9,PDIR)
#define PTD10_INT            PTX_BIT(D,10,PDIR)
#define PTD11_INT            PTX_BIT(D,11,PDIR)
#define PTD12_INT            PTX_BIT(D,12,PDIR)
#define PTD13_INT            PTX_BIT(D,13,PDIR)
#define PTD14_INT            PTX_BIT(D,14,PDIR)
#define PTD15_INT            PTX_BIT(D,15,PDIR)
#define PTD16_INT            PTX_BIT(D,16,PDIR)
#define PTD17_INT            PTX_BIT(D,17,PDIR)
#define PTD18_INT            PTX_BIT(D,18,PDIR)
#define PTD19_INT            PTX_BIT(D,19,PDIR)
#define PTD20_INT            PTX_BIT(D,20,PDIR)
#define PTD21_INT            PTX_BIT(D,21,PDIR)
#define PTD22_INT            PTX_BIT(D,22,PDIR)
#define PTD23_INT            PTX_BIT(D,23,PDIR)
#define PTD24_INT            PTX_BIT(D,24,PDIR)
#define PTD25_INT            PTX_BIT(D,25,PDIR)
#define PTD26_INT            PTX_BIT(D,26,PDIR)
#define PTD27_INT            PTX_BIT(D,27,PDIR)
#define PTD28_INT            PTX_BIT(D,28,PDIR)
#define PTD29_INT            PTX_BIT(D,29,PDIR)
#define PTD30_INT            PTX_BIT(D,30,PDIR)
#define PTD31_INT            PTX_BIT(D,31,PDIR)

//E端口输入
#define PTE0_INT             PTX_BIT(E,0,PDIR)
#define PTE1_INT             PTX_BIT(E,1,PDIR)
#define PTE2_INT             PTX_BIT(E,2,PDIR)
#define PTE3_INT             PTX_BIT(E,3,PDIR)
#define PTE4_INT             PTX_BIT(E,4,PDIR)
#define PTE5_INT             PTX_BIT(E,5,PDIR)
#define PTE6_INT             PTX_BIT(E,6,PDIR)
#define PTE7_INT             PTX_BIT(E,7,PDIR)
#define PTE8_INT             PTX_BIT(E,8,PDIR)
#define PTE9_INT             PTX_BIT(E,9,PDIR)
#define PTE10_INT            PTX_BIT(E,10,PDIR)
#define PTE11_INT            PTX_BIT(E,11,PDIR)
#define PTE12_INT            PTX_BIT(E,12,PDIR)
#define PTE13_INT            PTX_BIT(E,13,PDIR)
#define PTE14_INT            PTX_BIT(E,14,PDIR)
#define PTE15_INT            PTX_BIT(E,15,PDIR)
#define PTE16_INT            PTX_BIT(E,16,PDIR)
#define PTE17_INT            PTX_BIT(E,17,PDIR)
#define PTE18_INT            PTX_BIT(E,18,PDIR)
#define PTE19_INT            PTX_BIT(E,19,PDIR)
#define PTE20_INT            PTX_BIT(E,20,PDIR)
#define PTE21_INT            PTX_BIT(E,21,PDIR)
#define PTE22_INT            PTX_BIT(E,22,PDIR)
#define PTE23_INT            PTX_BIT(E,23,PDIR)
#define PTE24_INT            PTX_BIT(E,24,PDIR)
#define PTE25_INT            PTX_BIT(E,25,PDIR)
#define PTE26_INT            PTX_BIT(E,26,PDIR)
#define PTE27_INT            PTX_BIT(E,27,PDIR)
#define PTE28_INT            PTX_BIT(E,28,PDIR)
#define PTE29_INT            PTX_BIT(E,29,PDIR)
#define PTE30_INT            PTX_BIT(E,30,PDIR)
#define PTE31_INT            PTX_BIT(E,31,PDIR)

//方向控制寄存器***********************************************
//A端口
#define DDRA0                PTX_BIT(A,0,PDDR) 
#define DDRA1                PTX_BIT(A,1,PDDR) 
#define DDRA2                PTX_BIT(A,2,PDDR) 
#define DDRA3                PTX_BIT(A,3,PDDR) 
#define DDRA4                PTX_BIT(A,4,PDDR) 
#define DDRA5                PTX_BIT(A,5,PDDR) 
#define DDRA6                PTX_BIT(A,6,PDDR) 
#define DDRA7                PTX_BIT(A,7,PDDR) 
#define DDRA8                PTX_BIT(A,8,PDDR) 
#define DDRA9                PTX_BIT(A,9,PDDR) 
#define DDRA10               PTX_BIT(A,10,PDDR) 
#define DDRA11               PTX_BIT(A,11,PDDR) 
#define DDRA12               PTX_BIT(A,12,PDDR) 
#define DDRA13               PTX_BIT(A,13,PDDR) 
#define DDRA14               PTX_BIT(A,14,PDDR) 
#define DDRA15               PTX_BIT(A,15,PDDR) 
#define DDRA16               PTX_BIT(A,16,PDDR) 
#define DDRA17               PTX_BIT(A,17,PDDR) 
#define DDRA18               PTX_BIT(A,18,PDDR) 
#define DDRA19               PTX_BIT(A,19,PDDR)
#define DDRA20               PTX_BIT(A,20,PDDR) 
#define DDRA21               PTX_BIT(A,21,PDDR) 
#define DDRA22               PTX_BIT(A,22,PDDR) 
#define DDRA23               PTX_BIT(A,23,PDDR) 
#define DDRA24               PTX_BIT(A,24,PDDR) 
#define DDRA25               PTX_BIT(A,25,PDDR) 
#define DDRA26               PTX_BIT(A,26,PDDR) 
#define DDRA27               PTX_BIT(A,27,PDDR) 
#define DDRA28               PTX_BIT(A,28,PDDR) 
#define DDRA29               PTX_BIT(A,29,PDDR)
#define DDRA30               PTX_BIT(A,30,PDDR) 
#define DDRA31               PTX_BIT(A,31,PDDR) 

//B端口
#define DDRB0                PTX_BIT(B,0,PDDR) 
#define DDRB1                PTX_BIT(B,1,PDDR) 
#define DDRB2                PTX_BIT(B,2,PDDR) 
#define DDRB3                PTX_BIT(B,3,PDDR) 
#define DDRB4                PTX_BIT(B,4,PDDR) 
#define DDRB5                PTX_BIT(B,5,PDDR) 
#define DDRB6                PTX_BIT(B,6,PDDR) 
#define DDRB7                PTX_BIT(B,7,PDDR) 
#define DDRB8                PTX_BIT(B,8,PDDR) 
#define DDRB9                PTX_BIT(B,9,PDDR) 
#define DDRB10               PTX_BIT(B,10,PDDR) 
#define DDRB11               PTX_BIT(B,11,PDDR) 
#define DDRB12               PTX_BIT(B,12,PDDR) 
#define DDRB13               PTX_BIT(B,13,PDDR) 
#define DDRB14               PTX_BIT(B,14,PDDR) 
#define DDRB15               PTX_BIT(B,15,PDDR) 
#define DDRB16               PTX_BIT(B,16,PDDR) 
#define DDRB17               PTX_BIT(B,17,PDDR) 
#define DDRB18               PTX_BIT(B,18,PDDR) 
#define DDRB19               PTX_BIT(B,19,PDDR)
#define DDRB20               PTX_BIT(B,20,PDDR) 
#define DDRB21               PTX_BIT(B,21,PDDR) 
#define DDRB22               PTX_BIT(B,22,PDDR) 
#define DDRB23               PTX_BIT(B,23,PDDR) 
#define DDRB24               PTX_BIT(B,24,PDDR) 
#define DDRB25               PTX_BIT(B,25,PDDR) 
#define DDRB26               PTX_BIT(B,26,PDDR) 
#define DDRB27               PTX_BIT(B,27,PDDR) 
#define DDRB28               PTX_BIT(B,28,PDDR) 
#define DDRB29               PTX_BIT(B,29,PDDR)
#define DDRB30               PTX_BIT(B,30,PDDR) 
#define DDRB31               PTX_BIT(B,31,PDDR) 

//C端口
#define DDRC0                PTX_BIT(C,0,PDDR) 
#define DDRC1                PTX_BIT(C,1,PDDR) 
#define DDRC2                PTX_BIT(C,2,PDDR) 
#define DDRC3                PTX_BIT(C,3,PDDR) 
#define DDRC4                PTX_BIT(C,4,PDDR) 
#define DDRC5                PTX_BIT(C,5,PDDR) 
#define DDRC6                PTX_BIT(C,6,PDDR) 
#define DDRC7                PTX_BIT(C,7,PDDR) 
#define DDRC8                PTX_BIT(C,8,PDDR) 
#define DDRC9                PTX_BIT(C,9,PDDR) 
#define DDRC10               PTX_BIT(C,10,PDDR) 
#define DDRC11               PTX_BIT(C,11,PDDR) 
#define DDRC12               PTX_BIT(C,12,PDDR) 
#define DDRC13               PTX_BIT(C,13,PDDR) 
#define DDRC14               PTX_BIT(C,14,PDDR) 
#define DDRC15               PTX_BIT(C,15,PDDR) 
#define DDRC16               PTX_BIT(C,16,PDDR) 
#define DDRC17               PTX_BIT(C,17,PDDR) 
#define DDRC18               PTX_BIT(C,18,PDDR) 
#define DDRC19               PTX_BIT(C,19,PDDR)
#define DDRC20               PTX_BIT(C,20,PDDR) 
#define DDRC21               PTX_BIT(C,21,PDDR) 
#define DDRC22               PTX_BIT(C,22,PDDR) 
#define DDRC23               PTX_BIT(C,23,PDDR) 
#define DDRC24               PTX_BIT(C,24,PDDR) 
#define DDRC25               PTX_BIT(C,25,PDDR) 
#define DDRC26               PTX_BIT(C,26,PDDR) 
#define DDRC27               PTX_BIT(C,27,PDDR) 
#define DDRC28               PTX_BIT(C,28,PDDR) 
#define DDRC29               PTX_BIT(C,29,PDDR)
#define DDRC30               PTX_BIT(C,30,PDDR) 
#define DDRC31               PTX_BIT(C,31,PDDR) 

//D端口
#define DDRD0                PTX_BIT(D,0,PDDR) 
#define DDRD1                PTX_BIT(D,1,PDDR) 
#define DDRD2                PTX_BIT(D,2,PDDR) 
#define DDRD3                PTX_BIT(D,3,PDDR) 
#define DDRD4                PTX_BIT(D,4,PDDR) 
#define DDRD5                PTX_BIT(D,5,PDDR) 
#define DDRD6                PTX_BIT(D,6,PDDR) 
#define DDRD7                PTX_BIT(D,7,PDDR) 
#define DDRD8                PTX_BIT(D,8,PDDR) 
#define DDRD9                PTX_BIT(D,9,PDDR) 
#define DDRD10               PTX_BIT(D,10,PDDR) 
#define DDRD11               PTX_BIT(D,11,PDDR) 
#define DDRD12               PTX_BIT(D,12,PDDR) 
#define DDRD13               PTX_BIT(D,13,PDDR) 
#define DDRD14               PTX_BIT(D,14,PDDR) 
#define DDRD15               PTX_BIT(D,15,PDDR) 
#define DDRD16               PTX_BIT(D,16,PDDR) 
#define DDRD17               PTX_BIT(D,17,PDDR) 
#define DDRD18               PTX_BIT(D,18,PDDR) 
#define DDRD19               PTX_BIT(D,19,PDDR)
#define DDRD20               PTX_BIT(D,20,PDDR) 
#define DDRD21               PTX_BIT(D,21,PDDR) 
#define DDRD22               PTX_BIT(D,22,PDDR) 
#define DDRD23               PTX_BIT(D,23,PDDR) 
#define DDRD24               PTX_BIT(D,24,PDDR) 
#define DDRD25               PTX_BIT(D,25,PDDR) 
#define DDRD26               PTX_BIT(D,26,PDDR) 
#define DDRD27               PTX_BIT(D,27,PDDR) 
#define DDRD28               PTX_BIT(D,28,PDDR) 
#define DDRD29               PTX_BIT(D,29,PDDR)
#define DDRD30               PTX_BIT(D,30,PDDR) 
#define DDRD31               PTX_BIT(D,31,PDDR) 

//E端口
#define DDRE0                PTX_BIT(E,0,PDDR) 
#define DDRE1                PTX_BIT(E,1,PDDR) 
#define DDRE2                PTX_BIT(E,2,PDDR) 
#define DDRE3                PTX_BIT(E,3,PDDR) 
#define DDRE4                PTX_BIT(E,4,PDDR) 
#define DDRE5                PTX_BIT(E,5,PDDR) 
#define DDRE6                PTX_BIT(E,6,PDDR) 
#define DDRE7                PTX_BIT(E,7,PDDR) 
#define DDRE8                PTX_BIT(E,8,PDDR) 
#define DDRE9                PTX_BIT(E,9,PDDR) 
#define DDRE10               PTX_BIT(E,10,PDDR) 
#define DDRE11               PTX_BIT(E,11,PDDR) 
#define DDRE12               PTX_BIT(E,12,PDDR) 
#define DDRE13               PTX_BIT(E,13,PDDR) 
#define DDRE14               PTX_BIT(E,14,PDDR) 
#define DDRE15               PTX_BIT(E,15,PDDR) 
#define DDRE16               PTX_BIT(E,16,PDDR) 
#define DDRE17               PTX_BIT(E,17,PDDR) 
#define DDRE18               PTX_BIT(E,18,PDDR) 
#define DDRE19               PTX_BIT(E,19,PDDR)
#define DDRE20               PTX_BIT(E,20,PDDR) 
#define DDRE21               PTX_BIT(E,21,PDDR) 
#define DDRE22               PTX_BIT(E,22,PDDR) 
#define DDRE23               PTX_BIT(E,23,PDDR) 
#define DDRE24               PTX_BIT(E,24,PDDR) 
#define DDRE25               PTX_BIT(E,25,PDDR) 
#define DDRE26               PTX_BIT(E,26,PDDR) 
#define DDRE27               PTX_BIT(E,27,PDDR) 
#define DDRE28               PTX_BIT(E,28,PDDR) 
#define DDRE29               PTX_BIT(E,29,PDDR)
#define DDRE30               PTX_BIT(E,30,PDDR) 
#define DDRE31               PTX_BIT(E,31,PDDR) 

//8bit端口的控制寄存器**************************************************
//A端口
#define DDRA_BYTE0           PTX_BYTE(A,0,PDDR)
#define DDRA_BYTE1           PTX_BYTE(A,1,PDDR)
#define DDRA_BYTE2           PTX_BYTE(A,2,PDDR)
#define DDRA_BYTE3           PTX_BYTE(A,3,PDDR)

//B端口
#define DDRB_BYTE0           PTX_BYTE(B,0,PDDR)
#define DDRB_BYTE1           PTX_BYTE(B,1,PDDR)
#define DDRB_BYTE2           PTX_BYTE(B,2,PDDR)
#define DDRB_BYTE3           PTX_BYTE(B,3,PDDR)

//C端口
#define DDRC_BYTE0           PTX_BYTE(C,0,PDDR)
#define DDRC_BYTE1           PTX_BYTE(C,1,PDDR)
#define DDRC_BYTE2           PTX_BYTE(C,2,PDDR)
#define DDRC_BYTE3           PTX_BYTE(C,3,PDDR)

//D端口
#define DDRD_BYTE0           PTX_BYTE(D,0,PDDR)
#define DDRD_BYTE1           PTX_BYTE(D,1,PDDR)
#define DDRD_BYTE2           PTX_BYTE(D,2,PDDR)
#define DDRD_BYTE3           PTX_BYTE(D,3,PDDR)

//E端口
#define DDRE_BYTE0           PTX_BYTE(E,0,PDDR)
#define DDRE_BYTE1           PTX_BYTE(E,1,PDDR)
#define DDRE_BYTE2           PTX_BYTE(E,2,PDDR)
#define DDRE_BYTE3           PTX_BYTE(E,3,PDDR)

//8bit端口的输出**************************************************
//A端口
#define PTA_BYTE0_OUT        PTX_BYTE(A,0,PDOR)
#define PTA_BYTE1_OUT        PTX_BYTE(A,1,PDOR)
#define PTA_BYTE2_OUT        PTX_BYTE(A,2,PDOR)
#define PTA_BYTE3_OUT        PTX_BYTE(A,3,PDOR)

//B端口
#define PTB_BYTE0_OUT        PTX_BYTE(B,0,PDOR)
#define PTB_BYTE1_OUT        PTX_BYTE(B,1,PDOR)
#define PTB_BYTE2_OUT        PTX_BYTE(B,2,PDOR)
#define PTB_BYTE3_OUT        PTX_BYTE(B,3,PDOR)

//C端口
#define PTC_BYTE0_OUT        PTX_BYTE(C,0,PDOR)
#define PTC_BYTE1_OUT        PTX_BYTE(C,1,PDOR)
#define PTC_BYTE2_OUT        PTX_BYTE(C,2,PDOR)
#define PTC_BYTE3_OUT        PTX_BYTE(C,3,PDOR)

//D端口
#define PTD_BYTE0_OUT        PTX_BYTE(D,0,PDOR)
#define PTD_BYTE1_OUT        PTX_BYTE(D,1,PDOR)
#define PTD_BYTE2_OUT        PTX_BYTE(D,2,PDOR)
#define PTD_BYTE3_OUT        PTX_BYTE(D,3,PDOR)

//E端口
#define PTE_BYTE0_OUT        PTX_BYTE(E,0,PDOR)
#define PTE_BYTE1_OUT        PTX_BYTE(E,1,PDOR)
#define PTE_BYTE2_OUT        PTX_BYTE(E,2,PDOR)
#define PTE_BYTE3_OUT        PTX_BYTE(E,3,PDOR)


//8bit端口的输入**************************************************
//A端口
#define PTA_BYTE0_INT        PTX_BYTE(A,0,PDIR)
#define PTA_BYTE1_INT        PTX_BYTE(A,1,PDIR)
#define PTA_BYTE2_INT        PTX_BYTE(A,2,PDIR)
#define PTA_BYTE3_INT        PTX_BYTE(A,3,PDIR)

//B端口
#define PTB_BYTE0_INT        PTX_BYTE(B,0,PDIR)
#define PTB_BYTE1_INT        PTX_BYTE(B,1,PDIR)
#define PTB_BYTE2_INT        PTX_BYTE(B,2,PDIR)
#define PTB_BYTE3_INT        PTX_BYTE(B,3,PDIR)

//C端口
#define PTC_BYTE0_INT        PTX_BYTE(C,0,PDIR)
#define PTC_BYTE1_INT        PTX_BYTE(C,1,PDIR)
#define PTC_BYTE2_INT        PTX_BYTE(C,2,PDIR)
#define PTC_BYTE3_INT        PTX_BYTE(C,3,PDIR)

//D端口
#define PTD_BYTE0_INT        PTX_BYTE(D,0,PDIR)
#define PTD_BYTE1_INT        PTX_BYTE(D,1,PDIR)
#define PTD_BYTE2_INT        PTX_BYTE(D,2,PDIR)
#define PTD_BYTE3_INT        PTX_BYTE(D,3,PDIR)

//E端口
#define PTE_BYTE0_INT        PTX_BYTE(E,0,PDIR)
#define PTE_BYTE1_INT        PTX_BYTE(E,1,PDIR)
#define PTE_BYTE2_INT        PTX_BYTE(E,2,PDIR)
#define PTE_BYTE3_INT        PTX_BYTE(E,3,PDIR)


//16bit端口的输入**************************************************
//A端口
#define PTA_WORD0_INT        PTX_WORD(A,0,PDIR)
#define PTA_WORD1_INT        PTX_WORD(A,1,PDIR)

//B端口
#define PTB_WORD0_INT        PTX_WORD(B,0,PDIR)
#define PTB_WORD1_INT        PTX_WORD(B,1,PDIR)

//C端口
#define PTC_WORD0_INT        PTX_WORD(C,0,PDIR)
#define PTC_WORD1_INT        PTX_WORD(C,1,PDIR)


//D端口
#define PTD_WORD0_INT        PTX_WORD(D,0,PDIR)
#define PTD_WORD1_INT        PTX_WORD(D,1,PDIR)

//E端口
#define PTE_WORD0_INT        PTX_WORD(E,0,PDIR)
#define PTE_WORD1_INT        PTX_WORD(E,1,PDIR)





typedef enum PTxn
{
    /*  PTA端口    */ //0~31
    PTA0,
    PTA1,
    PTA2,
    PTA3,
    PTA4,
    PTA5,
    PTA6,
    PTA7,
    PTA8,
    PTA9,
    PTA10,
    PTA11,
    PTA12,
    PTA13,
    PTA14,
    PTA15,
    PTA16,
    PTA17,
    PTA18,
    PTA19,
    PTA20,
    PTA21,
    PTA22,
    PTA23,
    PTA24,
    PTA25,
    PTA26,
    PTA27,
    PTA28,
    PTA29,
    PTA30,
    PTA31,

    /*  PTB端口    */   //32~63
    PTB0,
    PTB1,
    PTB2,
    PTB3,
    PTB4,
    PTB5,
    PTB6,
    PTB7,
    PTB8,
    PTB9,
    PTB10,
    PTB11,
    PTB12,
    PTB13,
    PTB14,
    PTB15,
    PTB16,
    PTB17,
    PTB18,
    PTB19,
    PTB20,
    PTB21,
    PTB22,
    PTB23,
    PTB24,
    PTB25,
    PTB26,
    PTB27,
    PTB28,
    PTB29,
    PTB30,
    PTB31,

    /*  PTC端口    */
    PTC0,
    PTC1,
    PTC2,
    PTC3,
    PTC4,
    PTC5,
    PTC6,
    PTC7,
    PTC8,
    PTC9,
    PTC10,
    PTC11,
    PTC12,
    PTC13,
    PTC14,
    PTC15,
    PTC16,
    PTC17,
    PTC18,
    PTC19,
    PTC20,
    PTC21,
    PTC22,
    PTC23,
    PTC24,
    PTC25,
    PTC26,
    PTC27,
    PTC28,
    PTC29,
    PTC30,
    PTC31,

    /*  PTD端口    */
    PTD0,
    PTD1,
    PTD2,
    PTD3,
    PTD4,
    PTD5,
    PTD6,
    PTD7,
    PTD8,
    PTD9,
    PTD10,
    PTD11,
    PTD12,
    PTD13,
    PTD14,
    PTD15,
    PTD16,
    PTD17,
    PTD18,
    PTD19,
    PTD20,
    PTD21,
    PTD22,
    PTD23,
    PTD24,
    PTD25,
    PTD26,
    PTD27,
    PTD28,
    PTD29,
    PTD30,
    PTD31,

    /*  PTE端口    */
    PTE0,
    PTE1,
    PTE2,
    PTE3,
    PTE4,
    PTE5,
    PTE6,
    PTE7,
    PTE8,
    PTE9,
    PTE10,
    PTE11,
    PTE12,
    PTE13,
    PTE14,
    PTE15,
    PTE16,
    PTE17,
    PTE18,
    PTE19,
    PTE20,
    PTE21,
    PTE22,
    PTE23,
    PTE24,
    PTE25,
    PTE26,
    PTE27,
    PTE28,
    PTE29,
    PTE30,
    PTE31,

} PTxn;


void gpio_init (PTxn ptxn, GPIO_CFG cfg, unsigned short data) ;
void gpio_set (PTxn ptxn,unsigned short sata) ;
void gpio_turn(PTxn ptxn) ;
unsigned short gpio_get (PTxn ptxn) ;
void gpio_enable_port (void) ;
void gpio_Interrupt_init(PTxn ptxn, GPIO_CFG cfg, unsigned long mode)  ;

#endif      
