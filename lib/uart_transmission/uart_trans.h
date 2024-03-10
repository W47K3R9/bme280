#ifndef UART_TRANS_H
#define UART_TRANS_H

#include "ls_macros.h"

#define SYS_CLK 16000000UL


#define USART_CTRL_STATUS_A DEREF_8BIT(0xC0)
#define MPCM0 0
#define U2X0 1
#define UPE0 2
#define DOR0 3
#define FE0 4
#define UDRE0 5
#define TXC0 6
#define RXC0 7

#define USART_CTRL_STATUS_B DEREF_8BIT(0xC1)
#define TXB80 0
#define RXB80 1
#define UCSZ02 2
#define TXEN0 3
#define RXEN0 4
#define UDRIE0 5
#define TXCIE0 6
#define RXCIE0 7

#define USART_CTRL_STATUS_C DEREF_8BIT(0xC2)
#define UCPOL0 0
#define UCSZ00 1
#define UCPHA0 1
#define UCSZ01 2
#define UDORD0 2
#define USBS0 3
#define UPM00 4
#define UPM01 5
#define UMSEL00 6
#define UMSEL01 7

#define USART_BAUD_RATE_16 DEREF_16BIT(0xC4)

#define USART_BAUD_RATE_L DEREF_8BIT(0xC4)
#define UBRR0_0 0
#define UBRR0_1 1
#define UBRR0_2 2
#define UBRR0_3 3
#define UBRR0_4 4
#define UBRR0_5 5
#define UBRR0_6 6
#define UBRR0_7 7

#define USART_BAUD_RATE_H DEREF_8BIT(0xC5)
#define UBRR0_8 0
#define UBRR0_9 1
#define UBRR0_10 2
#define UBRR0_11 3

#define USART_DATA DEREF_8BIT(0xC6)
#define UDR0_0 0
#define UDR0_1 1
#define UDR0_2 2
#define UDR0_3 3
#define UDR0_4 4
#define UDR0_5 5
#define UDR0_6 6
#define UDR0_7 7

void init_uart_transmission(uint16_t baud_rate);
void send_char(const char to_send);
void send_string(const char* to_send);
void send_unsigned_decimal(uint64_t to_send);
void send_signed_decimal(int64_t to_send);
void send_float(float to_send, uint8_t precision);

#endif // UART_TRANS_H