#ifndef RTL_DEBUG_H
#define RTL_DEBUG_H

void RtlDebugPrint(const char *fmt, ...);
void RtlDebugPrintChar(char c);
void RtlDebugPrintInt(int i, int base, int sign);
void RtlDebugPrintString(const char *str);

#endif