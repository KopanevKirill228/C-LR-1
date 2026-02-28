// dynamic_array.c
#include "dynamic_array.h"
#include <stdlib.h>
#include <string.h>

// ← Полное определение структуры (скрыто от заголовка)
struct _dynamic_array {
    void* data;              // указатель на динамический массив
    size_t size;             // текущее количество элементов
    size_t capacity;         // вместимость (сколько влезет без realloc)
    const FieldInfo* type_info;
};

// ← Внутренние функции
static size_t DynamicArray_CalculateNewCapacity(size_t current_capacity, size_t min_required);
static int DynamicArray_Resize(DynamicArray* arr, size_t new_capacity);

// === Вспомогательные функции ===

static size_t DynamicArray_CalculateNewCapacity(size_t current_capacity, size_t min_required) {
    size_t new_capacity = current_capacity;
    while (new_capacity < min_required) {
        new_capacity = (new_capacity == 0) ? 4 : new_capacity * 2;
    }
    return new_capacity;
}

static int DynamicArray_Resize(DynamicArray* arr, size_t new_capacity) {
    if (arr == NULL || arr->type_info == NULL) {
        return -1;
    }
    
    if (new_capacity < arr->size) {
        new_capacity = arr->size;
    }
    
    size_t old_bytes = arr->capacity * arr->type_info->element_size;
    size_t new_bytes = new_capacity * arr->type_info->element_size;
    
    void* new_data = realloc(arr->data, new_bytes);
    if (new_data == NULL && new_capacity > 0) {
        return -1;
    }
    
    if (new_capacity > arr->capacity && new_data != NULL) {
        unsigned char* new_mem = (unsigned char*)new_data + old_bytes;
        size_t extra_bytes = new_bytes - old_bytes;
        memset(new_mem, 0, extra_bytes);
    }
    
    arr->data = new_data;
    arr->capacity = new_capacity;
    return 0;
}

// === Публичные функции ===

DynamicArray* DynamicArray_Create(const FieldInfo* type_info, size_t initial_size) {
    if (type_info == NULL) {
        return NULL;
    }
    
    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    if (arr == NULL) {
        return NULL;
    }
    
    arr->type_info = type_info;
    arr->size = 0;
    arr->capacity = DynamicArray_CalculateNewCapacity(0, initial_size);
    
    size_t bytes = arr->capacity * type_info->element_size;
    arr->data = malloc(bytes);
    if (arr->data == NULL) {
        free(arr);
        return NULL;
    }
    
    memset(arr->data, 0, bytes);
    return arr;
}

void DynamicArray_Destroy(DynamicArray* arr) {
    if (arr == NULL) {
        return;
    }
    
    if (arr->data != NULL && arr->type_info->destroy != NULL) {
        for (size_t i = 0; i < arr->size; i++) {
            unsigned char* byte_ptr = (unsigned char*)arr->data;
            void* elem = byte_ptr + i * arr->type_info->element_size;
            arr->type_info->destroy(elem);
        }
    }
    
    free(arr->data);
    free(arr);
}

int DynamicArray_Push(DynamicArray* arr, void* element) {
    if (arr == NULL || element == NULL || arr->type_info == NULL) {
        return -1;
    }
    
    if (arr->size >= arr->capacity) {
        size_t new_capacity = DynamicArray_CalculateNewCapacity(arr->capacity, arr->size + 1);
        if (DynamicArray_Resize(arr, new_capacity) != 0) {
            return -1;
        }
    }
    
    unsigned char* byte_ptr = (unsigned char*)arr->data;
    void* dest = byte_ptr + arr->size * arr->type_info->element_size;
    
    if (arr->type_info->copy != NULL) {
        void* copied = arr->type_info->copy(element);
        if (copied == NULL) {
            return -1;
        }
        memcpy(dest, copied, arr->type_info->element_size);
        if (copied != dest) {
            free(copied);
        }
    } else {
        memcpy(dest, element, arr->type_info->element_size);
    }
    
    arr->size++;
    return 0;
}

const void* DynamicArray_Get(const DynamicArray* arr, size_t index) {
    if (arr == NULL || index >= arr->size || arr->data == NULL) {
        return NULL;
    }
    
    const unsigned char* byte_ptr = (const unsigned char*)arr->data;
    return byte_ptr + index * arr->type_info->element_size;
}

int DynamicArray_Set(DynamicArray* arr, size_t index, void* element) {
    if (arr == NULL || element == NULL || index >= arr->size || arr->type_info == NULL) {
        return -1;
    }
    
    unsigned char* byte_ptr = (unsigned char*)arr->data;
    void* dest = byte_ptr + index * arr->type_info->element_size;
    
    if (arr->type_info->destroy != NULL) {
        arr->type_info->destroy(dest);
    }
    
    if (arr->type_info->copy != NULL) {
        void* copied = arr->type_info->copy(element);
        if (copied == NULL) {
            return -1;
        }
        memcpy(dest, copied, arr->type_info->element_size);
        if (copied != dest) {
            free(copied);
        }
    } else {
        memcpy(dest, element, arr->type_info->element_size);
    }
    
    return 0;
}

size_t DynamicArray_Size(const DynamicArray* arr) {
    return (arr != NULL) ? arr->size : 0;
}

int DynamicArray_CheckType(const DynamicArray* arr, const FieldInfo* expected_type) {
    if (arr == NULL || expected_type == NULL) {
        return 0;
    }
    return (arr->type_info == expected_type) ? 1 : 0;
}