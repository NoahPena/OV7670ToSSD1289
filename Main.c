#include "lm3s1968.h"
#include "SysTick.h"
#include "SSD1289.h"
#include "XPT2046.h"
#include "ADC.h"
#include "OV7670.h"

/**
*		Pin Defintions:
*		RD = Read(Active Low)
*		WR = Write(Active Low)
*		CS = Chip Select(Active Low)
*		RS = High = Data; Low = Command
*		RST = Low = Enable
*		DP_Lo = Data Bus {0 - 7}
*		DP_Hi = Data Bus {8 - 15}
*
*		LCD Screen Pinout:
*		RD = PD0
*		WR = PD1
*		CS = PD2
*		RS = PD3
*		RST = PG5
*		DB0-DB7 = PA0 - PA7
*		DB8-DB15 = PF0 - PF7
*
*		Touch Ability Pinout:
*		T_CLK = PG0
*		T_CS = PG1
*		T_DIN = PG3
*		T_DOUT = PG4
*		T_IRQ	= PG6
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
*		
*		
*		3.2" TFT LCD with SD Cage SSD_1289 Driver
**/
void PORTG_Init(int);
void PORTD_Init(void);
void PORTA_Init(void);
void PORTF_Init(void);
void PORTB_Init(void);
void PORTE_Init(int);
void PORTH_Init(void);
void ghettoDelay(int);

volatile unsigned long delay;

const unsigned int black = 0;
const unsigned int red = 63488;
const unsigned int green = 2016;
const unsigned int blue = 31;
const unsigned int white = 65535;
const unsigned int purple = 61727;
const unsigned int yellow = 65504;
const unsigned int cyan = 2047;
const unsigned int d_gray = 21130;
const unsigned int l_gray = 31727;

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

int main(void)
{
//	unsigned long x, a, b, c, d;
//	char cool[5];
	//int i = 0;
	SysTick_Init();
	ADC_Init(1);
	PORTG_Init(1);
	PORTD_Init();
	PORTA_Init();
	PORTF_Init();
	PORTB_Init();
	PORTE_Init(1);
	PORTH_Init();
	
	LCD_Init();
	Touch_Init();
	Camera_Init();
/*	while(1)
	{
	x = ADC_In();
	a = x/1000;
	x -= a * 1000;
	b = x/100;
	x -= b * 100;
	c = x/10;
	x -= c * 10;
	d = x;

	cool[0] = (char)(a + 48);
	cool[1] = (char)(b + 48);
	cool[2] = (char)(c + 48);
	cool[3] = (char)(d + 48);
	cool[4] = 0;
	
	TFT_Fill(purple);
	TFT_Text(&cool[0], 50, 50, 16, black, purple);
	}
	*/
	
	while(1)
	{
		GPIO_PORTG_DATA_R ^= 0x04;
		if(ADC_In() != 1023)
		{
			TFT_Fill(yellow);
		} else {
			TFT_Fill(purple);
		}
	}
	
	//TFT_H_Line(50,150,50,black);
	
	
	
//	return 1;
}

// Time in ms
void ghettoDelay(int time)
{
	SysTick_Wait(time*250000);
}

/*
*		T_CLK = PG0
*		T_CS = PG1
*		T_DIN = PG3				(Input)
*		T_DOUT = PG4
*		T_IRQ	= PG6				(Pseudo-Input)
*/
void PORTG_Init(int regular)
{
	if(regular == 1)
	{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTG_DIR_R |= 0xF7;
	GPIO_PORTG_AFSEL_R &= ~0xF7;
	GPIO_PORTG_DEN_R |= 0xF7;
	} else if(regular == 0)
	{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOG;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTG_DIR_R |= 0xB7;
	GPIO_PORTG_AFSEL_R &= ~0xB7;
	GPIO_PORTG_DEN_R |= 0xB7;
	}
}

void PORTD_Init()
{
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; // activate port D
  delay = SYSCTL_RCGC2_R;      // allow time for clock to stabilize
  GPIO_PORTD_DIR_R |= 0x0F;    // make PD3-0 out
  GPIO_PORTD_AFSEL_R &= ~0x0F; // regular port function 
  GPIO_PORTD_DEN_R |= 0x0F;    // enable digital I/O on PD3-0
}

void PORTA_Init()
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTA_DIR_R |= 0xFF;
	GPIO_PORTA_AFSEL_R &= ~0xFF;
	GPIO_PORTA_DEN_R |= 0xFF;
}

void PORTF_Init()
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_DIR_R |= 0xFF;
	GPIO_PORTF_AFSEL_R &= ~0xFF;
	GPIO_PORTF_DEN_R |= 0xFF;
}

void PORTB_Init()
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTB_DIR_R |= 0x00;
	GPIO_PORTB_AFSEL_R &= ~0x00;
	GPIO_PORTB_DEN_R |= 0x00;	
}

void PORTE_Init(int regular)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
	delay = SYSCTL_RCGC2_R;
	if(regular == 1)
	{
	GPIO_PORTE_DIR_R |= 0x03;
	GPIO_PORTE_AFSEL_R &= ~0x03;
	GPIO_PORTE_DEN_R |= 0x03;
	} else {
	GPIO_PORTE_DIR_R |= 0x01;
	GPIO_PORTE_AFSEL_R &= ~0x01;
	GPIO_PORTE_DEN_R |= 0x01;
	}
}

void PORTH_Init(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOH;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTH_DIR_R |= 0x0E;
	GPIO_PORTH_AFSEL_R &= ~0x0E;
	GPIO_PORTH_DEN_R |= 0x0E;
}


