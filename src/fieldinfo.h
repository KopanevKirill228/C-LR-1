// fieldinfo.h
#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stddef.h>
#include <stdio.h>

typedef struct _field_info {
    size_t element_size;
    void* (*copy)(void*);
    void (*destroy)(void*);
    int (*compare)(void*, void*);
    void (*print)(FILE* out, void* elem);  // ← Печать элемента
} FieldInfo;

const FieldInfo* GetCharFieldInfo(void);
const FieldInfo* GetIntFieldInfo(void);

#endif