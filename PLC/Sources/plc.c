#include "plc.h"

#include "stm32f10x.h"

#include "YBool.h"

// set/reset clk channel 0
#define CH0_CLK_HIGH GPIOA->BSRR |= GPIO_BSRR_BS1
#define CH0_CLK_LOW GPIOA->BSRR |= GPIO_BSRR_BR1
// set/reset data0 channel 0
#define CH0_DATA0_HIGH GPIOA->BSRR |= GPIO_BSRR_BS6
#define CH0_DATA0_LOW GPIOA->BSRR |= GPIO_BSRR_BR6
// set/reset data1 channel 0
#define CH0_DATA1_HIGH GPIOA->BSRR |= GPIO_BSRR_BS7
#define CH0_DATA1_LOW GPIOA->BSRR |= GPIO_BSRR_BR7
// set/reset data2 channel 0
#define CH0_DATA2_HIGH GPIOB->BSRR |= GPIO_BSRR_BS0
#define CH0_DATA2_LOW GPIOB->BSRR |= GPIO_BSRR_BR0
// set/reset data3 channel 0
#define CH0_DATA3_HIGH GPIOB->BSRR |= GPIO_BSRR_BS1
#define CH0_DATA3_LOW GPIOB->BSRR |= GPIO_BSRR_BR1

// set/reset clk channel 1
#define CH1_CLK_HIGH GPIOA->BSRR |= GPIO_BSRR_BS8
#define CH1_CLK_LOW GPIOA->BSRR |= GPIO_BSRR_BR8
// set/reset data0 channel 1
#define CH1_DATA0_HIGH GPIOB->BSRR |= GPIO_BSRR_BS6
#define CH1_DATA0_LOW GPIOB->BSRR |= GPIO_BSRR_BR6
// set/reset data1 channel 1
#define CH1_DATA1_HIGH GPIOB->BSRR |= GPIO_BSRR_BS7
#define CH1_DATA1_LOW GPIOB->BSRR |= GPIO_BSRR_BR7
// set/reset data2 channel 1
#define CH1_DATA2_HIGH GPIOB->BSRR |= GPIO_BSRR_BS8
#define CH1_DATA2_LOW GPIOB->BSRR |= GPIO_BSRR_BR8
// set/reset data3 channel 1
#define CH1_DATA3_HIGH GPIOB->BSRR |= GPIO_BSRR_BS9
#define CH1_DATA3_LOW GPIOB->BSRR |= GPIO_BSRR_BR9

// frequence prescalers for TIMs
#define TIM_ARR_60  0x0258 // 60.000,000 Hz - chanel 0
#define TIM_ARR_64  0x0233 // 63.943,161 Hz
#define TIM_ARR_68  0x0211 // 68.052,930 Hz
#define TIM_ARR_72  0x01F4 // 72.000,000 Hz
#define TIM_ARR_76  0x01DA // 75.949,367 Hz
#define TIM_ARR_80  0x01C2 // 80.000,000 Hz - channel1
#define TIM_ARR_84  0x01AD // 83.916,083 Hz
#define TIM_ARR_88  0x0199 // 88.019,559 Hz
#define TIM_ARR_92  0x0187 // 92.071,611 Hz
#define TIM_ARR_96  0x0177 // 96.000,000 Hz
#define TIM_ARR_200 0x00B4 // 200.000,000 Hz - channel 2
#define TIM_ARR_204 0x00B1 // 203.389,830 Hz
#define TIM_ARR_208 0x00AD // 208.092,485 Hz
#define TIM_ARR_212 0x00AA // 211.764,705 Hz
#define TIM_ARR_216 0x00A7 // 215.568,862 Hz
#define TIM_ARR_220 0x00A4 // 219.512,195 Hz - channel 3
#define TIM_ARR_224 0x00A1 // 223.602,484 Hz
#define TIM_ARR_228 0x009E // 227.848,101 Hz
#define TIM_ARR_232 0x009B // 232.258,064 Hz
#define TIM_ARR_236 0x0099 // 235.294,117 Hz
#define TIM_ARR_240 0x0096 // 240.000,000 Hz - channel 4
#define TIM_ARR_244 0x0094 // 243.243,243 Hz
#define TIM_ARR_248 0x0091 // 248.275,862 Hz
#define TIM_ARR_252 0x008F // 251.748,251 Hz
#define TIM_ARR_256 0x008D // 255.319,148 Hz
#define TIM_ARR_260 0x008B // 258.992,805 Hz - channel 5
#define TIM_ARR_264 0x0088 // 264.705,882 Hz
#define TIM_ARR_268 0x0086 // 268.656,716 Hz
#define TIM_ARR_272 0x0084 // 272.727,272 Hz
#define TIM_ARR_276 0x0082 // 276.923,076 Hz

