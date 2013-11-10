#ifndef _PLC_H_
#define _PLC_H_

#include <stdint.h>

#define PLC_CH0 0x00000000
#define PLC_CH1 0x00000001

void PlcInit(void);
void PlcSetChannel(uint32_t channel);
void PlcSendByte(uint8_t byte);

#endif
