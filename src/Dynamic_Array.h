#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "fieldinfo.h"
#include <stddef.h>

// Непрозрачный тип
typedef struct _dynamic_array DynamicArray;

//Конструктор / Деструктор
DynamicArray* DynamicArray_Create(const FieldInfo* type_info, size_t initial_size);
void DynamicArray_Destroy(DynamicArray* arr);

// Операции
int DynamicArray_Push(DynamicArray* arr, void* element);
const void* DynamicArray_Get(const DynamicArray* arr, size_t index);
int DynamicArray_Set(DynamicArray* arr, size_t index, void* element);
size_t DynamicArray_Size(const DynamicArray* arr);
int DynamicArray_CheckType(const DynamicArray* arr, const FieldInfo* expected_type);

const FieldInfo* DynamicArray_GetType(const DynamicArray* arr);

#endif