// enable/disable clk
#define PLC_CLK_EN_TIM TIM2->CR1 |= 0x0001
#define PLC_CLK_DIS_TIM TIM2->CR1 &= 0xFFFE
// enable/disable data0
#define PLC_DATA0_EN_TIM TIM3->CR1 |= 0x0001
#define PLC_DATA0_DIS_TIM TIM3->CR1 &= 0xFFFE
// enable/disable data1
#define PLC_DATA1_EN_TIM TIM4->CR1 |= 0x0001
#define PLC_DATA1_DIS_TIM TIM4->CR1 &= 0xFFFE
// enable/disable data2
#define PLC_DATA2_EN_TIM TIM5->CR1 |= 0x0001
#define PLC_DATA2_DIS_TIM TIM5->CR1 &= 0xFFFE
// enable/disable data3
#define PLC_DATA3_EN_TIM TIM6->CR1 |= 0x0001
#define PLC_DATA3_DIS_TIM TIM6->CR1 &= 0xFFFE

#define PLC_PERIODS_CNT 16

#define PLC_PINS_GROUP_0 0
#define PLC_PINS_GROUP_1 1

YBOOL ch0_clk_set = YFALSE;
YBOOL ch0_data0_set = YFALSE;
YBOOL ch0_data1_set = YFALSE;
YBOOL ch0_data2_set = YFALSE;
YBOOL ch0_data3_set = YFALSE;

uint32_t plc_sin_period_num = 0;
uint32_t plc_zero_period_num = 0;
uint8_t plc_half_byte_num = 0;
uint8_t plc_sending_byte;
uint8_t plc_pins_group;
YBOOL lpc_sin_periods_generate;
volatile YBOOL plc_transmission_complete = YFALSE;

void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~TIM_SR_UIF;
	
	// generate sin periods
	if (plc_sin_period_num < PLC_PERIODS_CNT)
	{
		if (plc_sin_period_num == 0)
		{
			// data0
			if (plc_sending_byte & (0x01 << plc_half_byte_num))
			{
				PLC_DATA0_EN_TIM;
			}
			// data1
			if (plc_sending_byte & (0x02 << plc_half_byte_num))
			{
				PLC_DATA1_EN_TIM;
			}
			// data2
			if (plc_sending_byte & (0x04 << plc_half_byte_num))
			{
				PLC_DATA2_EN_TIM;
			}
			// data3
			if (plc_sending_byte & (0x08 << plc_half_byte_num))
			{
				PLC_DATA3_EN_TIM;
			}
			lpc_sin_periods_generate = YTRUE;
		}
		
		// clk
		if (ch0_clk_set == YFALSE)
		{
			if (plc_pins_group == PLC_PINS_GROUP_0)
			{
				CH0_CLK_HIGH;
			}
			else
			{
				CH1_CLK_HIGH;
			}
			ch0_clk_set = YTRUE;
			return;
		}
		if (plc_pins_group == PLC_PINS_GROUP_0)
		{
			CH0_CLK_LOW;
		}
		else
		{
			CH1_CLK_LOW;
		}
		ch0_clk_set = YFALSE;
		plc_sin_period_num++;
		return;
	}
	else if (lpc_sin_periods_generate)
	{
		plc_zero_period_num = 0;
		lpc_sin_periods_generate = YFALSE;
		
		// Disable TIMs
		PLC_DATA0_DIS_TIM;
		PLC_DATA1_DIS_TIM;
		PLC_DATA2_DIS_TIM;
		PLC_DATA3_DIS_TIM;
		
		// TIMs is stoped, change pins to LOW
		CH0_CLK_LOW;
		CH0_DATA0_LOW;
		CH0_DATA1_LOW;
		CH0_DATA2_LOW;
		CH0_DATA3_LOW;
	}
	
	// generate zero periods
	if (plc_zero_period_num < PLC_PERIODS_CNT * 2)
	{
		plc_zero_period_num++;
		return;
	}
	else
	{
		plc_half_byte_num += 4;
		plc_sin_period_num = 0;
		plc_zero_period_num = 0;
	}
	
	if (plc_half_byte_num > 4)
	{
		// stop clk
		PLC_CLK_DIS_TIM;
		
		plc_transmission_complete = YTRUE;
	}
}

