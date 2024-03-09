#ifndef LS_MACROS_H
#define LS_MACROS_H

#include <stdint.h>
#define DEREF_8BIT(address) (*(volatile uint8_t*)(address))
#define DEREF_16BIT(address) (*(volatile uint16_t*)(address))

#endif // LS_MACROS_H