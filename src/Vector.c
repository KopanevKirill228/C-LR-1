#include "Vector.h"
#include <stdlib.h>
#include <string.h>

static size_t Vector_CalculateNewCapacity(size_t current_capacity, size_t min_required);
static int Vector_Resize(Vector* vec, size_t new_capacity);


static size_t Vector_CalculateNewCapacity(size_t current_capacity, size_t min_required) {
    size_t new_capacity = current_capacity;
    while (new_capacity < min_required) {
        new_capacity = (new_capacity == 0) ? 4 : new_capacity * 2;
    }
    return new_capacity;
}

static int Vector_Resize(Vector* vec, size_t new_capacity) {
    if (vec == NULL || vec->type_info == NULL) {
        return -1;
    }
    
    if (new_capacity < vec->size) {
        new_capacity = vec->size;
    }
    
    size_t old_bytes = vec->capacity * vec->type_info->element_size;
    size_t new_bytes = new_capacity * vec->type_info->element_size;
    
    void* new_data = realloc(vec->data, new_bytes);
    if (new_data == NULL && new_capacity > 0) {
        return -1;
    }
    
    if (new_capacity > vec->capacity && new_data != NULL) {
        unsigned char* new_mem = (unsigned char*)new_data + old_bytes;
        size_t extra_bytes = new_bytes - old_bytes;
        memset(new_mem, 0, extra_bytes);
    }
    
    vec->data = new_data;
    vec->capacity = new_capacity;
    return 0;
}



Vector* Vector_Create(const FieldInfo* type_info, size_t initial_size) {
    if (type_info == NULL) {
        return NULL;
    }
    
    Vector* vec = (Vector*)malloc(sizeof(Vector));
    if (vec == NULL) {
        return NULL;
    }
    
    vec->type_info = type_info;
    vec->size = 0;
    vec->capacity = Vector_CalculateNewCapacity(0, initial_size);
    
    size_t bytes = vec->capacity * type_info->element_size;
    vec->data = malloc(bytes);
    
    if (vec->data == NULL) {
        free(vec);
        return NULL;
    }
    
    memset(vec->data, 0, bytes);
    return vec;
}

void Vector_Destroy(Vector* vec) {
    if (vec == NULL) {
        return;
    }
    
    if (vec->data != NULL && vec->type_info->destroy != NULL) {
        for (size_t i = 0; i < vec->size; i++) {
            unsigned char* byte_ptr = (unsigned char*)vec->data;
            void* elem = byte_ptr + i * vec->type_info->element_size;
            vec->type_info->destroy(elem);
        }
    }
    
    free(vec->data);
    free(vec);
}

int Vector_Push(Vector* vec, void* element) {
    if (vec == NULL || element == NULL || vec->type_info == NULL) {
        return -1;
    }
    
    if (vec->size >= vec->capacity) {
        size_t new_capacity = Vector_CalculateNewCapacity(vec->capacity, vec->size + 1);
        if (Vector_Resize(vec, new_capacity) != 0) {
            return -1;
        }
    }
    
    unsigned char* byte_ptr = (unsigned char*)vec->data;
    void* dest = byte_ptr + vec->size * vec->type_info->element_size;
    
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
    
    vec->size++;
    return 0;
}

const void* Vector_Get(const Vector* vec, size_t index) {
    if (vec == NULL || index >= vec->size || vec->data == NULL) {
        return NULL;
    }
    const unsigned char* byte_ptr = (const unsigned char*)vec->data;
    return byte_ptr + index * vec->type_info->element_size;
}

int Vector_Set(Vector* vec, size_t index, void* element) {
    if (vec == NULL || element == NULL || index >= vec->size || vec->type_info == NULL) {
        return -1;
    }
    
    unsigned char* byte_ptr = (unsigned char*)vec->data;
    void* dest = byte_ptr + index * vec->type_info->element_size;
    
    if (vec->type_info->destroy != NULL) {
        vec->type_info->destroy(dest);
    }
    
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

size_t Vector_Size(const Vector* vec) {
    return (vec != NULL) ? vec->size : 0;
}

int Vector_CheckType(const Vector* vec, const FieldInfo* expected_type) {
    if (vec == NULL || expected_type == NULL) {
        return 0;
    }
    return (vec->type_info == expected_type) ? 1 : 0;
}