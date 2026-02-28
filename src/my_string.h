// my_string.h
#ifndef MY_STRING_H
#define MY_STRING_H

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "fieldinfo.h"
#include "Dynamic_Array.h"

typedef struct _string String;

// КОНСТРУКТОРЫ (ПОЛИМОРФНЫЕ — любой тип!)
String* String_Create(const void* data, size_t element_count, const FieldInfo* type_info);
String* String_CreateEmpty(size_t capacity, const FieldInfo* type_info);
void String_Destroy(String* s);

// ОПЕРАЦИИ
String* String_Concat(const String* s1, const String* s2);
String* String_Substring(const String* s, size_t start, size_t end);
int* String_Find(const String* s, const String* pattern, bool match_case, size_t* out_count);

// УТИЛИТЫ
size_t String_Length(const String* s);
const void* String_GetElement(const String* s, size_t index);
void String_Print(FILE* out, const String* s);  // ← Вывод как СТРОКА!
void String_PrintLine(const String* s);

// ПРОВЕРКА ТИПА
const FieldInfo* String_GetType(const String* s);
bool String_SameType(const String* s1, const String* s2);

#endif