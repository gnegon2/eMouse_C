#include "MKL46Z4.h"
#include "I2C.h"
#include "MMA8451.h"
#include "uart.h"

void MMA8451_Init(void)
{
	uint8_t CTRL_REG1;
	// Initialize I2C	
	I2C_Init();
	// Read actual state of System Control 1 Register
	CTRL_REG1 = MMA8451_ReadReg(MMA8451_CTRL_REG1);
	// Write to System Control 1 Register it's actual state with ACTIVE bit mask set
	MMA8451_WriteReg(MMA8451_CTRL_REG1, CTRL_REG1 | MMA8451_ACTIVE_BIT_MASK);	
}

void pause(void)
{
	int i;
	for( i = 0; i < 20; i++)
		__asm__ {NOP};
}

uint8_t MMA8451_ReadReg(uint8_t addr)
{
  	uint8_t result = 0;

    // Generate START signal
    I2C_Start(); 

    // Send slave address followed by an Write bit
    I2C_Write(MMA8451_I2C_ADDR | I2C_WRITE);
	
    //  Send register address from we will be read
    I2C_Write(addr);

    // Repeat START signal in order to communicate in a 
    // reveice mode without releasing the bus
    I2C_RepeatedStart();
    // Send slave address followed by an Read bit
    I2C_Write(MMA8451_I2C_ADDR | I2C_READ);
	
    // Now we can read from the previously selected register

    // Change transmit mode to Receive
    I2C_SetRXmode();

    // Clean data register
    result = I2C_ReadByte();
    I2C_Wait();

		// Send NACK to indicate to the slave
    // that one last byte is required
		I2C_SendNack();
		// Read value from slave device register
    result = I2C_ReadByte();
    I2C_Wait();

		// Generate STOP signal
    I2C_Stop();
		//Wait for STOP signal to propagate
		pause();

		return result;
}

void MMA8451_WriteReg(uint8_t addr, uint8_t data)
{
    // Generate START signal
		I2C_Start();

    // Send slave address followed by an Write bit
    I2C_Write(MMA8451_I2C_ADDR | I2C_WRITE);

    //  Send register address where we will be write data
    I2C_Write(addr);

    // Write data to the selected register
    I2C_Write(data);

    // Generate STOP signal
    I2C_Stop();

		pause();
}

void MMA8451_Read(uint8_t xy[2])
{	
    if( (MMA8451_ReadReg(0x00) & 0xf ) != 0 )
    {	
			// Read X value
			xy[0] = MMA8451_ReadReg(MMA8451_OUT_X_MSB);
			// Read Y value
			xy[1] = MMA8451_ReadReg(MMA8451_OUT_Y_MSB);							
    }
}



