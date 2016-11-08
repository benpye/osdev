#include <kernel.h>
#include <rtl/debug.h>

#include <stdarg.h>

void RtlDebugPrint(const char *fmt, ...)
{
    va_list argList;
    va_start(argList, fmt);

    int arg = 0;
    while(*fmt != 0)
    {
        if(arg == 1)
        {
            switch(*fmt)
            {
                case 'i':
                    RtlDebugPrintInt(va_arg(argList, int), 10, 1);
                    break;
                case 'u':
                    RtlDebugPrintInt(va_arg(argList, int), 10, 0);
                    break;
                case 'x':
                    RtlDebugPrintInt(va_arg(argList, int), 16, 0);
                    break;
                case 'b':
                    RtlDebugPrintInt(va_arg(argList, int), 2, 0);
                    break;
                case 's':
                    RtlDebugPrintString(va_arg(argList, const char *));
                    break;
                case 'c':
                    RtlDebugPrintChar(va_arg(argList, int));
                    break;
                default:
                    RtlDebugPrintChar('%');
                    RtlDebugPrintChar(*fmt);
                    break;
            }
            arg = 0;
        }
        else if(*fmt == '%')
        {
            arg = 1;
        }
        else
        {
            RtlDebugPrintChar(*fmt);
        }
        fmt++;
    }

    if(arg)
        RtlDebugPrintChar('%');

    va_end(argList);
}

void RtlDebugPrintChar(char c)
{
    ArDebugPrint(c);
}

void RtlDebugPrintInt(int val, int base, int sign)
{
    char tmp[33] = {0};
    int i = 31;
    unsigned int uval = val;

    if(val < 0 && base == 10 && sign)
        uval = -val;

    for(; uval && i; --i, uval /= base)
    {
        tmp[i] = "0123456789abcdef"[uval % base];
    }

    if(val < 0 && base == 10 && sign) RtlDebugPrintChar('-');
    if(val == 0) RtlDebugPrintChar('0');
    else RtlDebugPrintString(&tmp[i+1]);
}

void RtlDebugPrintString(const char *str)
{
    while(*str != 0)
    {
        RtlDebugPrintChar(*str);
        str++;
    }
}
