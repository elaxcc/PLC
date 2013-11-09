#include "YDebug.h"

/*!
 * \brief it is private members for debug system? don't use it manualy
 * \member YDebugSendFunctionPtr_ - pointer on transmitted data function
 * - \param[in] uint8_t value byte for transmition
 * \member YDebugCompleteFunctionPtr_ - pointer on checking transmission complete function
  - \retval must return non zero value if transmition is completed
 */
#ifdef YDEBUG

void (*YDebugSendFunctionPtr_)(uint8_t) = 0;
YBOOL (*YDebugCompleteFunctionPtr_)(void) = 0;

#define TIMEOUT 10000

#endif // YDEBUG

#ifdef YDEBUG

void YDebugInitFunc(void (*_send_func)(uint8_t), YBOOL (*_complete_func)(void))
{
	YDebugSendFunctionPtr_ = _send_func;
	YDebugCompleteFunctionPtr_ = _complete_func;
}

void YDebugSendMessageFunc(char* _message, uint32_t _message_size)
{
	uint32_t iter = 0;
	uint32_t timeout = 0;
	while (_message_size != iter)
	{
		YDebugSendFunctionPtr_(_message[iter]);
		while (YDebugCompleteFunctionPtr_() == YFALSE && timeout != TIMEOUT) { timeout++; }
		iter++;
	}
}

void YDebugSendEndlFunc(void)
{
	uint32_t timeout = 0;
	YDebugSendFunctionPtr_('\r');
	while (YDebugCompleteFunctionPtr_() == YFALSE && timeout != TIMEOUT) { timeout++; }
	timeout = 0;
	YDebugSendFunctionPtr_('\n');
	while (YDebugCompleteFunctionPtr_() == YFALSE && timeout != TIMEOUT) { timeout++; }
}

void YDebugSendByteFunc(uint8_t _byte)
{
	uint32_t iter;
	uint8_t buf[2];
	uint32_t timeout = 0;
	
	buf[0] = _byte >> 4;
	buf[1] = _byte & 0x0F;
	
	for(iter = 0; iter < 2; iter++)
	{
		if((buf[iter] >= 0x00) && (buf[iter] <= 0x09))
		{
			buf[iter] += 0x30;
		}
		else if ((buf[iter] >= 0x0A) && (buf[iter] <= 0x0F))
		{
			buf[iter] += 0x37;
		}
		
		YDebugSendFunctionPtr_(buf[iter]);
		while (YDebugCompleteFunctionPtr_() == YFALSE && timeout != TIMEOUT) { timeout++; }
	}
}

void YDebugSendWordFunc(uint16_t _word)
{
	uint32_t iter;
	uint8_t byte;
	uint32_t timeout = 0;
	
	for(iter = 2; iter > 0; --iter)
	{
		byte = (uint8_t)(_word >> (8 * (iter - 1)));
		YDebugSendByteFunc(byte);
		while (YDebugCompleteFunctionPtr_() == YFALSE && timeout != TIMEOUT) { timeout++; }
	}
}

void YDebugSendDWordFunc(uint32_t _dword)
{
	uint32_t iter;
	uint8_t byte;
	uint32_t timeout = 0;
	
	for(iter = 4; iter > 0; --iter)
	{
		byte = (uint8_t)(_dword >> (8 * (iter - 1)));
		YDebugSendByteFunc(byte);
		while (YDebugCompleteFunctionPtr_() == YFALSE && timeout != TIMEOUT) { timeout++; }
	}
}

void YDebugSendDecimalFunc(uint32_t _value)
{
	uint8_t digitals_cnt = 0;
	uint8_t digitals_buffer[10];
	uint32_t timeout = 0;
	
	// prepare value
	while(_value)
	{
		digitals_buffer[digitals_cnt] = (uint8_t)(_value % 10);
		_value /= 10;
		digitals_cnt++;
	}
	
	// decimal send
	for (;digitals_cnt > 0; --digitals_cnt)
	{
		YDebugSendFunctionPtr_(digitals_buffer[digitals_cnt - 1] + 0x30);
		while (YDebugCompleteFunctionPtr_() == YFALSE && timeout != TIMEOUT) { timeout++; }
	}
}

void YDebugSendFloatFunc(float _value)
{
	uint32_t iter = 10;
	uint32_t integer_part = (uint32_t)_value;
	uint32_t fractional_part = (uint32_t)((_value - ((float)integer_part)) * ((float)FLOAT_SENDING_ACCURACY));
	uint32_t timeout = 0;
	
	// send integer part
	YDebugSendDecimalFunc(integer_part);
	
	if (fractional_part)
	{
		// send separator
		YDebugSendMessageFunc(".", 1);
		
		// send fractional part
		while (((fractional_part * iter) < FLOAT_SENDING_ACCURACY))
		{
			// send zeros if need
			YDebugSendFunctionPtr_(0x30);
			while (YDebugCompleteFunctionPtr_() == YFALSE && timeout != TIMEOUT) { timeout++; }
			iter *= 10;
		}
		YDebugSendDecimalFunc(fractional_part);
	}
}
#endif // YDEBUG
