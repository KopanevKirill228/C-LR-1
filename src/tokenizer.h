#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "my_string.h"
#include "dynamic_array.h"


/*
 * Алгоритм:
 * 1. Пропускаем пробелы и разделители
 * 2. Если текущий символ - буква/цифра, собираем идентификатор
 * 3. Иначе если символ - оператор/разделитель, берём его как одиночную лексему
 * 4. Добавляем лексему в массив
 * 5. Повторяем до конца строки
 */


// разбитие строки на лексемы
DynamicArray* Tokenize(const String* s);

// tokens - массив лексем. освобождение его в функции ниже
void Tokenize_Destroy(DynamicArray* tokens);

// Печатает массив лексем в формате [лексема1, лексема2, ...]
void Tokenize_Print(const DynamicArray* tokens, FILE* out);

//Получает лексему по индексу
String* Tokenize_Get(const DynamicArray* tokens, size_t index);

// Возвращает количество лексем в массиве
size_t Tokenize_Count(const DynamicArray* tokens);


// Проверяет, является ли строка char-типом (поддерживает ли токенизацию)
bool Tokenize_Supported(const String* s);

#endif