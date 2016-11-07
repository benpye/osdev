#ifndef RTL_DEBUG_H
#define RTL_DEBUG_H

void RtlDebugPrint(const char *fmt, ...);
void RtlDebugPrintChar(char c);
void RtlDebugPrintInt(int i, int base, int sign);
void RtlDebugPrintString(const char *str);

#define RtlDebugAssert(x,fmt,...) \
    if(!(x)) \
    { \
        RtlDebugPrint("ASSERTION FAILURE @ %s:%i (%s): ", __FILE__, __LINE__, __func__); \
        RtlDebugPrint(fmt, ##__VA_ARGS__); \
        ArDebugHalt(); \
    }

#endif