#include "lm3s1968.h"
#include "Main.h"
/*
*
*		XPT2046 Touch Controller File
*
*		Touch Ability Pinout:
*		T_CLK = PG0
*		T_CS = PG1
*		T_DIN = PG3
*		T_DOUT = PG4
*		T_IRQ	= PG6
*/
unsigned int ReadFromCharFrom7843(void);
void WriteCharTo7843(unsigned char);

/*void Touch_Init(void);
int getX(void);
int getY(void);
int dataAvailable(void);
int touch_ReadData(void);
*/

//Touch Global Variables:
/*int prec = 10;
long _default_orientation;
int orient;
int display_model;
long disp_x_size, disp_y_size, default_orientation;
long touch_x_left, touch_x_right, touch_y_top, touch_y_bottom;
int TP_X, TP_Y;

#define PORTRAIT			0
#define LANDSCAPE			1

#define PREC_LOW			1
#define PREC_MEDIUM			2
#define PREC_HI				3
#define PREC_EXTREME		4

#define CAL_X 0x00378F66UL
#define CAL_Y 0x03C34155UL
#define CAL_S 0x000EF13FUL
*/

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

/*
*		T_CLK = PG0
*		T_CS = PG1
*		T_DIN = PG3				(Input)
*		T_DOUT = PG4
*		T_IRQ	= PG6				(Pseudo-Input)
*/
/*void Touch_Init()
{
	orient					= PORTRAIT;
	_default_orientation	= CAL_S>>31;
	touch_x_left			= (CAL_X>>14) & 0x3FFF;
	touch_x_right			= CAL_X & 0x3FFF;
	touch_y_top				= (CAL_Y>>14) & 0x3FFF;
	touch_y_bottom			= CAL_Y & 0x3FFF;
	disp_x_size				= (CAL_S>>12) & 0x0FFF;
	disp_y_size				= CAL_S & 0x0FFF;
	prec					= 10;
	
	GPIO_PORTG_DATA_R |= 0x47;
}



int dataAvailable()
{
	int avail = 0;
	
	PORTG_Init(0);
	avail = !((GPIO_PORTG_DATA_R & ( 1 << 6 )) >> 6);
	PORTG_Init(1);
	return avail;
}

void touch_WriteData(int data)
{
	int temp;
	int count = 0;

	temp=data;
	GPIO_PORTG_DATA_R &= 0xFE;

	for(count=0; count<8; count++)
	{
		if(temp & 0x80)
			GPIO_PORTG_DATA_R |= 0x08;
		else
			GPIO_PORTG_DATA_R &= 0xF7;
		temp = temp << 1; 
		GPIO_PORTG_DATA_R &= 0xFE;
		GPIO_PORTG_DATA_R |= 0x01;
	}
}

int touch_ReadData()
{
	int data = 0;
	int count = 0;
	for(count=0; count<12; count++)
	{
		data <<= 1;
		GPIO_PORTG_DATA_R |= 0x01;
		GPIO_PORTG_DATA_R &= 0xFE;                
		if (((GPIO_PORTG_DATA_R & (1 << 4)) >> 4))
			data++;
	}
	return(data);
}

void read()
{
	unsigned long tx=0, temp_x=0;
	unsigned long ty=0, temp_y=0;
	int datacount=0;
	int i = 0;
	
	GPIO_PORTG_DATA_R &= 0xFD;

	PORTG_Init(0);
	
	for (i=0; i<prec; i++)
	{
		if (!((GPIO_PORTG_DATA_R & ( 1 << 6 )) >> 6))
		{
			touch_WriteData(0x90);        
			GPIO_PORTG_DATA_R |= 0x01;
			GPIO_PORTG_DATA_R &= 0xFE;
			temp_x=touch_ReadData();

			if (!((GPIO_PORTG_DATA_R & ( 1 << 6 )) >> 6))
			{
				touch_WriteData(0xD0);      
				GPIO_PORTG_DATA_R |= 0x01;
				GPIO_PORTG_DATA_R &= 0xFE;
				temp_y=touch_ReadData();

				if ((temp_x>0) && (temp_x<4096) && (temp_y>0) && (temp_y<4096))
				{
					tx+=temp_x;
					ty+=temp_y;
					datacount++;
				}
			}
		}
	}
	PORTG_Init(1);

	GPIO_PORTG_DATA_R |= 0x02;                    
	if (datacount==prec)
	{
		if (orient == _default_orientation)
		{
			TP_X=ty/datacount;
			TP_Y=tx/datacount;
		}
		else
		{
			TP_X=tx/datacount;
			TP_Y=ty/datacount;
		}
	}
	else
	{
		TP_X=-1;
		TP_Y=-1;
	}
}

int getX()
{
	long c;

	if ((TP_X==-1) || (TP_Y==-1))
		return -1;
	if (orient == _default_orientation)
	{
		c = (long)((long)(TP_X - touch_x_left) * (disp_x_size)) / (long)(touch_x_right - touch_x_left);
		if (c<0)
			c = 0;
		if (c>disp_x_size)
			c = disp_x_size;
	}
	else
	{
		if (_default_orientation == PORTRAIT)
			c = (long)((long)(TP_X - touch_y_top) * (-disp_y_size)) / (long)(touch_y_bottom - touch_y_top) + (long)(disp_y_size);
		else
			c = (long)((long)(TP_X - touch_y_top) * (disp_y_size)) / (long)(touch_y_bottom - touch_y_top);
		if (c<0)
			c = 0;
		if (c>disp_y_size)
			c = disp_y_size;
	}
	return c;
}

int getY()
{
	int c;

	if ((TP_X==-1) || (TP_Y==-1))
		return -1;
	if (orient == _default_orientation)
	{
		c = (long)((long)(TP_Y - touch_y_top) * (disp_y_size)) / (long)(touch_y_bottom - touch_y_top);
		if (c<0)
			c = 0;
		if (c>disp_y_size)
			c = disp_y_size;
	}
	else
	{
		if (_default_orientation == PORTRAIT)
			c = (long)((long)(TP_Y - touch_x_left) * (disp_x_size)) / (long)(touch_x_right - touch_x_left);
		else
			c = (long)((long)(TP_Y - touch_x_left) * (-disp_x_size)) / (long)(touch_x_right - touch_x_left) + (long)(disp_x_size);
		if (c<0)
			c = 0;
		if (c>disp_x_size)
			c = disp_x_size;
	}
	return c;
}
*/

