#ifndef MMA8451Q_H_
#define MMA8451Q_H_

#define MMA8451Q_I2C_ADDRESS			0x1D

#define I2C_MWSR						0x00  /* Master write  */
#define I2C_MRSW						0x01  /* Master read */


#define MMA8451Q_I2C_Start()            I2C0_C1 |= 0x30;

#define MMA8451Q_I2C_Stop()             I2C0_C1 &= (~0x30);

#define MMA8451Q_I2C_RepeatedStart()    I2C0_C1 |= 0x04;

#define MMA8451Q_I2C_Disable_Ack()		I2C0_C1 |= 0x08;

#define MMA8451Q_I2C_Enable_Ack()		I2C0_C1 &= (~0x08);

#define MMA8451Q_I2C_Wait()				while((I2C0_S & 0x02)==0);I2C0_S |= 0x02;

#define MMA8451Q_I2C_EnterRxMode()      I2C0_C1 &= (~0x10);

////////////////////////////////////////////////////
void MMA8451Q_I2C_Init(void);
void MMA8451Q_Init();

void MMA8451Q_WriteRegister(unsigned char u8RegisterAddress, unsigned char u8Data);
unsigned char MMA8451Q_ReadRegister(unsigned char u8RegisterAddress);

void I2C_Delay(void);

#endif /* MMA8451Q_H_ */
