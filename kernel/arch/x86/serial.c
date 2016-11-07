#include <kernel.h>
#include <stdint.h>
#include <io.h>

#include "arch.h"

#define SERIAL_COM1 0x3F8

void ArDebugPrint(char c)
{
    while((inb(SERIAL_COM1 + 5) & 0x20) == 0);

    outb(SERIAL_COM1, c);

    if(c == '\n') ArDebugPrint('\r');
}
