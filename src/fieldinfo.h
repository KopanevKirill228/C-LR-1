#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stddef.h>

typedef struct _field_info {
    size_t element_size;
    void* (*copy)(void*);
    void (*destroy)(void*);
    int (*compare)(void*, void*);
} FieldInfo;

// Ленивая инициализация
const FieldInfo* GetCharFieldInfo(void);

#endif