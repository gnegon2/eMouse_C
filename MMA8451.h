#include "MKL46Z4.h"

/* External 3-axis accelerometer control register addresses */
#define MMA8451_CTRL_REG1 0x2A
/* MMA8451 3-axis accelerometer control register bit masks */
#define MMA8451_ACTIVE_BIT_MASK 0x01

// Slave address of MMA8451Q
// Perform logical left shift by one bit
// which is reserved for R/W bit 
#define MMA8451_I2C_ADDR (0x1D << 1)

#define MMA8451_OUT_X_MSB 0x01
#define MMA8451_OUT_Y_MSB 0x03
#define MMA8451_OUT_Z_MSB 0x05

void MMA8451_Init(void);
uint8_t MMA8451_ReadReg(uint8_t addr);
static void pause(void);
void MMA8451_WriteReg(uint8_t addr, uint8_t data);
void MMA8451_Read(uint8_t xy[2]);
