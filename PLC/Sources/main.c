#include "stm32f10x.h"

#include "YDebug.h"
#include "YDelay.h"

#include "Plc.h"

#ifdef YDEBUG
void DebugUartInit(void)
{
	// enable GPIOB
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	// configure USART pins
		// TX
	GPIOB->CRH |= GPIO_CRH_MODE10;
	GPIOB->CRH &= ~GPIO_CRH_CNF10;
	GPIOB->CRH |= GPIO_CRH_CNF10_1;
		// RX
	GPIOB->CRH &= ~GPIO_CRH_CNF11;
	GPIOB->CRH |= GPIO_CRH_CNF11_1;
	
	// enable USART3
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	// configure USART3
	USART3->CR1 |= (1 << 13) | (1 << 3) | (1 << 2); // emable USART & TX & RX
	USART3->BRR |= (uint32_t)(0x27 << 4); // baudrate 57600
}

void DebugUartTransmit(uint8_t byte)
{
	USART3->DR = (uint32_t) byte;
}

YBOOL DebugUartIsTransmissionComplete(void)
{
	if (USART3->SR & USART_SR_TC)
	{
		return YTRUE;
	}
	return YFALSE;
}
#endif

int main(void)
{
	#ifdef YDEBUG
	// UART initialization
	DebugUartInit();
	// Debug library initialization
	YDebugInit(&DebugUartTransmit, &DebugUartIsTransmissionComplete);
	#endif // YDEBUG
	
	__enable_irq();
	
	PlcInit();
	
	while(YTRUE)
	{
		PlcSendByte(0x13);
		//YDebugSendMessage("Infinit loop\r\n", 14);
		//YDelay(1000);
	}
	
	return 0;
}
