#include "Vector.h"
#include <stdlib.h>
#include <string.h>  // для memcpy

#define VECTOR_DEFAULT_CAPACITY 4

// Стратегия роста: удвоение емкости
static size_t Vector_CalculateNewCapacity(size_t current_capacity, size_t min_required) {
    size_t new_capacity = current_capacity;
    while (new_capacity < min_required) {
        new_capacity = (new_capacity == 0) ? VECTOR_DEFAULT_CAPACITY : new_capacity * 2;
    }
    return new_capacity;
}

// Внутренняя функция изменения емкости
static int Vector_Resize(Vector* vec, size_t new_capacity) {
    if (vec == NULL || vec->type_info == NULL) {
        return -1;
    }
    
    // Не уменьшаем capacity ниже текущего size
    if (new_capacity < vec->size) {
        new_capacity = vec->size;

    }
    
    size_t old_bytes = vec->capacity * vec->type_info->element_size;
    size_t new_bytes = new_capacity * vec->type_info->element_size;
    
    void* new_data = realloc(vec->data, new_bytes);
    if (new_data == NULL && new_capacity > 0) {
        return -1;  // Ошибка выделения памяти
    }
    
    // Обнуляем новую часть памяти (для безопасности)
    if (new_capacity > vec->capacity && new_data != NULL) {
        char* new_mem = (char*)new_data + old_bytes;
        size_t extra_bytes = new_bytes - old_bytes;
        memset(new_mem, 0, extra_bytes);
    }
    
    vec->data = new_data;
    vec->capacity = new_capacity;
    return 0;
}


// Создание нового вектора с заданной начальной емкостью
Vector* Vector_Create(const FieldInfo* type_info, size_t initial_capacity) {
    if (type_info == NULL) {
        return NULL;  // Ошибка: тип элемента обязателен
    }
    
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    if (vec == NULL) {
        return NULL;
    }
    
    vec->type_info = type_info;
    vec->size = 0;
    if (initial_capacity > 0) {
        vec->capacity = initial_capacity;
     } else {
        vec->capacity =sizeof(char);
     }
    
    // Выделяем память под элементы
    size_t bytes = vec->capacity * type_info->element_size;
    vec->data = malloc(bytes);
    
    if (vec->data == NULL) {
        free(vec);  // Очистка при ошибке
        return NULL;
    }
    
    // Инициализируем память с символами нулями
    memset(vec->data, '0', bytes);
    
    return vec;
}

// Уничтожение вектора и освобождение всех ресурсов
void Vector_Destroy(Vector* vec) {
    if (vec == NULL) {
        return;
    }
    
    // Важно: освобождаем каждый элемент через destroy-функцию типа
    // (например, для строк или сложных структур это критично)
    if (vec->data != NULL && vec->type_info->destroy != NULL) {
        for (size_t i = 0; i < vec->size; i++) {
            void* elem = (char*)vec->data + i * vec->type_info->element_size;
            vec->type_info->destroy(elem);
        }
    }
    
    free(vec->data);  // Освобождаем сам буфер
    free(vec);        // Освобождаем структуру вектора
}

// Добавление элемента в конец вектора
// Возвращает: 0 — успех, -1 — ошибка
int Vector_Push(Vector* vec, void* element) {
    if (vec == NULL || element == NULL || vec->type_info == NULL) {
        return -1;
    }
    
    // Если место закончилось — увеличиваем емкость
    if (vec->size >= vec->capacity) {
        size_t new_capacity = vec->capacity * 2;  // Стратегия удвоения
        if (Vector_Resize(vec, new_capacity) != 0) {
            return -1;  // Ошибка выделения памяти
        }
    }
    
    // Вычисляем адрес для вставки нового элемента
    void* dest = (char*)vec->data + vec->size * vec->type_info->element_size;
    
    // Копируем элемент с использованием функции копирования типа
    // Это обеспечивает глубокое копирование для сложных типов
    if (vec->type_info->copy != NULL) {
        void* copied = vec->type_info->copy(element);
        if (copied == NULL) {
            return -1;
        }
        // Копируем результат копирования в массив
        memcpy(dest, copied, vec->type_info->element_size);
        // Если copy выделил новую память, и она не совпадает с dest — освобождаем
        if (copied != dest) {
            free(copied);
        }
    } else {
        // Если copy не определен, делаем простое побайтовое копирование
        memcpy(dest, element, vec->type_info->element_size);
    }
    
    vec->size++;
    return 0;
}

// Получение указателя на элемент по индексу (только для чтения)
// Возвращает NULL при ошибке
const void* Vector_Get(const Vector* vec, size_t index) {
    if (vec == NULL || index >= vec->size || vec->data == NULL) {
        return NULL;
    }
    
    return (const char*)vec->data + index * vec->type_info->element_size;
}

// Установка значения элемента по индексу (замена)
// Возвращает: 0 — успех, -1 — ошибка
int Vector_Set(Vector* vec, size_t index, void* element) {
    if (vec == NULL || element == NULL || index >= vec->size || vec->type_info == NULL) {
        return -1;
    }
    
    void* dest = (char*)vec->data + index * vec->type_info->element_size;
    
    // Сначала уничтожаем старое значение (если нужно)
    if (vec->type_info->destroy != NULL) {
        vec->type_info->destroy(dest);
    }
    
    // Копируем новое значение
    if (vec->type_info->copy != NULL) {
        void* copied = vec->type_info->copy(element);
        if (copied == NULL) {
            return -1;
        }
        memcpy(dest, copied, vec->type_info->element_size);
        if (copied != dest) {
            free(copied);
        }
    } else {
        memcpy(dest, element, vec->type_info->element_size);
    }
    
    return 0;
}

// Получение текущего количества элементов
size_t Vector_Size(const Vector* vec) {
    return (vec != NULL) ? vec->size : 0;
}

// Получение текущей емкости
size_t Vector_Capacity(const Vector* vec) {
    return (vec != NULL) ? vec->capacity : 0;
}

// Проверка согласованности типов (для безопасности операций)
int Vector_CheckType(const Vector* vec, const FieldInfo* expected_type) {
    if (vec == NULL || expected_type == NULL) {
        return 0;
    }
    // Сравниваем указатели на FieldInfo (как рекомендовано в методичке, стр. 7)
    return (vec->type_info == expected_type) ? 1 : 0;
}