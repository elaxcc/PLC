#ifndef _PLC_H_
#define _PLC_H_

#include <stdint.h>

void PlcInit(void);
void PlcSendByte(uint8_t byte);

#endif
