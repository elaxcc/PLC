#include "YDelay.h"

#define YDELAY_CPU_SPEED 72000000

#warning "Change YDELAY_CPU_SPEED if need!"

void YDelay(uint32_t ms)
{
	uint64_t tmp = (uint64_t) YDELAY_CPU_SPEED * ((uint64_t)ms);
	uint64_t delay_ticks = tmp / (uint64_t)15000;
	while (delay_ticks)
	{
		__asm
		{
			nop
		}
		delay_ticks--;
	}
}
