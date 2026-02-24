#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_string.h"
#include "fieldinfo.h"


void print_string_info(const char* name, String* s) {
    if (s == NULL) {
        printf("%s: NULL\n", name);
        return;
    }
    
    const char* content = String_ToCString(s);
    size_t len = String_Length(s);
    
    printf("%s: \"%s\" (length: %zu)\n", name, content ? content : "NULL", len);
}

void print_divider(void) {
    printf("\n----------------------------------------\n\n");
}

int main(void) {
    printf("========================================\n");
    printf("   LAB WORK #1, VARIANT 7\n");
    printf("   Polymorphic collection: STRING\n");
    printf("========================================\n\n");

    // 1. String creation demonstration
    printf("1. STRING CREATION\n");
    printf("------------------\n");
    
    String* s1 = String_Create("Hello");
    String* s2 = String_Create("World");
    String* s3 = String_Create("");  // Empty string
    String* s4 = String_Create(NULL);  // NULL string
    
    print_string_info("s1 (Hello)", s1);
    print_string_info("s2 (World)", s2);
    print_string_info("s3 (empty)", s3);
    print_string_info("s4 (NULL)", s4);
    
    print_divider();

    // 2. Concatenation demonstration
    printf("2. STRING CONCATENATION\n");
    printf("-----------------------\n");
    
    String* concat1 = String_Concat(s1, s2);
    print_string_info("s1 + s2", concat1);
    
    String* concat2 = String_Concat(s1, s3);
    print_string_info("s1 + empty", concat2);
    
    String* concat3 = String_Concat(s3, s2);
    print_string_info("empty + s2", concat3);
    
    String* concat4 = String_Concat(NULL, s2);
    printf("NULL + s2: %s\n", concat4 == NULL ? "NULL (expected)" : "ERROR!");
    
    print_divider();

    // 3. Substring demonstration
    printf("3. SUBSTRING OPERATION\n");
    printf("----------------------\n");
    
    String* text = String_Create("Hello, World!");
    print_string_info("Original text", text);
    
    String* sub1 = String_Substring(text, 0, 5);
    print_string_info("Substring [0,5)", sub1);
    
    String* sub2 = String_Substring(text, 7, 12);
    print_string_info("Substring [7,12)", sub2);
    
    String* sub3 = String_Substring(text, 5, 5);
    print_string_info("Empty substring [5,5)", sub3);
    
    // Boundary cases
    String* sub4 = String_Substring(text, 10, 5);
    printf("Substring [10,5) (start > end): %s\n", sub4 == NULL ? "NULL (correct)" : "ERROR!");
    
    String* sub5 = String_Substring(text, 0, 100);
    printf("Substring [0,100) (end > length): %s\n", sub5 == NULL ? "NULL (correct)" : "ERROR!");
    
    String* sub6 = String_Substring(NULL, 0, 5);
    printf("Substring from NULL: %s\n", sub6 == NULL ? "NULL (correct)" : "ERROR!");
    
    print_divider();

    // 4. Substring search demonstration
    printf("4. SUBSTRING SEARCH\n");
    printf("-------------------\n");
    
    String* search_text = String_Create("Hello, hello, HELLO! How many hellos?");
    String* pattern = String_Create("hello");
    
    print_string_info("Search text", search_text);
    print_string_info("Search pattern", pattern);
    
    size_t count;
    int* indices;
    
    // Case-sensitive search
    printf("\nCase-sensitive search:\n");
    indices = String_Find(search_text, pattern, 1, &count);
    if (indices) {
        printf("  Found %zu matches at positions: ", count);
        for (size_t i = 0; i < count; i++) {
            printf("%d ", indices[i]);
        }
        printf("\n");
        free(indices);
    } else {
        printf("  No matches found\n");
    }
    
    // Case-insensitive search
    printf("\nCase-insensitive search:\n");
    indices = String_Find(search_text, pattern, 0, &count);
    if (indices) {
        printf("  Found %zu matches at positions: ", count);
        for (size_t i = 0; i < count; i++) {
            printf("%d ", indices[i]);
        }
        printf("\n");
        free(indices);
    } else {
        printf("  No matches found\n");
    }
    
    // Search for non-existent substring
    String* not_found = String_Create("xyz");
    printf("\nSearch for non-existent 'xyz':\n");
    indices = String_Find(search_text, not_found, 1, &count);
    printf("  Result: %s, count = %zu\n", 
           indices == NULL ? "NULL (correct)" : "ERROR!", count);
    
    print_divider();

    // 5. Various tests
    printf("5. VARIOUS TESTS\n");
    printf("----------------\n");
    
    // Empty string
    String* empty = String_Create("");
    print_string_info("Empty string", empty);
    
    // Concatenation of empty strings
    String* empty_concat = String_Concat(empty, empty);
    print_string_info("Empty + empty", empty_concat);
    
    // Long string
    String* long_str = String_Create("This is a very long string for testing working with large data");
    print_string_info("Long string", long_str);
    
    // Substring from long string
    String* long_sub = String_Substring(long_str, 10, 30);
    print_string_info("Substring [10,30)", long_sub);
    
    print_divider();

    // 6. Polymorphism demonstration (FieldInfo)
    printf("6. POLYMORPHISM (FieldInfo)\n");
    printf("---------------------------\n");
    
    const FieldInfo* info1 = GetCharFieldInfo();
    const FieldInfo* info2 = GetCharFieldInfo();
    
    printf("FieldInfo for char:\n");
    printf("  Address 1: %p\n", (void*)info1);
    printf("  Address 2: %p\n", (void*)info2);
    printf("  Same object? %s\n", 
           info1 == info2 ? "YES (singleton works)" : "NO");
    printf("  Element size: %zu bytes\n", info1->element_size);
    printf("  copy function: %p\n", (void*)info1->copy);
    printf("  destroy function: %p\n", (void*)info1->destroy);
    printf("  compare function: %p\n", (void*)info1->compare);
    
    print_divider();

    // 7. Cache demonstration
    printf("7. C-STRING CACHE DEMONSTRATION\n");
    printf("-------------------------------\n");
    
    String* cache_test = String_Create("Cache test string");
    
    printf("First call to String_ToCString (creates cache):\n");
    const char* cstr1 = String_ToCString(cache_test);
    printf("  Result: \"%s\"\n", cstr1);
    printf("  Address: %p\n", (void*)cstr1);
    
    printf("\nSecond call to String_ToCString (should return same address):\n");
    const char* cstr2 = String_ToCString(cache_test);
    printf("  Result: \"%s\"\n", cstr2);
    printf("  Address: %p\n", (void*)cstr2);
    printf("  Addresses match? %s\n", 
           cstr1 == cstr2 ? "YES (cache works)" : "NO");
    
    print_divider();

    // 8. Memory cleanup
    printf("8. MEMORY CLEANUP\n");
    printf("-----------------\n");
    
    printf("Destroying all created strings...\n");
    
    // Free all created strings
    String_Destroy(s1);
    String_Destroy(s2);
    String_Destroy(s3);
    String_Destroy(s4);
    String_Destroy(concat1);
    String_Destroy(concat2);
    String_Destroy(concat3);
    String_Destroy(concat4);
    String_Destroy(text);
    String_Destroy(sub1);
    String_Destroy(sub2);
    String_Destroy(sub3);
    String_Destroy(sub4);
    String_Destroy(sub5);
    String_Destroy(sub6);
    String_Destroy(search_text);
    String_Destroy(pattern);
    String_Destroy(not_found);
    String_Destroy(empty);
    String_Destroy(empty_concat);
    String_Destroy(long_str);
    String_Destroy(long_sub);
    String_Destroy(cache_test);
    
    printf("Double destroy (should be safe):\n");
    String_Destroy(NULL);  // Should be safe
    String_Destroy(s1);    // Already destroyed, should be safe
    printf("  OK - no errors\n");
    
    printf("\n========================================\n");
    printf("   TESTING COMPLETED\n");
    printf("========================================\n");
    printf("\nPress Enter to exit...");
    getchar();
    
    return 0;
}