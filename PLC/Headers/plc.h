#ifndef _PLC_H_
#define _PLC_H_

#include <stdint.h>

#define PLC_CHANNEL0 0x00000000
#define PLC_CHANNEL1 0x00000001
#define PLC_CHANNEL2 0x00000002
#define PLC_CHANNEL3 0x00000003
#define PLC_CHANNEL4 0x00000004
#define PLC_CHANNEL5 0x00000005

void PlcInit(void);
void PlcSetChannel(uint32_t channel);
void PlcSendByte(uint8_t byte);

#endif