/**
*		Touch Ability Pinout:
*		T_CLK = PG0
*		T_CS = PG1
*		T_DIN = PG3
*		T_DOUT = PG4
*		T_IRQ	= PG6
**/
void Touch_Init()
{
	GPIO_PORTG_DATA_R |= 0x0B;
	GPIO_PORTG_DATA_R |= 0x01;
}

void WriteCharTo7843(unsigned char num)          //SPI Write Data
{
  unsigned char count=0;
  unsigned char temp;
  unsigned nop;
  temp=num;
	GPIO_PORTG_DATA_R &= 0xFE;
  for(count=0;count<8;count++)
  {
    if(temp&0x80)
			GPIO_PORTG_DATA_R |= 0x08;
    else
      GPIO_PORTG_DATA_R &= 0xF7;
 
    temp=temp<<1; 
 
    GPIO_PORTG_DATA_R &= 0xFE;               
    nop++;
    nop++;
		GPIO_PORTG_DATA_R |= 0x01;
    nop++;
    nop++;
  }
}
 
unsigned int ReadFromCharFrom7843()             //SPI Read Data
{ 
  unsigned nop;
  unsigned char count=0;
  unsigned int Num=0;
  for(count=0;count<12;count++)
  {
    Num<<=1;
    GPIO_PORTG_DATA_R |= 0x01;            
    nop++;
    GPIO_PORTG_DATA_R &= 0xFE; 
    nop++;
    if(CHECK_BIT(GPIO_PORTG_DATA_R,5)) Num++;
  }
  return(Num);
}

