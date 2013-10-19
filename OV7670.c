#include "lm3s1968.h"
#include "main.h"
#include "OV7670reg.h"
#include <stdint.h>

/*
*		OV7670 Driver File
*
*		OV7670 Camera Pinout:
*		D0-D7 = PB0-7
*		SDIOC = PE0
*		SDIOD = PE1
*		VSYNC = PE2
*		HREF = PE3
*		PCLK = PH0
*		XCLK = PH1
*		RESET = PH2
*		PWDN = PH3
*/

#define OV7670_ADDR     0x42

#define QQVGA_HEIGHT 120
#define QQVGA_WIDTH 160

void Camera_Init(void);
int Camera_Set(char, char);
char Camera_Get(char);
void Camera_ReadFrame(void);
void Camera_ClearBuffers(void);
void Camera_CheckMissing(void);

char Camera_Get(char addr)
{
	I2C_ClearBuffers();
	I2CWriteLength = 2;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = OV7670_ADDR;
	I2CMasterBuffer[1] = addr;
	
	I2CEngine();
	
	delay = 0;
	
	I2C_ClearBuffers();
	I2CWriteLength = 0;
	I2CReadLength = 1;
	I2CMasterBuffer[0] = OV7670_ADDR | RD_BIT;
	
	while(I2CEngine() == I2CSTATE_SLA_NACK);
	
	return I2CSlaveBuffer[0];
	
}

int Camera_Set(char addr, char val)
{
	I2C_ClearBufferes();
	
	I2CWriteLength = 3;
	I2CMasterBuffer[0] = OV7670_ADDR;
	I2CMasterBuffer[1] = addr;
	I2CMasterBuffer[2] = val;
	
	return I2CEngine();
}

void Camera_Init()
{
	Camera_Set(REG_COM7, 0x80); /* reset to default values */
  Camera_Set(REG_CLKRC, 0x80);
  Camera_Set(REG_COM11, 0x0A);
  Camera_Set(REG_TSLB, 0x04);
  Camera_Set(REG_TSLB, 0x04);
  Camera_Set(REG_COM7, 0x04); /* output format: rgb */

  Camera_Set(REG_RGB444, 0x00); /* disable RGB444 */
  Camera_Set(REG_COM15, 0xD0); /* set RGB565 */
	
	Camera_Set(REG_HSTART, 0x16);
    Camera_Set(REG_HSTOP, 0x04);
    Camera_Set(REG_HREF, 0x24);
    Camera_Set(REG_VSTART, 0x02);
    Camera_Set(REG_VSTOP, 0x7a);
    Camera_Set(REG_VREF, 0x0a);
    Camera_Set(REG_COM10, 0x02);
    Camera_Set(REG_COM3, 0x04);
    Camera_Set(REG_COM14, 0x1a); // divide by 4
    //Camera_Set(REG_COM14, 0x1b); // divide by 8
    Camera_Set(REG_MVFP, 0x27);
    Camera_Set(0x72, 0x22); // downsample by 4
    //Camera_Set(0x72, 0x33); // downsample by 8
    Camera_Set(0x73, 0xf2); // divide by 4
    //Camera_Set(0x73, 0xf3); // divide by 8

    // test pattern
    //Camera_Set(0x70, 1 << 7);
    //Camera_Set(0x70, 0x0);

    // COLOR SETTING
    Camera_Set(0x4f, 0x80);
    Camera_Set(0x50, 0x80);
    Camera_Set(0x51, 0x00);
    Camera_Set(0x52, 0x22);
    Camera_Set(0x53, 0x5e);
    Camera_Set(0x54, 0x80);
    Camera_Set(0x56, 0x40);
    Camera_Set(0x58, 0x9e);
    Camera_Set(0x59, 0x88);
    Camera_Set(0x5a, 0x88);
    Camera_Set(0x5b, 0x44);
    Camera_Set(0x5c, 0x67);
    Camera_Set(0x5d, 0x49);
    Camera_Set(0x5e, 0x0e);
    Camera_Set(0x69, 0x00);
    Camera_Set(0x6a, 0x40);
    Camera_Set(0x6b, 0x0a);
    Camera_Set(0x6c, 0x0a);
    Camera_Set(0x6d, 0x55);
    Camera_Set(0x6e, 0x11);
    Camera_Set(0x6f, 0x9f);

    Camera_Set(0xb0, 0x84);
}
