#ifndef Vector_H
#define Vector_H

#include "fieldinfo.h"
#include <stddef.h>


typedef struct _vector {
    void* data; //сам массив
    size_t size;
    size_t capacity; // Выделенная память
    const FieldInfo* type_info;
} Vector;

Vector* Vector_Create(const FieldInfo* type_info, size_t initial_size);



// Уничтожение вектора
void Vector_Destroy(Vector* vec);

// Добавление элемента в конец
int Vector_Push(Vector* vec, void* element);

// Получение элемента по индексу (только чтение)
const void* Vector_Get(const Vector* vec, size_t index);

// Установка значения элемента по индексу
int Vector_Set(Vector* vec, size_t index, void* element);

// Получение текущего количества элементов
size_t Vector_Size(const Vector* vec);

int Vector_CheckType(const Vector* vec, const FieldInfo* expected_type);

#endif