#ifndef RTL_MEMORY_H
#define RTL_MEMORY_H

void *memcpy(void *destination, const void *source, size_t num);
void *memmove(void *destination, const void *source, size_t num);
void *memset(void *ptr, int value, size_t num);
int memcmp(const void *ptr1, const void *ptr2, size_t num);

void RtlZeroMemory(void *ptr, size_t len);

#endif
