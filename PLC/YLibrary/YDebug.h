#ifndef __YDEBUG_H_
#define __YDEBUG_H_

#include "YBool.h"

#include <stdint.h>

//#define YDEBUG

#ifdef YDEBUG

#define FLOAT_SENDING_ACCURACY 1000

/*!
 * \brief Function of initialization of debug system. Use this function
 * before using debug system
 * \param[in] _send_func - address of transmitted byte function
 * \param[in] _complete_func - address of checking transmission completed function
 */
void YDebugInitFunc(void (*_send_func)(uint8_t), YBOOL (*_complete_func)(void));

/*!
 * \brief Transmitted message function
 * before using debug system
 * \param[in] _message - transmitted message
 * \param[in] _message_size - transmitted message size
 */
void YDebugSendMessageFunc(char* _message, uint32_t _message_size);

/*!
 * \brief Function transmit new line character
 */
void YDebugSendEndlFunc(void);

/*!
 * \brief Function transmit byte
 * \param[in] _byte - transmited byte
 */
void YDebugSendByteFunc(unsigned char _byte);

/*!
 * \brief Function transmit word
 * \param[in] _word - transmited word
 */
void YDebugSendWordFunc(uint16_t _word);

/*!
 * \brief Function transmit double word
 * \param[in] _word - transmited double word
 */
void YDebugSendDWordFunc(uint32_t _dword);

/*!
 * \brief Function send integer value in decimal format
 * \param[in] value for sending
 */
void YDebugSendDecimalFunc(uint32_t _value);

/*!
 * \brief Function send float value in decimal format
 * \param[in] value for sending
 */
void YDebugSendFloatFunc(float _value);
#endif // YDEBUG



/*!
 * \brief Function of initialization of debug system. Use this function
 * before using debug system
 * \param[in] _send_func - address of transmitted byte function
 * \param[in] _complete_func - address of checking transmission completed function
 */
#ifdef YDEBUG
	#define YDebugInit(_send_func, _complete_func) \
		YDebugInitFunc(_send_func, _complete_func);
#else
	#define YDebugInit(_send_func, _complete_func)
#endif // YDEBUG

/*!
 * \brief Transmitted message function
 * before using debug system
 * \param[in] _message - transmitted message
 * \param[in] _message_size - transmitted message size
 */
#ifdef YDEBUG
	#define YDebugSendMessage(_message, _message_size) \
		YDebugSendMessageFunc(_message, _message_size)
#else
	#define YDebugSendMessage(_message, _message_size)
#endif // YDEBUG

/*!
 * \brief Function transmit new line character
 */
#ifdef YDEBUG
	#define YDebugSendEndl() \
		YDebugSendEndlFunc()
#else
	#define YDebugSendEndl()
#endif // YDEBUG

/*!
 * \brief Function transmit byte
 * \param[in] _byte - transmited byte
 */
#ifdef YDEBUG
	#define YDebugSendByte(_byte) \
		YDebugSendByteFunc(_byte)
#else
	#define YDebugSendByte(_byte)
#endif // YDEBUG

/*!
 * \brief Function transmit word
 * \param[in] _word - transmited word
 */
#ifdef YDEBUG
	#define YDebugSendWord(_word) \
		YDebugSendWordFunc(_word)
#else
	#define YDebugSendWord(_word)
#endif // YDEBUG

/*!
 * \brief Function transmit double word
 * \param[in] _sword - transmited double word
 */
#ifdef YDEBUG
	#define YDebugSendDWord(_dword) \
		YDebugSendDWordFunc(_dword)
#else
	#define YDebugSendDWord(_word)
#endif // YDEBUG

/*!
 * \brief Function send integer value in decimal format
 * \param[in] value for sending
 */
#ifdef YDEBUG
	#define YDebugSendDecimal(_value) \
		YDebugSendDecimalFunc(_value)
#else
	#define YDebugSendDecimal(_value)
#endif // YDEBUG

/*!
 * \brief Function send float value in decimal format
 * \param[in] value for sending
 */
#ifdef YDEBUG
	#define YDebugSendFloat(_value) \
		YDebugSendFloatFunc(_value)
#else
	#define YDebugSendFloat(_value)
#endif // YDEBUG

/*!
 * \brief Function transmit message if condition is false and
 * then continue program
 * \param[in] _condition - condition
 * \param[in] _message - transmitted message
 * \param[in] _message_size - transmitted message size
 */
#ifdef YDEBUG
	#define YASSERT_CONTINUE(_condition, _message, _message_size) \
		if (_condition == YFALSE) \
		{ \
			YDebugSendMessage(_message, _message_size); \
		}
#else
	#define YASSERT_CONTINUE(_value, _message, _message_size)
#endif // YDEBUG

/*!
 * \brief Function transmit message if condition is false and
 * then stop program execution
 * \param[in] _condition - condition
 * \param[in] _message - transmitted message
 * \param[in] _message_size - transmitted message size
 */
#ifdef YDEBUG
	#define YASSERT_STOP(_condition, _message, _message_size) \
		if (_condition == YFALSE) \
		{ \
			YDebugSendMessage(_message, _message_size); \
			while(YTRUE) {} \
		}
#else
	#define YASSERT_CONTINUE(_value, _message, _message_size)
#endif // YDEBUG

#endif // __YDEBUG_H_