void TIM3_IRQHandler(void)
{
	TIM3->SR &= ~TIM_SR_UIF;
	if (ch0_data0_set == YFALSE)
	{
		if (plc_pins_group == PLC_PINS_GROUP_0)
		{
			CH0_DATA0_HIGH;
		}
		else
		{
			CH1_DATA0_HIGH;
		}
		ch0_data0_set = YTRUE;
		return;
	}
	if (plc_pins_group == PLC_PINS_GROUP_0)
	{
		CH0_DATA0_LOW;
	}
	else
	{
		CH1_DATA0_LOW;
	}
	ch0_data0_set = YFALSE;
}

void TIM4_IRQHandler(void)
{
	TIM4->SR &= ~TIM_SR_UIF;
	if (ch0_data1_set == YFALSE)
	{
		if (plc_pins_group == PLC_PINS_GROUP_0)
		{
			CH0_DATA1_HIGH;
		}
		else
		{
			CH1_DATA1_HIGH;
		}
		ch0_data1_set = YTRUE;
		return;
	}
	if (plc_pins_group == PLC_PINS_GROUP_0)
	{
		CH0_DATA1_LOW;
	}
	else
	{
		CH1_DATA1_LOW;
	}
	ch0_data1_set = YFALSE;
}

void TIM5_IRQHandler(void)
{
	TIM5->SR &= ~TIM_SR_UIF;
	if (ch0_data2_set == YFALSE)
	{
		if (plc_pins_group == PLC_PINS_GROUP_0)
		{
			CH0_DATA2_HIGH;
		}
		else
		{
			CH1_DATA2_HIGH;
		}
		ch0_data2_set = YTRUE;
		return;
	}
	if (plc_pins_group == PLC_PINS_GROUP_0)
	{
		CH0_DATA2_LOW;
	}
	else
	{
		CH1_DATA2_LOW;
	}
	ch0_data2_set = YFALSE;
}

void TIM6_IRQHandler(void)
{
	TIM6->SR &= ~TIM_SR_UIF;
	if (ch0_data3_set == YFALSE)
	{
		if (plc_pins_group == PLC_PINS_GROUP_0)
		{
			CH0_DATA3_HIGH;
		}
		else
		{
			CH1_DATA3_HIGH;
		}
		ch0_data3_set = YTRUE;
		return;
	}
	if (plc_pins_group == PLC_PINS_GROUP_0)
	{
		CH0_DATA3_LOW;
	}
	else
	{
		CH1_DATA3_LOW;
	}
	ch0_data3_set = YFALSE;
}

void SetChannel0ToTIM(void)
{
	TIM2->ARR = TIM_ARR_60;
	TIM3->ARR = TIM_ARR_64;
	TIM4->ARR = TIM_ARR_68;
	TIM5->ARR = TIM_ARR_72;
	TIM6->ARR = TIM_ARR_76;
}

void SetChannel1ToTIM(void)
{
	TIM2->ARR = TIM_ARR_80;
	TIM3->ARR = TIM_ARR_84;
	TIM4->ARR = TIM_ARR_88;
	TIM5->ARR = TIM_ARR_92;
	TIM6->ARR = TIM_ARR_96;
}

void SetChannel2ToTIM(void)
{
	TIM2->ARR = TIM_ARR_200;
	TIM3->ARR = TIM_ARR_204;
	TIM4->ARR = TIM_ARR_208;
	TIM5->ARR = TIM_ARR_212;
	TIM6->ARR = TIM_ARR_216;
}

void SetChannel3ToTIM(void)
{
	TIM2->ARR = TIM_ARR_220;
	TIM3->ARR = TIM_ARR_224;
	TIM4->ARR = TIM_ARR_228;
	TIM5->ARR = TIM_ARR_232;
	TIM6->ARR = TIM_ARR_236;
}

void SetChannel4ToTIM(void)
{
	TIM2->ARR = TIM_ARR_240;
	TIM3->ARR = TIM_ARR_244;
	TIM4->ARR = TIM_ARR_248;
	TIM5->ARR = TIM_ARR_252;
	TIM6->ARR = TIM_ARR_256;
}

