#include "derivative.h" /* include peripheral declarations */
#include "MMA8451Q.h"

/*******************************************************************/
/*!
 * I2C Initialization
 * Set Baud Rate and turn on I2C0
 */
/*******************************************************************/
void MMA8451Q_I2C_Init(void)
{
    SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK; //Turn on clock to I2C0 module
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;//Turn on clock to PORTB module
    
    /* configure GPIO for I2C0 function */
    PORTE_PCR24 = PORT_PCR_MUX(5);
    PORTE_PCR25 = PORT_PCR_MUX(5);
    
    I2C0_F  = 0x14;       /* set MULT and ICR */

    I2C0_C1 = 0x80;       /* enable IIC */
}

/*******************************************************************/
/*!
 * MMA8451Q Initialization
 * Set MMA8451Q Control Register
 */
/*******************************************************************/
void MMA8451Q_Init()
{
	MMA8451Q_I2C_Init();
	
	//MMA8451Q_ID=MMA8451Q_ReadRegister(0x0D);  //read device ID
	
	MMA8451Q_WriteRegister(0x2A, 0x01);	// set MMA8451Q in ACTIVE mode
}

/*******************************************************************/
/*!
 * Write a byte of Data to specified register on MMA8451Q
 * @param u8RegisterAddress is Register Address
 * @param u8Data is Data to write
 */
void MMA8451Q_WriteRegister(unsigned char u8RegisterAddress, unsigned char u8Data)
{

	/* send start signal */
	MMA8451Q_I2C_Start();
	
	/* send ID with W bit */
	I2C0_D=((MMA8451Q_I2C_ADDRESS<<1) | I2C_MWSR);
	MMA8451Q_I2C_Wait();
	
	/* Write Register Address */
	I2C0_D=(u8RegisterAddress);
	MMA8451Q_I2C_Wait();
	
	/* Write Register Data */	
	I2C0_D=(u8Data);
	MMA8451Q_I2C_Wait();
	
	MMA8451Q_I2C_Stop();
	
	I2C_Delay();
}

/*******************************************************************/
/*!
 * Read a register from the MMA8451Q
 * @param u8RegisterAddress is Register Address
 * @return Data stored in Register
 */
unsigned char MMA8451Q_ReadRegister(unsigned char u8RegisterAddress)
{
	unsigned char result;
	unsigned int j;
	
	/* send start signal */
	MMA8451Q_I2C_Start();
	
	/* send ID with W bit */
	I2C0_D=((MMA8451Q_I2C_ADDRESS<<1) | I2C_MWSR);
	MMA8451Q_I2C_Wait();
	
	/* Write Register Address */
	I2C0_D=(u8RegisterAddress);
	MMA8451Q_I2C_Wait();
	
	/* Do a repeated start */
	MMA8451Q_I2C_RepeatedStart();

	/* send ID with R bit */
	I2C0_D=((MMA8451Q_I2C_ADDRESS<<1) | I2C_MRSW);
	MMA8451Q_I2C_Wait();
	
	/* Put in Rx Mode */
	MMA8451Q_I2C_EnterRxMode();
	
	/* Turn off ACK */
	MMA8451Q_I2C_Disable_Ack();
	
	/* Dummy read */
	result = I2C0_D;
	MMA8451Q_I2C_Wait();

	
	/* Send stop */
	MMA8451Q_I2C_Stop();
	result = I2C0_D ;
	
	/* Turn on ACK */
	MMA8451Q_I2C_Enable_Ack();
	
	I2C_Delay();
	return result;
}


/*******************************************************************/
/*!
 * I2C_Delay Routine
 */
void I2C_Delay(void){
    int n;
    for(n=1;n<50;n++) {
      asm("nop");
    }
}
