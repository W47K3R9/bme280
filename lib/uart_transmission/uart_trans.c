#include "uart_trans.h"
#include <Print.h>
void init_uart_transmission(uint16_t baud_rate)
{
    // Configure value for register containing baud rate.
    uint16_t reg_rate = SYS_CLK / 16 / baud_rate - 1;

    // reg_rate = 103;
    // USART_BAUD_RATE_16 = reg_rate;
    USART_BAUD_RATE_L = (uint8_t) (reg_rate & 0xFF);
    USART_BAUD_RATE_H = (uint8_t) (reg_rate >> 8);

    // Enable Transmission (Receiving is not needed).
    // Let other default values as they are.
    USART_CTRL_STATUS_B |= (1 << TXEN0);
    USART_CTRL_STATUS_C = 0b00000110;
}

void send_char(char to_send)
{
    // Wait for emptied out data register
    while (!(USART_CTRL_STATUS_A & (1 << UDRE0)));
    USART_DATA = to_send;
}

void send_string(const char* to_send)
{
    while (*to_send)
    {
        send_char(*to_send);
        to_send++;
    }
}

void send_float(double number, uint8_t digits) 
{ 
  size_t n = 0;
  
  if (isnan(number)) return send_string("nan");
  if (isinf(number)) return send_string("inf");
  if (number > 4294967040.0) return send_string ("ovf");  // constant determined empirically
  if (number <-4294967040.0) return send_string ("ovf");  // constant determined empirically
  
  // Handle negative numbers
  if (number < 0.0)
  {
     n += print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    n += print('.'); 
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int)(remainder);
    n += print(toPrint);
    remainder -= toPrint; 
  } 
  
  return n;
}