void SetChannel5ToTIM(void)
{
	TIM2->ARR = TIM_ARR_260;
	TIM3->ARR = TIM_ARR_264;
	TIM4->ARR = TIM_ARR_268;
	TIM5->ARR = TIM_ARR_272;
	TIM6->ARR = TIM_ARR_276;
}

void PlcInit(void)
{
	// enable GPIOx
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	// initialization pins for transmission
		// clk channel 0
	GPIOA->CRL |= GPIO_CRL_MODE1;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
		// data0 channel 0
	GPIOA->CRL |= GPIO_CRL_MODE6;
	GPIOA->CRL &= ~GPIO_CRL_CNF6;
		// data1 channel 0
	GPIOA->CRL |= GPIO_CRL_MODE7;
	GPIOA->CRL &= ~GPIO_CRL_CNF7;
		// data2 channel 0
	GPIOB->CRL |= GPIO_CRL_MODE0;
	GPIOB->CRL &= ~GPIO_CRL_CNF0;
		// data3 channel 0
	GPIOB->CRL |= GPIO_CRL_MODE1;
	GPIOB->CRL &= ~GPIO_CRL_CNF1;
		// clk channel 1
	GPIOA->CRH |= GPIO_CRH_MODE8;
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
		// data0 channel 1
	GPIOB->CRL |= GPIO_CRL_MODE6;
	GPIOB->CRL &= ~GPIO_CRL_CNF6;
		// data1 channel 1
	GPIOB->CRL |= GPIO_CRL_MODE7;
	GPIOB->CRL &= ~GPIO_CRL_CNF7;
		// data2 channel 1
	GPIOB->CRH |= GPIO_CRH_MODE8;
	GPIOB->CRH &= ~GPIO_CRH_CNF8;
		// data3 channel 1
	GPIOB->CRH |= GPIO_CRH_MODE9;
	GPIOB->CRH &= ~GPIO_CRH_CNF9;
	
	// initialization Timers for transmission
		// clk channel 0
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	NVIC->ISER[0] |= (uint32_t) (1 << 28); // TIM2 global interrupts
	TIM2->DIER |= (uint16_t) TIM_DIER_UIE;
		// data0 channel 0
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	NVIC->ISER[0] |= (uint32_t) (1 << 29); // TIM3 global interrupts
	TIM3->DIER |= (uint16_t) TIM_DIER_UIE;
		// data1 channel 0
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	NVIC->ISER[0] |= (uint32_t) (1 << 30); // TIM4 global interrupts
	TIM4->DIER |= (uint16_t) TIM_DIER_UIE;
		// data2 channel 0
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	NVIC->ISER[1] |= (uint32_t) (1 << 11); // TIM5 global interrupts
	TIM5->DIER |= (uint16_t) TIM_DIER_UIE;
		// data3 channel 0
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	NVIC->ISER[1] |= (uint32_t) (1 << 15); // TIM6 global interrupts
	TIM6->DIER |= (uint16_t) TIM_DIER_UIE;
}

void PlcSetChannel(uint32_t channel)
{
	switch(channel)
	{
		case 0x00000000 :
		{
			SetChannel0ToTIM();
			plc_pins_group = PLC_PINS_GROUP_0;
			break;
		}
		case 0x00000001 :
		{
			SetChannel1ToTIM();
			plc_pins_group = PLC_PINS_GROUP_0;
			break;
		}
		case 0x00000002 :
		{
			SetChannel2ToTIM();
			plc_pins_group = PLC_PINS_GROUP_1;
			break;
		}
		case 0x00000003 :
		{
			SetChannel3ToTIM();
			plc_pins_group = PLC_PINS_GROUP_1;
			break;
		}
		case 0x00000004 :
		{
			SetChannel4ToTIM();
			plc_pins_group = PLC_PINS_GROUP_1;
			break;
		}
		case 0x00000005 :
		{
			SetChannel5ToTIM();
			plc_pins_group = PLC_PINS_GROUP_1;
			break;
		}
		default :
		{
			SetChannel0ToTIM();
			plc_pins_group = PLC_PINS_GROUP_0;
		}
	}
}

void PlcSendByte(uint8_t byte)
{
	plc_sin_period_num = 0;
	plc_half_byte_num = 0;
	plc_sending_byte = byte;
	plc_transmission_complete = YFALSE;
	
	PLC_CLK_EN_TIM;
	TIM2->SR &= ~TIM_SR_UIF;
	
	while (plc_transmission_complete != YTRUE)
	{
	}
}
